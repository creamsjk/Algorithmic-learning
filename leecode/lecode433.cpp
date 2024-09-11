/*************************************************************************
    > File Name: lecode433.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月06日 星期五 20时52分38秒

    方法:
      BFS

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


int minMutation(string startGene, string endGene, vector<string>& bank) {

    int result = 0;

    queue<string> que;
    que.push(startGene);

    set<string> index;
    index.insert(startGene);

    while(!que.empty()){

        int my_size = que.size();

        for(int i = 0; i < my_size; ++i){

            string tmp = que.front();
            que.pop();

            if(tmp == endGene)
                return result;

            for(auto it = bank.begin(); it != bank.end();){

                if(is_transfer(tmp, *it) && index.find(*it) == index.end()){
                    
                    que.push(*it);
                    index.insert(*it);
                    bank.erase(it);
                    
                }else{
                    ++it;
                }
            }
        }
        ++result;
    }
    

    return -1;

}

void test(){

    string start = "AACCGGTT";
    /* string end = "AACCGGTA"; */
    /* vector<string> bank = {"AACCGGTA" }; */
    string end = "AAACGGTA";
    vector<string> bank = {"AACCGGTA","AACCGCTA","AAACGGTA"};

    cout << minMutation(start, end, bank) << endl;

}

int main(){
    test();
    return 0;
}
