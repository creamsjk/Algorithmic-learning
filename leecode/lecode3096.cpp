/*************************************************************************
    > File Name: lecode3096.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月19日 星期五 09时18分36秒
    accept
    使用贪心算法

 ************************************************************************/

#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>


using namespace std;


int main(){
    vector<int> possible = {0,0,0};
    
    int count = 0;
    int sum = 0;
    int impossible=0;
    for(int i:possible){
        if( i == 1)
            sum += 2;
        else {
            sum -= 2;
            impossible++;
        }
    }
    cout<<sum<<endl;

     sum /= 2;
     cout<<sum<<endl;
     int cur = 0;
    for(int i:possible){
        if(i == 1){
            cur += 2;
            count++;
        }
        else{
            cur -= 2;
            count++;
        }
        if(cur > sum && count >0){
            break;
        }
    }
    
    if(count == 0 || count == possible.size() )
        count=-1;

    cout<<count<<endl;



    return 0;
}
