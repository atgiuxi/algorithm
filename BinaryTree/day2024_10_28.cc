#include <vector>
#include <iostream>

using namespace std;

// leetcode.1863. 找出所有子集的异或总和再求和
class Solution1 {
public:
    int ret = 0;
    int path = 0;
    
    int subsetXORSum(vector<int>& nums) 
    {
        dfs(nums,0);
        return ret;
    }

    void dfs(vector<int>& nums,int index)
    {
        ret += path;

        for(int i = index;i < nums.size();i++)
        {
            // 相同的两个数异或为0
            path ^= nums[i];
            dfs(nums,i + 1);
            path ^= nums[i];
        }
    }
};





