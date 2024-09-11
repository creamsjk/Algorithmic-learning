/*************************************************************************
    > File Name: lecode3151.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月13日 星期二 09时18分33秒
    方法:
           使用位运算符来判断是不是特殊数组

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

    vector<int> nums = {4, 3, 1, 6};

    if(nums.size() <= 1){
        printf("good \n");
        return true;
    }

    for(int j=1; j<nums.size(); ++j){

        if((nums[j] & 1) == (nums[j -  1] & 1)){

            printf("false \n");
            return false;
        }
    }

    printf("goood \n");
    return 0;
}
