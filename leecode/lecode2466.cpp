/*************************************************************************
    > File Name: lecode2466.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月19日 星期四 14时05分31秒

    动态规划

    通过 !!!
    时间复杂度 O(N)
    空间复杂度 O(N)
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

long long  result = 0;

void dp(int low, int high, int zero, int one, string s){ 

    /* cout << s << endl; */

    if((int)s.size() >= low &&(int) s.size() <= high){ 
         result++;
    }
    if((int)s.size() > high)
        return ;
    //add 0
    string a(zero, '0');
    dp(low, high, zero, one, s + a);

    // add 1
    string b(one, '1');
    dp(low, high, zero, one, s + b);


}



int countGoodStrings(int low, int high, int zero, int one) {

   /* string s; */
   /* dp(low, high, zero, one, s); */
  vector<long long> dp(high + 1, 0);
  dp[0] = 1;

  for(size_t i = 0; i < dp.size(); ++i){ 

      if((int)i >= zero)
          dp[i] += dp[i - zero];

      if((int) i >= one)
          dp[i] += dp[i - one];

      dp[i] %=(1000000000 + 7);

  }

  /* for(auto &i : dp) */
  /*     cout << i << " "; */
  /* cout << endl; */

  for(int i = low; i <= high; ++i)
      result += dp[i];

  return result%(1000000000 + 7);

}


void test(){
    int low = 500;
    int high = 500;
    int zero = 5;
    int one = 2;

    cout << countGoodStrings(low, high, zero, one) << endl;

}

int main(){
    test();
    return 0;
}
