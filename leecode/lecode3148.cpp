/*************************************************************************
    > File Name: lecode3148.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月15日 星期四 09时33分13秒
     
      使用动态规划来解决问题

      accept  但是可以再去优化空间复杂度 
      动态规划 通过
      时间复杂度O(mn)
      空间复杂度O(mn)
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
  
    vector<vector<int>> grid = {
      /*  {
            9, 5, 7, 3
        },
        {
            8, 9, 6, 1
        },
        {
            6, 7, 14, 3
        },
        {
            2, 5, 3,  1
        }
        
       /* {
            4, 3, 2
        },
        {
            3, 2, 1
        }
        */
        {
            4, 9
        },
        {
            5, 2
        },
        {
            3, 1
        }
    };

    vector<vector<int>> result(grid.size(),  vector<int>(grid[0].size(), 0));
    int max_result = INT_MIN;

 
    for(int i=0; i< grid.size(); ++i){

        for(int j=0; j<grid[0].size(); ++j){
            if(j == 0 && i == 0){
                continue;
            }else if(i == 0){

                if(result[i][j-1] > 0){
                    result[i][j] = grid[i][j] + result[i][j-1] - grid[i][j-1];
                }else{

                    result[i][j] = grid[i][j] - grid[i][j-1];
                }
            }else if (j == 0 && i != 0){

                
                if(result[i-1][j] > 0){
                    result[i][j] = grid[i][j] + result[i-1][j] - grid[i-1][j];
                }else{

                    result[i][j] = grid[i][j] - grid[i-1][j];
                }

            }else{

                int a = 0;
                int b = 0;
                
                if(result[i][j-1] > 0){
                    a = grid[i][j] + result[i][j-1] - grid[i][j-1];
                }else{

                    a  = grid[i][j] - grid[i][j-1];
                }


                if(result[i-1][j] > 0){
                    b  = grid[i][j] +  result[i-1][j] - grid[i-1][j];
                }else{

                    b  = grid[i][j] - grid[i-1][j];
                }


                result[i][j] = max(a, b);


            }

            if(result[i][j] > max_result)
                max_result = result[i][j];
        }
    }

  //  for(int j=0; j < grid[0].size() ; ++j){
  //      cout << result[0][j] <<endl;
  //  }

    for(auto i : result){
        for(auto j : i)
            cout<< j << " ";

        cout<<endl;
    }


    cout<<"max == " << max_result << endl;


    return 0;
}
