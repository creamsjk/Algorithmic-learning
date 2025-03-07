/*************************************************************************
    > File Name: log.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月18日 星期四 21时50分12秒
 ************************************************************************/

#ifndef  __LOG_H
#define  __LOG_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <pthread.h>
#include "block_queue.h"

class Log{

public:
    //懒汉模式 开始就创建一个对象
    static Log* get_instance(){
      static  Log log;
        return &log;
    }
    
    //异步调用的线程启动函数
    static void* flush_log_thread(void* args){
        Log::get_instance()->async_write_log();    
        
       return NULL;
    }
    //可选择的参数有日志文件、日志缓冲区大小、最大行数以及最长日志条队列
    bool init(const char *file_name, int close_log, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);

    //向日志中写入东西
    void write_log(int level, const char *format, ...);

    //刷新缓冲区
    void flush(void);
private:
    Log();
    virtual ~Log();
    //封装好的线程启动函数
    void* async_write_log(){
        string single_log;
        //从阻塞队列中取出一个日志string，写入文件
        while (m_log_queue->pop(single_log))
        {
            m_mutex.lock();
            fputs(single_log.c_str(), m_fp);
            m_mutex.unlock();
        }
        return NULL;
    }


private:
    char dir_name[128];  //路径名字
    char log_name[128];  //日志名字
    int m_split_lines;   //日志最大行数
    int m_log_buf_size;  //日志缓冲区大小
    long long m_count;   //日志行数记录
    int m_today;           //当前日期
    FILE* m_fp;          //日志文件描述符
    char* m_buf;             
    block_queue<string> *m_log_queue;  // 阻塞队列
    bool m_is_async;                   //是否开启异步标志位
    locker m_mutex;                    //互斥锁
    int m_close_log; //关闭日志

};

#define LOG_DEBUG(format, ...) Log::get_instance()->write_log(0, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Log::get_instance()->write_log(1, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) Log::get_instance()->write_log(2, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Log::get_instance()->write_log(3, format, ##__VA_ARGS__)

#endif
