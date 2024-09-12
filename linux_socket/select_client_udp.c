/*************************************************************************
    > File Name: hello_client.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月17日 星期三 15时58分57秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>

int main(int argc, char* argv[] ){

    //校验参数
    if(argc != 3){
        printf("Usage:./hello_client <IP>  <port> \n");
        exit(-1);
    }

    int server_sock;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    //设置套接字
    server_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(server_sock == -1){
        printf("socker failed \n");
        exit(-1);
    }
    
    //绑定服务器地址
    memset(&clnt_addr,0,sizeof(struct sockaddr_in));

    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
    clnt_addr.sin_port = htons(atoi(argv[2]));
    fd_set fd;

    FD_ZERO(&fd);
    FD_SET(server_sock,&fd);
    FD_SET(0,&fd);
    char buff[1024]={0};
    clnt_addr_size = sizeof(clnt_addr);
    
    fd_set tmp;
    printf("clint over \n");
    while(1){
        tmp = fd;
        int result = select(server_sock+1,&tmp,NULL,NULL,NULL);

    
       if(result == -1){
            return -1;
        }
        else {

        
        if(FD_ISSET(server_sock, &tmp)){
            memset(buff,0,sizeof(buff)); 
            printf("before..    ");
            recvfrom(server_sock,buff,1024,0,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
            printf("form server:  %s \n",buff);
        }
        if(FD_ISSET(0, &tmp)){
            printf("put....   ");
            memset(buff,0,sizeof(buff)); 
            fgets(buff,sizeof(buff),stdin);
            sendto(server_sock,  buff,1024,0,(const  struct sockaddr *)&clnt_addr,clnt_addr_size);
        }
      }
    }


    //关闭套接字
    close(server_sock);
    return 0;
}
