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

int main(int argc, char* argv[] ){

    //校验参数
    if(argc != 3){
        printf("Usage:./hello_client <IP>  <port> \n");
        exit(-1);
    }

    int client_sock;
    struct sockaddr_in serv_addr;

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

    //读取服务器发送的数据
    char buff[1024] =" ";
    read(client_sock,buff,sizeof(buff));
    printf("the message is %s \n",buff);

    //关闭套接字
    close(client_sock);
    return 0;
}
