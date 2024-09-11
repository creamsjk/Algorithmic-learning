/*************************************************************************
    > File Name: lecode127.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月06日 星期五 20时10分38秒
    方法:
       BFS

       通过
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

bool is_transfer(string s1, string s2){

    
    int res = 0;
    for(size_t i = 0; i < s1.size(); ++i){
        if(s1[i] != s2[i])
            ++res;
    }
    /* cout << "s1: " << s1 << " s2: " << s2 << "   res=" << res  << endl; */
    
    if(res == 1)
        return true;
    else
        return false;
}

int ladderLength(string beginWord, string endWord, vector<string>& wordList) {

    int result = 1;
    
    queue<string> que;
    set<string>  index;
    index.insert(beginWord);
    que.push(beginWord);

    while(!que.empty()){

        int my_size = que.size();
        cout << my_size << endl;

        for(int i = 0; i < my_size; ++i){

            string tmp = que.front();
            que.pop();
            /* cout << tmp << endl; */ 
            if(tmp == endWord)
                return result;
            
            for(auto it = wordList.begin(); it != wordList.end(); ){

                if(is_transfer(tmp, *it) &&  index.find(*it) == index.end()){
                     
                    /* cout <<"it =  " <<*it << endl; */
                    que.push(*it);
                    index.insert(*it);
                    wordList.erase(it);
                    
                }else
                    it++;
                                 
            }

        }
        ++result;
    }


    return 0;
}


void test(){

    string beginWord = "hit";
    string endWord = "cog";
    /* vector<string> wordList = {"hot","dot","dog","lot","log","cog"}; */
    vector<string> wordList = {"hot","dot","dog","lot","log"};

    cout << ladderLength(beginWord, endWord, wordList) << endl;

}

int main(){
    test();
    return 0;
}
