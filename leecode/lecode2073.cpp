/*************************************************************************
    > File Name: lecode2073.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月29日 星期日 09时11分11秒

    正常做法
    时间复杂度 O(N)
    通过
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

int timeRequiredToBuy(vector<int>& tickets, int k) {

    long long res = 0;
    int tmp = tickets[k];

    for(int i = 0; i < (int)tickets.size(); ++i){ 

        if(tickets[i] >= tmp){ 

            res  += tmp;
            tickets[i] -= tmp;

            if(i > k)
                --res;

        }else if ( tickets[i] >= 0 ){ 

            res += tickets[i];
            tickets[i] = 0;
        }
    }

    return res;

}


void test(){

    /* vector<int> tickets = { 2 , 3 , 2}; */
    /* int k = 2; */
    /* vector<int> tickets = { 5, 1, 1, 1}; */
    /* int k = 0; */
    vector<int> tickets = { 84,49,5,24,70,77,87,8};
    int k = 3;

    cout << timeRequiredToBuy(tickets, k) << endl;

}

int main(){
    test();
    return 0;
}
