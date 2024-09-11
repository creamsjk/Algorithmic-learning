/*************************************************************************
    > File Name: lecode115.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月11日 星期三 10时54分50秒
    方法:
        动态规划

        accept   2024年 09月 11日 星期三 11:23:59 CST

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

vector<vector<int>> memo;

int dp(string &s, int i, string &t, int j){

    int res = 0;
    //匹配成功
    if((size_t)j == t.size())
        return 1;

    if(s.size() - i < t.size() - j)
        return 0;

    //备忘录 减少时间复杂度
    if(memo[i][j] != -1)
        return memo[i][j];

    //状态转移方程  结果 = 本次合适 +  本次不合适
    if(s[i] == t[j])
       res = dp(s, i+1, t, j+1) + dp(s, i + 1, t, j); 
    else
        res =  dp(s, i+1, t, j);

    memo[i][j] = res;

    return res;
}


int numDistinct(string s, string t) {

    memo = vector<vector<int>> (s.size(), vector<int>(t.size(), -1));
    return dp(s, 0, t, 0);
  
}


void test(){
    string s = "rabbbit";
    string t = "rabbit";
    cout << numDistinct(s, t) << endl;

}

int main(){
    test();
    return 0;
}
