/*************************************************************************
    > File Name: easy_threadpool.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月16日 星期二 22时21分41秒
 ************************************************************************/

#include "easy_threadpool.h"
#define sizeThread 8

//线程执行任务函数
void* start_routine(void* arg){
   BlockQ* block = (BlockQ*)arg;
   pthread_t tid = pthread_self();

	for (;;) {
		E task_id = blockq_pop(block); // 如果没有任务，则阻塞
		if (task_id == -1) {
			pthread_exit(NULL);
		}

		printf("Thread 0x%lx: executing task #%d\n", tid, task_id);
		sleep(1);
		printf("Thread 0x%lx: task #%d done\n", tid, task_id);
	}
    
   return NULL;
}


// 初始化线程池 当阻塞队列中的值为-1时线程退出
Threadpool*  Threadpool_create(int n){
    Threadpool* pool = (Threadpool*)malloc(sizeof(Threadpool));
    if(!pool){
        printf("malloc Threadpool failed \n");
        exit(-1);
    }
   pool->num = n;
   pool->threads = (pthread_t*)malloc(sizeof(pthread_t)*n);
   if(!pool->threads){
       printf("malloc pthread failed \n");
       exit(-1);
   }
   pool->q = blockq_create();
   int err;
   for(int i=0;i<n;i++){
     err = pthread_create(pool->threads + i,NULL,start_routine,pool->q);

     if(err){
         printf("pthread_create  failed  \n");
         exit(-1);
     }
   }//线程池创建完毕

   return pool;
}


void thread_destory(Threadpool* pool){
    blockq_destroy(pool->q);
    free(pool->threads);
    pool->threads = NULL;
    free(pool);
    pool = NULL;

}

