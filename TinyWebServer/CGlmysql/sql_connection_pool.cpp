/*************************************************************************
    > File Name: sql_connection_pool.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月22日 星期一 16时11分29秒
 ************************************************************************/

#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

connection_pool::connection_pool(){
    m_MaxConn = 0;
    m_FreeConn=0;
}

//单例初始化
connection_pool* connection_pool::GetInstance(){
    static connection_pool con;
    return &con;
}

//初始化连接
void connection_pool::init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log ){

    m_url = url;
    m_Port = Port;
    m_User = User;
    m_PassWord = PassWord;
    m_DatabaseName = DataBaseName;
    m_close_log = close_log;
    

    //初始化连接数
    for(int i=0;i<MaxConn;i++){
        MYSQL* con = NULL;
        con = mysql_init(con);
        if(con == NULL){
            LOG_ERROR("con init failed ");
            exit(1);
        }
        
        con = mysql_real_connect(con,url.c_str(),User.c_str(),PassWord.c_str(),DataBaseName.c_str(),Port,NULL,0);
        if(con == NULL){
            LOG_ERROR("con mysql_real_connect  failed ");
            exit(1);
        }
   
        //将初始化好的连接加入list
       connList.push_back(con);
       ++m_FreeConn;
    }
    //初始化信号量 信号量不为0就说明还有连接
    reserve = sem(m_FreeConn);
    //初始化最大值
    m_MaxConn = m_FreeConn;
}
//获得连接
MYSQL*  connection_pool::GetConnection(){
    MYSQL* con=NULL;
    if(0==connList.size())
        return NULL;

    //如果现在锁着 就wait
    reserve.wait();

    lock.lock();
    con = connList.front();
    connList.pop_front();
    --m_FreeConn;
    ++m_CurConn;
    lock.unlock();

    return con;

}

//释放连接 使用完毕放回list
bool connection_pool::ReleaseConnectoin(MYSQL *conn){
    if(conn == NULL)
        return false;
    //上锁
    lock.lock();

    connList.push_back(conn);
    ++ m_FreeConn;
    --m_CurConn;

    lock.unlock();

    //操作完成post
    reserve.post(); 
    return true;

}

int connection_pool::GetFreeConn(){
    return this->m_FreeConn;
}

//销毁所有线程池的连接
void connection_pool::DestroyPool(){
    lock.lock();
	if (connList.size() > 0)
	{
        //一个一个释放连接
		list<MYSQL *>::iterator it;
		for (it = connList.begin(); it != connList.end(); ++it)
		{
			MYSQL *con = *it;
			mysql_close(con);
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connList.clear();
	}

	lock.unlock();   
}

connection_pool::~connection_pool(){
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool){
	*SQL = connPool->GetConnection();

	conRAII = *SQL;
	poolRAII = connPool;
}

connectionRAII::~connectionRAII(){
	poolRAII->ReleaseConnectoin(conRAII);
}







