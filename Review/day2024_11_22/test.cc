#include <unordered_map>
#include <vector>
#include <stack>


using namespace std;


class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) 
    {
        unordered_map<int,int> map;
        for(int i =0;i < nums1.size();i++)
        {
            map[nums1[i]] = i;
        }
        vector<int> ret(nums1.size(),-1);

        stack<int> st;

        st.push(0);

        for(int i = 1;i < nums2.size();i++)
        {
            while(!st.empty() && nums2[i] > nums2[st.top()])
            {
                if(map.count(nums2[st.top()]) > 0)
                {
                    ret[nums2[st.top()]] = nums2[i];
                }
                st.pop();
            }
            st.push(nums2[i]);
        }

        return ret;
    }
};

int main()
{

	Solution s;
	vector<int> nums1 = {4,1,2};
	vector<int> nums2 = {1,3,4,2};
	s.nextGreaterElement(nums1,nums2);
	

	return 0;
}
