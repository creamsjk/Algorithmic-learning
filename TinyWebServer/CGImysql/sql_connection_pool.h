/*************************************************************************
    > File Name: sql_connection_pool.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月22日 星期一 15时28分23秒
 ************************************************************************/

#ifndef __CONNECTION_POOL_
#define __CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "../lock/locker.h"
#include "../log/log.h"

using namespace std;



class connection_pool{

public:
    MYSQL* GetConnection();                //获取数据库连接
    bool ReleaseConnectoin(MYSQL* conn);  //释放连接
    int GetFreeConn();                     //获取空闲连接数量
    void DestroyPool();                    //销毁所有连接

    //单例模式 懒汉模式
    static connection_pool* GetInstance();

    //初始化数据库连接
    void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log);

private:
    connection_pool();
    ~connection_pool();

    int m_MaxConn;        //最大连接数
    int m_CurConn;        //当前已使用的连接数
    int m_FreeConn;       //当前空闲的连接数
    locker lock;           
    list<MYSQL *> connList;  //连接池
    sem reserve;

public:
    string m_url;
    string m_Port;
    string m_User;
    string m_PassWord;
    string m_DatabaseName;
    int m_close_log;
};

class connectionRAII{

public:
    connectionRAII(MYSQL **con, connection_pool *connPool);
    ~connectionRAII();

private:
    MYSQL* conRAII;
    connection_pool *poolRAII;
};



#endif
