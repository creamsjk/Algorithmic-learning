/*************************************************************************
    > File Name: tool.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月25日 星期四 17时45分44秒
 ************************************************************************/
#ifndef  __TOOL_H
#define  __TOOL_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<time.h>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<error.h>

int work(int pipefd);
int transfer(int fd );
int recvn(int sockfd, void * buff, int len);

void ERROR_CHECK(int a,int b, char * c){
    if( a == b){
        printf("%s \n",c);
        exit(1);
    }
}

//发送一个文件描述符fd 到 pipefd中
int sendFd(int pipeFd, int fd){
    char buff[32]={0};
    struct msghdr hdr;
    memset(&hdr,0,sizeof(hdr));

    struct iovec iov;
    iov.iov_base = buff;
    iov.iov_len = sizeof(buff);

    //给最后一个选项分配空间并计算cmsghdr的空间
    int len = CMSG_LEN(sizeof(fd));
    struct cmsghdr * phdr = (struct cmsghdr*)calloc(1,len);
    phdr->cmsg_level = SOL_SOCKET;
    phdr->cmsg_type = SCM_RIGHTS;
    phdr->cmsg_len = len;
    int *p = (int*)CMSG_DATA(phdr);
    *p = fd;
    
    //初始化hdr的内容 4 个元素
    hdr.msg_control = phdr;
    hdr.msg_controllen = len;
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
   
    int ret = sendmsg(pipeFd,&hdr,0);
    ERROR_CHECK(ret, -1, "sendmsg failed ");
    printf("send: %d \n",ret);

    free(phdr);
 
    return 0;
}

//接收一个文件描述符
int recvFd(int pipeFd, int * rfd){
    char buff[32]={0};
    struct msghdr hdr;
    memset(&hdr,0,sizeof(hdr));

    struct iovec iov;
    iov.iov_base = buff;
    iov.iov_len = sizeof(buff);

    //给最后一个选项分配空间并计算cmsghdr的空间
    int len = CMSG_LEN(sizeof(int));
    struct cmsghdr * phdr = (struct cmsghdr*)calloc(1,len);
    //初始化四个元素
    phdr->cmsg_level = SOL_SOCKET;
    phdr->cmsg_type = SCM_RIGHTS;
    phdr->cmsg_len = len;
    int *p = (int*)CMSG_DATA(phdr);
    //*p代表的是传回来的文件描述符
    
    //初始化四个元素
    hdr.msg_control = phdr;
    hdr.msg_controllen = len;
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
   
    int ret = recvmsg(pipeFd,&hdr,0);
    ERROR_CHECK(ret, -1, "recvmsg failed ");
    printf("send: %d \n",ret);

    *rfd = *p;
    free(phdr);
 
    return 0;
}
typedef enum{
    BUSY=0,
    FREE
}child_t;

typedef struct{
    pid_t pid;
    child_t status;
    int pipfd;

}process_child;

typedef struct {
    int len;
    char buff[1020];

}message_t;


int createChild(int number,process_child *child){
   
    //this is a error  不能传一个指针来分配大小  回去就不知道大小了 会出现错误
//    child = (process_child*)malloc(sizeof(process_child)*number);

    if(child == NULL){
        printf("createChild malloc failed \n");
        exit(1);
    }

    for(int i=0;i<number;i++){
        int pipes[2];
        socketpair(AF_LOCAL,SOCK_STREAM,0,pipes);
        pid_t pid = fork();
        if(pid == 0){
            close(pipes[1]);
            work(pipes[0]);
            exit(0);
        }
        close(pipes[0]);
        child[i].pid = pid;
        child[i].status = FREE;
        //和子进程通信的管道
        child[i].pipfd = pipes[1];


    }

    return 0;

}

//进程的工作函数 接受父亲从管道传过来的clientfd 将其复制到fd中
int work(int pipefd ){

    printf("proces %d is doTask...\n", getpid());
    while(1){
        int fd ;
        //没有任务就会堵塞在这里
        recvFd(pipefd,&fd);
       //处理任务

      //  send(fd, "hello,client",12,0);
        transfer(fd);
        printf("child %d send finish.\n", getpid());

        close(fd);
         int one = 1;
        write(pipefd, &one, sizeof(one));
    }
}

int tcpInit(char* addr, char *port ){
    int sockfd = socket(PF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket failed ");
    //设置端口复用
    int on = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ERROR_CHECK(ret,-1,"setsockopt failed ");


    struct sockaddr_in server_add;
    memset(&server_add,0,sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = inet_addr(addr);
    server_add.sin_port = htons(atoi(port));

    ret = bind(sockfd,(struct sockaddr*)&server_add,(socklen_t)sizeof(server_add));
    ERROR_CHECK(ret,-1,"bind failed ");

    ret  = listen(sockfd,10);
    ERROR_CHECK(ret,-1,"listen failed ");

    return sockfd;

}
int epollReadAdd(int epfd, int fd){
    printf("this fd is : %d \n",fd);
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    ERROR_CHECK(ret,-1,"epoll_ctl add failed ");    
    return 0;

}

int epollReadDel(int epfd, int fd){

    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    ERROR_CHECK(ret,-1,"epoll_ctl del failed ");    

    return 0;
}
int transfer(int fd){
    //读取数据发送到客户端 fd 是客户端文件描述符
    
    char* filename = "asd";
    int rfd = open(filename,O_RDONLY);
    ERROR_CHECK(rfd,-1,"open failed ");

    message_t srcMessage;
    memset(&srcMessage,0,sizeof(srcMessage));
    
    //传输文件名
    strcpy(srcMessage.buff,filename);
    srcMessage.len = strlen(srcMessage.buff);

    //发送全部4 + buff 的内容
    int ret = send(fd,&srcMessage,4+srcMessage.len,0);
    printf("send firse %d \n",4+srcMessage.len);

    struct stat st;
    fstat(rfd,&st);
    //all bits
    int total = st.st_size;
    int send_bit = 0;
    int len = send(fd,&total,sizeof(total),0);
    printf("send total success  \n");

    while(send_bit < total){
        memset(&srcMessage,0,sizeof(srcMessage));
        
        //从文件中读一些
        len = read(rfd,srcMessage.buff,sizeof(srcMessage.buff));
       // printf("len == %d \n",len);
        srcMessage.len = len;
        if(srcMessage.len <=0 )
            break;
       // printf("srcMessage.len  =  %d  %s \n",srcMessage.len,srcMessage.buff);
        
        //分两次发送 第一次发送的是这次发送的文件大小 第二次是真正的文件
        ret = send(fd,&srcMessage.len,sizeof(srcMessage.len),0);

        ret = send(fd,srcMessage.buff,srcMessage.len,0);

        send_bit += ret;
    }

    return 0;

}
//接收确定的字节数的数据 等接受到注定数量的数据在结束
int recvn(int sockfd, void * buff, int len)
{
    int left = len;
    char * pbuf =(char*) buff;
    int ret = -1;
    while(left > 0) {
        ret = recv(sockfd, pbuf, left, 0);
        if(ret == 0) {
            break;
        } else if(ret < 0) {
            perror("recv");
            return -1;
        }
        left -= ret;
        pbuf += ret;
    }
    return len - left;
}




#endif
