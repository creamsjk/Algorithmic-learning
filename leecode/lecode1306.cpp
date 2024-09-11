/*************************************************************************
    > File Name: lecode1306.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月06日 星期五 16时30分58秒
    方法:
       DFS or BFS
       深度优先搜索 广度优先搜索 都通过!!!
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

bool DFS(vector<int> &arr, int start, vector<bool>& index){
    
    bool a = false;
    bool b = false;
    int f = start + arr[start];
    int len = arr.size();

    if(f < len && index[f] == true){
        if(arr[f] == 0)
            return true;
        index[f] = false;
      a =  DFS(arr, f , index);
    }

    int e = start - arr[start];

    if(e >= 0 && index[e] == true){
        if(arr[e] == 0)
            return true;
        index[e] = false;
       b =  DFS(arr, e , index);
    }

    /* cout << "front: " << f << " end: " << e << endl; */ 
    if(a || b)
        return true;
    else
        return false;

        
}


//BFS
bool canReach(vector<int>&arr, int start){
    set<int> target;
    int len = arr.size();

    for(size_t i = 0; i < arr.size(); ++i){
        if(arr[i] == 0)
            target.insert(i);
    }
    /* for(auto &i: target) */
    /*     cout << i << " "; */
    /* cout << endl; */

    set<int> index;
    queue<int> que;
    que.push(start);
    //避免一直循环
    index.insert(start);

    while(!que.empty()){

        int my_size = que.size();
        /* cout << my_size << endl; */

        for(int i = 0 ; i < my_size; ++i){

            int tmp = que.front();
            que.pop();

            /* cout << tmp << endl; */
            if(target.find(tmp) != target.end())
                return true;

            int f = tmp + arr[tmp];
            if(f < len && index.find(f) ==  index.end()){
                que.push(f);
                index.insert(f);
            }

            int e = tmp - arr[tmp];
            if(e >= 0 && index.find(e) ==  index.end()){
                que.push(e);
                index.insert(e);
            }
            /* cout << "front: " << f << " end: " << e << endl; */ 
        }
    }


    return false;


}


void test(){

    /* vector<int> arr = {4,2,3,0,3,1,2}; */
    /* int start = 0; */
    vector<int> arr = {3,0,2,1,2};
    int start = 2;

    vector<bool> index(arr.size(), true);

    /* cout << canReach(arr, start) << endl; */
     /* if(canReach(arr, start)) */
     /*     cout << "true" << endl; */
    /* else */ 
     /*    cout << "false" << endl; */


     if(DFS(arr, start, index))
         cout << "true" << endl;
    else 
        cout << "false" << endl;
}

int main(){
    test();
    return 0;
}
