/*************************************************************************
    > File Name: lecode47.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月17日 星期三 14时11分48秒
    > Describe: 给定一个可包含重复数字的序列 nums 
                按任意顺序 返回所有不重复的全排列。
      method: 回溯算法!!!
      accept 
              2024年 07月 17日 星期三 15:03:38 CST
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

vector<vector<int>> result;
vector<int> tmp;
vector<bool> isValid;

//回溯函数
void backtrack(const vector<int>& nums, int index){

    //退出条件
    if(tmp.size() == nums.size()){
        result.push_back(tmp);
        return;
    }

    //循环主体 循环 nums.size()次
    for(int i = index;i< index + nums.size();i++){
        int n = nums.size();
        int j=i%n;
        //如果这个数被加入过就跳过
        if(isValid[j]){
            continue;
        }
        
        //去重剪枝 如果这两个数相同 并且左边那个还在tmp数组里面
        //再加进去就会造成重复,因为排过序,所以相同元素一定相邻
        if( j>0 && nums[j] == nums[j-1] && !isValid[j-1] ){
            continue;
        }

        //不断枚举回溯
        isValid[j] = true;
        tmp.push_back(nums[j]);
        backtrack(nums,index+1);
        tmp.pop_back();
        isValid[j] = false;
    }
}

//打印result数组 方便调试
void pri(const vector<vector<int>> res){

    for(vector<int> i:res){
      for(int j:i){
          cout<<" "<<j;
      }
      cout<<endl;
    }
}

int main(){
    vector<int> nums={1,1,2};

    //初始化
    isValid.assign(nums.size(),false);
    result.clear();
    tmp.clear();
    sort(nums.begin(),nums.end());
    
    //开始回溯
    backtrack(nums,0);

   pri(result);
   cout<<result.size()<<endl;
    return 0;
}



















