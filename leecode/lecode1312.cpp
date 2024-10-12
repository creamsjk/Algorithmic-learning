/*************************************************************************
    > File Name: lecode1312.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月26日 星期四 14时16分34秒

    自顶向下的动态规划
    自顶向下的动态规划
    时间复杂度  O(N * N)
    空间复杂度 O(N*N);
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

vector<vector<int>> memo;
int dp(const string &s , int i, int j){ 

    // 结束条件
    if( i >= j)
        return 0;

    //如果能在备忘录找到
    if(memo[i][j] != -1)
        return memo[i][j];

    //相等  往下走
    if(s[i] == s[j]){ 

       memo[i][j] =  dp(s, i + 1, j - 1);
    }else{ 

       //在左边插入  和  右边插入的最小值
       memo[i][j] = min( 1 + dp(s, i + 1, j), 1 + dp(s, i, j - 1));
    }

    return memo[i][j];

}

int minInsertions(string s) {

    memo = vector<vector<int>>(s.size(), vector<int>(s.size(), -1));
    return dp(s, 0, s.size() - 1);

}


void test(){

    /* string s = "mbadm"; */
    string s = "leetcode";

    cout << minInsertions(s) << endl;

}

int main(){
    test();
    return 0;
}
