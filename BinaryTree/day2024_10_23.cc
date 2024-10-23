#include <iostream>
#include <vector>

using namespace std;

struct TreeNode 
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// leetcode 404.左叶子之和
class Solution1 {
public:
    int ret = 0;
    
    int sumOfLeftLeaves(TreeNode* root) 
    {
        dfs(nullptr,root);
        return ret;
    }

    void dfs(TreeNode* parent,TreeNode* cur)
    {
        if(cur == nullptr) return;
        // 1、判断是叶子 2、判断是左叶子，注意parent != nullptr
        if(cur->left == nullptr && cur->right == nullptr && 
            parent != nullptr && cur == parent->left) ret += cur->val;

        dfs(cur,cur->left);
        dfs(cur,cur->right);
    }
};

// leetcode.513. 找树左下角的值
// 给定一个二叉树的 根节点 root，请找出该二叉树的 最底层 最左边 节点的值。假设二叉树中至少有一个节点。
class Solution2 {
public:
    TreeNode* ret = nullptr;
    int maxDepth = -1;

    int findBottomLeftValue(TreeNode* root) 
    {
        dfs(0,root);
        return ret->val;
    }

    void dfs(int depth,TreeNode* root)
    {
        if(root == nullptr) return;

        // 根据深度更新，不需要判断是不是叶子，到叶子也会统计
        if(depth > maxDepth)
        {
            ret = root;
            maxDepth = depth;
        }
        
        // 注意顺序，先是左子树！
        dfs(depth + 1,root->left);
        dfs(depth + 1,root->right);
    }
};


int main()
{

    return 0;    
}