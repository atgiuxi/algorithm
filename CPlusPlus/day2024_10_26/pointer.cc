#include <stdio.h>
#include <iostream>
#include "SingleList.h"

using namespace std;

// 一级指针：char*字符指针代表
void test1()
{
    // ptr存放的是字符常量的首元素（字符）的地址。
    const char* ptr = "hello word";

    char ptr1[] = "hello world";
    char ptr2[] = "hello world";

    const char* ptr3 = "hello world";
    const char* ptr4 = "hello world";

    // ptr1 和 ptr2 用相同的常量去初始化，会开辟不同的空间
    // ptr3 和 ptr4 指向的是一块内存空间（相同的常量只有一份）
    printf("the ptr1 : %p , the ptr2 : %p\n",ptr1,ptr2);
    printf("the ptr3 : %p , the ptr4 : %p\n",ptr3,ptr4);
}

// 二级指针：以单链表为例子
void test2()
{
    // 这里创建单链表的头节点
    // 为什么不Node pHead = {0,NULL};
    Node* pHead = NULL;

    PushBack(&pHead,1);
    PushBack(&pHead,2);
    PushBack(&pHead,3);

    PopBack(&pHead);

    Print(pHead);
}

int sum(int a,int b)
{
    return a + b;
}

// 定义函数指针
int (*funPtr)(int,int);
// 函数指针
void test3()
{
    funPtr = sum;

    cout << "use funPtr : " << funPtr(5,10) << endl;
}

int cmp_int(const void* a,const void* b)
{
    int *p1 = (int*)a;
    int *p2 = (int*)b;
    return *p1 - *p2;
}

void test4()
{
    int arr[] = {9,1,2,5,8,3};
    int len = sizeof(arr)/sizeof(arr[0]);

    qsort(arr,len,sizeof(arr[0]),cmp_int);  

    for(int i = 0;i < len;i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}


void test5()
{
    int arr[10] ={0};
    int (*ptr)[10] = &arr; 
}


int main()
{
    test2();

    return 0;
}