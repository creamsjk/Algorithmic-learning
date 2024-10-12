/*************************************************************************
    > File Name: lecode2390.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月14日 星期六 09时19分48秒
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

string removeStars(string s) {

    string res;
    int n = s.size();

    int j = n - 1;
    for(int i = n - 1; i >= 0 ; --i){ 

        if(s[i] == '*'){ 

            while( j >= 0){ 

                if(s[j] != '*' && s[j] != '0' && j < i){ 

                    s[j] = '0';
                    break;
                }
                --j;

            }

        }

        if(s[i] != '0'&& s[i] != '*')
            res.push_back(s[i]);
    }

    /* cout << s << endl; */
    /* cout << res << endl; */
    reverse(res.begin(), res.end());
    
    return res;
}


void test(){

    string s = "leet**cod*e";
    cout << removeStars(s) << endl;
}

int main(){
    test();
    return 0;
}
