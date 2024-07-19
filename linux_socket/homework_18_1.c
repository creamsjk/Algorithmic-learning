/*************************************************************************
    > File Name: homework_18_1.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月18日 星期四 19时55分00秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>

int main(void){
    int num = 12345678;
    char* p =(char*)&num;

    printf("%d \n",*p);
}
