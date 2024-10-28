#include <stdio.h>
#include <stdlib.h>


// 1.定义单链表节点。单链表节点中有什么
// 数据、指向下一个节点的指针
typedef struct Node
{
    int data;
    struct Node* next;
}Node;


void PushBack(Node** ppHead,int data);
void PopBack(Node** ppHead);
void Print(Node* phead);
