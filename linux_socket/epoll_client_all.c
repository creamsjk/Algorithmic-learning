/*************************************************************************
    > File Name: epoll_client_all.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月22日 星期一 20时20分57秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>

#define MAXSIZE 1024

int main(int argc, char* argv[] ){

    //校验参数
    if(argc != 3){
        printf("Usage:./hello_client <IP>  <port> \n");
        exit(-1);
    }

    int client_sock;
    struct sockaddr_in serv_addr;
    int str_len,recv_len,recv_cnt;

    //设置套接字
    client_sock = socket(PF_INET,SOCK_STREAM,0);
    if(client_sock == -1){
        printf("socker failed \n");
        exit(-1);
    }
    
    //绑定服务器地址
    memset(&serv_addr,0,sizeof(struct sockaddr_in));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //连接服务器
    if(-1 == connect(client_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))){
        printf("connect serv failed \n ");
        exit(-1);
    }
    else{
        printf("coonect server success \n");
    }
    //读取服务器发送的数据
    char buff[MAXSIZE];
    int ret;
    fd_set rdset;
    FD_ZERO(&rdset);
    int clientfd = client_sock;
    while(1){
   
         FD_SET(STDIN_FILENO, &rdset);
        FD_SET(clientfd, &rdset);
        select(clientfd + 1, &rdset, NULL, NULL, NULL);
        //当select函数返回时，rdset会被修改的

        if(FD_ISSET(STDIN_FILENO, &rdset)) {
            //读取从键盘输入的字符串
            memset(buff, 0, sizeof(buff));
            //通过read函数会把'\n'也读进来
            ret = read(STDIN_FILENO, buff, sizeof(buff));
            if(strcmp(buff, "bye\n") == 0) {
                break;
            }
            //在发送时，不需要发送'\n'
            send(clientfd, buff, ret - 1, 0);
            continue;
        }

         if(FD_ISSET(clientfd, &rdset)) {
            //从服务器接收数据
            memset(buff, 0, sizeof(buff));
            ret = recv(clientfd, buff, sizeof(buff), 0);
            if(ret == 0) {
                printf("byebye.\n");
                break;
            }
            printf("ret: %d, recv: %s\n", ret, buff);
        }

    }
    //关闭套接字
    close(client_sock);
    return 0;
}
