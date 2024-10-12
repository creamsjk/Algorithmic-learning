/*************************************************************************
    > File Name: lecode2414.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月19日 星期四 09时12分29秒

    没什么方法
    遍历一遍  时间复杂度 O(N)
                 空间复杂度O(1):
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

int longestContinuousSubstring(string s) {

    int result = 0;
    int sz = 0;
    char e;

    for(auto &i : s){ 

        if(sz != 0 && i != e + 1 ){ 

            result = max(result, sz);
            sz = 0;
        }
        sz++;
        e = i;
    }
    result = max(result, sz);

    return result;
        
        
}


void test(){

    /* string s = "abacaba"; */
    string s = "awy";
    cout << longestContinuousSubstring(s) << endl;
}

int main(){
    test();
    return 0;
}
