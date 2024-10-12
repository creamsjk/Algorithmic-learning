/*************************************************************************
    > File Name: lecode2848.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月15日 星期日 09时33分09秒

    1.双重for循环暴力解决
    2.差分数组

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

void display(vector<int> nums){ 

    for(auto &i:nums)
        cout << i << " ";
    cout << endl;

}


int numberOfPoints(vector<vector<int>>& nums) {
    /* 暴力
    unordered_set<int> s;
    for(auto &i : nums){ 

        for(int j = i[0]; j <= i[1]; ++j){ 

            s.insert(j);
        }

    }

    return s.size();
    */

    //差分  和前缀和是逆运算

    int max_end = 0;
    for(auto &i : nums)
        if(i[1] > max_end)
         max_end = i[1];

    vector<int> a(max_end + 2, 0);

    for(auto &i: nums){ 

        a[i[0] ]++;
        a[i[1] + 1]--;
        display(a);
    }
    int ans = 0, s = 0;
    for(int d: a){ 
        s += d;
        ans += s > 0;
    }


    return ans;




}


void test(){

    vector<vector<int>> nums = { 
      { 3,6},
      { 1, 5},
      { 4, 7}


    };

    cout << numberOfPoints(nums) << endl;

}

int main(){
    test();
    return 0;
}
