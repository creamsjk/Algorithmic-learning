#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
#include<climits>

//ac成功  得使用一点数学办法不然容易超时 时间复杂度O(N)
//空间复杂度O(1)
using namespace std;


int main(){

    vector<int> nums = {1,0,1,0};
    long long  len = 0;
    for (long long  i = 0, j = 0; j < nums.size();) {

        if (i == j)
            len++;
        else if ( i<nums.size() && nums[i] != nums[i - 1]) {
            len++;
        }
        else {
            len += ((i + 1 - j) * (i - j) / 2)-(i-j);

                j = i;
            continue;
        }
        i++;


    }
   cout<<len<<endl;
    return 0;
}
