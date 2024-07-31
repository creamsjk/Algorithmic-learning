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
            if(bytes_read == -1){
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


//解析http请求的一个头部信息
http_conn::HTTP_CODE http_conn::parse_headers(char *text){
    
    //执行下面其中一个 再有解析其他的再添加
    if(text[0] == '\0'){

        if(m_content_length != 0){

            m_check_state = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;

    }else if(strncasecmp(text, "Connection:", 11) == 0){

        text += 11;
        text += strspn(text, " \t");
        if(strcasecmp(text, "keep-alive") == 0){

            m_linger = true;
        }

    }else if(strncasecmp(text, "Content-length:", 15) == 0){

        text += 15;
        text += strspn(text, " \t");
        m_content_length = atol(text);

    }else if(strncasecmp(text, "Hose:", 5) == 0){

        text += 5;
        text += strspn(text, " \t");
        m_host = text;
    }else{

        LOG_INFO("oop!unknowheader :%s", text);
    }

    return NO_REQUEST;
}

//判断http请求是否被完整读入
http_conn::HTTP_CODE http_conn::parse_content(char *text){

   if(m_read_idx >= (m_content_length + m_checked_idx)){
     
       text[m_content_length]  = '\0';
       //POST 请求中最后输入的用户名和密码
       m_string = text;
       return GET_REQUEST;
   }
   return NO_REQUEST;
}

//对读进来的请求进行处理
http_conn::HTTP_CODE http_conn::process_read(){

    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char *text = 0;

    //如果读到了请求体 或者 读完一行 这两个都成功
    while((m_check_state == CHECK_STATE_CONTENT && line_status == LINE_OK) || ((line_status = parse_line()) == LINE_OK)){

        //获得一行
        text = get_line();
        m_start_line = m_checked_idx;
        LOG_INFO("%s",text);

        //有限状态机
        switch(m_check_state){

            //请求行判断
        case CHECK_STATE_REQUESTLINE:
            {
                ret = parse_request_line(text);
                if(ret == BAD_REQUEST)
                    return BAD_REQUEST;
                break;
            }
            //检查头部    
        case CHECK_STATE_HEADER:
            {
                ret = parse_headers(text);
                if(ret == BAD_REQUEST)
                    return BAD_REQUEST;
                else if(ret == GET_REQUEST){

                    return do_request();
                }
                break;
            }
            //判断http请求是否被完整读入
        case CHECK_STATE_CONTENT:
            {
                ret = parse_content(text);
                if(ret == GET_REQUEST)
                    return do_request();
                line_status = LINE_OPEN;
                break;
            }
        default:
            return INTERNAL_ERROR;
        }

    }
    return NO_REQUEST;

}

http_conn::HTTP_CODE http_conn::do_request(){

    strcpy(m_real_file,doc_root);
    int len = strlen(doc_root);
    //printf("murl:%s \n",m_url);
    const char *p = strrchr(m_url,'/');

    //处理cgi
    if(cgi == 1 && (*(p + 1) == '2'|| *(p + 1) == '3')){

        //根据标志判断是登陆检测还是注册检测
        char flag = m_url[1];

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/");
        strcat(m_url_real, m_url + 2);
        strncpy(m_real_file + len, m_url_real, FILENAME_LEN - len - 1);
        free(m_url_real);

        //将用户名和密码提取出来
        //user=123&passwd=123

        char name[100],password[100];
        int i;
        for(int i=5;m_string[i] != '&';++i){
            name[i-5] = m_string[i];
        }
        //现在char * name = 用户名 
        name[i-5] = '\0';

        //此处有修改
        int j=0;
        for(i =i+8;m_string[i] != '\0';++i,++j){
            password[j]=m_string[i];
        }
        password[j] = '\0';

        if(*(p + 1) == '3'){

            //如果是注册,就先检测数据库是否有重名
            //没重名就添加进去 拼接一个insert语句
            char *sql_insert = (char *)malloc(sizeof(char)* 200);
            strcpy(sql_insert,"INSERT INFO user(username,passwd) VALUSE(");
            strcat(sql_insert,"'");
            strcat(sql_insert,name);
            strcat(sql_insert,"','");
            strcat(sql_insert,password);
            strcat(sql_insert,"')");

            //没找到就进入注册界面  否则进出错界面
            if(users.find(name) == users.end()){

                m_lock.lock();
                int res = mysql_query(mysql,sql_insert);
                users.insert(pair<string,string>(name,password));
                m_lock.unlock();


                if(!res)
                    strcpy(m_url,"/log.html");
                else
                    strcpy(m_url,"/registerError.html");
            }
            else
                strcpy(m_url,"/registerError.html");
        }
        //如果是登陆 直接检查
        //若是浏览器输入的用户名和密码可以在表中查到,返回 1  否则返回 0
        else if(*(p + 1 ) == 2){

            if(users.find(name) != users.end() && users[name] == password)
                strcpy(m_url,"welcome.html");
            else
                strcpy(m_url,"/logError,html");
        }
    }
    
    //注册界面
    if(*(p + 1) == '0'){

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real,"/register.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }else if(*(p + 1) == '1'){//登陆界面

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real,"/log.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }else if(*(p + 1) == '5'){//照片界面

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real,"/picture.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }else if(*(p + 1) == '6'){//视频界面

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real,"/video.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }else if(*(p + 1) == '7'){//呃呃呃界面

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real,"/fans.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }else// //如果以上均不符合，即不是登录和注册，直接将url与网站目录拼接  这里的情况是welcome界面，请求服务器上的一个图片
        strncpy(m_real_file + len ,m_url, FILENAME_LEN - len - 1 );

    //通过stat获取请求资源文件信息，成功则将信息更新到m_file_stat结构体
    //失败返回NO_RESOURCE状态，表示资源不存在
    if(stat(m_real_file, &m_file_stat) < 0)
        return NO_RESOURCE;
    
    //判断文件的权限，是否可读，不可读则返回FORBIDDEN_REQUEST状态
    if(!(m_file_stat.st_mode & S_IROTH))
        return FORBIDDEN_REQUEST;

    //判断文件类型，如果是目录，则返回BAD_REQUEST，表示请求报文有误
    if(S_ISDIR(m_file_stat.st_mode))
        return BAD_REQUEST;

    //0拷贝  以只读方式获取文件描述符，通过mmap将该文件映射到内存中
    int fd = open(m_real_file, O_RDONLY);
    m_file_address = (char*)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    //后续不再使用 就直接关闭
    close(fd);

    //表示请求文件存在，且可以访问
    return FILE_REQUEST;


}

void http_conn::unmap(){

    //释放分配的地址空间  safe and effective
    if(m_file_address){

        munmap(m_file_address, m_file_stat.st_size);
        m_file_address = 0;
    }
}


bool http_conn::write()
{
    int temp = 0;

    int newadd = 0;

    //若要发送的数据长度为0
    //表示响应报文为空，一般不会出现这种情况
    if(bytes_to_send==0)
    {
        modfd(m_epollfd,m_sockfd,EPOLLIN,m_TRIGMode);
        init();
        return true;
    }

    while (1)
    {   
        //将响应报文的状态行、消息头、空行和响应正文发送给浏览器端
        temp=writev(m_sockfd,m_iv,m_iv_count);

        //正常发送，temp为发送的字节数
        if (temp > 0)
        {
            //更新已发送字节
            bytes_have_send += temp;
            //偏移文件iovec的指针
            newadd = bytes_have_send - m_write_idx;
        }
        if (temp <= -1)
        {
            //判断缓冲区是否满了
            if (errno == EAGAIN)
            {
                //第一个iovec头部信息的数据已发送完，发送第二个iovec数据
                if (bytes_have_send >= m_iv[0].iov_len)
                {
                    //不再继续发送头部信息
                    m_iv[0].iov_len = 0;
                    m_iv[1].iov_base = m_file_address + newadd;
                    m_iv[1].iov_len = bytes_to_send;
                }
                //继续发送第一个iovec头部信息的数据
                else
                {
                    m_iv[0].iov_base = m_write_buf + bytes_to_send;
                    m_iv[0].iov_len = m_iv[0].iov_len - bytes_have_send;
                }
                //重新注册写事件
                modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
                return true;
            }
            //如果发送失败，但不是缓冲区问题，取消映射
            unmap();
            return false;
        }

        //更新已发送字节数
        bytes_to_send -= temp;

        //判断条件，数据已全部发送完
        if (bytes_to_send <= 0)
        {
            unmap();

            //在epoll树上重置EPOLLONESHOT事件
            modfd(m_epollfd,m_sockfd,EPOLLIN,m_TRIGMode);

            //浏览器的请求为长连接
            if(m_linger)
            {
                //重新初始化HTTP对象
                init();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}


bool http_conn::add_response(const char *format,...){

    if(m_write_idx >= WRITE_BUFFER_SIZE)
        return false;

    //定义可变参数列表
    va_list arg_list;

    //将变量arg_list初始化为传入参数
    va_start(arg_list, format);

    //将数据format从可变参数列表写入缓冲区写，返回写入数据的长度
    int len = vsnprintf(m_write_buf + m_write_idx, WRITE_BUFFER_SIZE - 1 - m_write_idx, format, arg_list);

    //如果写入的数据长度超过缓冲区剩余空间，则报错
    if(len >= (WRITE_BUFFER_SIZE -1 -m_write_idx )){

        va_end(arg_list);
        return false;
    }

    //更新m_write_idx位置
    m_write_idx += len;

    // //清空可变参列表
    va_end(arg_list);
    
    LOG_INFO("request:%s", m_write_buf);

    return true;
}


//下面这一系列函数 都是为了构建响应报文一部分 不包括响应体
bool http_conn::add_status_line(int status, const char *title){

    return add_response("%s %d %s\r\n","HTTP/1.1", status, title);
}

bool http_conn::add_headers(int content_len){

    return add_content_length(content_len) && add_linger() && add_blank_line();
}

bool http_conn::add_content_length(int content_len){

    return add_response("Content-Length:%d\r\n",content_len);
}

bool http_conn::add_content_type(){

    return add_response("Content-Type:%s\r\n","text/html");
}

bool http_conn::add_linger(){

    return add_response("Connection:%s\r\n", (m_linger == true) ? "keep-alive" : "close");
}

bool http_conn::add_blank_line(){

    return add_response("%s", "\r\n");
}

bool http_conn::add_content(const char *content){

    return add_response("%s", content);
}




bool http_conn::process_write(HTTP_CODE ret){

    switch(ret){

        //内部错误，500
        case INTERNAL_ERROR:
        {

            add_status_line(500,error_500_title);
            add_headers(strlen(error_500_form));
            if(!add_content(error_500_title))
                return false;
            break;
        }
        //报文语法有误，404
        case BAD_REQUEST: 
        {

            add_status_line(404, error_404_title);
            add_headers(strlen(error_404_form));
            if(!add_content(error_404_form))
                return false;
            break;
        }
        //资源没有访问权限，403
        case FORBIDDEN_REQUEST:
        {
            add_status_line(403, error_403_title);
            add_headers(strlen(error_403_form));
            if(!add_content(error_403_form))
                return false;
            break;
        }
        //文件存在，200
        case FILE_REQUEST:
        {
            add_status_line(200, ok_200_title);
            //如果请求的资源存在
            if(m_file_stat.st_size != 0){

                add_headers(m_file_stat.st_size);
                //第一个iovec指针指向响应报文缓冲区，长度指向m_write_idx
                m_iv[0].iov_base = m_write_buf;
                m_iv[0].iov_len = m_write_idx;
                //第二个iovec指针指向mmap返回的文件指针，长度指向文件大小
                m_iv[1].iov_base = m_file_address;
                m_iv[1].iov_len = m_file_stat.st_size;
                m_iv_count = 2;
                ////发送的全部数据为响应报文头部信息和文件大小
                bytes_to_send = m_write_idx + m_file_stat.st_size;
                return true;
            }else{

                 //如果请求的资源大小为0，则返回空白html文件
                const char *ok_string = "<html><body></body></html>";
                add_headers(strlen(ok_string));
                if(!add_content(ok_string))
                    return false;
            }

        }
        default:
               return false;

    }
    //除FILE_REQUEST状态外，其余状态只申请一个iovec，指向响应报文缓冲区
    m_iv[0].iov_base = m_write_buf;
    m_iv[0].iov_len = m_write_idx;
    m_iv_count = 1;
    bytes_to_send = m_write_idx;
    return true;

}

//子线程调用的函数
void http_conn::process(){

    HTTP_CODE read_ret = process_read();
    if(read_ret == NO_REQUEST){

        modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
        return ;
    }
    bool write_ret = process_write(read_ret);
    if(!write_ret){

        close_conn();
    }
    modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
}



























