#include <stdio.h>
#include <stdlib.h>

void show(int *arr[], int row, int col)
{

}

int test1()
{
	int arr[2][3] = {{1,2,3},{3,4,5}};

	// show(arr,2,3);

	return 0;
}

char* iswithin(char* str,const char* ch)
{
	while (*str != '\n') 
	{	
		if(*str == *ch) return str;
		str++;
	}
	return NULL;
}

int *buy(int size)
{
	int* ret = (int*)malloc(sizeof(int) * size);

	for(int i = 0;i < size;i++)
	{
		ret[i] = i + 1;
	}
	return ret;
}

int test2()
{
	int size;

	printf("请输入要申请的空间的个数:");
	scanf("%d",&size);

	int* p = buy(size);


	return 0;
}

int add(int a,int b)
{
	return a + b;
}
int sub(int a,int b)
{
	return a - b;
}
int mul(int a,int b)
{
	return a * b;
}
int dev(int a,int b)
{
	return a / b;
}

int callback(int(*p)(int,int),int a,int b)
{
	return p(a,b);
}

int test3()
{
	// add sub mul dev 的类型都是 int (int ,int )
	printf("%d\n",callback(add,5,5));
	printf("%d\n",callback(sub,5,5));

	int (*p[4])(int,int) = {add,sub,mul,dev};

	for(int i = 0; i < 4;i++)
	{
		printf("%d\n",p[i](5,5));
	}

	return 0;
}

// const 关键字的作用
int test4()
{
	const int a = 5;
	int const b = 10;

	// a = 10;
	// b = 5;	
	int c = 5;
	int const *p = &c;
	p = &b;

	// *p = 10;

	printf("%d\n",*p);	
	return 0;
}

extern int b;
extern void show_hello();
int a = 0;


void func()
{
	a++;
	b++;
	printf("a = %d : b = %d\n",a,b);
	show_hello();
}

int b = 5;

void show_hello()
{	
	printf("hello world\n");
}


void fun1()
{
	static int x = 3;
	x++;
	printf("x is %d\n",x);
}

int* fun2()
{
	// int a[5];
	static int a[5];
	for(int i = 0; i < 5;i++)
	{
		a[i] = i * 10;
	}
	return a;
}

int test5()
{
	int *p = fun2();
	for(int i = 0; i < 5;i++)
	{
		printf("%d\n",p[i]);
	}	
	
	return 0;
}

#define SHOW                                                                   \
  do {                                                                         \
    printf("  *\n");                                                           \
    printf(" ***\n");                                                          \
    printf("*****\n");                                                         \
  } while (0);

#define DEBUY


int test6()
{
#ifdef DEBUY 
	printf("buy!\n");
	printf("buy!\n");
	printf("buy!\n");
	printf("buy!\n");
	printf("buy!\n");
#else

	printf("no buy!\n");
	printf("no buy!\n");
	printf("no buy!\n");
	printf("no buy!\n");
	printf("no buy!\n");

#endif
	return 0;
}


int main()
{
	return 0;
}

