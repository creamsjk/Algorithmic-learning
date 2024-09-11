/*************************************************************************
    > File Name: lecode354.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月10日 星期二 11时11分08秒
    方法:
         动态规划

         超时!!!!!!!!
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

int maxEnvelopes(vector<vector<int>>& envelopes) {

    int result = 1;
    sort(envelopes.begin(), envelopes.end());
    vector<int> dp(envelopes.size(), 0);
    dp[0] = 1;

    for(size_t i = 1; i < dp.size(); ++i){
        int tmp = 1;
        for(size_t j = 0; j < i; ++j ){
            if(envelopes[i][0] > envelopes[j][0] && envelopes[i][1] > envelopes[j][1]){

                tmp = max(tmp, dp[j] + 1);
                continue;
            }
        }
        dp[i] = tmp;
        if(dp[i] > result)
            result = dp[i];
    }

    /* for(auto &i : dp) */
    /*     cout << i << " "; */
    /* cout << endl; */
            


    /* for(auto &i: envelopes){ */
    /*     for(auto &j : i) */
    /*         cout <<  j << " " ; */
    /*     cout << endl; */
    /* } */

    return result;
}


void test(){
    vector<vector<int>> envelopes = {
        {
            5, 4
        },
        {
            6, 4
        },
        {
            6, 7
        },
        {
            2, 3
        }
    };
    cout << maxEnvelopes(envelopes) << endl;

}

int main(){
    test();
    return 0;
}
