/*************************************************************************
    > File Name: lecode1926.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月06日 星期五 21时16分00秒
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

void display(vector<int> &nums){

    cout << "[ " << nums[0] << "," << nums[1] << " ]" << endl;
}

int nearestExit(vector<vector<string>>& maze, vector<int>& entrance) {
    int result = 0;

     queue<vector<int>> que; 
     que.push(entrance);
     int row = maze.size();
     int conlum = maze[0].size(); 

     set<vector<int>> index;
     index.insert(entrance);

     while(!que.empty()){
         int my_size = que.size();

         /* cout << my_size << endl; */
         for(int i = 0; i < my_size; ++i){

             vector<int> tmp = que.front();
             que.pop();
             if(tmp[1] == 0 || tmp[0] ==0 || tmp[0] == row -1 || tmp[1] == conlum - 1){
                 if(tmp != entrance)
                     return result;
                     
             }

             //开始上下左右 判断
             //上
             if(tmp[0]-1 >= 0){
                 int a = tmp[0] -1;
                 vector<int> f = {a, tmp[1]};
                 if(index.find(f) == index.end() && maze[a][tmp[1]] == "."){
                       que.push(f);
                       index.insert(f);
                       /* display(f); */
                 }
             }
             //下
             if(tmp[0] + 1 <row){
                 int a = tmp[0] +1;
                 vector<int> f = {a, tmp[1]};
                 if(index.find(f) == index.end() && maze[a][tmp[1]] == "."){
                     que.push(f);
                     index.insert(f);
                     /* display(f); */
                 }
             }

             //左
             if(tmp[1] -1 >= 0){
                 int a = tmp[1] -1;
                 vector<int> f = {tmp[0], a};
                 
                 if(index.find(f) == index.end() && maze[tmp[0]][a] == "."){
                     que.push(f);
                     index.insert(f);
                     /* display(f); */
                 }

             }

             //右
             if(tmp[1] + 1 < conlum){
                 int a = tmp[1] + 1;
                 vector<int> f = {tmp[0], a};
                 
                 if(index.find(f) == index.end() && maze[tmp[0]][a] == "."){
                     que.push(f);
                     index.insert(f);
                     /* display(f); */
                 }

             }
             
         }
         ++result;
     }

    
    return -1;
}


void test(){
    vector<vector<string>> maze = {  
        {
            "+","+",".","+"
        },
        {
            ".",".",".","+"
        },
        {
            "+","+","+","."
        }
    };

    vector<int> entrance = { 1, 2};

    cout << nearestExit(maze, entrance) << endl;

}

int main(){
    test();
    return 0;
}
