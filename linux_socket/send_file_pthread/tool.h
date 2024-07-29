/*************************************************************************
    > File Name: tool.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月27日 星期六 15时47分28秒
    tool && queue
 ************************************************************************/

#ifndef  __TOOL_H
#define  __TOOL_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<time.h>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<error.h>
#include<sys/wait.h>
#include<pthread.h>
#include<semaphore.h>

void ERROR_HEAD_CHECK(int ret,char* s);

void ERROR_NULL_CHECK(void *ret,char *s);
void ERROR_CHECK(int a,int b, char * c);
 


typedef struct task_s{
    int peerfd;
    struct task_s *pNext;
}task_t;

typedef struct task_queue_s{
    task_t *pFront;
    task_t *pRear;
    int queSize;
    pthread_mutex_t mutex;
    sem_t sem;
    pthread_cond_t cond;
    int flag; //0 exit
    
}task_queue_t;


int queueInit(task_queue_t *que);
int queueDestroy(task_queue_t *que);
int queueIsEmpty(task_queue_t *que);
int taskSize(task_queue_t *que);
int taskEnque(task_queue_t *que, int peerfd);
int taskDeque(task_queue_t *que);
int broadcastAll(task_queue_t *que);
   
int tcpInit(char* addr, char *port );
int epollReadAdd(int epfd, int fd);
int epollReadDel(int epfd, int fd);

#endif
