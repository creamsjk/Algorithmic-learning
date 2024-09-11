/*************************************************************************
    > File Name: lecode3174.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年09月05日 星期四 09时16分52秒
 ************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <math.h>
#include <climits>


using namespace std;

string clearDigits(string s){

  
    for(size_t i = 0;i < s.size(); ++i){

        if(s[i] <= '9'){
            s[i] = 'A';
            for(int  j = i; j >=0 ; j--){
                
                if(s[j] > '9' && s[j] != 'A'){

                    s[j]= 'A';
                    break;
                }
            }
        }
    }

    string result ;
    for(auto &i: s)
        if(i != 'A')
            result.push_back(i);


  
    return result;

}




void test(){

    /* string s = "cb34"; */
    string s = "ac3";
    cout << clearDigits(s) << endl;

}

int main(){
    test();
    return 0;
}
