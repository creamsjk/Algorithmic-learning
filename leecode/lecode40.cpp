/*************************************************************************
    > File Name: lecode40.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月01日 星期四 09时15分05秒
     方法:
         我尼玛,面向结果编程 , 
         贪心 + 回溯 解决 修炼两年解决问题, 回溯得剪枝,不剪枝时间复杂度太高了!!!!!
      accept
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
int result = 0;

void pathtrack(vector<int> &nums, int index, int cnt, int sum){

 
    if( cnt <= 0){
        if(sum%2 == 0 && sum > result){
            result = sum;
           
        }
    printf("sum == %d \n", sum);

        return;
    }

    int n = nums.size();
    for(int i=index; i<n && cnt > 0 && i+cnt <=n ; i++){

        sum += nums[i];
        pathtrack(nums,index+1,cnt-1,sum);
        sum -= nums[i];
    }
    return ;

}


int main(){




    vector<int> cards = {9,5,9,1,6,10,3,4,5,1};
    int cnt = 2;

    sort(cards.rbegin(), cards.rend());
    
    pathtrack(cards, 0, cnt, 0);

    cout<<result<<endl;

    return 0;
}
