#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "test.h"

void show_array(int* arr,int size)
{
    for(int i = 0;i < size;i++)
    {
        printf("%d ",*arr++);
    } 
    printf("\n");
}
void test1()
{
    int arr[5] = {10,20,30,40,50};

    int *p = arr;
    printf("%d\n",*p+1);
    printf("%d\n",*p++);
    printf("%d\n",*p);

    // printf("%d\n",*p + 1);
    // printf("%d\n",*(p + 1));
}


void test2()
{
    char str[25] = "hello";
    str[0] = 'H';

    const char* s = "hello";

    printf("%s : %ld \n",str,sizeof(str));
    printf("%s : %ld \n",s,sizeof(s));

    printf("%s : %ld \n",str,strlen(str));
    printf("%s : %ld \n",s,strlen(s));

}

void show(int (*p)[3])
{

    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            // printf("%d ",p[i][j]);
            printf("%d ",*(*(p + i) + j));
        }
        printf("\n");
    }

}

void get_max(int (*p)[3],int* back)
{
    *back = p[0][0];
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            if(p[i][j] > *back) *back = p[i][j];
        }
    }
}

int test3()
{
    int arr[4][3] = {{1,2,3},{4,5,6},{7,8,9},{11,22,334}};


    int (*p)[3] = arr;
    
    

    int back = 0;
    get_max(arr,&back);
    printf("%d\n",back);


    char* names[5] = {"xia","xuan","xu","zhao","qiu"};

    for(int i = 0;i < 5;i++)
    {
        printf("%s ",names[i]);
    }
    printf("\n");


} 

void test4()
{
    int a = 5;
    int b = 6;
    int c = 7;

    int* arr[3] = {&a,&b,&c};

    for(int i = 0;i < 3;i++)
    {
        printf("%d ",*(arr[i]));
    }
    printf("\n");


}

void test5()
{
    int a[] = {1,2,3};
    int b[] = {4,5,6};
    int c[] = {7,8,9};

    int *arr[3] = {a,b,c};

    for(int i = 0; i< 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            printf("%d ",arr[i][j]);
        }
        printf("\n");

    }

}

int change_num(const char* str)
{
    assert(str != NULL);
    int front = 0; // 进位
    int ret = 1;

    while(*str != '\0')
    {
        ret = ret * front + (*str - '0');
        str++;
        front = front * 10; 
    }
    return ret;
}


int test6(int argc,char* argv[])
{
    int num1 = change_num(argv[1]);
    int num2 = change_num(argv[3]);    
    
    char op = argv[2][0];

    int ret = 0;
    switch (op) 
    {
        case '+':
            ret = num1 + num2;
            break;
        case '-':
            ret = num1 - num2;
            break;
        case '*':
            ret = num1 * num2;
            break;
        case '/':
            ret = num1 / num2;
            break;

        default:
            printf("操作符不支持\n");
            return -1;
    }


    printf("%d\n",ret);


}


void show_a(int (*p)[4],int row,int col)
{
    for(int i = 0;i < row;i++)
    {
        for(int j = 0;j < col;j++)
        {
            printf("%d ",p[i][j]);
        }
        printf("\n");
    }
}

void get_min(int (*p)[4],int row,int col,int *back)
{
    *back = p[0][0];
    for(int i = 0;i < row;i++)
    {
        for(int j = 0;j < col;j++)
        {
            if(p[i][j] < *back)*back = p[i][j];
        }
    }
}

int test7()
{
    int a[5][4] = {{1,2,34,56},{5,6,78,12},{11,22,334,67},
        {11,22,45,34},{56,78,90,32} 
    };

    show_a(a,5,4);

    int min = 0;
    get_min(a,5,4,&min);
    printf("%d\n",min);

    return 0;

}

int is_within(char* str,char ch)
{
    while(*str != '\0')
    {
        if(ch == *str) return 1;
        str++;
    }
    return 0;
}

void test8()
{
    char str[64] = {0};
    char ch;

    printf("请输入一个字符串:");
    scanf("%s",str);
    printf("请输入一个字符:");
    scanf("%s",&ch);

    int ret = is_within(str,ch);

    printf("%d\n",ret);
}

#define LEN 10

void showYangHui(int(*p)[LEN],int n)
{
    for(int i = 1;i <= n;i++)
    {
        for(int j = 1;j <= i;j++)
        {
            if(j == 1 || j == i)
            {
                p[i-1][j -1] = 1;
            }
            else
            {
                p[i-1][j-1] = p[i -2][j- 1] + p[i -2][j -2];
            }
        }
    }
}

void showY(int (*p)[LEN],int row,int col)
{
    for(int i = 0;i < row;i++)
    {
        for(int j = 0;j < col;j++)
        {
            if(p[i][j] != 0)
                printf("%d ",p[i][j]);
        }
        printf("\n");
    }
}

void test9()
{

    int arr[LEN][LEN] = {};

    showYangHui(arr,LEN);
    showY(arr,LEN,LEN);
}

void findMax(int* arr,int len,int* back,int* index)
{
    *back = arr[0];
    *index = 0;

    for(int i = 0;i < len;i++)
    {
        if(arr[i] > *back) {*back = arr[i],*index = i;}
    }
}

void test10()
{
    int arr[8] = {75,28,31,92,89,73,12,84};

    int back,index;
    findMax(arr,8,&back,&index);

    printf("the num is : %d , the index is %d\n",back,index);
}


int myAtoi(const char* str)
{
    assert(str != NULL);
    int front = 0; // 进位
    int ret = 1;
    int flag = 0;
    while(*str != '\0')
    {
        if(*str == '-') {flag = 1;str++;continue;}
        ret = ret * front + (*str - '0');
        str++;
        front = front * 10; 
    }

    return flag == 1 ?  -ret : ret;
}

int isPrime(int num)
{
    if(num <= 1) return 0;
    if(num == 2) return 1;
    if(num %2 == 0) return 0;

    for(int i = 3; i < sqrt(num);i += 2)
    {
        if(num % i == 0) return 0;
    }

    return 1;
}

int test11(int argc,char* argv[])
{
    if(argc < 2)
    {
        printf("请输入正确的参数! Usage : 命令 参数\n");
        return -1;
    }

    char *str = argv[1];

    int num = myAtoi(str);

    int ret = isPrime(num);

    printf("%d : %d\n",num,ret);

    return 0;

}

void test12()
{
    int arr[5] = {1,2,3,4,5};
    printf("%p\n",arr);
    printf("%p\n",&arr);

    printf("%p\n",arr + 1);
    printf("%p\n",&arr + 1);


}

int main(int argc,char* argv[])
{
    const char* str1 = "hello world";
    const char* str2 = "hello world";
    char str3[] = "hello world";
    char str4[] = "hello world";

    printf("str1 = %p : str2 = %p",str1,str2);
    printf("str3 = %p : str4 = %p",str3,str4);
    
    return 0;
}
