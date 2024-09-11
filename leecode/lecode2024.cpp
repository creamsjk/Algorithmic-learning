/*************************************************************************
  > File Name: lecode2024.cpp
  > Author: sunrie
  > Mail: 1102490208@qq.com 
  > Created Time: 2024年09月02日 星期一 09时56分52秒
  方法:
        滑动窗口
         
        accept
        时间复杂度 O(N)
        空间复杂度 O(1)
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

int maxConsercutiveAnswers(string answerKey, int k){

    int maxResult = 0;
    int left = 0;
    int right = 0;
    int tVal = 0;
    int fVal = 0;

    while(right < answerKey.size()){

        if(answerKey[right++] == 'T'){
            tVal++;
        }else{
            fVal++;
        }


        while(tVal > k && fVal >k){

            if(answerKey[left++] == 'T'){
                tVal--;
            }else{
                fVal--;
            }

            
        }

        maxResult = max(maxResult, fVal + tVal);
    }

    return maxResult;

}

void test(){

    string answerKey = "TFFT";
    int k = 1;
    int result =  maxConsercutiveAnswers(answerKey, k);
    cout << result << endl;

}


int main(){
    test();
    return 0;
}
