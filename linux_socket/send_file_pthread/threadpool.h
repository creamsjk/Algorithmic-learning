/*************************************************************************
    > File Name: threadpool.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月29日 星期一 10时58分01秒
 ************************************************************************/

#ifndef  __PTHREADPOOL_H
#define  __PTHREADPOOL_H

#include"./tool.h"

typedef struct threadpool_s{
    pthread_t *pthreads;
    int pthreadNum;
    task_queue_t que;
}threadpool_t;


int threadpoolInit(threadpool_t *pool,int num);
int threadpoolDestory(threadpool_t *pool);
int threadpoolStart(threadpool_t *pool);
int threadpoolStop(threadpool_t *pool);



#endif 
