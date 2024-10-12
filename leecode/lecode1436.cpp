/*************************************************************************
    > File Name: lecode1436.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年10月08日 星期二 13时57分38秒

    正常做法!!!
    时间复杂度O(N);
    空间复杂度O(N);


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



string destCity(vector<vector<string>>& paths) {

    unordered_set<string>  city_start;
    for(auto &start: paths)
        city_start.insert(start[0]);
    
    for(auto &end: paths){ 
        if(city_start.find(end[1]) == city_start.end())
            return end[1];
    }

    return "good";
}

void test(){

    /* vector<vector<string>> paths = { {"B", "C" }, { "D", "B"}, { "C", "A"} }; */
    vector<vector<string>> paths = { {"B", "C" }, { "D", "B"}, { "C", "A"} };

    cout << destCity(paths) << endl;
}

int main(){
    test();
    return 0;
}
