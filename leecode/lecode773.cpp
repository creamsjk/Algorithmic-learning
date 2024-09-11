/*************************************************************************
    > File Name: lecode773.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月03日 星期二 21时56分03秒
    方法:
         BFS

         accept
         通过!!! 细心一点 !!

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

int slidingPuzzle(vector<vector<int>> &board){
    int result = 0;
     
    vector<int> index = board[0];
    for(auto &i : board[1])
        index.push_back(i);

    vector<int> target = {
        1, 2, 3, 4, 5, 0
    };

    /* for(auto i : index ) */
    /*     cout << i << " "; */
    /* cout << endl; */

    set<vector<int>> dead;
    queue<vector<int>> que;
    que.push(index);

    while(!que.empty()){
        int size = que.size();

        /* cout << size << endl; */

        for(int i = 0; i<size; ++i){
            
            const vector<int> tmp =  que.front();
            que.pop();
            
            if(dead.find(tmp) != dead.end())
                continue;

            if(tmp == target)
                return result;
            
    /* for(auto i : tmp ) */
    /*     cout << i << " "; */
    /* cout << endl; */

            dead.insert(tmp);

            if(tmp[0] == 0){

                vector<int> t1 = tmp;
                vector<int> t2 = tmp;
                int a = tmp[1];
                int b = tmp[3];

                t1[0] = a;
                t1[1] = 0;

                if(dead.find(t1) == dead.end())
                    que.push(t1);


                t2[0] = b;
                t2[3] = 0;
            
                if(dead.find(t2) == dead.end())
                    que.push(t2);

            }else if( tmp[1] == 0){

               vector<int> t1 = tmp;
               vector<int> t2 = tmp;
               vector<int> t3 = tmp;

               int a = tmp[0];
               int b = tmp[2];
               int c = tmp[4];

               t1[1] = a;
               t1[0] = 0;

               if(dead.find(t1) == dead.end())
                   que.push(t1);


               t2[1] = b;
               t2[2] = 0;
               if(dead.find(t2) == dead.end())
                   que.push(t2);

               t3[1] = c;
               t3[4] = 0;
               if(dead.find(t3) == dead.end())
                   que.push(t3);







              
            }else if (tmp[2] == 0){

                vector<int> t1 = tmp;
                vector<int> t2 = tmp;
                int a = tmp[1];
                int b = tmp[5];

                t1[2] = a;
                t1[1] = 0;

                if(dead.find(t1) == dead.end())
                    que.push(t1);


                t2[2] = b;
                t2[5] = 0;
            
                if(dead.find(t2) == dead.end())
                    que.push(t2);

            }else if (tmp[3] == 0){
                
                vector<int> t1 = tmp;
                vector<int> t2 = tmp;
                int a = tmp[0];
                int b = tmp[4];

                t1[3] = a;
                t1[0] = 0;

                if(dead.find(t1) == dead.end())
                    que.push(t1);


                t2[3] = b;
                t2[4] = 0;
            
                if(dead.find(t2) == dead.end())
                    que.push(t2);


                
            }else if(tmp[4] == 0){

               vector<int> t1 = tmp;
               vector<int> t2 = tmp;
               vector<int> t3 = tmp;

               int a = tmp[1];
               int b = tmp[3];
               int c = tmp[5];

               t1[4] = a;
               t1[1] = 0;

               if(dead.find(t1) == dead.end())
                   que.push(t1);


               t2[4] = b;
               t2[3] = 0;
               if(dead.find(t2) == dead.end())
                   que.push(t2);

               t3[4] = c;
               t3[5] = 0;
               if(dead.find(t3) == dead.end())
                   que.push(t3);


            }else  if(tmp[5] == 0){
                
                vector<int> t1 = tmp;
                vector<int> t2 = tmp;
                int a = tmp[2];
                int b = tmp[4];

                t1[5] = a;
                t1[2] = 0;

                if(dead.find(t1) == dead.end())
                    que.push(t1);


                t2[5] = b;
                t2[4] = 0;
            
                if(dead.find(t2) == dead.end())
                    que.push(t2);
            }


        }
        /* cout << endl; */
        ++result;

        /* if(result > 3) */
        /*     break; */
    }


    return -1;
}


void test(){

    vector<vector<int>> board =  {
        {
            1, 2, 3 
        },
        {
            4, 0 ,5
        }
    };

    cout << slidingPuzzle(board) << endl;

}

int main(){
    test();
    return 0;
}
