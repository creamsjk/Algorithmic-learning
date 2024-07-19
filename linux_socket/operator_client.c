/*************************************************************************
    > File Name: operator_client.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月17日 星期三 20时33分47秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXSIZE 1024
#define OPSZ 4

int main(int argc, char* argv[] ){

    //校验参数
    if(argc != 3){
        printf("Usage:./hello_client <IP>  <port> \n");
        exit(-1);
    }

    int client_sock;
    struct sockaddr_in serv_addr;
    char buff[MAXSIZE];

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
        printf("connect server success !! \n");
    }

    int result , opnd_cnt;
    //读取服务器发送的数据
    fputs("Operand count: ",stdout);
    scanf("%d",&opnd_cnt);
    buff[0] = (char)opnd_cnt;

    for(int i=0; i<opnd_cnt; i++){
        printf("Operand %d: ",i+1);
        scanf("%d",(int *)&buff[i*OPSZ+1]);
    }
    fgetc(stdin);
    fputs("Operator: ",stdout);
    scanf("%c",&buff[opnd_cnt * OPSZ +1 ]);
    printf(" %s  \n",buff);
    write(client_sock,buff,opnd_cnt*OPSZ+2);
    read(client_sock,&result,4);
    printf("Operation result : %d \n", result);


    //关闭套接字
    close(client_sock);
    return 0;
}
