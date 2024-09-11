/*************************************************************************
    > File Name: lecode931.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月10日 星期二 22时23分24秒
    方法:
        动态规划
        本最小值 =  min(上一行(中, 左, 右)) + 本值

        accept  2024年 09月 10日 星期二 22:39:24 CST

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


int minFallingPathSum(vector<vector<int>>& matrix) {

    int result = INT_MAX;
    vector<int> dp(matrix[0].begin(), matrix[0].end());

    for(size_t i = 1; i < matrix.size(); ++i){

        vector<int> tmp = dp;
        for(size_t j = 0; j < matrix.size(); ++j){
           
            if(j == 0){
                dp[j] = min(tmp[j], tmp[j + 1]) + matrix[i][j];
            }else if( j == matrix.size() - 1){
                dp[j] = min(tmp[j], tmp[j - 1]) + matrix[i][j];
            }else{
                dp[j] = min(tmp[j - 1], min(tmp[j], tmp[j + 1])) + matrix[i][j];
            }
            
        }
    }

    for(auto &i : dp)
        if(i < result)
            result = i;

    return result;

}


void test(){

    vector<vector<int>> matrix = {
        /* { */
        /*     2, 1, 3 */
        /* }, */
        {
            -19, 57
        },
        {
            -40, -5
        }
    };

    cout << minFallingPathSum(matrix) << endl;

}

int main(){
    test();
    return 0;
}
