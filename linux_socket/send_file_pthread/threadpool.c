/*************************************************************************
    > File Name: threadpool.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月29日 星期一 11时05分58秒
 ************************************************************************/
#include"./threadpool.h"

typedef struct {
    int len;
    char buff[1020];

}message_t;



int  transferFile(int fd){
    //读取数据发送到客户端 fd 是客户端文件描述符

    printf("fd == %d \n",fd);
    // while(1);
    char* filename = "qwe.txt";
    int rfd = open(filename,O_RDONLY);
    ERROR_CHECK(rfd,-1,"open failed ");

    message_t srcMessage;
    memset(&srcMessage,0,sizeof(srcMessage));

    //传输文件名
    strcpy(srcMessage.buff,filename);
    srcMessage.len = strlen(srcMessage.buff);

    //发送全部4 + buff 的内容
    int ret = send(fd,&srcMessage,4+srcMessage.len,0);
   // printf("send firse %d \n",4+srcMessage.len);

    struct stat st;
    fstat(rfd,&st);
    //all bits
    int total = st.st_size;
    int send_bit = 0;
    int len = send(fd,&total,sizeof(total),0);
    printf("send total success  \n");

    while(send_bit < total){
        memset(&srcMessage,0,sizeof(srcMessage));

        //从文件中读一些
        len = read(rfd,srcMessage.buff,sizeof(srcMessage.buff));
        // printf("len == %d \n",len);
        srcMessage.len = len;
        if(srcMessage.len <=0 )
            break;
        // printf("srcMessage.len  =  %d  %s \n",srcMessage.len,srcMessage.buff);

        //分两次发送 第一次发送的是这次发送的文件大小 第二次是真正的文件
        ret = send(fd,&srcMessage.len,sizeof(srcMessage.len),0);

        ret = send(fd,srcMessage.buff,srcMessage.len,0);

        send_bit += ret;
    }
    return 0;
}

void *work(void* arg){
    printf("work .......\n");
    threadpool_t *pool = (threadpool_t* )arg;
    
    while(1){
        int peerfd = taskDeque(&pool->que);
        if(peerfd > 0){
            transferFile(peerfd);
        }else{
            break;
        }

    }
    printf("sub thread %ld is exiting. \n",pthread_self());
    return NULL;

}

int threadpoolInit(threadpool_t *pool,int num){
    
    ERROR_NULL_CHECK(pool,"threadpoolInit  failed \n");
    
    pool->pthreads = (pthread_t*) malloc(num * sizeof(pthread_t));
    memset(pool->pthreads,0,sizeof(num*sizeof(pthread_t)));
    pool->pthreadNum = num;
    queueInit(&pool->que);
    //printf("threadpoolInit success \n");

    return 0;
     
   }

int threadpoolDestory(threadpool_t *pool){
 
    //printf("threadpoolDestory  begin.. \n");
    if(pool == NULL){
      printf("threadpoolDestory faile  ");
      exit(1);
    }
    if(pool->pthreads != NULL){
        free(pool->pthreads);
    }
    pool->pthreads = NULL;
   // printf("aaaaaaaaa\n");
    queueDestroy(&pool->que);

    printf("threadpoolDestory success \n");

    return 0;
}

int threadpoolStart(threadpool_t *pool){

    //printf("pthreaSTARt begin \n");
    int ret  = 0; 
    int num = pool->pthreadNum;
    for(int i=0;i<num;i++){
        ret =  pthread_create(&pool->pthreads[i],NULL,work,pool);
        ERROR_HEAD_CHECK(ret,"pthread_create");
        //分离线程
        ret = pthread_detach(pool->pthreads[i]);
        ERROR_HEAD_CHECK(ret,"pthread_create");
    }
    //printf("threadpoolStart success \n");
    return 0;


}

int threadpoolStop(threadpool_t *pool){

    if(pool == NULL){
        return 0;
    }

    for(int i = 0 ;i < pool->pthreadNum;i++){
        taskEnque(&pool->que,-1);
    }
    //printf("STOP success \n");

   return 0;
}












