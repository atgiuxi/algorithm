#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int DataType;

typedef struct SingleList
{
	DataType data;
	// struct SingleList next; // 这样定义不对
	struct SingleList* next;
}Node;


void PushBack(Node** pphead,DataType data);
void PrintList(Node* phead);
void PushFront(Node** pphead,DataType data);
void PopBack(Node** pphead);
void PopFront(Node** pphead);

Node* Find(Node* phead,DataType data);

void Insert(Node** pphead,Node* pos,DataType data);
void Erase(Node** pphead,Node* pos);