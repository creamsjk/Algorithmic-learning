/*************************************************************************
    > File Name: lecode3128.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月02日 星期五 09时21分27秒
    方法:
        先看右边再看下边
        三种情况
        **   *     * *   
        *    * *     *
     time out...............
 ************************************************************************/

#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>


using namespace std;


int main(){
  // vector<vector<int>> grid ={{1,0,0,0}, {0 , 1, 0, 1}, {1, 0, 0, 0}};
  //  vector<vector<int>> grid ={{1,0, 1}, {1, 0, 0}, {1, 0, 0}};
   //  vector<vector<int>> grid ={{1,1}, {1, 1}};
   
    vector<vector<int>> grid ={{1,1}, {1, 1}, {1,1}};
    //当前行总1数量
    vector<int> row;
    //当前列总1数量
    vector<int>colum;
    
    for(auto i:grid){
        int tmp = 0;
        for(auto j:i){
            tmp += j;
        }
        row.push_back(tmp);
    }

      int result = 0;

    //m行 n列
    int  m = grid.size();
    int  n = grid[0].size();

     for(int i=0; i< n; i++){
         int tmp = 0;
        for(int j = 0;j<m;j++)
            tmp += grid[j][i];

        colum.push_back(tmp);
    }


     //当前列剩余1数量
   vector<int> less(colum);
   vector<int> le(row);

    for(int i = 0; i< m-1 ;i++){
        for(int j = 0; j < n; j++){

            if(grid[i][j] == 1){
                
              less[j]--;
              le[i]--;
                //第二种情况
                for(int k = i+1; k<m; k++){
                    if(grid[k][j] == 1){
                        int tmp = row[k]-1;
                        result += tmp;
                    }
                }

                result += (le[i] * less[j]);

                //第一种和第三种情况
                for(int k=j+1; k<n; k++){
                    
                    if(grid[i][k] == 1 )
                        result += less[k]-1;

                }

                cout<<"result == "<< result<<"i == " << i << "j == "<< j <<endl;
                
            }
        }
    }


    cout<<result<<endl;

    return 0; 
}
