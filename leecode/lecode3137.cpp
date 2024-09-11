/*************************************************************************
    > File Name: lecode3137.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月17日 星期六 09时41分23秒
    方法:
       以k为间隔将word  进行分隔并放入hashmap 然后统计 最多的那个是多少
       剩下的都要改变 因为整除  所以最小个体是k

       通过  时间复杂度O(N)
             空间复杂度O(N)
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

    //string word = "leetcodeleet";
    string word = "leetcoleet";
    int k = 2;
    int result = 0;
    unordered_map<string, int> index;
    int max_index = 0;

    for(int i=0 ; i<word.size(); i+=k){

        string s1(word.begin()+i, word.begin() + k +i );
       // cout << s1 <<endl;
        index[s1]++;
        if(index[s1] > max_index)
            max_index = index[s1];
    }
    //cout<<max_index<<endl;
    result = word.size()/k - max_index;
    cout<<result<<endl;

    return 0;
}
