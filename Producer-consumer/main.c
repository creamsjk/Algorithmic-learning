/*************************************************************************
    > File Name: main.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月16日 星期二 23时03分58秒
 ************************************************************************/
#include"easy_threadpool.h"

int main(void){

     int n = 8;
     Threadpool* pool = Threadpool_create(n);

     for(int i=0;i<=100;i++){
         blockq_push(pool->q,i);
     }
     for(int i=0;i<n;i++){
         blockq_push(pool->q,-1);
     }

     for(int i=0;i<n;i++){
         pthread_join(pool->threads[i],NULL);
     }
     printf("the threadpool end! \n");

    return 0;
}
