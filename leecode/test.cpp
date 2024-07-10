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
    vector<int> s = { 3,4,6,1,22,3,45,67,89};
    sort(s.begin(),s.end());
    for(int i: s)
        cout<<i<<endl;
    return 0;
}
