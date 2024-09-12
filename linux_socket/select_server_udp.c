/*************************************************************************
    > File Name: select_server_udp.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月23日 星期二 19时36分16秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
//最大监听数量
#define LISTENSOCKET 5

int main(int argc, char* argv[]){

    //校验参数
    if(argc != 2){
        printf("Usage:./hello_server port \n");
        exit(-1);
    }

    int server_sock;
    int clnt_sock;

    struct sockaddr_in server_addr;
    struct sockaddr_in clnt_addr;

    socklen_t server_addr_size;

    char message[] = "Hello world";

    //绑定套接字
    server_sock = socket(PF_INET,SOCK_DGRAM,0);
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
    if(-1 == bind(server_sock,(const struct sockaddr *)&server_addr,sizeof(server_addr))){
        printf("bind failed \n");
        exit(-1);
    }

   fd_set fd;
    
    FD_ZERO(&fd);
    FD_SET(server_sock,&fd);
    FD_SET(0,&fd);
    char buff[1024]={0};
    memset(&clnt_addr,0,sizeof(clnt_addr));
    server_addr_size = sizeof(server_addr);

    fd_set tmp;
    printf("server over \n");
    while(1){
        tmp = fd;
        int result = select(server_sock+1,&tmp,NULL,NULL,0);

      //  memset(&clnt_addr,0,sizeof(clnt_addr));
        if(FD_ISSET(server_sock, &tmp)){
 
            memset(buff,0,sizeof(buff));
            printf("before..     ");
            recvfrom(server_sock,buff,1024,0,(struct sockaddr *)&server_addr,&server_addr_size);
            printf("form client:  %s \n",buff);
        }
        if(FD_ISSET(STDIN_FILENO, &tmp)){
            memset(buff,0,sizeof(buff));

            printf("server inpute   ");
            fgets(buff,sizeof(buff),stdin);
            sendto(server_sock,  buff,1024,0,( struct sockaddr *)&server_addr,server_addr_size);
        }
    }


   close(server_sock);

    return 0;
}
