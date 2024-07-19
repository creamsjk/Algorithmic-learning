/*************************************************************************
    > File Name: sort_index.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月11日 星期四 22时18分45秒
  本程序功能 :
   根据下标排序
   sort(ids, ids + n, [&](int i, int j) { return nums2[i] < nums2[j]; });
   正常的话用这个排序就可以了
 ************************************************************************/

#include<iostream>
#include<vector>

using namespace std;

int main(void){

    vector<int> nums={1,3,0,0,2};

    vector<int> idx(nums.size(),0);
    for(int i=0;i<nums.size();i++){
       idx[i]=i;
    }

    //根据下标排序
    for(int i=0;i<nums.size();i++){
        for(int j=0;j<nums.size()-1;j++)
            if(nums[idx[j]] > nums[idx[j+1]]){
                int tmp =  idx[j];
                idx[j] = idx[j+1];
                idx[j+1] = tmp;
            }
    }
    for(int i:idx)
        cout<<i<<endl;

    return 0;
}

