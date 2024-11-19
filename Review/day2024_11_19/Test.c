#include <stdio.h>
#include <stdlib.h>

struct student{
	int age;
	char name[20];
};


int test()
{
	struct student s;
	s.age = 10;
	// s.name = "lisa";  // 要用strcpy，

	return 0;
}

// 1.带头的单向循环链表
typedef struct Node
{
	int data;
	struct Node* next;
}linknode_t;

linknode_t* buyNode(int data)
{
	linknode_t* node = (linknode_t*)malloc(sizeof(linknode_t));
	if(node == NULL)
	{
		perror("malloc fail");
		return NULL;
	}

	node->data = data;
	node->next = NULL;

	return node;			
}

linknode_t* createEmptyLinklist()
{
	return buyNode(-1);
}

int insertIntoLinklist(linknode_t* p,int pos,int data)
{
	if(pos < 1)
	{
		printf("插入位置不合理!");
		return -1;
	}

	for(int i = 0;i < pos - 1;i++)
	{		
		p = p->next;
	}

	linknode_t* node = buyNode(data);

	linknode_t* next = p->next;
	p->next = node;
	node->next = next;

	return 0;
}

int isEmptyLinklist(linknode_t* p)
{
	return p->next == NULL;
}

int getLenthLinklist(linknode_t* p)
{
	int count = 0;

	while(p->next != NULL)
	{
		count++;
		p = p->next;
	}
	return count;
}

int deleteFromLinklist(linknode_t* p,int pos)
{
	if(pos <1)
	{
		printf("删除位置不合理!\n");
		return 0;
	}

	for(int i = 0;i < pos - 1;i ++)
	{
		p = p->next;
	}

	linknode_t* del = p->next;
	p->next = p->next->next;
	free(del);
	return 0;	
}

int searchFromLinklist(linknode_t* p,int data)
{
	int count = 1;
	while(p->next != NULL)
	{
		if(p->next->data == data)
		{
			return count;
		}
		count++;
		p = p->next;
	}
	return -1;
}

void clearLinklist(linknode_t* p)
{
	linknode_t* cur = p->next;
	while(cur != NULL)
	{
		linknode_t* del = cur;
		cur = cur->next;
		p->next = cur;
		free(del);
		del = NULL;
	}
}

void destoryLinklist(linknode_t* p)
{
	free(p);
}

void showLinkListlist(linknode_t* p)
{
	while(p->next != NULL)
	{
		printf("%d ",p->next->data);
		p = p->next;
	}
	printf("\n");
}


int main()
{

	linknode_t* list = createEmptyLinklist();
	insertIntoLinklist(list,1,1);
	insertIntoLinklist(list,2,2);
	insertIntoLinklist(list,1,3);

	showLinkListlist(list);
	printf("%d\n",searchFromLinklist(list,2));
	printf("%d\n",getLenthLinklist(list));
	deleteFromLinklist(list,1);

	showLinkListlist(list);

	clearLinklist(list);
	printf("%d\n",getLenthLinklist(list));
	destoryLinklist(list);

	return 0;
}