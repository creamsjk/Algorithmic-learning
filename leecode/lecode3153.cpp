/*************************************************************************
    > File Name: lecode3153.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月30日 星期五 09时23分47秒
    方法:
        使用一个数组来统计 每一个数字  的同一位数字
        然后 进行计算 这样时间复杂度就降低了

        accept 通过
 ************************************************************************/

#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>


using namespace std;

/* int diffnum(int a, int b){ */

/*     int diff =  0; */

/*     while( a >0 && b>0){ */
/*         int fa = a%10; */
/*         int fb = b%10; */
/*         if(fa != fb) */
/*            ++diff; */
/*         a /=10; */
/*         b /=10; */
/*     } */
/*     return diff; */
/* } */

int diffnum(int &a){
 
    int result = a%10;
    a /=10;
    return result;
}


long long sumDigitDifferences(vector<int> &nums){

    long long result = 0;

    vector<int> index = nums;

    vector<int> all(10, 0);
    int sum_num = 0;
    while(1){

        for(auto &i : index){

            if(i <= 0 )
                continue;

            int tmp = diffnum(i);
            all[tmp]++;
            sum_num++;
        }
        
        if(sum_num == 0)
            break;

        for(auto &i: all){

            sum_num -= i;
            result += i * sum_num; 
            i = 0;
        }
    }

    return result;


}


int main(){

    vector<int> nums = {
        10, 10, 10
    };

    cout << sumDigitDifferences(nums) << endl;


    return 0;
}
