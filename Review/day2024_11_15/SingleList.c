#include "SingleList.h"

Node* BuyNode(DataType data)
{
	Node* node = (Node*)malloc(sizeof(Node));
	if(node == NULL)
	{
		perror("malloc create node fail\n");
		return NULL;
	}

	// 创建结点成功!初始化结点
	node->data = data;
	node->next = NULL;

	return node;
}

void PushBack(Node** pphead,DataType data)
{
	assert(pphead != NULL);
	Node* newNode = BuyNode(data);

	// 链表为空时需要单独去判断,不然tail = *pphead 为NULL,tail->next崩溃了
	if(*pphead == NULL) 
	{
		*pphead = newNode;
	}
	else 
	{
		Node* tail = *pphead;	// 让tail从头开始
		while(tail->next != NULL /*这里是循环的条件,当taile->next == NULL 循环结束找到尾*/)
		{
			tail = tail->next;
		}
		// 链接结点
		tail->next = newNode;	
	}
}

void PrintList(Node* phead)
{
	Node* cur = phead;
	while(cur != NULL)
	{
		printf("%d->",cur->data);
		cur = cur->next;	
	}
	printf("NULL\n");
}

void PushFront(Node** pphead,DataType data)
{
	assert(pphead != NULL);
	Node* newNode = BuyNode(data);

	// 直接让newNode指向头,*pphead为NULL也不怕,然后更新头节点
	newNode->next = *pphead;	
	*pphead = newNode;
}

void PopBack(Node** pphead)
{
	assert(pphead);
	assert(*pphead != NULL); // 链表为NULL,那就不能删了

	// 尾删的时候是找到为尾部的前一个,让perv->next = NULL.所以只有一个结点的时候单独处理.
	if((*pphead)->next == NULL)
	{
		free(*pphead);
	}
	else
	{
		Node* prev = *pphead;

		while(prev->next->next != NULL) {prev = prev->next;}

		free(prev->next);
		prev->next = NULL;
	}
}

void PopFront(Node** pphead)
{
	assert(pphead != NULL);
	assert(*pphead != NULL);

	Node* next = (*pphead)->next;
	free(*pphead);
	*pphead = next;
}


Node* Find(Node* phead,DataType data)
{
	Node* cur = phead;

	while(cur != NULL)
	{
		if(cur->data == data) return cur;
		cur = cur->next;
	}
	return NULL;	
}

void Insert(Node** pphead,Node* pos,DataType data)
{
	assert(pphead);
	assert(pos);

	if(pos == *pphead){PushFront(pphead,data);}
	else
	{
		Node* prev = *pphead;
		// 一般来说: Insert接口是配合Find接口使用的,传入进来的pos要么是pos要么是链表中的结点
		// 如果pos不是链表中的元素,查找的时候就会出错.
		while(prev->next != pos){ prev = prev->next; }

		Node* newNode = BuyNode(data);
		prev->next = newNode;
		newNode->next = pos;
	}
}

void Erase(Node** pphead,Node* pos /*删除pos位置*/)
{
	assert(*pphead != NULL);
	assert(pos);

	if(*pphead == pos){PopFront(pphead);}
	else
	{
		Node* prev = *pphead;
		// 一般来说: Erase接口也是配合Find接口使用的,传入进来的pos要么是pos要么是链表中的结点
		while(prev->next != pos){ prev = prev->next; }

		prev->next = pos->next;
		free(pos);
		pos = NULL;
	}
}