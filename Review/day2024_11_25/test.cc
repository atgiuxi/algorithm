#include <vector>
#include <stack>


using namespace std;
class Solution 
{
public:
    vector<int> nextGreaterElements(vector<int>& nums) 
    {
        vector<int> ret(nums.size(),-1);

        stack<int> st;

        for(int i = 0;i < nums.size() * 2;i++)
        {
            while(!st.empty() && nums[i % nums.size()] > st.top())
            {
                ret[st.top()] = nums[i % nums.size()];
                st.pop();
            }
            st.push(i % nums.size());
        }
        return ret;
    }
};



int main()
{


	Solution s;
	vector<int> nums1 = {1,2,1};

    s.nextGreaterElements(nums1);

	return 0;
}
