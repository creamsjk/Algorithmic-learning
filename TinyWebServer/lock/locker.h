/*************************************************************************
    > File Name: locker.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月18日 星期四 09时39分01秒
 ************************************************************************/
#ifndef  __LOCKER_H
#define  __LOCKER_H

#include<exception>
#include<pthread.h>
#include<semaphore.h>

//封装 sem_t 的类
class sem{
 
private:
    sem_t m_sem;

public:
    sem(){
        if(sem_init(&m_sem,0,0) != 0){
           throw std::exception();
        }
    }
    sem(int num){
        if(sem_init(&m_sem,0,num) != 0){
            throw std::exception();
        }
    }
    ~sem(){
        sem_destroy(&m_sem);
    }

    bool wait(){
        return sem_wait(&m_sem);
    }

    bool post(){
        return sem_post(&m_sem);
    }

};

//封装 pthread_mutex_t 的类
class locker{

private:
    pthread_mutex_t m_mutex;
public:
    locker(){
        if(pthread_mutex_init(&m_mutex,NULL) != 0){
            throw std::exception();
        }
    }
    ~locker(){
        pthread_mutex_destroy(&m_mutex);
    }
    bool lock(){
         return pthread_mutex_lock(&m_mutex) == 0;
    }

    bool unlock(){
       return  pthread_mutex_unlock(&m_mutex) == 0;
    }

    pthread_mutex_t* get(){
        return &m_mutex;
    }

};

//封装 pthread_cond_t 的类
class cond{

private:
    //static pthread_mutex_t mutex;
    pthread_cond_t m_cond;
public:
    cond(){
        if(pthread_cond_init(&m_cond,NULL) != 0){
            throw std::exception();
        }
    }
    ~cond(){
        pthread_cond_destroy(&m_cond);
    }

    bool  wait(pthread_mutex_t *mutex){
        int res = 0;
        //pthread_mutex_lock(mutex);
        res =   pthread_cond_wait(&m_cond,mutex);
        //pthread_mutex_unlock(mutex);

        return res == 0;
    }

    bool timewait(pthread_mutex_t *mutex, struct timespec t){
        int res = 0;
        //pthread_mutex_lock(mutex);
        res = pthread_cond_timedwait(&m_cond,mutex,&t);
        //pthread_mutex_unlock(mutex);
        return res == 0;
    }

    bool signal(){
        return pthread_cond_signal(&m_cond) == 0;
    }

    bool broadcast(){
         return pthread_cond_broadcast(&m_cond) == 0;
    }

};

#endif
