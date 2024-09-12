/*************************************************************************
 
    > File Name: homework_19_1.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月19日 星期五 20时46分56秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<netinet/in.h>

int main(int argc, char* argv[]){
    struct hostent* pHost = gethostbyname(argv[1]);

    printf("the host name is : %s \n",pHost->h_name);
    printf("thie host addres tye: %d \n",pHost->h_addrtype);

     if(pHost ->h_aliases[0] == NULL)
         printf("what? \n");
    for(int i=0; pHost->h_aliases[i] != NULL;i++){
        printf("aliases= %s \n",pHost->h_aliases[i]);
        printf("asdasdasd \n");
    }


    printf("the host ip is :  ");

    for(int i=0;pHost->h_addr_list[i] != NULL ;i++){
        printf("  %s ",inet_ntoa(*(struct in_addr*)pHost->h_addr_list[i]));
    }
    printf("\n");

    return 0;
}
