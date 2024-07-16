/*************************************************************************
  > File Name: BlockQ.c
  > Author: sunrie
  > Mail: 1102490208@qq.com 
  > Created Time: 2024年07月16日 星期二 20时55分31秒
 ************************************************************************/

//阻塞队列函数实现
#include"BlockQ.h"


BlockQ* blockq_create(void){

    BlockQ* q = (BlockQ*)malloc(sizeof(BlockQ));
    if(!q){
        printf("malloc  failed \n");
        exit(-1);
    }
    q->front = 0;
    q->rear  = 0;
    q->size  = 0;
    pthread_mutex_init(&q->mutex,NULL);
    pthread_cond_init(&q->not_full,NULL);
    pthread_cond_init(&q->not_empty,NULL);

    return q;
}
void  blockq_destroy(BlockQ* q){
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
    pthread_mutex_destroy(&q->mutex);

    free(q);

}

bool blockq_empty(BlockQ* q){
    pthread_mutex_lock(&q->mutex);
    int result = q->size;
    pthread_mutex_unlock(&q->mutex);
    return result == 0;
}
bool blockq_full(BlockQ* q){

    pthread_mutex_lock(&q->mutex);
    int result = q->size;
    pthread_mutex_unlock(&q->mutex);
    return result == N ;
}
void blockq_push(BlockQ* q, E val){
    pthread_mutex_lock(&q->mutex);

    int err;
    while(q->size == N){
        err = pthread_cond_wait(&q->not_full,&q->mutex);
        if(err){
            printf("pthread_cond_wait failed \n");
            pthread_mutex_unlock(&q->mutex);
        }

    }
    q->elements[q->rear] = val;
    q->rear = (q->rear + 1)%N;
    q->size++;

    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}
E blockq_pop(BlockQ* q){
    pthread_mutex_lock(&q->mutex);
    int err;
    while(q->size == 0 ){
        err = pthread_cond_wait(&q->not_empty,&q->mutex);
        if(err){
            printf("pthread_cond_wait failed \n");
            pthread_mutex_unlock(&q->mutex);
        }
    }
    E result = q->elements[q->front];
    q->front = (q->front + 1)%N;
    q->size--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return result;

}
E blockq_peek(BlockQ* q){
    pthread_mutex_lock(&q->mutex);

    int err;
    while(q->size == 0 ){
        err = pthread_cond_wait(&q->not_empty,&q->mutex);
        if(err){
            printf("pthread_cond_wait failed \n");
            pthread_mutex_unlock(&q->mutex);
        }

    }
    E result = q->elements[q->front];
    pthread_mutex_unlock(&q->mutex);
    return result;
}

