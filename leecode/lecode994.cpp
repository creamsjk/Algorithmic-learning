/*************************************************************************
    > File Name: lecode994.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月07日 星期六 10时22分14秒
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

int orangesRotting(vector<vector<int>>& grid) {

    int result = 0;    
    int orang =  0 ;
    queue<vector<int>> que;
    set<vector<int>> index;
    int row = grid.size();
    int conlum = grid[0].size();
    
    for(auto &i : grid){
        for(auto &j : i)
            if(j == 1)
                ++orang;
    }
    if(orang == 0)
        return result;

    for(int i = 0; i < row; ++i){
        for(int j = 0 ; j <conlum; ++j)
            if(grid[i][j] == 2){
                que.push({i,j});
                index.insert({i,j });
            }
        
    }
    /* cout << "前期工作准备完成" << endl; */



    while(!que.empty()){
        int my_size = que.size();
        /* cout << my_size << endl; */

        for(int i = 0;i < my_size; ++i){

            vector<int> tmp = que.front();
            que.pop();
            if(grid[tmp[0]][tmp[1]] == 0)
                continue;


            //右边
            if(tmp[1] < conlum -1){
                int a = tmp[1] + 1;
                vector<int> f = {tmp[0],a};
                if(grid[tmp[0]][a] == 1 && index.find(f) == index.end()){

                    grid[tmp[0]][a] = 2;
                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }
            //左边
             if(tmp[1]  > 0){
                int a = tmp[1] - 1;
                vector<int> f = {tmp[0],a};
                if(grid[tmp[0]][a] == 1 && index.find(f) == index.end()){

                    grid[tmp[0]][a]  = 2;
                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }

            //下边
            if(tmp[0] < row -1){
                int a = tmp[0] + 1;
                vector<int> f = {a,tmp[1]};
                if(grid[a][tmp[1]] == 1 && index.find(f) == index.end()){

                    grid[a][tmp[1]] = 2;
                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }
          //上方
             if(tmp[0] > 0){
                int a = tmp[0] - 1;
                vector<int> f = {a,tmp[1]};
                if(grid[a][tmp[1]] == 1 && index.find(f) == index.end()){
                    
                    grid[a][tmp[1]] = 2;
                    que.push(f);
                    index.insert(f);
                    /* display(f); */
                }
            }

        }
        ++result;
    }
   
    for(auto &i : grid){
        for(auto &j : i)
            if(j == 1)
                return -1;
    }


    return result -1 ;


}


void test(){
    vector<vector<int>> grid = {
        {
            0, 2, 2
        },
        {
            0, 1, 0
        },
        /* { */
        /*     0,1,1 */
        /* } */
    };

    cout <<  orangesRotting(grid) << endl;

}

int main(){
    test();
    return 0;
}
