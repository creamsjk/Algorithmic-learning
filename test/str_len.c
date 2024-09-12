/*************************************************************************
    > File Name: str_len.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月06日 星期二 20时46分16秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>

int main(void){

    char * s =(char*) malloc(sizeof(char) * 200);
    strcpy(s,"asd");

    int len = strlen(s);

    printf("len == %d  s == %s \n", len, s);

    free(s);

    return 0;

}
