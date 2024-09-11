/*************************************************************************
    > File Name: lecode2860.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月04日 星期三 09时32分06秒
    方法:
         回溯?NO

         accept
         时间复杂度 取决于排序
         空间复杂度 O(1)
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

int countWays(vector<int> &nums){
    int result = 0;
    sort(nums.begin(), nums.end());
    
    int select = 0;
    //全不选的情况
    if(nums[0] > 0)
        result++;

    for(size_t i = 0; i < nums.size(); ++i){

        //满足条件选中
        if(select + 1  > nums[i]){
            
            if( nums.size() -1 - i >=1  && nums[i+1] > select + 1){
                
             select++;
             result++;
            }else if(select + 1  == nums.size()) {

             select++;
             result++;

            }else{
                select++;
            }
            
        }else{
        //选中但是不满足条件
            
            select++;
        }
    }

    return result;
}



void test(){

    vector<int> nums = {6, 0, 3, 3, 6, 7, 2, 7};
    /* vector<int> nums = {1, 1}; */
    /* vector<int> nums = {1,1,0,1}; */


    cout << countWays(nums) << endl;

}

int main(){
    test();
    return 0;
}
