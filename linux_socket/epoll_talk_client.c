/*************************************************************************
    > File Name: epoll_talk_client.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月21日 星期日 16时03分01秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXSIZE 1024

//传消息的结构体  typedef uint32_t in_addr_t;
typedef struct {
char buff[1024];
struct in_addr dst_addr;

}talk;




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
    //char buff[MAXSIZE];
    talk message;
    while(1){

        //输入信息并组装
        fputs("Input message(Q to quit):",stdout);
        fgets(message.buff,sizeof(message.buff),stdin);

        fputs("Input dst addr :",stdout);
        char s[4]={0};
        fgets(s,4,stdin);
        message.dst_addr.s_addr = inet_addr(s);
        
        //输入q 或者 Q 接回车退出
        if(!strcmp(message.buff,"Q\n") || !strcmp(message.buff,"q\n"))
            break;
        printf("the message is : %s , the dst ip  is : %d \n",message.buff,message.dst_addr.s_addr);

       //将信息发给服务区
       str_len =  write(client_sock,&message,sizeof(message));

       //读发回来的数据
       recv_len = read(client_sock,&message,sizeof(message));
       if(recv_len <= 0)
           break;
       //循环读一个,防止服务端一下子没全部发过来造成读取问题
       printf("come from  %d  the message is %s \n",message.dst_addr.s_addr,message.buff);

        

    }
    //关闭套接字
    close(client_sock);
    return 0;
}
