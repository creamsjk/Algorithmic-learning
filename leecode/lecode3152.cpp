/*************************************************************************
    > File Name: lecode3152.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月14日 星期三 09时43分24秒
    方法:
        使用前缀和  看灵神的
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


int main(){

    vector<int> nums = {4, 3, 1, 6};
    vector<vector<int>> queries = { {0, 2}, {2, 3}};

    
    vector<int> index;
    index.push_back(0);
    int all = 0;

    for(int i=1; i< nums.size(); ++i){
        if((nums[i - 1] & 1) == (nums[i] & 1))
            index.push_back(++all);
        else 
            index.push_back(all);
        
    }

    vector<bool> result;

    for(auto i : queries){

        if(index[i[0]] == index[i[1]])
            result.push_back(1);
        else
            result.push_back(0);

        
    }

    cout<<endl;
    for(auto i : index){

        cout<<i<<endl;
    }

     

    cout<<endl;
    for(auto i : result){

        cout<<i<<endl;
    }



    return 0;
}
