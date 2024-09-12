/*************************************************************************
    > File Name: usertest.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月31日 星期三 19时55分48秒
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
    char *m_string = "user=1239994&passwd=123456";

    char name[100],password[100];
    int i;
    for (i = 5; m_string[i] != '&'; ++i)
        name[i - 5] = m_string[i];
    name[i - 5] = '\0';

    int j = 0;
    for (i = i + 8; m_string[i] != '\0'; ++i, ++j)
        password[j] = m_string[i];
    password[j] = '\0';



  printf("%s \n",name);
  printf("%s \n",password);



}
