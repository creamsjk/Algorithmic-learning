/*************************************************************************
    > File Name: lecode1091.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月07日 星期六 09时25分45秒
    方法:
        BFS

        accept
        2024年 09月 07日 星期六 10:13:42 CST

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
void display(vector<int>& nums){

    cout << "[" << nums[0] << " , " << nums[1] << "]" << endl;
}

int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
    int result = 1;

    if(grid[0][0] == 1)
        return -1;

    queue<vector<int>> que;
    set<vector<int>> index;
    que.push({0,0});
    index.insert({0,0 });
    int row = grid.size();
    int conlum = grid[0].size();

    while(!que.empty()){
        int my_size = que.size();
        /* cout << my_size << endl; */

        for(int i = 0;i < my_size; ++i){
            
            vector<int> tmp = que.front();
            que.pop();

            if(tmp[0] == row-1 && tmp[1] == conlum -1)
                return result;

            //右边
            if(tmp[1] < conlum -1){
                int a = tmp[1] + 1;
                vector<int> f = {tmp[0],a};
                if(grid[tmp[0]][a] == 0 && index.find(f) == index.end()){

                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }
            //左边
             if(tmp[1]  > 0){
                int a = tmp[1] - 1;
                vector<int> f = {tmp[0],a};
                if(grid[tmp[0]][a] == 0 && index.find(f) == index.end()){

                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }

            //下边
            if(tmp[0] < row -1){
                int a = tmp[0] + 1;
                vector<int> f = {a,tmp[1]};
                if(grid[a][tmp[1]] == 0 && index.find(f) == index.end()){

                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }
            //右下
            if(tmp[0] < row -1 && tmp[1] < conlum -1){
                
                int a = tmp[0] + 1;
                int b = tmp[1] + 1;
                vector<int> f = {a,b};
                if(grid[a][b] == 0 && index.find(f) == index.end()){

                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }

            }
            //左上
            if(tmp[0] > 0 && tmp[1]  > 0){
                
                int a = tmp[0] - 1;
                int b = tmp[1] - 1;
                vector<int> f = {a,b};
                if(grid[a][b] == 0 && index.find(f) == index.end()){

                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }

            //右上
             if(tmp[0] > 0 && tmp[1] < conlum -1){
                
                int a = tmp[0] - 1;
                int b = tmp[1] + 1;
                vector<int> f = {a,b};
                if(grid[a][b] == 0 && index.find(f) == index.end()){

                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }

            }
            //上方
             if(tmp[0] > 0){
                int a = tmp[0] - 1;
                vector<int> f = {a,tmp[1]};
                if(grid[a][tmp[1]] == 0 && index.find(f) == index.end()){

                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }
            //左下
             if(tmp[0] < row -1 && tmp[1] > 0){
                
                int a = tmp[0] + 1;
                int b = tmp[1] - 1;
                vector<int> f = {a,b};
                if(grid[a][b] == 0 && index.find(f) == index.end()){

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
    /* vector<vector<int>> grid = { */
    /*     { */
    /*         1,0,0 */
    /*     }, */
    /*     { */
    /*         1,1,0 */
    /*     }, */
    /*     { */
    /*         1,1,0 */
    /*     } */
    /* }; */

    vector<vector<int>> grid = {
        {
            0,1,1,0,0,0
        },
        {
            0,1,0,1,1,0
        },
        {
            0,1,1,0,1,0
        },
        {
            0,0,0,1,1,0
        },
        {
            1,1,1,1,1,0
        },
        {
            1,1,1,1,1,0
        }
    };
    
    cout << shortestPathBinaryMatrix(grid) << endl;

}

int main(){
    test();
    return 0;
}
