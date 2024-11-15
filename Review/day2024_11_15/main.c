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


int main()
{
	TestPopFront();
	return 0;
}