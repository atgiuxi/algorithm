#include <stdio.h>
#include <stdlib.h>


typedef struct stack
{
	int *arr;
	int top;
	int max_size;
}stack_t;

stack_t* createEmptyStack(int len)
{
	stack_t* st = (stack_t*)malloc(sizeof(stack_t));

	if(st == NULL)
	{
		perror("malloc fail");
		return NULL;
	}

	int* arr = (int*)malloc(sizeof(int) * len);
	if(st == NULL)
	{
		perror("malloc fail");
		return NULL;
	}

	st->arr = arr;
	st->max_size = len;
	st->top = 0;

	return st;
}

int pushStack(stack_t* st,int num)
{
	if(st->top > st->max_size) return -1; 
	st->arr[st->top++] = num;
	
	return 0;	
}

int getTopStack(stack_t* st)
{
	return st->arr[st->top -1];
}

int isFullStack(stack_t* st)
{
	return st->top = st->max_size;
}

int isEmpthStack(stack_t* st)
{
	return st->top == 0;
}

int popStack(stack_t* st)
{
	if(isEmpthStack(st)) return -1; 
	st->top--;
	return 0;
}

void clearStack(stack_t* p)
{
	p->top = 0;
}

void getBit(stack_t* p,int num)
{
	while(num)
	{
		pushStack(p,num % 2);
		num /= 2;
	}
}


int test1()
{
	stack_t* st = createEmptyStack(10);	
	int num = 0;

	printf("请输入一个整数:");
	scanf("%d",&num);
	getBit(st,num);
	while (!isEmpthStack(st)) 
	{
		printf("%d ",getTopStack(st));	
		popStack(st);
	}
	printf("\n");



	return 0;
}

typedef struct Node
{
	int data;
	struct Node* next;
}Node;

Node* buy(int n)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->data = n;
	node->next = NULL;
	return node;
}

Node* create(int n)
{
	Node* head = NULL;
	if(n < 0) return NULL;
	head = buy(1);
	Node* tail = head;

	for(int i = 2;i <= n;i++)
	{
		Node* cur = buy(i);
		tail->next = cur;
		tail = cur;
	}

	tail->next = head;
	return tail;
}



int main()
{
	int n = 6;
	printf("请输入一个整数n:");

	scanf("%d",&n);	
	printf("请输入一个整数m:");
	int m = 5;
	scanf("%d",&m);	
	Node* head = create(n);

	


	while(head->next != head)
	{
		int len = n;

		for(int i = 0;i < m -1;i++)
		{
			head = head->next;
		}

		Node* del = head->next;
		head->next = del->next;
		free(del);

		head = head->next;
	}

	printf("%d号猴子是大王\n",head->data);

	return 0;
}