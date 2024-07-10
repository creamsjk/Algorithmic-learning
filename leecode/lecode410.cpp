#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>

//成功ac  和1011题一样 转换一下子很快就做出来了
using namespace std;

//每次做多运输n个东西 看看需要几次才能运输完毕
//题目是真的绕圈啊  头晕 淦
int f(vector<int> &nums, int n){

    int res = 0;
    int sum = 0;
    for(int i=0;i<nums.size();i++){
        if(sum < n && sum + nums[i] < n){
            sum += nums[i];
        }
        else if( sum < n && sum + nums[i] > n){
            sum = 0;
            res++;
            sum += nums[i];
        }
        else if((sum + nums[i])  == n){
            res++;
            sum = 0;
        }
        else{
            sum += nums[i];
        }
    }
    if(sum > 0)
        res++;
   // cout<<n<<"  "<<res<<endl;
    return res;
}



int main(){
    int k = 2;
    vector<int> nums = {1,2,3,4,5};
    int left = 0;
    int right = 0;
    for(int i:nums){
        if( i > left)
            left = i;
        right += i;
    }
        
    right = right +1;
    while(left < right ){
        int mid = left + (right - left )/2;
       // cout<<f(nums,mid)<<"  "<<mid<<endl;
       
        if(f(nums,mid) > k){
            left = mid + 1;
        }
        else{
            right = mid;
        }
        

    }
    cout<<left<<endl;
    
    return 0;
}
