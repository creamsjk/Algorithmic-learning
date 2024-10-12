/*************************************************************************
    > File Name: lecode583.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月25日 星期三 22时23分01秒

    dp
    方法一  通过!!!
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

int dp(const string &s1, int i, const string &s2, int j){ 

    if(i ==(int) s1.size() || j ==(int) s2.size()){ 

        return max((int)s1.size()- i, (int)s2.size() - j)  ;
    }

    if(memo[i][j] != -1)
        return memo[i][j];

    if(s1[i] == s2[j]){ 

        memo[i][j] =   dp(s1, i + 1, s2, j + 1);
    }else{ 

        memo[i][j] = min(1 + dp(s1, i + 1, s2, j), 1 + dp(s1, i, s2, j + 1));

    }

    return memo[i][j];

}

int minDistance(string word1, string word2) {


    memo = vector<vector<int>>(word1.size(), vector<int>(word2.size(), -1));
    return dp(word1, 0, word2, 0);

    //方法二
    //将memo 当作dp数组 表示 不会哈哈哈
    /* memo = vector<vector<int>>(word1.size(), vector<int>(word2.size(), -1)); */





}



void test(){
    string word1 = "leetcode";
    string word2 = "etco";

    cout << minDistance(word1, word2) << endl;

}

int main(){
    test();
    return 0;
}
