/*************************************************************************
    > File Name: lecode215.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月31日 星期六 17时26分51秒
    方法
        使用快速排序 每次只排序包含k的数组

        我草超时了  头晕 
        通过

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

int mySort(vector<int> &nums, int k, int left, int right){


    //快速排序
    int start = nums[left];
    int tmp = nums[left];

    int l = left;
    int r = right;

    while(l < r){
        
        while(l < r && nums[r] > start)
            r--;
        nums[l] = nums[r];

        while(l < r && nums[l] <= start)
            l++;
        nums[r] = nums[l];
    }
    if(l == r )
    nums[l] = tmp;

     if(l == k)
         return nums[l];
    if( l > k)
       return  mySort(nums, k, left, l-1);
    else
      return  mySort(nums, k, l + 1, right);


}

void test(){

  vector<int> nums = {
      2,1
  };
  int k = 1;

  int new_k = nums.size() - k;
  /* cout << new_k << endl; */

   int result = mySort(nums, new_k, 0 , nums.size()-1);
  cout << result << endl;

  for(auto i: nums)
      cout << i << " ";
  cout << endl;


}

int main(){

    test();
    return 0;
}
