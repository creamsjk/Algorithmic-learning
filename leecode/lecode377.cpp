/*************************************************************************

    > File Name: lecode377.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月18日 星期三 09时53分35秒

    dp  
    accept!!!


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

int combinationSum4(vector<int>& nums, int target) {

    vector<unsigned> dp(target + 1, 0);
    
    /* for(auto &i:dp) */
    /*     cout << i <<" "; */
    /* cout << endl; */

    dp[0] = 1;
    for(size_t i = 0; i <= dp.size(); ++i){ 

        for(int j : nums)
            if(i >= j)
                dp[i] += dp[i - j];
    }

    /* for(auto &i:dp) */
    /*     cout << i <<" "; */
    /* cout << endl; */


    return dp[target];
}


void test(){

    vector<int> nums = { 1, 2, 3};
    int target = 4;
    cout << combinationSum4(nums, target) << endl;

}

int main(){
    test();
    return 0;
}
