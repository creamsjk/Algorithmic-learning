/*************************************************************************
    > File Name: block_queue.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月18日 星期四 13时59分28秒
 ************************************************************************/
#ifndef __BLOCK_QUEUE_H
#define __BLOCK_QUEUE_H

#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include<sys/time.h>
#include"../lock/locker.h"

using namespace std;

template<class T> 
class block_queue{

private:

   //这两个锁创建之后调用无参构造函数就初始化了
    locker m_mutex;
    cond m_cond;

    T *m_array;
    int m_size;   //当前队列元素数量
    int m_max_size; //队列可容纳哦的元素数量
    int m_front;  //m_front 指向队首元素
    int m_back;  // m_back 指向队尾元素 下一个要插入的位置是 m_back = (m_back+1)%m_max_size;
public:
    block_queue(int max_size = 1000){
       if(max_size <= 0){
           exit(-1);
       }
      
       m_size = 0;
       m_max_size = max_size;
       m_front = -1;
       m_back  = -1;
       m_array = new T[max_size]; 
        
    }
    //清理函数
    void clear(){
        //多线程需要上锁
        m_mutex.lock();
        m_size = 0;
        m_front = -1;
        m_back = -1;
        m_mutex.unlock();
    }
  

    ~block_queue(){
        //多线程需要上锁
        m_mutex.lock();
        if(m_array != NULL){
            delete[] m_array;
            m_array = NULL;
        }
        m_mutex.unlock();
    }

    //判断队列是否满了
    bool full(){
        m_mutex.lock();
        bool result =  false;
        if(m_size >= m_max_size )
            result = true;
        m_mutex.unlock();
        return result;
    }

    //判断队列是否为空
    bool empty(){
        m_mutex.lock();
        bool result = false;
        if(m_size == 0)
            result = true;
        m_mutex.unlock();
        return result;
    }
   
    //返回队首元素 &是取别名
    bool front(T &value){
        m_mutex.lock();
        if(m_size == 0)
            return false;
        value = m_array[m_front];
        m_mutex.unlock();
        return false;
    }

    //返回队尾元素
    bool back(T &value){
       m_mutex.lock();
       if(m_size == 0)
           return false;
       value = m_array[m_back];
       m_mutex.unlock();
       return true;
    }

    //返回当前队列元素数量
    int size(){
        m_mutex.lock();
        int result = m_size;
        m_mutex.unlock();
        return result;
    }
    
    //返回当前队列可容纳最大元素数量
    int max_size(){
         m_mutex.lock();
        int result = m_max_size;
        m_mutex.unlock();
        return result;

    }

    //往队列添加元素，需要将所有使用队列的线程先唤醒
    //当有元素push进队列,相当于生产者生产了一个元素
    //若当前没有线程等待条件变量,则唤醒无意义
    bool push(T &item){
        m_mutex.lock();
        if(m_size >= m_max_size){
            //队列满了 唤醒其他被阻塞的线程(其实就是调用pop的线程) 本次任务失败 
            m_cond.broadcast();
            m_mutex.unlock();
            return false;
        }
        //先移动再添加
        m_back = (m_back + 1)%m_max_size;
        m_array[m_back] = item;
        m_size++;
        m_mutex.unlock();
        return true;

    }
    //pop时,如果当前队列没有元素,将会等待条件变量 
    bool pop(T &item){
        m_mutex.lock();

        while(m_size <= 0){
            //队列没有元素 释放锁 等待(等待被调用push的线程唤醒) 本次任务失败!!
            if(!m_cond.wait(m_mutex.get())){

                m_mutex.unlock();
                return false;
            }
        }

        //m_front 初始是-1 所以先移动在添加
        m_front = (m_front + 1 )%m_max_size;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();
        return true;
    }

    //超时pop
    bool pop(T &item, int ms_timeout){
        struct timespec t = {0, 0};
        struct timeval now = {0, 0};
        gettimeofday(&now, NULL);
        m_mutex.lock();
        if (m_size <= 0)
        {
            t.tv_sec = now.tv_sec + ms_timeout / 1000;
            t.tv_nsec = (ms_timeout % 1000) * 1000;
            //等待一段时间结束当前任务,而不是一直等待
            if (!m_cond.timewait(m_mutex.get(), t))
            {
                m_mutex.unlock();
                return false;
            }
        }

        if (m_size <= 0)
        {
            m_mutex.unlock();
            return false;
        }

        //先移动再添加
        m_front = (m_front + 1) % m_max_size;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();
        return true;
    }


};


#endif
