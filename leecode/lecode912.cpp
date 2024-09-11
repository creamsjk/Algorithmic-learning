/*************************************************************************
  > File Name: lecode912.cpp
  > Author: sunrie
  > Mail: 1102490208@qq.com 
  > Created Time: 2024年08月21日 星期三 22时29分29秒
  方法:
     归并排序加深理解

     通过
 ************************************************************************/

#include<iostream>
#include<string>
//#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>

using namespace std;

vector<int> tmp;
void merge(vector<int> & nums, int left, int mid, int right){


    for(int i=left; i<=right; ++i){

        tmp[i] = nums[i];
    }

    int i = left;
    int j = mid + 1;
    

    for(int h = left ; h <= right ;  ++h){
        
        if( i <= mid && j <= right &&  tmp[i] <= tmp[j] ){
            nums[h] = tmp[i];
            i++;
        }else if ( i <= mid && j <= right && tmp[j] <= tmp[i] ){
            nums[h] = tmp[j];
            j++;
        }else if( j> right){
            nums[h] = tmp[i];
            i++;
        }else if( i >mid){
            nums[h] = tmp[j];
            j++;
        }
    }
   

}

void sort(vector<int>& nums, int left, int right){
    if(left == right){
        return;
    }

    int mid = left + (right - left)/2;

    sort(nums, left, mid);

    sort(nums, mid + 1, right);

    merge(nums,left, mid, right);


}
vector<int> sortArray(vector<int>& nums) ;


int main(){

   //vector<int> nums = { 2,3, 1, 4};
   
   vector<int> nums = { 5,1,1,2,0,0};

   tmp.reserve(nums.size());

   //merge(nums, 0, 1, 3);
   sort(nums,0, nums.size() - 1);

   for(auto i: nums)
       cout << i <<" ";
   cout << endl;


    return 0;
}
