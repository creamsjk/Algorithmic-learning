/*************************************************************************
    > File Name: lecode3106.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月27日 星期六 11时04分11秒
    方法:
         贪心算法
         到a是利益最大化的做法,如果能到a就到a
         到不了a就向下减最大值
    accept!!
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


  string s = "nk";
  int k = 87;

  int len = k;
  for(int i=0;i<s.size();i++){
      int one = 'a'+26-s[i];
      int two = s[i]-'a';

      cout<<"one = "<<one<<"two == "<<two<<endl;
      if(one <= two && len >= one){
          s[i] = 'a';
          len -= one;
      }
      else if(one > two && len >=two){
          s[i] = s[i]-two;
          len -= two;
      }
      else if(len < two || len < one){//到不了a就只能向下减
          s[i] = s[i] - len;
          break;
      }
      else{//len == 0
          break;
      }
  }

  cout<<s<<endl;




















    return 0;
}
