/*************************************************************************
    > File Name: client_puts.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月07日 星期三 09时30分26秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<time.h>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<error.h>
#include<sys/wait.h>



typedef struct {
    int len;
    char buff[1020];

}message_t;

//接收确定的字节数的数据 等接受到注定数量的数据在结束
int recvn(int sockfd, void * buff, int len)
{
    int left = len;
    char * pbuf =(char*) buff;
    int ret = -1;
    while(left > 0) {
        ret = recv(sockfd, pbuf, left, 0);
        if(ret == 0) {
            break;
        } else if(ret < 0) {
            perror("recv");
            return -1;
        }
        left -= ret;
        pbuf += ret;
    }
    return len - left;
}


int main(void){


     //创建客户端的套接字
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    //指定使用的是IPv4的地址类型 AF_INET
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8080);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //连接服务器
    int ret = connect(clientfd, (struct sockaddr*)&serveraddr,
                      sizeof(serveraddr));
    printf("connect success.\n");

    //进行数据的接受
    //
    int fd = clientfd;
      char* filename = "sunrise";
    int rfd = open(filename,O_RDONLY);

    message_t srcMessage;
    memset(&srcMessage,0,sizeof(srcMessage));
    
    //传输文件名
    strcpy(srcMessage.buff,filename);
    srcMessage.len = strlen(srcMessage.buff);

    //发送全部4 + buff 的内容
     ret = send(fd,&srcMessage,4+srcMessage.len,0);
    printf("send firse %d \n",4+srcMessage.len);

    struct stat st;
    fstat(rfd,&st);
    //all bits
    int total = st.st_size;
    int send_bit = 0;
    int len = send(fd,&total,sizeof(total),0);
    printf("send total success  \n");

    while(send_bit < total){
        memset(&srcMessage,0,sizeof(srcMessage));
        
        //从文件中读一些
        len = read(rfd,srcMessage.buff,sizeof(srcMessage.buff));
       // printf("len == %d \n",len);
        srcMessage.len = len;
        if(srcMessage.len <=0 )
            break;
       // printf("srcMessage.len  =  %d  %s \n",srcMessage.len,srcMessage.buff);
        
        //分两次发送 第一次发送的是这次发送的文件大小 第二次是真正的文件
        ret = send(fd,&srcMessage.len,sizeof(srcMessage.len),0);

        ret = send(fd,srcMessage.buff,srcMessage.len,0);

        send_bit += ret;
    }
   printf("puts 完成\n");

   return 0;

}
