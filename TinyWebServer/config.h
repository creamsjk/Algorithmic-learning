/*************************************************************************
    > File Name: config.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月02日 星期五 20时06分19秒
 ************************************************************************/

#ifndef  __CONFIG_H
#define  __CONGIG_H

#include"webserver.h"

using namespace std;

class Config{

public:
    Config();
    ~Config(){};

    void parse_arg(int argc, char *argv[]);

    //端口号
    int PORT;

    //日志写入方式
    int LOGWrite;

    //触发组合模式
    int TRIGMode;

    //listen触发模式
    int LISTENTrigmode;

    //connfd触发模式
    int CONNTrigmode;

    //优雅关闭链接
    int OPT_LINGER;

    //数据库连接池数量
    int sql_num;

    //线程池内线程数量
    int thread_num;

    //是否关闭日志
    int close_log;

    //并发模型选择
    int actor_model;
};

#endif

