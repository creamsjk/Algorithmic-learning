/*************************************************************************
    > File Name: test.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月25日 星期四 20时22分55秒
 ************************************************************************/

#include"tool.h"
int pipefd[2];
void sigFunc(int signum){
    printf("signum = %d\n",signum);
    write(pipefd[1],"1",1);
}

int main(int argc, char* argv[]){
    
  //校验参数
  if(argc != 4){
      printf("Usages: %s <ip>  <port>  <number> \n",argv[0]);
      exit(1);
  }
  int number = atoi(argv[3]);
  //忽略管道信号
  //  signal(SIGPIPE,SIG_IGN);
  process_child *child = (process_child*)malloc(sizeof(process_child)*number);
  //创建子进程数组
  createChild(number,child);
  signal(SIGUSR1,sigFunc);
 
  //初始化tcp连接 监听10个
  int listenfd = tcpInit(argv[1],argv[2]);
  //创建epfd
  int epfd = epoll_create(10); 
  ERROR_CHECK(epfd,-1,"epoll_reate failed ");
  struct epoll_event * ev = (struct epoll_event*)malloc(sizeof(struct epoll_event)*10);
  if(ev == NULL){
      printf("epoll mallco failed \n");
      exit(1);
  }
  memset(ev,0,sizeof(struct epoll_event)*10);
  //监听父进程fd 用来处理程序
  epollReadAdd(epfd,listenfd);  
  printf("listenfd = %d \n",listenfd);
 //监听父子进程之间管道 通信
  for(int i=0;i<number;i++){
      epollReadAdd(epfd,child[i].pipfd);
  }
  //监听退出管道
  pipe(pipefd);
  epollReadAdd(epfd,pipefd[0]);


  while(1){
      //等待事件发生
     int res_len =  epoll_wait(epfd,ev,10,0);

     for(int i=0;i<res_len;i++){
         if(ev[i].data.fd == listenfd){//来新的客户了给进程分配任务
             struct sockaddr_in clint_addr;
             socklen_t clint_len;
             int client = accept(ev[i].data.fd,(struct sockaddr*)&clint_addr,&clint_len);
             printf("new client : %d \n",client);
             //遍历找一个空闲的进程 0==BUSY 给他发送任务 
             for(int j=0;j<number;j++){
                 if(child[j].status == 1){
                     child[j].status = BUSY;
                     //给那个子进程 clien代表的文件描述符
                     sendFd(child[j].pipfd,client);
                   //  child[j].status = FREE;
                     break;
                 }

             }
             //主进程中关闭这个文件描述符
             close(client); 
         }else if(ev[i].data.fd == pipefd[0]){//推出进程池
             //int howmany =0;
             // read(ev[i].data.fd, &howmany, sizeof(howmany));

             for(int j=0;j<number;j++){
                 kill(child[j].pid,SIGINT);
                 puts("send signal to worker!");
             }

             for(int j=0;j<number;j++){
                 wait(NULL);
             }
             printf("Parent process exit! \n");
             exit(0);

         }
         else{//管道发生事件 子进程操作完毕了
             //int howmany =0;
             //read(ev[i].data.fd, &howmany, sizeof(howmany)); 
             for(int j=0;j<number;j++){
                 if(ev[i].data.fd == child[j].pipfd){
                     printf("child is not busy : %d \n",child[j].pid);
                     child[j].status = FREE;
                     break;
                 }
             }

         }
     }
  }


        
 free(ev);
 free(child);
 close(listenfd);
 close(epfd);


    return 0;
}
