/*
 * 
 * 力扣528题 按权重随机选择 
 * 使用前缀和 + 二分查找实现 按权重随机选择
 * 思路: 使用前缀和把所有权重求出来
 *       使用random 生成一个 0 -- n-1的随机数字
 *       使用二分查找找到小于随机数那个数 的下标就是我们要找的那个数字
 *       over
 *
 *       success 思路正确 代码有一点点小问题
 *       二分补充: 查找是左边界  比如  1 3 5  查找4就需要  索引为2
 *                                            查找5也需要  索引为2
 *       int left = 0;
 *       int right = nunms.size();
 *       while(left < right){
 *         int mid = left + (right - left)/2;
           if(nums[mid] >= target )
              right = mid;
           else
               left = mid + 1;
 *       }//left == right;

*/
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

int main(){
   
    srand(time(NULL));
    vector<int> nums = {3,14,1,7};
    vector<int> perSum(nums.size(),0);
    perSum[0] = nums[0];
    int n = nums.size();

    for(int i=1;i<nums.size();i++){
        perSum[i] = nums[i] + perSum[i-1];
    }
int len =50;
while(len--){


    int r = rand()%perSum[n-1]+1;
    //cout<<r<<endl;
    
    int left = 0;
    int right = n;
    while(left < right){
        int mid = left + (right - left)/2;
        
        if(perSum[mid] > r){
            right = mid;
        }
        else if(perSum[mid] < r){
            left = mid + 1;
        }
        else{
            left = mid;
            break;
        }


    }//left == right

    cout<<left<<endl;

}
    return 0;
}
