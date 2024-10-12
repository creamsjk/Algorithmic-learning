/*************************************************************************
    > File Name: lecode516.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月26日 星期四 21时44分09秒

    自顶向下的动态规划
    nice  轻轻松松 自顶向下的动态规划
    也可能是做的太多了
    时间复杂度O(M N)
    空间复杂度O(M N)
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
int dp(const string & s, int i, int j){ 

    if(i == j)
        return 1;
    else if (i > j)
        return 0;

    if(memo[i][j] != -1)
        return memo[i][j];

    if(s[i] == s[j]){ 

        memo[i][j] = dp(s, i + 1, j - 1) + 2;
    }else{ 

        memo[i][j] = max( dp(s, i + 1, j), dp(s, i, j - 1));

    }

    return memo[i][j];
}


int longestPalindromeSubseq(string s) {

    memo = vector<vector<int>>(s.size(), vector<int>(s.size(), -1));
    return dp(s, 0, s.size() -  1);

}

void test(){

    /* string s = "bbbab"; */
    string s = "cbbd";

    cout << longestPalindromeSubseq(s) << endl;

}

int main(){
    test();
    return 0;
}
