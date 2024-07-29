/*************************************************************************
    > File Name: tool.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月27日 星期六 16时26分43秒
 ************************************************************************/

#include"tool.h"


void ERROR_HEAD_CHECK(int ret,char* s){
    if(ret != 0){
        printf("%s \n",s);
        exit(1);
    }
}
void ERROR_NULL_CHECK(void *ret,char *s){
    if(ret == NULL){
        printf("%s \n",s);
        exit(1);
    }
}
void ERROR_CHECK(int a,int b, char * c){
    if( a == b){
        printf("%s \n",c);
        exit(1);
    }
}



int queueInit(task_queue_t *que){
    
    if(!que){
        printf("que is  NULL \n");
        exit(-1);
    }
    que->pFront = (task_t*)malloc(sizeof(task_t));
    if(que->pFront == NULL){
        printf("queueInit  malloc  failed \n");
        exit(1);
    }
    que->pFront->pNext = NULL;
    que->pRear = (task_t*)malloc(sizeof(task_t));
    ERROR_NULL_CHECK(que->pRear,"queueInit malloc failed ");
    que->pRear->pNext = NULL;
    que->queSize = 0;
    int ret = pthread_mutex_init(&que->mutex,NULL);
    ERROR_HEAD_CHECK(ret,"pthread_init failed ");
    ret = sem_init(&que->sem,0,0);
    ERROR_HEAD_CHECK(ret,"sem_init  failed ");
    ret = pthread_cond_init(&que->cond,NULL);
    ERROR_HEAD_CHECK(ret,"pthread_cond_init  failed ");
    que->flag = 1;
   
    // printf("queue init success \n");
    return 0;

}

int queueDestroy(task_queue_t *que){

    if(que == NULL){
        exit(1);
    }
 
  pthread_mutex_destroy(&que->mutex);
  pthread_cond_destroy(&que->cond);
  

  if(que->pFront->pNext){
      task_t *tmp = que->pFront;
      task_t *t = NULL;
      while(tmp->pNext){
          t = tmp->pNext;
          tmp->pNext = tmp->pNext->pNext;
          free(t);
      }

  }

  if(que->pFront){
      free(que->pFront);
      que->pFront = NULL;
  }

  if(que->pRear){
      free(que->pRear);
      que->pRear = NULL;
  }

  //对锁的认知还不够充足

  printf("queueDestroy success \n");
  return 0;

}

int queueIsEmpty(task_queue_t *que){
    
   return que->queSize == 0;
}

int taskSize(task_queue_t *que){
    
    return que->queSize;

}

int taskEnque(task_queue_t *que, int peerfd){
    //尾插插法
    
    if(!que){
        printf("que is  NULL \n");
        exit(-1);
    }
    
    int ret = pthread_mutex_lock(&que->mutex);
    ERROR_HEAD_CHECK(ret,"taskDeque lock failed \n");

    task_t *node = (task_t*)malloc(sizeof(task_t));
    ERROR_NULL_CHECK(node,"taskEnque failed \n");
    node->pNext = NULL;
    node->peerfd = peerfd;

    if(que->queSize == 0){
        que->pFront->pNext = node;
        que->pRear->pNext = node;
        
    }
    else{
     que->pRear->pNext->pNext = node;
     que->pRear->pNext = node;
    }
    que->queSize++;

    ret = pthread_cond_signal(&que->cond);
    ERROR_HEAD_CHECK(ret,"taskDeque lock failed \n");
    ret = pthread_mutex_unlock(&que->mutex);
    ERROR_HEAD_CHECK(ret,"taskDeque lock failed \n");

    printf("queue insert success \n");
    return 0;
    
}

int taskDeque(task_queue_t *que){
    //从头部拿出来一个
    
    //printf("task begin \n");
    if(!que){
        printf("que is  NULL \n");
        exit(-1);
    }
    int ret = pthread_mutex_lock(&que->mutex);
    ERROR_HEAD_CHECK(ret,"taskDeque lock failed \n");

    while(que->queSize == 0){
       pthread_cond_wait(&que->cond,&que->mutex); 
    }
    //一定不为0
    int result = que->pFront->pNext->peerfd;
    task_t *t = que->pFront->pNext;
    que->pFront->pNext = t->pNext;
    free(t);
    t = NULL;
    que->queSize--;
    
    ret = pthread_mutex_unlock(&que->mutex);
    ERROR_HEAD_CHECK(ret,"taskDeque lock failed \n");

    //printf("queue take success \n");
   
    return result;

}

int broadcastAll(task_queue_t *que){

    if(!que){
        printf("que is  NULL \n");
        exit(-1);
    }
    int ret = pthread_cond_broadcast(&que->cond);
    ERROR_HEAD_CHECK(ret,"broadcastAll failed ");
    return 0;
}



int tcpInit(char* addr, char *port ){
    int sockfd = socket(PF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket failed ");
    //设置端口复用
    int on = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ERROR_CHECK(ret,-1,"setsockopt failed ");


    struct sockaddr_in server_add;
    memset(&server_add,0,sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = inet_addr(addr);
    server_add.sin_port = htons(atoi(port));

    ret = bind(sockfd,(struct sockaddr*)&server_add,(socklen_t)sizeof(server_add));
    ERROR_CHECK(ret,-1,"bind failed ");

    ret  = listen(sockfd,10);
    ERROR_CHECK(ret,-1,"listen failed ");

    return sockfd;

}
int epollReadAdd(int epfd, int fd){
    //printf("this fd is : %d \n",fd);
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    ERROR_CHECK(ret,-1,"epoll_ctl add failed ");
    return 0;

}

int epollReadDel(int epfd, int fd){

    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    ERROR_CHECK(ret,-1,"epoll_ctl del failed ");

    return 0;
}

