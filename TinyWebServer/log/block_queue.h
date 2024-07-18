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

    bool full(){
        m_mutex.lock();
        bool result =  false;
        if(m_size >= m_max_size )
            result = true;
        m_mutex.unlock();
        return result;
    }

    bool empty(){
        m_mutex.lock();
        bool result = false;
        if(m_size == 0)
            result = true;
        m_mutex.unlock();
        return result;
    }
   
    //返回队首元素
    bool front(T &value){
        m_mutex.lock();
        if(m_size == 0)
            return false;
        *value = m_array[m_front];
        m_mutex.unlock();
        return false;
    }

    //返回队尾元素
    bool back(T &value){
       m_mutex.lock();
       if(m_size == 0)
           return false;
       *value = m_array[m_back];
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

};


#endif
