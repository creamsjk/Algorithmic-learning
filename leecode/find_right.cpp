#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>

//查找右边界的二分查找
using namespace std;


int main(){
    vector<int> nums={2,3,4,5,6};
    int target = 7;
    int left = 0;
    int right = nums.size();
    while(left < right){
    int mid = left + (right - left)/2;
    if(nums[mid] > target){
        right = mid;
    }
    else if(nums[mid] < target){
        left = mid + 1;
    }
    
}//结束条件 left== right

  cout<<left<<endl;
    return 0;
}
