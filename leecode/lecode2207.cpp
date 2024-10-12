/*************************************************************************
    > File Name: lecode2207.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月24日 星期二 09时17分02秒

    没什莫方法 按着做就行

     通过  时间复杂度O(N)
           空间复杂度O(N)
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


long long maximumSubsequenceCount(string text, string pattern) {
    long long result = 0;
    string s;
    map<char, int> index;
    map<char, int> index2;
    for(auto &i : text){ 

        if(i ==pattern[1] || i == pattern[0]){ 
            s += i;
            index[i]++;
        }
    }
    // 要实现最多 只能加在开头或者结尾
    
    if(index[pattern[0]] < index[pattern[1]]){ 

        s = pattern[0] + s;
        index[pattern[0]]++;
    }else{ 

        s = s + pattern[1];
        index[pattern[1]]++;
    }
    
    cout << s << endl;
    for(auto &i : s){ 

        /* if(pattern[0] == pattern[1] && index[pattern[0]] <= 1) */
        /*     break; */

        if(i == pattern[0]){ 

            index[i]--;
            result += index[pattern[1]];
        }
        else if (i == pattern[1])
            index[i]--;
    }

    return result;

}


void test(){
    /* string text = "abdcdbc"; */
    /* string patten = "ac"; */
    string text = "mffiqmrvjmkfmbnaivajzecfdta";
    string patten = "hh";

    cout << maximumSubsequenceCount(text, patten) << endl;

}

int main(){
    test();
    return 0;
}
