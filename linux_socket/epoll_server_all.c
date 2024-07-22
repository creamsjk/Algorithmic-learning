/*************************************************************************
    > File Name: epoll_server.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月20日 星期六 14时15分31秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<time.h>

//最大监听数量
#define LISTENSOCKET 10
#define MAXSIZE 1024
#define EPOLLSIZE 100
//int all_fd[1024]={-1};
typedef struct {
    int fd ;
    time_t now;
}fd_message;

fd_message all_fd[1024]={0};

int main(int argc, char* argv[]){

    //校验参数
    if(argc != 2){
        printf("Usage:. %s  <port> \n",argv[0]);
        exit(-1);
    }

    int server_sock;
    int clnt_sock;
    int str_len;

    struct sockaddr_in server_addr;
    struct sockaddr_in clnt_addr;
    struct epoll_event *evt;
    struct epoll_event  event;
    int epfd;
    int event_cnt;

    socklen_t clnt_addr_size;

    char message[MAXSIZE];

    //绑定套接字
    server_sock = socket(PF_INET,SOCK_STREAM,0);
    if(server_sock == -1){
        printf("socket failed \n");
        exit(-1);
    }

    //设置地址
    memset(&server_addr,0,sizeof(struct sockaddr_in));
   
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    
    //绑定地址
    if(-1 == bind(server_sock,(struct sockaddr *)&server_addr,sizeof(server_addr))){
        printf("bind failed \n");
        exit(-1);
    }

    //监听
    if(-1 == listen(server_sock,LISTENSOCKET)){
        printf("listen failed \n");
        exit(-1);
    }

    //初始化epoll例程
    epfd = epoll_create(EPOLLSIZE);
   
    //注册到epoll里面
    event.data.fd = server_sock;
    event.events = EPOLLIN ;

    evt = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLLSIZE);

    epoll_ctl(epfd,EPOLL_CTL_ADD,server_sock,&event);

    while(1){
        event_cnt =  epoll_wait(epfd,evt,EPOLLSIZE,-1);

        if(event_cnt == -1){
            printf("epoll_wait failed \n");
            exit(-1);
        }
        //超过十秒直接踢出去
        time_t later;
        time(&later);
        for(int k=0;k<1024;k++){

            if(all_fd[k].fd >= 3 && later - all_fd[k].now > 10)
            {
                send(all_fd[k].fd,"time out\n",10,0);
                epoll_ctl(epfd,EPOLL_CTL_DEL,all_fd[k].fd,NULL);
                printf("disconnect client: %d \n",all_fd[k].fd);
                close(all_fd[k].fd);
                all_fd[k].fd = -1;
                all_fd[k].now = 0;

            }

        }



        for(int i=0;i<event_cnt;i++){

            //说明有新的连接连进来了
            if(evt[i].data.fd == server_sock){
                clnt_addr_size = sizeof(clnt_addr);
                clnt_sock = accept(server_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
                if(clnt_sock == -1){
                    printf("accept failed \n");
                    exit(-1);
                }
                event.data.fd = clnt_sock;
                event.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connect client %d  the ip is : %s \n",clnt_sock,inet_ntoa(clnt_addr.sin_addr));

                for(int k=0;k<1024;k++){
                    if(all_fd[k].fd <=3){
                        all_fd[k].fd = clnt_sock;
                        time(&all_fd[k].now);
                        break;
                    }
                }
                
            }
            else{
                
                memset(message,0,MAXSIZE);
                str_len = read(evt[i].data.fd,message,MAXSIZE);
                              
                //strlen <= 0 说明读完了 关闭连接
                if(str_len <= 0){
                    //从集合中删除 并且关闭文件描述符 不再监听 
                    epoll_ctl(epfd,EPOLL_CTL_DEL,evt[i].data.fd,NULL);
                    close(evt[i].data.fd);
                    printf("disconnect client : %d \n",evt[i].data.fd);
                   // for(int k=0;k<1024;k++){
                   // if(all_fd[k].fd  == evt[i].data.fd){
                   //     all_fd[k].fd = 0;
                   //     break;
                   // }
                   //}            
              
                    //结束本次循环
                    continue;
                }
                //重置时间
                for(int k=0;k<1024;k++){
                    if(all_fd[k].fd == evt[i].data.fd){
                        time(&all_fd[k].now);
                    }

                }
                printf("message is :%s \n",message);

                // for(int k=0;k<1024;k++){
               //     if(all_fd[k].fd >= 3)
               //         printf("%d  %ld \n",all_fd[k].fd,all_fd[k].now);
               // }

                //回发信息
                for(int k=0;k<1024;k++){
                     
                    if(all_fd[k].fd >= 3 && later - all_fd[k].now <= 10)
                        send(all_fd[k].fd,message,sizeof(message),0);
              
                }

 
                                

            }
        }

    }





    //结束服务器任务
    close(server_sock);
    close(epfd);

    return 0;
}
