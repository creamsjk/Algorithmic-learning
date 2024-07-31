/*************************************************************************
    > File Name: lecode3111.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月31日 星期三 09时30分51秒
    方法:
         只计算横坐标,纵坐标对结果没有影响
         找到一个端点, 将其扩展至w 然后走到下一不在这个矩形的位置 count++
         重复上面操作

         时间复杂度 O(N)  空间复杂度 O(N)
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


    vector<vector<int>> points = {{2,1},{1,0},{1,4},{1,8},{3,5},{4,6}};
    int w = 1;
 //   vector<vector<int>> points = {{1,1},{0,0},{2,2},{3,3},{5,5},{4,4},{6,6}};
 //   int w = 2;
  //  vector<vector<int>> points = {{2,3},{1,2}};
  //  int w = 0;

     
    vector<int> result;
        for(auto i:points){
            result.push_back(i[0]);
        }


        sort(result.begin(),result.end());
        for(auto i : result)
            cout<<i<<endl;

        cout<<endl;
        int count = 0;
        int start= 0;
        int n = result.size();
        for(int i=0;i<n;i++){
            start = result[i];
            count++;
            while(i < n &&  result[i] <= start+w){
                cout<<result[i]<<"   " << i <<endl;
                 i++;
            }
            i--;
            continue;
        }

      cout<<endl<<count<<endl;



    return 0;
}
