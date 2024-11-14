#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int dfs1(int n)
{
	if(n == 1) return 1;
	return n * dfs1(n - 1);
}

void test1()
{
	printf("%d\n",dfs1(5));
}

int sum(int num)
{
	int ret = 0;
	while(num != 0)
	{
		ret += num % 10;
		num /= 10;
	}
	return ret;
}

void test2()
{
	int num = 1234;
	printf("%d\n",sum(num));

}

int dfs2(int n,int m)
{
	if(m == 0) return 1;
	int ret = n * dfs2(n,m -1);
	return ret;
}

int dfs3(int n)
{
	if(n == 1 || n == 2) return 1;
	int ret = dfs3(n -1) + dfs3(n -2);
	return ret;
}

void buy(int** back,int size)
{
	*back = (int*)malloc(sizeof(int) * size);
}

int* re_buy(int size)
{

	int *back = (int*)malloc(sizeof(int) * size);
	return back;
}


int test3()
{
	int * addr = re_buy(10);

	for(int i = 0;i < 10;i++)
	{
		addr[i] = i + 1;
	}

	for(int i = 0;i < 10;i++)
	{
		printf("%d ",addr[i]);
	}
	printf("\n");

	free(addr);
	addr = NULL;
	
	return 0;
}

void change(int **ptr,int* new)
{
	*ptr = new;
}


int test4()
{
	int a = 5;
	int b = 10;
	int* ptr1 = &a;

	printf("before *ptr1 : %d\n",*ptr1);

	change(&ptr1,&b);

	printf("change *ptr1 : %d\n",*ptr1);

	return 0;
}

void showName(char** name,int size)
{
	for(int i = 0;i < size;i++)
	{
		printf("%s ",name[i]);
	}
	printf("\n");
}

int test5()
{
	char* names[5] = {"张三","李四","王五","赵六","田七"};

	showName(names,5);
	return 0;
}

void swap(char** a,char** b)
{
	char* tmp = *a;
	*a = *b;
	*b = tmp;
}

void sort(char** names,int size)
{
	for(int i = 0; i< size - 1;i++)
	{
		for(int j = 0;j < size - 1 -j;j++)
		{
			if(strcmp(names[j],names[j + 1]))
			{
				swap(&names[j],&names[j + 1]);
			}
		}
	}
}

int test6()
{
	char* names[5] = {"张三","李四","王五","赵六","田七"};

	showName(names,5);

	return 0;
}

void swap1(int* a,int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int main()
{

	int a = 5;
	int b = 10;

	printf("before a = %d,b= %d\n",a,b);
	swap1(&a,&b);
	printf("after  a = %d,b= %d\n",a,b);


}