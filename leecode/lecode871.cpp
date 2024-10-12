/*************************************************************************
    > File Name: lecode871.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年10月08日 星期二 14时19分18秒

    动态规划!!

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

int dp(const int target, const int startFuel, vector<vector<int>>& stations, int index){ 

    if(startFuel < 0 && target > 0)
        return -1;

}

int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {


    return dp(target, startFuel, stations, 0);

}


void test(){

    int target = 100;
    int startFuel = 10;
    vector<vector<int>> stations = { { 10, 60}, { 20, 30}, { 30, 40}, { 60, 40}} ;

    cout << minRefuelStops(target, startFuel, stations) << endl;

}

int main(){
    test();
    return 0;
}
