#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>

//没有实现 我的想法有一点问题 failed

using namespace std;


int main(){

    vector<int> weights = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int days = 5;
    int n = weights.size(); 
    int sum = 0;
    for(int i:weights)
        sum += i;
    int res = sum/days;
    int start = res;
    int tmp = 0;
    for(int i=0;i<weights.size();i++){
      
      int x = tmp +weights[i];

        cout<<"tmp == "<<tmp<<"  tmp+weights[i] == "<<tmp+weights[i]<<endl;
        cout<< "res == "<<res<<"  start== "<<start<<endl;
       if(tmp+weights[i] == start){
           tmp=0;
           days--;
       }
       else if(days >= n-i-1){
           break;
       }
       else if(tmp+weights[i] > start && tmp+weights[i] < res){
          tmp = 0;
          days--;
       }
       else if(tmp + weights[i] > start && tmp+weights[i] > res){
           res = max(tmp+weights[i],res);
           tmp=0;
           days--;
       }
       else{
           tmp += weights[i];
       }

       if(tmp == 0 && days >= 1){
           sum -= x;
           start = sum/days;
       }

    }
   cout<<res<<endl;

    return 0;
}
