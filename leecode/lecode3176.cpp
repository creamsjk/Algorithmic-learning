/*************************************************************************
    > File Name: lecode3176.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月06日 星期五 09时16分47秒

    方法:
        .....没通过

        gg 看不懂题目
 ************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <math.h>
#include <climits>


using namespace std;


int maximumLength(vector<int>& nums, int k){
    int result = 0;

    if(k == 0){
        map<int, int> num_map;
        for(auto &i : nums){
            num_map[i]++;
            result = max(result, num_map[i]);
        }

        return result;
    }

    if(nums.size() <= 2 || nums.size() <= (size_t)k)
        return nums.size();


    for(size_t i = 0; i < nums.size() - 1; ++i){

        cout << nums[i] << endl;
        if(nums[i] != nums[i+1])
            result++;
        if(result > k){
           
            int l = nums.size() - i - 1; 
            result = i + min(2, l);
            if(result > nums.size())
                result = nums.size();
            
            return  result;
        }
    }
    cout << endl;
    return nums.size();



}


void test(){
    /* vector<int> nums =  {1, 2, 1, 1, 3}; */
    /* int k = 2; */
    /* vector<int> nums =  {1,2,3,4,5,1}; */
    /* int k = 0; */
    /* vector<int> nums =  {48,49,49,48,48}; */
    vector<int> nums =  {7,19,17,29};
    int k = 1;


    cout << maximumLength(nums, k) << endl;

}

int main(){
    test();
    return 0;
}
