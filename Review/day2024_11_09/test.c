#include <stdio.h>

// 数据类型的大小
void test1()
{
    printf("%ld\n", sizeof(char));
    printf("%ld\n", sizeof(short));
    printf("%ld\n", sizeof(int));
    printf("%ld\n", sizeof(long));
    printf("%ld\n", sizeof(long long));
    printf("%ld\n", sizeof(float));
    printf("%ld\n", sizeof(double));
    printf("%ld\n", sizeof(long double));
}

void test2()
{
    char c;
    scanf("%c", &c);
    switch (c)
    {
    case '1':
        printf("1");
    case '2':
        printf("2");
    case '3':
        printf("3");

    default:
        printf("error");
    }
}

void test3()
{
    int i = 0, a = 0, b = 2, c = 3, d = 4;
    // i = a++ || ++b || d++;
    i = a++ && ++b && d++;
    printf("i = %d\na = %d\nb = %d\nc = %d\nd = %d\n", i, a, b, c, d);
}

#define sqt(x) x *x

void test4()
{
    printf("%d\n", sqt(5 + 1));
}

void set(int *a)
{
    *a = (*a) | 4;
}

void clear(int *a)
{
    *a = (*a) & (~4);
}

void test5()
{
    int a = 3;
    set(&a);
    printf("%d\n", a);
    clear(&a);
    printf("%d\n", a);
}

void test6()
{
    int a = 5;
    int b = 10;

    printf("after a = %d : b = %d\n", a, b);

    a = a ^ b; // a 是异或的结果，b还是原来的数
    b = a ^ b; // a ^ b 就得到了原理的a
    a = a ^ b; // b是原来的a，a还是异或的结果，在异或就得到了b

    printf("after a = %d : b = %d\n", a, b);
}

void test7()
{
    int number = 7;

    int count = 0;
    while (number)
    {
        if (number % 2 != 0)
            count++;
        number /= 2;
    }

    printf("%d\n", count);
}

void test8()
{
    int number = 7;
    int count = 0;

    for (int i = 0; i < 36; i++)
    {
        if ((number & 1) == 1)
            count++;
        number = number >> 1;
    }

    printf("%d\n", count);
}

#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define SWAP(x, y)  \
    do              \
    {               \
        (x) ^= (y); \
        (y) ^= (x); \
        (x) ^= (y); \
    } while (0)

#define MAX 100

void test9()
{
    printf("%s\n", __FILE__); // 进行编译的源文件
    printf("%d\n", __LINE__); // 文件当前的行号
    printf("%s\n", __DATE__); // 文件被编译的日期
    printf("%s\n", __TIME__); // 文件被编译的时间
    printf("%d\n", __STDC__); // 如果编译器遵循ANSI C，其值为1，否则未定义
}

#define SQUARE(x) (x) * (x)

#define SUM(x) ((x) + (x))

void test10()
{
    int a = 5;
    printf("%d\n", 10 * SUM(5));
}

#define Mul(x, y) ++x * ++y

void test11()
{
    int a = 1;

    int b = 2;

    int c = 3;

    printf("%d\n", Mul(a + b, b + c));
 
}

#define PRINT1(FORMAT, VALUE)\
    printf("the value is "FORMAT"\n", VALUE);

#define PRINT2(FORMAT, VALUE)\
    printf("the value of " #VALUE " is "FORMAT "\n", VALUE);

int test12()
{
    int i = 0;
    char* p = "hello"" world";
    printf("hello"" world\n");
    printf("%s\n",p);


    PRINT1("%d",i + 5);
    PRINT2("%d",i + 5);

    return 0;
}


#define ADD_TO_SUM(num, value) \
 sum##num += value;

int test13() {
    int sum5 = 0;  

    ADD_TO_SUM(5, 10); 

    printf("sum5 = %d\n", sum5);  
    return 0;
}

#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

int main()
{
    int x = 5;
    int y = 8;
    int z = MAX(x+1, y+1);
    printf("x=%d y=%d z=%d\n", x, y, z);
}