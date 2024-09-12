/*************************************************************************
    > File Name: echo_server.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月17日 星期三 17时03分35秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

//最大监听数量
#define LISTENSOCKET 5
#define MAXSIZE 1024

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

    //循环接受连接
    clnt_addr_size = sizeof(clnt_addr);
    for(int i=0; i< LISTENSOCKET; i++){

        clnt_sock = accept(server_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock == -1){
            printf("accept failed \n");
            exit(-1);
        }
        else{
            printf("connect client %d \n",i+1);
        }

        //循环接受 发送数据 但是只能同时服务一个客户端
        memset(message,0,MAXSIZE);
        while(1){
           

            
            str_len = read(clnt_sock,message,MAXSIZE);
            if(str_len <= 0){
                break;
            }
         
            printf("client %d : %s  \n",i+1,message);
            
            fgets(message,MAXSIZE-1,stdin);

            write(clnt_sock,message,MAXSIZE);
        }
        printf("the %d  client is end  \n",i+1);     

        //关闭文件描述符 结束这个客户端的任务
        close(clnt_sock);
    }
    //结束服务器任务
    close(server_sock);

    return 0;
}
