#include <stdio.h>

#define MIN(x,y) x > y ? y : x


int find1(int *arr,int left,int right,int x)
{
	while(left <= right)
	{
		int mid = (right - left) / 2 + left;

		if(arr[mid] > x) right = mid -1;
		else if(arr[mid] < x)left = mid+1;
		else return mid;
	}
	return -1;
}

int find2(int *arr,int left,int right,int x)
{
	if(left > right) return -1;

	int mid = (right - left) / 2 + left;

	if(arr[mid] > x) find2(arr,left,mid- 1,x);
	else if(arr[mid] < x)find2(arr,mid + 1,right,x);	
	return mid;
}


int main()
{

	printf("%d\n",MIN(1,2));

	return 0;
}