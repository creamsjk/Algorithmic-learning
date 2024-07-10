/*************************************************************************
    > File Name: test1.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月02日 星期二 15时46分45秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>

using namespace std;

bool is_num(int x){
    for(int i=2;i<=sqrt(x);i++){
          if(x%i == 0)
              return false;
    }

    return true;
}

int main(){
    vector<int> nums = {4,8,2,8 };
    int left =0;
    int right = nums.size()-1;
    int len = 0;
    while(left < right){
        while(!is_num(nums[left]) && left <=right){
            left++;
        }
        while(!is_num(nums[right]) && left <= right){
            right--;
        }
        len = max(len,right - left);
        cout<<left<<endl;
        cout<<right<<endl;
        break;
    }
     cout<<len<<endl;
     return 0;
}
