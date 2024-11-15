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
	assert(*pphead != NULL);

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
	assert(*pphead);
	Node* newNode = BuyNode(data);

}