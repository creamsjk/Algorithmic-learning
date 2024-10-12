/*************************************************************************
    > File Name: lecode1143.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月25日 星期三 09时22分15秒

    dp
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

int dp(string &s1, int i, string &s2, int j){ 
    
    if(i == (int)s1.size() || j == (int)s2.size())
        return 0;

    if(memo[i][j] != -1)
        return memo[i][j];

    int res;
    if(s1[i] == s2[j]){ 

        res = 1  + dp(s1, i + 1, s2, j + 1);
    }else{ 

       res = max(dp(s1, i + 1, s2, j), dp(s1, i, s2, j + 1)); 
    }

    memo[i][j] = res;

    return res;


}

int longestCommonSubsequence(string text1, string text2) {

    
    memo = vector<vector<int>>(text1.size(), vector<int>(text2.size(), -1));
    return dp(text1 , 0, text2, 0);
}


void test(){

    string s1 = "abcde";
    string s2 = "ace";


    cout << longestCommonSubsequence(s1, s2) << endl;

}

int main(){
    test();
    return 0;
}
