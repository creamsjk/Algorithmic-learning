/*************************************************************************
    > File Name: threadpool.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月23日 星期二 21时44分11秒
 ************************************************************************/

#ifndef  __THREADPOOL_H
#define  __THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "../lock/locker.h"
#include "../CGImysql/sql_connection_pool.h"

template<typename  T>
class threadpool{

public:
      /*thread_number是线程池中线程的数量，max_requests是请求队列中最多允许的、等待处理的请求的数量*/
     threadpool(int actor_model, connection_pool* connPool, int thread_number = 8, int max_request = 10000);
     ~threadpool();
     bool append(T* request, int state);
     bool append_p(T* request);

private:
      /*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
    static void* worker(void* arg);
    void run();

private:
    int m_thread_number;    //线程池中线程数量
    int m_max_request;      //请求队列中允许的最大请求数
    pthread_t* m_threads;   //描述线程池的数组,大小为m_thread_number
    std::list<T*> m_workqueue;  //请求队列
    locker m_queuelocker;       //保护请求队列的互斥锁 
    sem m_queuestat;            //是否有任务需要处理
    connection_pool* m_connPool; //数据库连接
    int m_actor_model;            //模型切换

};

template<typename T>
threadpool<T>::threadpool(int actor_model, connection_pool* connPool, int thread_number , int max_request ):m_actor_model(actor_model),m_connPool(connPool),m_thread_number(thread_number),m_max_request(max_request),m_threads(NULL)
{
    //判断不为0
    if(thread_number <= 0 || max_request <= 0 ){
        throw::exception();
    }
    
    //判断不为空
    m_threads = new pthread_t[thread_number];
    if(!m_threads){
        throw  std::exception();
    }

    for(int i=0;i<thread_number;i++){
        //创建线程    
        if(pthread_create(m_threads+i,NULL,worker,this) != 0){
            delete[] m_threads;
            throw std::exception();
        }
        //分离线程
        if(pthread_detach(m_threads[i])){
            delete[] m_threads;
            throw std::exception();
        }
            
    }


    
}
template<typename T>
threadpool<T>::~threadpool(){
  //释放自己分配的资源即可
  delete[] m_threads;
}

template<typename T>
bool threadpool<T>::append(T* request, int state){
    //上锁
    m_queuelocker.lock();
    //判断队列是否满了
    if(m_workqueue.size() >= m_max_request){
        m_queuelocker.unlock();
        return false;
    }
    //加入队列
   request->m_state = state;
   m_workqueue.push_back(request);
   m_queuelocker.unlock();
   //信号量加一
   m_queuestat.post();
   
   return true;
}

template<typename T>
bool threadpool<T>::append_p(T* request){

    m_queuelocker.lock();
    if(m_workqueue.size() >= m_max_request){
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;

}

template<typename T>
void* threadpool<T>::worker(void* arg){
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;

}

//i dont no this 
template<typename T>
void threadpool<T>::run()
{
    while (true)
    {
        m_queuestat.wait();
        m_queuelocker.lock();
        if (m_workqueue.empty())
        {
            m_queuelocker.unlock();
            continue;
        }
        T *request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if (!request)
            continue;
        if (1 == m_actor_model)
        {
            if (0 == request->m_state)
            {
                if (request->read_once())
                {
                    request->improv = 1;
                    connectionRAII mysqlcon(&request->mysql, m_connPool);
                    request->process();
                }
                else
                {
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
            else
            {
                if (request->write())
                {
                    request->improv = 1;
                }
                else
                {
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
        }
        else
        {
            //从m_connPool中取出一个连接  request->msql = 取出来的那个连接
            connectionRAII mysqlcon(&request->mysql, m_connPool);
            request->process();
        }
    }
}




#endif

