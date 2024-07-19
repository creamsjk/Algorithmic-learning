/*************************************************************************
    > File Name: time_test.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月19日 星期五 11时41分50秒
 ************************************************************************/

#include<iostream>
#include<time.h>
#include<sys/time.h>

using namespace std;


int main(){

    struct timeval now = {0,0};
    gettimeofday(&now,NULL);

    cout<<now.tv_sec<<endl;
    time_t t = now.tv_sec;
    cout<<t<<endl;
    struct tm *sys_tm = localtime(&t);
    cout<<"hour: "<< sys_tm->tm_hour<<endl;
    cout<<"day: "<< sys_tm->tm_mday<<endl;
    cout<<"years: "<< sys_tm->tm_year<<endl;
    cout<<"sec: "<< sys_tm->tm_sec<<endl;
    
    cout<<"mon: "<< sys_tm->tm_mon<<endl;

    return 0;
}

