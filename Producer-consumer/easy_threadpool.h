/*************************************************************************
    > File Name: easy_threadpool.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月16日 星期二 23时35分26秒
 ************************************************************************/
#ifndef __THREADPOOL
#define __THREADPOOL
#include"BlockQ.h"

//线程池结构体
typedef struct{
    pthread_t* threads;
    int num;
    BlockQ* q;
}Threadpool;

void* start_routine(void* arg);

Threadpool*  Threadpool_create(int n);
void thread_destory(Threadpool* pool);

#endif 
