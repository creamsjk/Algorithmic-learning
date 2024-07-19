/*************************************************************************
    > File Name: log.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月18日 星期四 23时26分02秒
 ************************************************************************/
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include "log.h"
#include <pthread.h>

using namespace std;

Log::Log(){
    
    m_count = 0;
    m_is_async = false;
}

Log::~Log(){
    if(m_fp != NULL){
        fclose(m_fp);
    }
}

bool Log::init(const char* file_name, int log_buf_size, int split_lines,int max_queue_size){
   
    //max_queue_size >=1 说明使用异步来写日志
    if(max_queue_size >= 1){
       m_is_async = true;
      
       //生成多条阻塞队列来进行异步存取 日志
       m_log_queue = new block_queue<string>(max_queue_size);
       pthread_t tid;
       //flush_log_thread为回调函数,这里表示创建线程异步写日志
       pthread_create(&tid,NULL,flush_log_thread,NULL);
    }

    //初始化缓冲区和日志最大行数
    m_log_buf_size = log_buf_size;
    m_buf = new char[log_buf_size];
    memset(m_buf,'\0',m_log_buf_size);
    m_split_lines = split_lines;

    //获取现在时间
    time_t t = time(NULL);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    //C 库函数 char *strrchr(const char *str, int c) 在参数 str 所指向的字符串中搜索最后一次出现字符 c（一个无符号字符）的位置。
    const char *p = strrchr(file_name, '/');
    char log_full_name[256] = {0};

    //如果只传进来一个文件名 就是 p==NULL
    if (p == NULL)
    {
        snprintf(log_full_name, 255, "%d_%02d_%02d_%s", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, file_name);
    }
    else
    {
        //在本目录下面创建一个新的日志文件
        strcpy(log_name, p + 1);
        strncpy(dir_name, file_name, p - file_name + 1);
        snprintf(log_full_name, 255, "%s%d_%02d_%02d_%s", dir_name, my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, log_name);
    }

    //今天
    m_today = my_tm.tm_mday;

    //创建并读写这个文件
    m_fp = fopen(log_full_name, "a");
    if (m_fp == NULL)
    {
        return false;
    }

    return true;


}

void Log::write_log(int level,const char* format, ...){
    //初始化时间
    struct timeval now = {0,0};
    gettimeofday(&now,NULL);
    time_t t = now.tv_sec;
    struct tm* sys_tim = localtime(&t);
    struct tm my_tm = *sys_tim;

    char s[16] = {0};
    switch (level)
    {
    case 0:
        strcpy(s, "[debug]:");
        break;
    case 1:
        strcpy(s, "[info]:");
        break;
    case 2:
        strcpy(s, "[warn]:");
        break;
    case 3:
        strcpy(s, "[erro]:");
        break;
    default:
        strcpy(s, "[info]:");
        break;
    }
    //写入一个log，对m_count++, m_split_lines最大行数
    m_mutex.lock();
    m_count++;

    ////今天满了  或者  新的一天
    if (m_today != my_tm.tm_mday || m_count % m_split_lines == 0) //everyday log
    {

        char new_log[256] = {0};
        fflush(m_fp);
        fclose(m_fp);
        char tail[16] = {0};

        //拼接日期
        snprintf(tail, 16, "%d_%02d_%02d_", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday);

        if (m_today != my_tm.tm_mday)
        {
            snprintf(new_log, 255, "%s%s%s", dir_name, tail, log_name);
            m_today = my_tm.tm_mday;
            m_count = 0;
        }
        else
        {
            snprintf(new_log, 255, "%s%s%s.%lld", dir_name, tail, log_name, m_count / m_split_lines);
        }
        m_fp = fopen(new_log, "a");
    }

    m_mutex.unlock();

   //定义一个可变参数函数，至少要有一个固定参数（即 last 参数）。
   //声明一个 va_list 变量，用于存储可变参数列表的信息。
   //使用 va_start 宏初始化 va_list 变量。
   //使用 va_arg 宏依次获取每个可变参数。
   //使用 va_end 宏清理 va_list 变量。
    va_list valst;
    va_start(valst, format);

    string log_str;
    m_mutex.lock();

    //写入的具体时间内容格式
    int n = snprintf(m_buf, 48, "%d-%02d-%02d %02d:%02d:%02d.%06ld %s ",
                     my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
                     my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec, s);

    //使用vsnprintf()用于向一个字符串缓冲区打印格式化字符串，且可以限定打印的格式化字符串的最大长度。
    //从第n个字符开始格式化 到m_log_buf_size - 1 个字符
    int m = vsnprintf(m_buf + n, m_log_buf_size - 1, format, valst);
    m_buf[n + m] = '\n';
    m_buf[n + m + 1] = '\0';
    log_str = m_buf;

    m_mutex.unlock();

    //异步写入 与 同步写入
    if (m_is_async && !m_log_queue->full())
    {
        //异步  放到阻塞队列里面 线程会自动取
        m_log_queue->push(log_str);
    }
    else
    {
        //同步 写入到m_fp 中
        m_mutex.lock();
        fputs(log_str.c_str(), m_fp);
        m_mutex.unlock();
    }

    // 清理 valst
    va_end(valst);

}


void Log::flush(){
    //上锁 因为会有多线程
    m_mutex.lock();
    //强制刷新缓冲区
    fflush(m_fp);
    m_mutex.unlock();
}
