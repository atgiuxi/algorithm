#include "SingleList.h"

int TestPushBack()
{
	Node* plist = NULL;	

	PushBack(&plist,1);
	PushBack(&plist,2);
	PushBack(&plist,3);
	PushBack(&plist,4);
	PushBack(&plist,5);

	PrintList(plist);
	return 0;
}

int TestPushFront()
{
	Node* plist = NULL;	

	PushFront(&plist,1);
	PushFront(&plist,2);
	PushFront(&plist,3);
	PushFront(&plist,4);
	PushFront(&plist,5);

	PrintList(plist);
	return 0;
}

int TestPopBack()
{
	Node* plist = NULL;	

	PushBack(&plist,1);
	PushBack(&plist,2);
	PushBack(&plist,3);
	PushBack(&plist,4);
	PushBack(&plist,5);

	PrintList(plist);
	PopBack(&plist);
	PrintList(plist);
	PopBack(&plist);
	PopBack(&plist);

	PrintList(plist);

	return 0;
}
int TestPopFront()
{
	Node* plist = NULL;	

	PushFront(&plist,1);
	PushFront(&plist,2);
	PushFront(&plist,3);
	PushFront(&plist,4);
	PushFront(&plist,5);

	PrintList(plist);
	PopFront(&plist);
	PrintList(plist);
	PopFront(&plist);
	PopFront(&plist);
	PopFront(&plist);
	PrintList(plist);
	return 0;
}

int TestInsert()
{
	Node* plist = NULL;	

	PushFront(&plist,1);
	Insert(&plist,plist,2);
	Insert(&plist,plist,3);
	Insert(&plist,plist,4);

	Node* pos = Find(plist,3);

	Insert(&plist,pos,5);	
	Insert(&plist,pos,6);	
	Insert(&plist,pos,7);	

	PrintList(plist);
}


int TestErase()
{
	Node* plist = NULL;	

	PushFront(&plist,1);
	Insert(&plist,plist,2);
	Insert(&plist,plist,3);
	Insert(&plist,plist,4);

	PrintList(plist);
	Node* pos = Find(plist,3);
	Erase(&plist,pos);

	PrintList(plist);
}

extern Node* BuyNode(DataType);

void TestOtherNode()
{	
	Node* node = BuyNode(5);
	Node* plist = NULL;	

	PushBack(&plist,1);
	PushBack(&plist,2);
	PushBack(&plist,3);
	PushBack(&plist,4);
	PushBack(&plist,5);

	// 测试Insert不和find使用,就出错了.
	Insert(&plist,node,6);
}

int main()
{
	TestOtherNode();
	return 0;
}