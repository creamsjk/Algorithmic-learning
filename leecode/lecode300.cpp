/*************************************************************************
    > File Name: lecode300.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月10日 星期二 09时41分00秒
    方法:
        使用动态规划来解决问题
        时间复杂度 O(N^2)
        空间复杂度 O(N)

        accept
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


int lengthOfLIS(vector<int>& nums) {

    int result = 1;
    vector<int> dp(nums.size(), 0);
    dp[0] = 1;
    if(nums.size() == 0)
        return 0;

    /* dp[1] = 1; */

    for(size_t i = 1; i < dp.size(); ++i){
        
        int tmp = 1;
        for(size_t  j = 0; j < i ; ++j){
            
            if(nums[i] > nums[j])
                tmp = max(tmp, dp[j] + 1);
        }
        dp[i] = tmp;
        if(dp[i] > result)
          result = dp[i];
    }
    for(auto &i:dp)
        cout << i << " ";
    cout << endl;


     return result;
}

void test(){
    /* vector<int> nums = {10,9,2,5,3,7,101,18}; */
    /* vector<int> nums = {0,1,0,3,2,3}; */
    /* vector<int> nums = {7, 7, 7, 7, 7, 7 ,7 ,7 ,7 }; */
    /* vector<int> nums = {1,3,6,7,9,4,10,5,6}; */
    vector<int> nums = {0};
    cout << lengthOfLIS(nums) << endl;

}

int main(){
    test();
    return 0;
}
