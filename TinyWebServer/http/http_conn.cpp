/*************************************************************************
    > File Name: http_conn.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月29日 星期一 20时10分45秒
 ************************************************************************/

#include "http_conn.h"

#include <mysql/mysql.h>
#include <fstream>

//定义http响应的一些状态信息
const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";

locker m_lock;
map<string, string> users;

//从数据库中拿到所有账号密码
void http_conn::initmysql_result(connection_pool *connPoll){
    //从连接池中拿出一个连接
    MYSQL *mysql = NULL;
    connectionRAII(&mysql,connPoll);
    
    if(mysql_query(mysql,"select user,password from user")){
        LOG_ERROR("select error:%s \n",mysql_error(mysql));
    }

    //从表中检索完整的结果集
     MYSQL_RES *result = mysql_store_result(mysql);

     //返回结果集中的列数
     int num_fields = mysql_num_fields(result);

     //返回所有字段结构的数组
     MYSQL_FIELD *fields = mysql_fetch_fields(result);

     //从结果集中获取下一行,将对应的用户名和密码 存入map
     while(MYSQL_ROW row = mysql_fetch_row(result)){
         string us(row[0]);
         string pas(row[1]);
         users[us] = pas;
     }

}

//对文件描述符设置非阻塞
int setnoblocking(int fd){
    //获取旧的状态
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    //设置旧的状态
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

//将内核事件表注册读事件 ET模式  选择开启EPOLLONESHOT
void addfd(int epollfd, int fd, bool one_shot, int TRIGmode){
    
   epoll_event event;
   event.data.fd = fd;

   //EPOLLRDHUP 用来判断对端是否关闭
   if(1 == TRIGmode){
       event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
   }
   else{
       event.events = EPOLLIN |  EPOLLRDHUP;
  
   }

   //EPOLLONESHOT  通过手动的方式来保证同一SOCKET只能被一个线程处理，不会跨越多个线程
   if(one_shot){
       event.events |= EPOLLONESHOT;
   }

   epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
   setnoblocking(fd);
}

void  removefd(int epollfd, int fd){

    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}

//将事件重置为EPOLLONESHOT
void modfd(int epollfd, int fd, int ev, int TRIGmode){
   
    epoll_event event;
    event.data.fd = fd;
    
    if(1 == TRIGmode){
        event.events = ev | EPOLLIN | EPOLLONESHOT | EPOLLRDHUP;
    }
    else{
        event.events = ev |  EPOLLONESHOT | EPOLLRDHUP ;
    }

    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

//一个用户退出就从epoll中删去 并减少用户数量
void http_conn::close_conn(bool real_close){
    if(real_close && (m_sockfd != -1)){

        printf("close %d \n",m_sockfd);
        removefd(m_epollfd,m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
}

//初始化连接,外部调用初始化套接字地址
void http_conn::init(int sockfd, const sockaddr_in &addr, char *root, int TRIGmode, int close_log, string user, string passwd, string sqlname){

    //初始化一些属性
    m_sockfd = sockfd;
    m_address = addr;
    addfd(m_epollfd, sockfd, true, m_TRIGMode);
    m_user_count++;

    //当浏览器出现连接重置时,可能是网站根目录出错 或 http相应格式出错或者访问的文件中内容完全为空
    doc_root = root;
    m_TRIGMode = TRIGmode;
    m_close_log = close_log;

    strcpy(sql_user,user.c_str());
    strcpy(sql_passwd,passwd.c_str());
    strcpy(sql_name,sqlname.c_str());
    init();

}

//初始化新接受的连接
//check_state  默认为分析请求行状态
void http_conn::init(){
    mysql = NULL;
    bytes_to_send = 0;
    bytes_have_send = 0;
    m_check_state = CHECK_STATE_REQUESTLINE;
    m_linger = false;
    m_method = GET;
    m_url = 0;
    m_version = 0;
    m_content_length = 0;
    m_host = 0;
    m_start_line = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    m_write_idx = 0;
    cgi = 0;
    m_state = 0;
    timer_flag = 0;
    improv = 0;
    
    memset(m_read_buf, '\0', READ_BUFFER_SIZE);
    memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
    memset(m_real_file, '\0' ,FILENAME_LEN);

}

//从状态机, 用于分析出一行内容
//返回值为行的读取状态, 有LINE_OK,  LINE_BAD, LINE_OPEN
//LINE_OK，完整读取一行   LINE_BAD，报文语法有误   LINE_OPEN，读取的行不完整
http_conn::LINE_STATUS http_conn::parse_line(){

    char temp;
    for(; m_checked_idx < m_read_idx; ++m_checked_idx){
        
        temp = m_read_buf[m_checked_idx];
        //检查是不是读到'\r' or '\n' 只有读到 '\r''\n' 才是完整的一行 
        if(temp == '\r'){

            if((m_checked_idx + 1) == m_read_idx)
                 return LINE_OPEN;
            else if(m_read_buf[m_checked_idx + 1] == '\n'){
                m_read_buf[m_checked_idx++] = '\0'; 
                m_read_buf[m_checked_idx++] = '\0'; 
                return LINE_OK;
            }
            return LINE_BAD;
        }else if(temp == '\n'){
            if(m_checked_idx > 1 && m_read_buf[m_checked_idx - 1] == '\r'){
                m_read_buf[m_checked_idx - 1] = '\0';
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }

    return LINE_OPEN;

}

//循环读取客户数据,直到无数据可读或对方关闭连接
//非阻塞ET工作模式下,需要一次性将数据读完
bool http_conn::read_once(){
    
    if(m_read_idx >= READ_BUFFER_SIZE){
        return false;
    }
    int bytes_read = 0;

    //LT读数据
    if(0 == m_TRIGMode){

        bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx,0);
        m_read_idx += bytes_read;

        if(bytes_read <= 0){

            return false;
        }
        return true;
    } else {//ET读数据   循环读

        while(true){

            bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx,0);
            if(bytes_read = -1){
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                return false;
            }else if(bytes_read == 0){

                return false;
            }
            m_read_idx += bytes_read;
        }
        return true;

    }
}

//解析http请求行，获得请求方法，目标url及http版本号
http_conn::HTTP_CODE http_conn::parse_request_line(char *text){

    //检索字符串 str1 中第一个匹配字符串 str2 中字符的字符
    //请求行中最先含有空格和\t任一字符的位置并返回
    m_url = strpbrk(text, " \t");
    if(!m_url){

        return BAD_REQUEST;
    }
    //将该位置改为\0，用于将前面数据取出
    *m_url++ = '\0';
    char *method = text;
    //本项目只使用了 GET 和 POST 判断字符是否相等忽略大小写
    if(strcasecmp(method, "GET") == 0)
        m_method = GET;
    else if(strcasecmp(method, "POST") == 0){
        m_method = POST;
        cgi = 1;
    }
    else
        return BAD_REQUEST;
    
    //m_url此时跳过了第一个空格或\t字符，但不知道之后是否还有
    //将m_url向后偏移，通过查找，继续跳过空格和\t字符，指向请求资源的第一个字符
    m_url += strspn(m_url, " \t");
    m_version = strpbrk(m_url, " \t");
   if(!m_version)
       return BAD_REQUEST;
   *m_version++ = '\0';
   m_version += strspn(m_version, " \t");
   if(strcasecmp(m_version, "HTTP/1.1") != 0 )
       return BAD_REQUEST;
   if(strncasecmp(m_url, "http://",7) == 0){
       
       m_url += 7;
       m_url = strchr(m_url, '/');
   }
   if(strncasecmp(m_url, "https://", 8) == 0){

       m_url += 7;
       m_url = strchr(m_url, '/');
   }

   if(!m_url)
       return BAD_REQUEST;

   //show menu
   if(strlen(m_url) == 1)
       strcat(m_url,"judge.html");

   m_check_state = CHECK_STATE_HEADER;
   return NO_REQUEST;

}










































