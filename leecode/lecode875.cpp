#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>

//success accept

using namespace std;
int f(const vector<int>& nums, int n){

     int h = 0;
    for(int i=0;i<nums.size();i++){
       if(n > nums[i]){
           h++;
       }
       else{
           h += (nums[i]+n-1)/n;
       }
    }
    return  h;
}

int main(){
    
    vector<int> piles = {30,11,23,4,20};
    int h = 6;
    int left = 1;
    int right = -1;

    for(int i=0;i<piles.size();i++){
        if(piles[i] > right)
            right =  piles[i];
    }
    right += 1;

     int res = 0;
    while(left < right){
        int mid = left + (right - left)/2;
        if(f(piles,mid) == h){
            right = mid;
        }
        else if(f(piles,mid) > h){
            left = mid + 1;
        }
        else if(f(piles,mid) < h){
           right = mid;
        }
        cout<<left<<"  "<<right<<endl;
    }
    cout <<left<<endl;

    return 0;
}
