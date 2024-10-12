/*************************************************************************
    > File Name: lecode2576.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月12日 星期四 09时39分54秒


    怎么感觉题做多了, 越来越怂了呢.............
    这个感觉是脑筋急转弯题
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
#include <iterator>


using namespace std;

int maxNumOfMarkedIndices(vector<int>& nums) {

    int result = 0;
    sort(nums.begin(), nums.end());
    unordered_set<int> index;
    /* for(auto &i : nums) */
    /*     cout << i << " "; */
    /* cout << endl; */

    for(size_t i = 0, j = nums.size()/2; j < nums.size(); ++j){

        while(index.find(i) != index.end())
            ++i;

        if(nums[j] >= nums[i] * 2){
            ++result;
            ++i;
            index.insert(j);
        }
    }

    /* for(auto &i : index) */
    /*     cout << i << " "; */
    /* cout << endl; */

       return  result  * 2;
    
}


void test(){

    /* vector<int> nums = {9, 2, 5, 4}; */
    /* vector<int> nums = {3, 5, 2, 4}; */
    vector<int> nums = {42,83,48,10,24,55,9,100,10,17,17,99,51,32,16,98,99,31,28,68,71,14,64,29,15,40};
    cout << maxNumOfMarkedIndices(nums) << endl;
}

int main(){
    test();
    return 0;
}
