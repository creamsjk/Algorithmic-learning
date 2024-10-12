/*************************************************************************
    > File Name: lecode2516.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月27日 星期五 09时14分20秒

    自顶向下的动态规划
    暴力失败 时间复杂度超过了
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

/* map<char, int> index; */

vector<vector<int>> memo;

int dp(const string & s, int i, int j, int a, int b, int  c, map<char, int> index){ 

    if(a <= 0 && b <= 0 && c <= 0)
        return 0;

    if(memo[i][j] != -1)
        return memo[i][j];

    map<char, int> tmp1 = index;
    tmp1[s[i]]--;
    
    map<char, int> tmp2 = index;
    tmp2[s[j]]--;

    memo[i][j] = min(1 + dp(s, i + 1, j,tmp1['a'], tmp1['b'], tmp1['c'], tmp1)  ,  1 + dp(s, i, j - 1,tmp2['a'], tmp2['b'], tmp2['c'], tmp2)); 


    return memo[i][j];




}


int takeCharacters(string s, int k) {

    map<char, int> index;
    index['a'] = k;
    index['b'] = k;
    index['c'] = k;

    int a = k;
    int b = k;
    int c = k;
    for(auto &i: s){ 

        if(i == 'a')
            a--;
        else if(i == 'b')
            b--;
        else
            c--;
    }

    if(a >0 || b >0 || c > 0)
        return -1;




    memo = vector<vector<int>>(s.size(), vector<int>(s.size(), -1));
    return dp(s, 0, s.size() - 1, k, k, k, index);
}


void test(){
    
    string s = "aabaaaacaabc";
    int k = 2;
    /* string s = "a"; */
    /* int k = 1; */
    
    cout << takeCharacters(s, k) << endl;

}

int main(){
    test();
    return 0;
}
