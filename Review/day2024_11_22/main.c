#include <stdio.h>
#include <string.h>

typedef struct
{
	int max;
	int post;
}index_t;

int find(index_t* idxs,int* arr,int iSzie,int aSize,int x)
{
	int begin,end;

	int i = 0;
	for(;i < iSzie;i++)
	{
		
	}
	
	return -1;
}


int test1()
{
	int arr[19] = {18,10,9,8,21,20,38,42,19,50,51,72,56,55,76,100,90,88,108};

	index_t b[4] = {{18,0},{42,4},{72,9},{108,14}};


	return 0;
}

typedef struct student
{
	char name[20];
	int age;
}stu,*stu_p;


int main()
{
	stu s;
	stu_p stu = &s;

	stu->age = 10;
	strcpy(stu->name,"tom");


	return 0;
}
