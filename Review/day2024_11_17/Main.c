#include <stdio.h>
#include <stdlib.h>

int add(int a,int b)
{
	return a + b;
}

int sub(int a,int b)
{
	return a - b;
}

int calculate(int cal(int,int),int a,int b)
{
	return cal(a,b);
}

int test1()
{
	printf("%d\n",calculate(add,5,10));
	printf("%d\n",calculate(sub,10,5));

	return 0;
}

void _swap(void* p1,void* p2,int size)
{
	for(int i = 0;i < size;i++)
	{
		char ch = *((char*)p1 + i);
		*((char*)p1 + i) = *((char*)p2 + i);
		*((char*)p2 + i) = ch;
	}
}

void my_qsort (void* base, size_t num, size_t size,int (*compar)(const void*,const void*))
{
	for(int i = 0; i < num - 1;i++)
	{
		for(int j= 0;j < num -1 -i;j++)
		{
			if(compar((char*)base + j * size,(char*)base + (j + 1) * size) > 0)
			{				
				_swap((char*)base + (j* size),((char*)base + (j+ 1)* size),size);
			}
		}
	}
}

int int_cmp(const void* p1,const void* p2)
{
	return *(int*)p1 - *(int*)p2;
}

int test2()
{
	int arr[] = {5,1,2,3,8,9,3};

	int size = sizeof(arr) / sizeof(arr[0]);

	qsort(arr,size,sizeof(int),int_cmp);
	for(int i = 0;i < size;i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");

	return 0;
}

int main()
{
	int (*p[2])(int,int) = {add,sub};
	for(int i = 0;i < 2;i++)
	{
		printf("%d\n",p[i](10,5));
	}

	int (*(*pp)[2])(int,int) = &p;
}


