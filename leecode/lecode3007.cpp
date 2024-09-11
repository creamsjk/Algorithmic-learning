/*************************************************************************
    > File Name: lecode3007.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月21日 星期三 10时23分00秒
    方法 
        右移与1 &
        使用一个常数记录上一次val增加 是的数next


        O(N)
        O(1)
        都不行吗!! 这都超时 我服了  没有通过
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

    long long k = 32785;
    const int x = 5;

    int num = 0;
    int result = 0;
    long long next = 1<<(x-1);
    int val = 1;
    if(x == 1)
        next = 1;
    for(long long i = 1; i > 0; i++){

        long long tmp = i;

        if(x > 1){
            tmp >>= (x-1);

        }

        //cout << "tmp: "<< tmp <<endl;
       int this_num = 0;
       while(tmp >0){

           int l = tmp <<(x-1);
           
          if(x > 1 && l >= next && l /next <2 ){
            //  cout << "l= "<< l << "i= " << i << endl;
              this_num += val;
              break;
         }


           if((tmp & 1) != 0)
               this_num++;

           tmp >>= x;

       }

       if(x > 1 && i == (1<<x) *  next + (1<<(x-1)) ){
           next = i;
           val++;
       } 
       
           num += this_num;
    

      //  cout << i << "  "<< num << endl;

      //  if(i == 10)
      //      break;
        
        if(num <= k)
            result = i;
        else
            break;
    }

  //  for(auto i:index){
  //       cout<<"key: "<< i.first << "Value: "<< i.second << endl;
  //  }
     
    cout << result << endl;
    return 0;
}
