/*************************************************************************
  > File Name: lecode712.cpp
  > Author: sunrie
  > Mail: 1102490208@qq.com 
  > Created Time: 2024年09月25日 星期三 22时46分26秒

  dp
  顺利通过 yes  自定向下的dp 
  时间复杂度 O(M * N)
  空间复杂度 O(M * N)
  
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
//备忘录
vector<vector<int>> memo;
int dp(const string& s1, int i, const string& s2, int j) {

    //如果其中一个到末尾了
    if (i == (int)s1.size()) {

        int res = 0;
        for (size_t k = j; k < s2.size(); ++k) {

            res += (int)s2[k];
        }
        return res;

    } else if (j == (int)s2.size()) {

        int res = 0;
        for (size_t k = i; k < s1.size(); ++k) {

            res += (int)s1[k];
        }
        return res;
    }

    //如果走过这条路
    if (memo[i][j] != -1)
        return memo[i][j];

    //相同 往下走
    if (s1[i] == s2[j]) {

        memo[i][j] = dp(s1, i + 1, s2, j + 1);
    } else {
        //不同取最小的
        memo[i][j] = min((int)s1[i] + dp(s1, i + 1, s2, j),
                         (int)s2[j] + dp(s1, i, s2, j + 1));
    }

    //返回结果
    return memo[i][j];
}

int minimumDeleteSum(string s1, string s2) {

    memo = vector<vector<int>>(s1.size(), vector<int>(s2.size(), -1));
    return dp(s1, 0, s2, 0);
}


void test(){

    string s1 = "delete";
    string s2 = "leet";

    cout << minimumDeleteSum(s1, s2) << endl;

}

int main(){
    test();
    return 0;
}
