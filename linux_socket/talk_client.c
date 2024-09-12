/*************************************************************************
    > File Name: talk_client.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月19日 星期五 21时25分02秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXSIZE 1024

int main(int argc, char* argv[] ){

    //校验参数
    if(argc != 3){
        printf("Usage:./hello_client <IP>  <port> \n");
        exit(-1);
    }

    int client_sock;
    struct sockaddr_in serv_addr;
    int recv_cnt;

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
    while(1){

        fputs("Input message(Q to quit):",stdout);
        fgets(buff,MAXSIZE-1,stdin);
        
        //输入q 或者 Q 接回车退出
        if(!strcmp(buff,"Q\n") || !strcmp(buff,"q\n"))
            break;

       //将信息发给服务区
        write(client_sock,buff,strlen(buff)+1);

       //循环读一个,防止服务端一下子没全部发过来造成读取问题
        recv_cnt = read(client_sock,buff,MAXSIZE);
        if(recv_cnt <= 0)
            break;
        printf("the server  message is %s \n",buff);

    }
    //关闭套接字
    close(client_sock);
    return 0;
}
