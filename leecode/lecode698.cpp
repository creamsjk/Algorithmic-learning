/*************************************************************************
    > File Name: lecode698.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月26日 星期一 10时57分16秒
    方法
        回溯?

        失败没有写出来
        超时了但是

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

vector<int> index;
vector<bool> use;

void pathtrack(vector<int> &nums, int sum, int k, vector<bool> &use){

    if(sum == k){
        for(auto &i:index){
            cout << i << " ";
            nums[i] = -1;
            use[i] = false;
        }
        cout << endl;
        return;
    }else if (sum > k){

        return;
    }

    for(int i=0; i<nums.size(); ++i){

         if(nums[i] == -1)
             continue; 

         if(use[i] == false)
             continue;

            index.push_back(i);
            use[i] = false;
            
            pathtrack(nums, sum+nums[i], k, use);
        
            if(sum + nums[i] == k){
                sum = 0;
                index.clear();
                break;
            }
            index.pop_back(); 
            use[i] = true;
    }

    
}

//桶视角
bool pathtrack2(vector<int> &nums, int start, vector<int> &index, int k, int target, vector<bool> &use){

    if(k == 0) return true;

    if(index[k] == target ){

        return pathtrack2(nums, 0, index, k-1, target, use);
    }

    for(int i= start; i< nums.size(); ++i){
       
        if(use[i])
            continue;

        if(index[k] + nums[i] > target)
            break;


        index[k] += nums[i];
        use[i] = true;

       if(pathtrack2(nums, i+1, index, k, target, use))
           return true;


        index[k] -= nums[i];
        use[i] = false;


    }

    return false;


}


int main(){
   // vector<int> nums = { 4, 3, 2, 3, 5, 2, 1};
    vector<int> nums = {1, 2, 3, 4};
    int k = 3;

    sort(nums.begin(), nums.end());
    use.resize(nums.size(), false);
    index.resize(k+1, 0);

    int sum = 0;
    for(auto &i: nums){
        sum += i;
    }

    if(sum % k != 0){
        cout<< "false" << endl;
        return 0;
    }
  //  pathtrack(nums, 0, sum/k,use);
   bool result =  pathtrack2(nums, 0, index, k, sum/k, use);

   // for(auto &i:nums)
   //     cout<< i << " " ;
   // cout << endl;

   cout << result << endl;




    return 0;
}
