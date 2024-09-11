/*************************************************************************
    > File Name: lecode3133.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月22日 星期四 09时37分11秒
    方法:
         暴力  时间复杂度O(N)  通不过太高了

         记录可以存多少个  草
         不暴力 也超时!!!

         accept   
         时间复杂度 O(N)
         遍历n 把符合的数组以及 两个头之间距离找到
         计算 最后一个的位置   计算 即可得出来最后的结果
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
     
    int n = 48270567;
    int x = 77098793;

    long long  one_index = 1;

    long long  tmp = x;
    //计算组之间的距离
    while(tmp > 0){
        tmp >>= 1;
        one_index <<= 1;
    }
    
    long long result = x;
   vector<long long > nums;
   //将第一组符合计算出来 并放入nums
    for(long long  i=x; i< one_index; ++i){

        if((x&i) == x){
            nums.push_back(i);
        }
    }

    long long  len = nums.size();

     //如果位于第一组
     if(n <= len){
         result = nums[n-1];
     }else {
         //如果位于第二组 以及之后

         int two = 0;

         if(n%len == 1)
             two = 0;
         else if(n%len == 0){
             two = len-1;
         }
         else 
             two = n%len -1;
         
          //计算结果
         result = (n-1)/len * one_index + nums[two] ; 
        
     }
    

    cout<< result << endl;
   


    return 0;
}
