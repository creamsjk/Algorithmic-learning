/*************************************************************************
    > File Name: lecode2374.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月21日 星期六 09时28分24秒

    空间换时间  遍历 
    accept!!
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

int edgeScore(vector<int>& edges) {

    vector<long long> result(edges.size(), 0);
    for(size_t i = 0; i < edges.size(); ++i){ 

        result[edges[i]] += i;
    }
    long long tmp = 0;

    int res = 0;
    for(size_t i = 0; i < edges.size(); ++i){ 

        if(result[i] > tmp){ 
            tmp = result[i];
            res = i;

        }

    }

    return res;
    
}



void test(){
    /* vector<int> edges = { 1,0,0,0,0,7,7,5}; */
    vector<int> edges = { 2,0,0,2};
    cout << edgeScore(edges) << endl;

}

int main(){
    test();
    return 0;
}
