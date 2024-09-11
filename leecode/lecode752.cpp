/*************************************************************************
    > File Name: lecode752.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月03日 星期二 16时41分27秒
    方法:
       BFS

       accept
       队列记得从队头取东西(front)  而不是队尾 (back)
 ************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <math.h>
#include <climits>


using namespace std;

int openLock(vector<string> & deadends, string target){
   int result = 0 ;
   set<string> dead(deadends.begin(), deadends.end());
    
   queue<string> index;
   index.push("0000");

   while(!index.empty()){
       int size = index.size();

       /* cout << size << endl; */
       for(int i = 0; i<size; ++i){

           string tmp =  index.front();
           index.pop();

           if(dead.find(tmp) != dead.end())
                continue;

           if(tmp == target){

                /* cout << tmp << endl; */
                return result;
           }

           dead.insert(tmp);

           for(size_t j = 0; j < tmp.size(); ++j){
               
               int a = tmp[j] - '0';
               string  s1  = tmp;
               s1[j] = (a + 1)%10 + '0';

               if(dead.find(s1) == dead.end())
               index.push(s1);

               int b = tmp[j] - '0';
               string  s2  = tmp;
               s2[j] = (b + 9)%10 + '0';

               if(dead.find(s2) == dead.end())
               index.push(s2);

               /* cout << s1 << endl; */
               /* cout << s2 << endl; */
           }

       }
       ++result;
   }

   /* cout << "size" << index.size() << endl; */  
   /* cout << endl; */

   return -1;

}


void test(){

    vector<string> deadends = { "0201","0101","0102","1212","2002"};
    string target = "0202";
    /* vector<string> deadends = { "8887","8889","8878","8898","8788","8988","7888","9888"}; */
    /* string target = "8888"; */
    /* vector<string> deadends = {"8888"}; */
    /* string target = "0009"; */

    cout << openLock(deadends, target) << endl;;
}

int main(){
    test();
    return 0;
}
