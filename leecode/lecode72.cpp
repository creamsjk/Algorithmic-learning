/*************************************************************************
    > File Name: lecode72.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月14日 星期六 20时02分28秒
    
    动态规划

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

    if(i == -1) return j + 1;
    if(j == -1) return i + 1;

    if(memo[i][j] != -1)
        return memo[i][j];

    if(s[i] == t[j]){ 
        //base 什么都不做 下一步
        
        int tmp =  dp(s, i - 1, t, j - 1);
        memo[i][j] = tmp;
        return tmp;

    }else{ 
        //添加
        int a = dp(s, i, t, j - 1) + 1;

        //替换
        int b = dp(s, i - 1, t, j - 1) + 1;

        //删除
        int c = dp(s, i - 1, t, j) + 1;

        int tmp =  min(a, min(b , c));
        memo[i][j] = tmp;
        return tmp;

    }

}

int minDistance(string word1, string word2) {

    int a = word1.size();
    int b = word2.size();
    memo = vector<vector<int>>(a + 1, vector<int>(b + 1, -1));
    
    return dp(word1, a - 1, word2, b - 1);
}


void test(){

    string word1 = "intention";
    string word2 = "execution";

    cout << minDistance(word1, word2) << endl;
}

int main(){
    test();
    return 0;
}
