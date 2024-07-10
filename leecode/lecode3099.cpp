#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>

using namespace std;


int main(){
    int x = 23;
    int start = x;
    int sum = 0;
    while(x>0){
        int tmp = x%10;
        sum += tmp;
        x /= 10;
    }

    if(start%sum == 0)
    cout<<sum<<endl;
   else 
       cout<<-1<<endl;
    
    return 0;
}
