/*************************************************************************
    > File Name: lecode2708.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月03日 星期二 09时17分51秒
    通过!!!
 ************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <vector>
#include <stack>
#include <math.h>
#include <climits>


using namespace std;

long long maxStrength(vector<int> &nums){
     long long result = 1;
    sort(nums.rbegin(), nums.rend());

     int len = 0;
     for(auto &i : nums){
        if(i < 0)
          len++;
     }


    if(nums[0] == 0 && len == 1)
        return 0;
    else if (nums.size() == 1)
        return nums[0];
    else if(nums[0] == 0 && len == 0)
        return 0;



     len = 0;
    for(auto &i : nums){
        if(i <= 0)
            break;
        result *= i;
        ++len;
    }

    vector<int> tmp;
     for(auto &i : nums){
        if(i < 0)
            tmp.push_back(i);
    }

    sort(tmp.begin(), tmp.end());


    int x = tmp.size()/2;
    for(int i = 0;  i < tmp.size() && i <= 2 * x - 1; ++i ){
        result *= tmp[i];
    }

    return result;
}

void test(){

    /* vector<int> nums = { 3, -1, -5, 2, 5 , -9}; */
    vector<int> nums = { 0,-1};

    cout <<  maxStrength(nums) << endl;

}

int main(){
    test();
    return 0;
}
