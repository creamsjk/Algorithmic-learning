/*************************************************************************
    > File Name: lecode1870.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年10月06日 星期日 09时10分01秒
    

    yes 通过!!!!

 ************************************************************************/

#include <iostream>
#include <iomanip>　
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


double fx(const vector<int> &dist, int speed){ 

    double result = 0;
    for(size_t i = 0; i < dist.size() - 1; ++i){ 

        double costs_time = dist[i]/(double)speed;
        
        int tmp = costs_time;
        if(costs_time > tmp)
            tmp = tmp + 1;

        /* cout << "costs_time=" << costs_time << "tmp=" << tmp << endl; */
        result += tmp;
    }

    auto ss = dist.end() - 1;
    result +=((*ss)/ (double)speed);
    /* cout << *ss << endl; */
    /* while(1); */

    /* cout<<  result  << "------"<<setprecision(2); */
    cout <<"result=" <<  result << "speed=" << speed << endl;
    
    return result;
}


int minSpeedOnTime(vector<int>& dist, double hour) {
    
    if(dist.size() - 1 >= hour)
        return -1;

    int right = -1;
    int left = 1;
    for(auto &j:dist){ 
        if(j > right)
            right = j;
    }
    double tmp = hour -(int)hour;
    int max_speed = right/tmp;
    right = max_speed + 10;
    cout << "right= " << right  << endl;

    while(left < right){ 

        int mid = left + (right - left)/2;

        if(fx(dist, mid) > hour){ 

            left = mid + 1;
        }else{ 

            right = mid;
        }
    }

    return left;


}


void test(){

    /* vector<int> dist = { 1, 3, 2}; */
    /* double hour = 6; */
    /* double hour = 2.7; */

    /* vector<int> dist = { 1,1,100000}; */
    /* double hour = 2.01; */
    vector<int> dist = { 1, 5};
    double hour = 1.09;
    cout << minSpeedOnTime(dist, hour) << endl;
}

int main(){
    test();
    return 0;
}
