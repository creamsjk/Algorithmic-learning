/*************************************************************************
    > File Name: main.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月02日 星期五 20时13分27秒
        1
 ************************************************************************/

#include"config.h"

int main(int argc, char *argv[]){

    string user = "root";
    string passwd = "root";
    string databasename = "yourdb";

    //命令行解析
    Config config;
    config.parse_arg(argc, argv);

    WebServer server;

    //初始化
    server.init(config.PORT, user, passwd, databasename, config.LOGWrite, 
                config.OPT_LINGER, config.TRIGMode, config.sql_num, config.thread_num,
                config.close_log, config.actor_model);

    //日志
    server.log_write();

    //数据库
    server.sql_pool();

    //线程池
    server.thread_pool();

    //触发模式
    server.trig_mode();

    //监听
    server.eventListen();

    //运行
    server.eventLoop();


    return 0;
}

