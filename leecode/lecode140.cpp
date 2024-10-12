/*************************************************************************
    > File Name: lecode140.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月12日 星期四 20时07分30秒

    回溯吧  构建子串
    先把 词 分开 根据分开的顺序开始构建子串   分解完成 就加入 result

    yes  一次通过   0ms!!!!!!!!!!!
    accept 
    2024年 09月 12日 星期四 21:30:38 CST
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


vector<vector<string>> result;
vector<string> tt;
unordered_set<string> memo;

void pathtrack(string s, unordered_set<string> &memo, unordered_set<string> &index, int sz){ 
    
    if((size_t)sz == s.size()){ 
        /* for(auto &i:tt) */
        /*     cout << i << " "; */
        /* cout << endl; */
        result.push_back(tt);
        return;
    }

    for(size_t i = sz + 1; i <= s.size(); ++i ){ 
         
        string tmp(s.begin()+ sz, s.begin() + i);
        string e(s.begin()+ i,s.end());

        if(memo.find(e) != memo.end())
            continue;


        /* cout << "tmp: " << tmp << "  sz: " << sz   << " i: " << i << endl; */
        if(index.find(tmp) != index.end()){ 
           
        /* cout << "tmp: " << tmp << "  sz: " << sz   << " i: " << i <<  " end:" << e << endl; */
            tt.push_back(tmp);

            size_t a = result.size();
            pathtrack(s, memo, index, i);

            if(a == result.size())
                memo.insert(e);
            tt.pop_back();
        }
        
    }

}
 

vector<string> wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> index(wordDict.begin(), wordDict.end());
    pathtrack(s, memo, index, 0); 

    vector<string> sp;
    tt.clear();
    string ff;

    for(auto &i : result){ 

        for(auto j = i.begin(); j != i.end(); ++j){ 
            if(j == i.end() -1 )
              ff.append(*j);
            else
                ff.append(*j).append(" ");
        }
        sp.push_back(ff);
        ff.clear();
    }
    

    return sp; 
}


void test(){

    /* string s = "catsanddog"; */
    /* vector<string> wordDict = { "cat","cats","and","sand","dog"}; */
    string s = "pineapplepenapple";
    vector<string> wordDict = { "apple","pen","applepen","pine","pineapple"};
    /* string s = "catsandog"; */
    /* vector<string> wordDict = { "cats","dog","sand","and","cat"}; */


    vector<string> sp = wordBreak(s, wordDict);
    for(auto &i: sp)
        cout << i <<endl;

/*     cout << endl; */
/*     for(auto &i : result){ */ 
/*         for(auto &j : i) */
/*             cout << j << " "; */
/*         cout << endl; */

/*     } */

/*     cout << endl; */
/*     for(auto &i: memo) */
/*         cout << i << " "; */
/*     cout << endl; */
}

int main(){
    test();
    return 0;
}
