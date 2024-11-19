#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int DataType;

#define INIT_CAPACITY 5

typedef struct SeqList
{
	int *arr;		// 指向动态开辟空间
	int size;		// 顺序表中存储的数据
	int capacity;	// 顺序表的存储容量
}SeqList;

void Init(SeqList* seq);
void Destroy(SeqList* seq);
void CheckCapacity(SeqList* seq);
void Print(SeqList* seq);
void Insert(SeqList* seq,int pos,DataType data);
void Erase(SeqList* seq,int pos);