#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// 冒泡排序
void sort(int *nums, int len)
{
    // 躺数：len - 1
    for (int i = 0; i < len - 1; i++)
    {
        // 每一趟比较的次数：len - 第几躺 - 1(防止越界)，从左往右
        for (int j = 0; j < len - i - 1; j++)
        {
            if (nums[j] > nums[j + 1])
            {
                int tmp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = tmp;
            }
        }
    }
}

void test1()
{
    int nums[] = {92, 80, 70, 50, 60, 75};

    int len = sizeof(nums) / sizeof(nums[0]);

    sort(nums, len);

    for (int i = 0; i < len; i++)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");
}

// 字符数组
void test2()
{
    int i = 0;
    char a[] = "hello";
    char b[] = {'h', 'e', 'l', 'l', 'o'};

    char c[] = "你好";

    printf("the len of a is %ld\n", sizeof(a) / sizeof(a[0]));
    printf("the len of b is %ld\n", sizeof(b) / sizeof(b[0]));
    printf("the len of c is %ld\n", sizeof(c) / sizeof(c[0]));

    // 方式1
    for (i = 0; i < sizeof(b) / sizeof(b[0]); i++)
    {
        printf("%c\n", b[i]);
    }

    // 方式2
    printf("%s\n", c);

    // 方式3
    int index = 0;
    printf("=======================\n");
    while (a[i] != '\0')
    {
        printf("%c\n", a[i]);
        i++;
    }
}

void test3()
{
    char a[128] = {0};

    printf("please write : ");
    // gets(a);

    puts(a);
}

void test4()
{
    char a[] = "hello";

    printf("using strlen to calculate the str size is %ld\n", strlen(a));
    printf("using sizeof to calculate the str size is %ld\n", sizeof(a) / sizeof(a[0]));
}

// strcpy的使用
void test5()
{
    char a[128] = {0};

    // gets(a);
    printf("please write : ");

    char b[128];
    strcpy(b, a);

    printf("%s\n", a);
    printf("%s\n", b);
}

// strcmp的使用
void test6()
{
    printf("the value is %d\n", strcmp("ab", "ab"));
    printf("the value is %d\n", strcmp("ab", "ac"));
    printf("the value is %d\n", strcmp("ab", "aa"));
}

void test7()
{
    printf("%ld\n", sizeof(1.0 + 1));
}

// TODO
int findM(int **arr, int num)
{
    int row = sizeof(arr) / sizeof(arr[0]);
    int col = sizeof(arr[0]) / sizeof(arr[0][0]);

    int count = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (arr[i][j] == num)
            {
                count++;
            }
        }
    }

    return count;
}

// 二维数组的遍历
void test8()
{
    int arr[][3] = {{1, 2, 3}, {4, 5, 6}};

    printf("%ld\n", sizeof(arr) / sizeof(arr[0]));
    printf("%ld\n", sizeof(arr[0]) / sizeof(arr[0][0]));

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    // TODO
    // int ret = findM(arr,6);
}

void test9()
{
    int arr[][3] = {{1, 2, 3}, {4, 5, 6}};

    printf("%p\n", &arr[0][0]);
    printf("%p\n", &arr);
    printf("%p\n", arr);
    printf("%p\n", arr[0]);
}

void test10()
{
    int num = 0;

    printf("%ld\n", sizeof(char));
    printf("%ld\n", sizeof num);
    printf("%ld\n", sizeof(1 + 1));
    printf("%ld\n", sizeof(1.0 + 1));
}

void test11()
{
    time_t t = time(NULL);
    printf("%ld\n", t);
}

void test12()
{
    int num = 0;
    // 伪随机
    // srand(1); // 每个人都一样

    srand(time(NULL));
    while (1)
    {
        num = rand();
        printf("%d\n", num);
        // sleep(1);
    }
}

void game()
{
    int number = 0; // 需要猜的数
    srand(time(NULL));
    number = rand() % 9 + 1; // 生成1 -10的数

    printf("%d\n", number);

    int num = 0; // 你可以猜的次数
    printf("you can think the num : ");
    scanf("%d", &num);

    // 你think思考的数
    int think = 0;
    while (num != 0)
    {
        printf("please write your think : ");
        scanf("%d", &think);
        if (num > number)
        {
            printf("your num bager\n");
        }
        else if (num < number)
        {
            printf("your num lower\n");
        }
        else
        {
            printf("you win\n");
            return;
        }
        num--;
    }
    printf("you fail\n");
}

void test13()
{
    char *str = "123";
    int num = atoi(str);

    printf("the num is %d\n",num);
}

size_t my_strlen1(const char* str)
{
    size_t ret = 0;
    while((*str) != '\0')
    {
        ret++;
        str++;
    } 

    return ret;
}

void test14()
{
    char *str = "hello";
    printf("%ld\n",my_strlen1(str));
}

#include <limits.h> // C99
void test15()
{
    printf("char min number the value is %d\n",CHAR_MIN);
    printf("char max number the value is %d\n",CHAR_MAX);

    printf("unsign char max number the value is %d\n",UCHAR_MAX);

    char a = 128;
    printf("%d\n",a);
    a++;
    printf("%d\n",a);

    unsigned char b = 256;
    printf("%d\n",b);

    // TODO
    int c = 1000;
    char d = c;
    printf("%d\n",d);
}

// TODO
void test16()
{
    unsigned char a = 200;
    unsigned char b = 100;
    unsigned char c = 0;

    c = a + b;
    printf("%d %d\n",a + b,c);
}

void test17()
{
    // 方式1：
    char* str1 = "";
    // 方式2：
    char arr1[] = "hello"; // 系统会默认添加一个\0，注意比较arr1和arr2
    char arr2[] = {'h','e','l','l','o'};
    char arr3[] = ""; // 默认一个\0
    // 方式3：
    char* str2 = (char*)malloc(128 * sizeof(char));
    strcpy(str2,"hello world");

    // 注意比较：strlen 和 sizeof的不同
    printf("use sizeof to calculate : char arr1[] = %ld\n",sizeof(arr1));
    printf("use strlen to calculate : char arr1[] = %ld\n",strlen(arr1));

    printf("use sizeof to calculate : char arr2[] = %ld\n",sizeof(arr2));
    printf("use sizeof to calculate : char arr3[] = %ld\n",sizeof(arr3));

    printf("%s\n",str2);
}



int main()
{
    test17();
    
    return 0;
}
