/*************************************************************************
    > File Name: lecode690.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月26日 星期一 09时48分48秒
    方法
       每次 读一个  total 增加 并且将他的下属加入

       时间复杂度 O(N)
       空间复杂度 O(N)
       accept

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

class Employee {
public:
    int id;
    int importance;
    vector<int> subordinates;
};

int main(){




    vector<Employee> employees = { {1,2,{5},}, {5,-3,{},}, };
    int id =  5;

     vector<int> result;
        map<int, int> index;

    for(int i =0 ;i< employees.size(); ++i){

        index[ employees[i].id] =  i;
    }
    result.push_back(index[id]);

     int total = 0;

        //开始dfs
        while(result.size() != 0){
            int tmp = result.back();
            result.pop_back();
            total += employees[tmp].importance;
            for(auto &i: employees[tmp].subordinates){
                result.push_back(index[i]);
            }
        }

        cout<<total <<endl;















    return 0;
}
