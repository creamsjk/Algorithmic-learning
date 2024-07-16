/*************************************************************************
    > File Name: VlockQ.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月16日 星期二 20时50分05秒
 ************************************************************************/
#ifndef __MY_BLOCKQ_H
#define __MY_BLOCKQ_H

//阻塞队列头文件
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<error.h>

typedef int E;
#define  N 1024

typedef struct {
    E elements[N];
    int front;
    int rear;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} BlockQ;

BlockQ* blockq_create(void);
void    blockq_destroy(BlockQ* q);

bool blockq_empty(BlockQ* q);
bool blockq_full(BlockQ* q);
void blockq_push(BlockQ* q, E val);
E blockq_pop(BlockQ* q);
E blockq_peek(BlockQ* q);

#endif
