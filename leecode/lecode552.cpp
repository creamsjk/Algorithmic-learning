/*************************************************************************
    > File Name: lecode552.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月19日 星期一 10时01分00秒
    方法:
         使用回溯算法来解决问题
         失败 超时!!!

    方法二: 
            使用dp 
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

long long  result = 0;
vector<string> s = {
    "A","L","P"
};


void pathtrack(vector<string> &nums, int n, int A_size){

    if(A_size >= 2)
         return ;

    if(n <=0  ){
        
        result++;
      //  for(auto i: nums)
      //      cout << i <<" ";
      //  cout << endl;
      //  cout<< result <<endl;
        return ;
    }
    
    for(int i=0; i<3; i++){

        
        if(s[i] == "A")
            A_size++;
        else if(s[i] == "L" && nums.size() >= 2){
            int len = nums.size();
            if(nums[len -1] == "L" && nums[len -2 ] == "L")
                continue;
        }
      
        nums.push_back(s[i]);

        pathtrack(nums, n-1, A_size);


        nums.pop_back();

        if(s[i] == "A")
            A_size--;
        
    }

    return ;
     

}




int main(){

    int n = 4;
    vector<string> nums;

    pathtrack(nums, n, 0);
    
  //  int a = 4;
  //  int ll=1;
  //  int e = 3;
  //  n -= 2;
  //  result = a + ll + e;
  //  while( n > 0){

  //      a = a * 2;
  //      ll = ll *2;
  //      e = e * 3;
  //  }

  //  result = a + ll + e;
    cout<< result%(1000000000 + 7) << endl;

    return 0;
}
