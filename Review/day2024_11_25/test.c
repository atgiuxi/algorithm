#include <stdlib.h>
#include <stdio.h>

typedef struct node{
    char data;
    struct node* left;
    struct node* right;
}tree_node_t;

tree_node_t* buy(int x)
{
    tree_node_t* node = (tree_node_t*)malloc(sizeof(tree_node_t));

    node->data = x;
    node->left = NULL;
    node->right =NULL;

    return node;
}

tree_node_t* create()
{
    tree_node_t* A = buy('A');
    tree_node_t* B = buy('B');
    tree_node_t* C = buy('C');
    tree_node_t* D = buy('D');
    tree_node_t* E = buy('E');
    tree_node_t* F = buy('F');

    A->left = B;A->right = C;
    C->left = E;C->right = F;
    B->left = D;

    return A;
}

void perorder(tree_node_t* p)
{
    if(p == NULL) return;

    printf("%c ",p->data);

    perorder(p->left);
    perorder(p->right);
}

tree_node_t* createTree()
{
    char ch;
    scanf("%c",&ch);

    if(ch == '#') return NULL;

    tree_node_t* root = buy(ch);

    root->left = createTree();
    root->right = createTree();

    return root;
}

void freeBinTree(tree_node_t* p)
{
    if(p == NULL) return;

    freeBinTree(p->left);
    freeBinTree(p->right);

    free(p);
}

void bubbleSort(int* arr,int left,int right)
{}

void inserSort(int* arr,int left,int right)
{
    for(int i = 0;i < right;i++)
    {
        for(int j = i;j > 0;j--)
        {
            if(arr[j] < arr[j-1])
            {
                int tmp = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = tmp;
            }
            else{break;}
        }
    }
}

void swap(int* a,int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void quick_sort(int* arr,int begin,int end)
{
    if(begin >= end) return;
    int keyi = begin;
    int left = begin,right = end;
    while(left < right)
    {
        while(left < right && arr[right] >= arr[keyi]) { right--;}
        while(left < right && arr[left] <= arr[keyi]){left++;}

        swap(&arr[left],&arr[right]);
    }

    swap(&arr[left],&arr[keyi]);

    quick_sort(arr,begin,left -1);
    quick_sort(arr,left + 1,end);
}


int main()
{
    int arr[6] = {2,1,1,3,1,9};
    quick_sort(arr,0,5);

    for(int i = 0;i < 6;i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");
    return 0;
}

