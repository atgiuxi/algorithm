#include <stdio.h>

#define Mul(x, y) ++x * ++y

void test1()
{
    int a = 1;
    int b = 2;
    int c = 3;

    printf("%d\n", Mul(a + b, b + c));
}

#define SWAP(a, b)       \
    do                   \
    {                    \
        (a) = (a) ^ (b); \
        (b) = (a) ^ (b); \
        (a) = (a) ^ (b); \
    } while (0)


void test2()
{
    int a = 5;
    int b = 10;
    SWAP(a, b);
    printf("a = %d : b = %d\n", a, b);

}

// 写一个程序，只允许使用两个指针，不使用其他变量，实现将一个数组倒序
void sort(int* start,int* end)
{
    int len = (end - start);

    for(int i = 0;i < len -1;i++)
    {
        for(int j = 0; j< len - 1 -i;j++)
        {
            if(start[j] < start[j + 1])
            {
                (*(start + j)) = (*(start + j)) ^ (*(start +j + 1));
                (*(start + j + 1)) = (*(start + j)) ^ (*(start +j+ 1)); 
                (*(start + j)) =  (*(start + j)) ^ (*(start +j+ 1));
            }
        }
    }
}

void test3()
{
    int arr[] = {1,5,3,7,5,9};

    int len = sizeof(arr) / sizeof(arr[0]);

    sort(arr,arr + len);

    for(int i = 0;i < len;i++)
    {
        printf("%d\n",arr[i]);
    }
}


int main()
{
    test3();
    return 0;
}
