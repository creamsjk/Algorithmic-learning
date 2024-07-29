/*************************************************************************
    > File Name: main.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月29日 星期一 14时59分09秒
      使用一个父进程给子进程发送信息停止进程池
      异步拉起同步的方法!!
 ************************************************************************/

#include"./tool.h"
#include"./threadpool.h"

int expectfd[2];

void sigFunc(int signal){
    printf("all pthread will stop \n");
    write(expectfd[1],"1",1);

}

int main(int argc, char *argv[]){
 
    if(argc != 4){
        printf("Usage: %s <IP>  <PORT>  NUM  \n",argv[0]);
        exit(1);
    }
    pipe(expectfd);
   
    pid_t pid = fork();

    if(pid == 0){
        signal(SIGUSR1,SIG_IGN);
        close(expectfd[1]);


        //创建并初始化线程池
        threadpool_t *pool = (threadpool_t*)malloc(sizeof(threadpool_t));
        ERROR_NULL_CHECK(pool,"pool malloc failed ");

        threadpoolInit(pool,atoi(argv[3]));
        threadpoolStart(pool);

        int listenfd = tcpInit(argv[1],argv[2]); 

        int epfd = epoll_create(10);
        epollReadAdd(epfd,listenfd);
        epollReadAdd(epfd,expectfd[0]);

        int num = atoi(argv[3]);
        struct epoll_event *ev = (struct epoll_event*)malloc(sizeof(struct epoll_event) * num);
        int nready = 0;
        int ret = 0;
        int re = 0;
        while(1){
            nready = epoll_wait(epfd,ev,10,-1);
            for(int i=0;i<num;i++){
                if(ev[i].data.fd == listenfd){
                    struct sockaddr_in clint_addr;
                    socklen_t cline_len;
                    ret = accept(ev[i].data.fd,(struct sockaddr*)&clint_addr,&cline_len);
                    printf("%s %d \n",inet_ntoa(clint_addr.sin_addr),clint_addr.sin_port);
                    //将客户端放到阻塞队列里面 
                    taskEnque(&pool->que,ret);

                }else if (ev[i].data.fd == expectfd[0]){
                    int howmoney;
                    read(expectfd[0],&howmoney,1);
                    threadpoolStop(pool);
                    threadpoolDestory(pool);
                    printf("wait end ... \n");
                    close(listenfd);
                    close(expectfd[1]);

                    re = 1;
                    break;
                }

            }
            if(re == 1){
                free(ev);
                ev = NULL;
                free(pool);
                pool = NULL;
                printf("end ....\n");
                break;
            }
        }

    }
    else{

            close(expectfd[0]);
            signal(SIGUSR1,sigFunc);
            //等待子进程退出 
            wait(NULL);
            close(expectfd[1]);
            exit(0);
    }


    return 0;
}
