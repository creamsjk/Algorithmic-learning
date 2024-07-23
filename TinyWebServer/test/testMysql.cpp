/*************************************************************************
    > File Name: testMysql.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月23日 星期二 10时32分11秒
    测试cql_connection_pool.h 这个头文件即实现是否出现问题
 ************************************************************************/

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>

#include"./CGlmysql/sql_connection_pool.h"
#include"./log/log.h"
#include"./lock/locker.h"
using namespace  std;
int main(){

    
    connection_pool::GetInstance()->init("118.25.193.224","root","104962dec5f28b2e","sjk",3306,1,0);
    MYSQL* qqq = connection_pool::GetInstance()->GetConnection(); 
    char buff[] = "select * from qqqqq;";
    mysql_query(qqq,buff);

    MYSQL_RES * result = mysql_store_result(qqq);
    int field = mysql_num_rows(result);
    cout<<field<<endl;

    //打印字段名
    MYSQL_FIELD *f;
    while(f = mysql_fetch_field(result)){

        cout<<f->name<<endl;
    }
    //获取每一条记录 他将所有都返回集都放在了,r里
    MYSQL_ROW r;
    while(r = mysql_fetch_row(result)){
        for(int i=0;i<field;i++){
            cout<<r[i]<<"  ";
            break;
        }
            cout<<endl;
    }

   // printf("the result is : %s \n",buff);

   connection_pool::GetInstance()->ReleaseConnectoin(qqq);
   connection_pool::GetInstance()->DestroyPool();
   printf("end \n");


    return 0;
}
