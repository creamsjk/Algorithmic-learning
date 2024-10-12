/*************************************************************************
    > File Name: lecode983.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年10月04日 星期五 19时56分26秒


    nice  深度优先 通过  使用了备忘录去重的算法
    时间复杂度不是很会算
    空间复杂度O(N)

    计算机就要坚持不放弃!!!!

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

vector<int> memo;

int dfs(const vector<int>& days, const vector<int>& costs, const int index){ 

    if(index == (int)days.size() - 1){ 


    cout << "index = " << index  << endl;
    return min(costs[0], min(costs[1], costs[2]));
    }

    if(memo[index] != -1)
        return memo[index];

    int a = 0, b = 0, c = 0;

    a = dfs(days, costs, index + 1) + costs[0];

    { 
        int tmp = 0;

        for(int i = index; i < (int)days.size(); ++i){ 

            if(days[i] - days[index] >= 7){ 

                tmp = i;
                break;
            }
        }

        if(tmp == 0)
            b = costs[1];
        else
            b = dfs(days, costs, tmp) + costs[1];


    }

    { 
        int tmp = 0;

        for(int i = index; i < (int)days.size(); ++i){ 

            if(days[i] - days[index] >= 30){ 

                tmp = i;
                break;
            }
        }

        if(tmp == 0)
            c = costs[2];
        else
            c = dfs(days, costs, tmp) + costs[2];
    }

    cout << "index = " << index << " " << a << "  " << b << "  " << c << endl;

    memo[index] = min(a, min(b, c));

    return memo[index];
}

int mincostTickets(vector<int>& days, vector<int>& costs) {

    memo = vector<int>(days.size(), -1);
    return dfs(days, costs, 0);
}


void test(){

    /* vector<int> days = { 1, 4, 6, 7, 8, 20}; */
    /* vector<int> days = {1,2,3,4,5,6,7,8,9,10,30,31 }; */
    /* vector<int> costs = { 2, 7, 15}; */
    /* vector<int> days = {1,4,6,7,8,20}; */
    /* vector<int> costs = { 7, 2, 15}; */

    vector<int> days = {3,5,6,8,9,10,11,12,13,14,15,16,20,21,23,25,26,27,29,30,33,34,35,36,38,39,40,42,45,46,47,48,49,51,53,54,56,57,58,59,60,61,63,64,67,68,69,70,72,74,77,78,79,80,81,82,83,84,85,88,91,92,93,96};
    vector<int> costs = { 3, 17, 57};

    cout << mincostTickets(days, costs) << endl;

}

int main(){
    test();
    return 0;
}
