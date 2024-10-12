/*************************************************************************
    > File Name: lecode1014.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月23日 星期一 10时57分19秒

    通过 
    时间复杂度  O(N)
    空间复杂度  O(1)
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

int maxScoreSightseeingPair(vector<int>& values) {

    int key = 0;
    int val = values[0];
    int result = val;

    for(size_t i = 1; i < values.size(); ++i){ 
        
        int tmp = val + values[i] + key - i;
        cout << "i=" << i << "  tmp=" << tmp <<" key=" << key <<" val=" << val << " result=" << result  << endl; 
        if(tmp > result){ 

            result = tmp;
        }
        if(values[i] +(int) i >= key + val){ 

            key = i;
            val = values[i];
        }
    }

    return result;
}


void test(){

    /* vector<int> values = { 8,1,5,2,6}; */
    vector<int> values = {1,2};
    /* vector<int> values = {4, 7, 5, 8}; */
    cout << maxScoreSightseeingPair(values) << endl;

}

int main(){
    test();
    return 0;
}
