/*************************************************************************
    > File Name: sql_test.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月23日 星期二 14时23分05秒
 ************************************************************************/

#include<iostream>
#include <mysql/mysql.h>
#include <stdio.h>
#include <iomanip>


using namespace std;

int main(){
    MYSQL *qqq; 

    qqq = mysql_init(NULL);
  qqq = mysql_real_connect(qqq, "118.25.193.224","root","104962dec5f28b2e","sjk",3306,NULL,0); 
  //  qqq = mysql_real_connect(qqq, "localhost","root","root","yourdb",3306,NULL,0); 

     if(qqq == NULL){
         printf("failed  %s\n",mysql_error(qqq));
         return 0;
     }
   
     char buff[] = "select * from user;";
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
 
   mysql_close(qqq);
     
     
     


    return 0;
}
