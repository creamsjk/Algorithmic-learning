/*************************************************************************
    > File Name: operator_server.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月17日 星期三 20时10分47秒
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
#define OPSZ 4

int calculate(int opnux, int opnds[], char op);

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

    int opnd_cnt,result;
    int recv_len,recv_cnt;
    //接受连接
    clnt_addr_size = sizeof(clnt_addr);
    for(int i =0;i<LISTENSOCKET;i++){

        clnt_sock = accept(server_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock == -1){
            printf("accept failed \n");
            exit(-1);
        }
        else{
            printf("connect client \n");
        }
        //work

        read(clnt_sock,&opnd_cnt,1);
        
        recv_len = 0;
        while((opnd_cnt*OPSZ + 1)>recv_len){
            recv_cnt = read(clnt_sock,&message[recv_len],MAXSIZE-1);
            recv_len += recv_cnt;
        }
        result = calculate(opnd_cnt,(int *)message,message[recv_len-1]);
        write(clnt_sock,(char*)&result,sizeof(result));

        close(clnt_sock);
    }
    close(server_sock);

    return 0;
}

int calculate(int opnum, int opnds[], char op){
    int result = opnds[0],i;

    switch(op){
    case '+':
        for(i=0;i<opnum;i++) result += opnds[i];
        break;
    case '-':
        for(i=0;i<opnum;i++) result -= opnds[i];
        break;
    case '*':
        for(i=0;i<opnum;i++) result *= opnds[i];
        break;
     case '/':
        for(i=0;i<opnum;i++) result /= opnds[i];
        break;

    }
    return result;

}
