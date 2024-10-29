#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// malloc的使用
void test1()
{
    // 使用malloc开辟10个int大小的内存空间并且初始化成1-10
    int size = 10;
    int *arr = (int *)malloc(sizeof(int) * size);

    // 注意判空，如果malloc申请失败就会返回NULL，避免对空指针的访问
    if (arr != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            // malloc开辟空间默认是不初始化的
            arr[i] = i + 1;
        }
    }

    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // 记得释放
    free(arr);
    // 良好的编程习惯
    arr = NULL;
}

void test2()
{
    int size = 10;
    int *arr = (int *)calloc(sizeof(int), size);

    assert(arr);

    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    arr = NULL;
}

void print_number(int *myInt)
{   
    // 如果为NULL,就报错。
    // 那么判断为false，assert就生效。那么判断条件设置为 myInt != NULL
    assert(myInt != NULL);
    printf("%d\n", *myInt);
}

void test3()
{
    int a = 10;
    int *b = NULL;
    int *c = NULL;

    b = &a;

    print_number(b);
    print_number(c);
}

// realloc
void test4()
{
    int size = 10;
    int *arr = (int *)malloc(sizeof(int) * size);

    if (arr != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            arr[i] = i + 1;
        }
    }

    int reSize = 20;

    // arr = realloc(arr,sizeof(int) * reSize); // 这样要是申请失败arr就废了
    int *reArr = NULL;
    reArr = (int*)realloc(arr/*原空间*/,sizeof(int) * reSize);
    
    if(reArr != NULL){arr = reArr;}

    // realloc扩容完之后会不会影响原来的内容1-10(不会)
    for (int i = 0; i < reSize; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // 原来的空间realloc已经处理掉
    free(arr);
    arr = NULL;
}

int main()
{
    test4();

    return 0;
}