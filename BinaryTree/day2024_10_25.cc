#include <iostream>
#include <vector>

using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// leetcode.面试题 08.06. 汉诺塔问题
class Solution1
{
public:
    void hanota(vector<int> &A, vector<int> &B, vector<int> &C)
    {
        dfs(A, B, C, A.size());
    }

    // a 代表有盘子的柱子
    // b 代表中间柱子
    // c 代表目标柱子
    void dfs(vector<int> &a, vector<int> &b, vector<int> &c, int n)
    {
        if (n == 1)
        {
            c.push_back(a.back());
            a.pop_back();
            return;
        }

        // 把a柱子中n -1的盘中，通过c柱子搬到b柱子中
        dfs(a, c, b, n - 1);
        // 将a柱子底下的盘子，放到目标柱子
        c.push_back(a.back());
        a.pop_back();
        // 把b柱子上的东西通过a柱子搬到c柱子中
        dfs(b, a, c, n - 1);
    }
};

// leetcode.21. 合并两个有序链表
class Solution2
{
public:
    ListNode *mergeTwoLists(ListNode *list1, ListNode *list2)
    {
        // 1.如果有个为空，直接返回另一个
        if (list1 == nullptr)
            return list2;
        if (list2 == nullptr)
            return list1;

        // 2.如果list1->val <= list2->val,让list1->next,和list2合并。
        if (list1->val <= list2->val)
        {
            list1->next = mergeTwoLists(list1->next, list2);
            return list1;
        }
        else
        {
            list2->next = mergeTwoLists(list1, list2->next);
            return list2;
        }
    }
};

// leetcode.206. 反转链表
class Solution3
{
public:
    ListNode *reverseList(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
            return head;
        ListNode *newHead = reverseList(head->next);

        // newHead->next = head; // 错误
        // 带入 3->4->5
        head->next->next = head;
        head->next = nullptr;

        return newHead;
    }
};

// leetcode.24. 两两交换链表中的节点
class Solution4
{
public:
    ListNode *swapPairs(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
            return head;

        ListNode *newHead = swapPairs(head->next->next);

        ListNode *ret = head->next; // 注意这个！！！
        head->next->next = head;
        ;
        head->next = newHead;

        return ret;
    }
};
// 50. Pow(x, n)
class Solution5 {
public:
    // 正数：直接 x * myPow(x,n-1) 这样有点慢
    double myPow(double x, int n) {
        // 当n ==-2^31墙转成int会 
        return n >=0 ? Pow(x,n) : 1 / Pow(x,(long long)n);
    }
    // 折半求，但是会遇到奇数和偶数的情况。
    double Pow(double x,int n)
    {
        if(n == 0) return 1.0;

        double mid = Pow(x, n / 2);
        return n % 2 == 0 ? mid * mid : mid * mid * x; 
    }
};  

// 2331. 计算布尔二叉树的值.这里一个后序遍历就可以了
class Solution6 {
public:
    bool evaluateTree(TreeNode* root) 
    {
        if(root == nullptr )return true;
        if(root->left == nullptr && root->right == nullptr) return root->val == 0 ? false : true;

        bool left = evaluateTree(root->left);
        bool right = evaluateTree(root->right);

        bool ret = false;
        if(root->val == 2) ret = left || right;
        else if(root->val == 3) ret = left && right;

        return ret;
    }
};