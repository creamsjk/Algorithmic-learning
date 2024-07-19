/*************************************************************************
    > File Name: main.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月19日 星期五 14时18分44秒
 ************************************************************************/

#include<iostream>
#include"./log/block_queue.h"
#include"./log/log.h"
#include"./lock/locker.h"

using namespace std;

int main(){


     Log::get_instance()->init("ServerLog", 2000, 800000, 0);
      LOG_ERROR("%s", "epoll failure");
      LOG_INFO("%s", "epoll info");
      LOG_DEBUG("%s", "debuge");
      LOG_WARN("%s", "warn");
    return 0;
}

