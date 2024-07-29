/*************************************************************************
    > File Name: lecode682.cpp
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月29日 星期一 09时15分07秒
    方法:
        维护一个vector 就可以了 因为可能会有多个c操作 只用几个数字不行
        时间复杂度O(N)   空间复杂度O(N)
        accept
 ************************************************************************/



#include<iostream>
#include<stdlib.h>
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

 vector<string> operations = {"8373","C","9034","-17523","D","1492","7558","-5022","C","C","+","+","-16000","C","+","-18694","C","C","C","-19377","D","-25250","20356","C","-1769","-8303","C","-25332","29884","C","+","C","-29875","-15374","C","+","14584","13773","+","17037","-28248","+","16822","D","+","+","-19357","-26593","-8548","4776","D","-8244","378","+","-19269","-25447","18922","-16782","2886","C","-17788","D","-22256","C","308","-9185","-19074","1528","28424","D","8658","C","7221","-13704","8995","-21650","22567","-29560","D","-9807","25632","6817","28654","D","-18574","C","D","20103","7875","C","9911","23951","C","D","C","+","18219","-20922","D","-24923"};
 vector<int> index;
 int sum = 0;
 for(int i = 0; i< operations.size();i++){
            if(operations[i] == "C"){

                int n = index.size();
                int tmp =  index[n-1];
                sum -= tmp;
                index.pop_back();
            }
            else if(operations[i] =="D"){
                int n = index.size();
                int tmp = index[n-1]+index[n-1];
                sum += tmp;
                index.push_back(tmp);

            }
            else if(operations[i] == "+"){
                int n = index.size();
                int tmp = index[n-2] + index[n-1];
                sum += tmp;
                index.push_back(tmp);

            }
            else{
               int tmp =  atoi(operations[i].c_str());
               sum += tmp;
               index.push_back(tmp);

            }
        }



 cout<<sum<<endl;



    return 0;
}
