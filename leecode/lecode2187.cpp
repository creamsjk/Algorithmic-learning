/*************************************************************************
    > File Name: lecode2187.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年10月05日 星期六 09时17分10秒

    二分查找

    寻找上界与下界
    accept!!!
    注意数据类型
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

//判断花费时间 能运行的公交车次数  
long long fx(const vector<int>& times, long long  costs_time){ 

     unsigned long long result = 0;

    for(size_t i = 0; i < times.size(); ++i ){ 

        result += (unsigned long long )(costs_time/times[i]);
    }

    cout << "costs_time=" << costs_time << " result = "  << result << endl;
    return result;



}

long long minimumTime(vector<int>& times, int totalTrips) {

        sort(times.begin(), times.end());
        long long right = (long long )((long)times[0] * (long)totalTrips);
        long long left = 0;

        // 二分查找  不断缩小范围  寻找左边界  就是最小花费时间   当找不到的时候 右边界就是结果
        while (left < right) {

            long long mid = left + (right - left) / 2;

            if (fx(times, mid) >= totalTrips) {

                right = mid;
            } else if (fx(times, mid) < totalTrips) {

                left = mid + 1;
            }

            /* cout << "mid == " << mid << endl; */
        }

        return left;
}

void test(){

    /* vector<int> times = { 1, 2, 3}; */
    /* int totalTrips = 5; */
    /* vector<int> times = {2}; */
    /* int totalTrips = 1; */

    vector<int> times = { 88690,69213};
    int totalTrips = 90165;

    cout << minimumTime(times, totalTrips) << endl;

}

int main(){
    test();
    return 0;
}
