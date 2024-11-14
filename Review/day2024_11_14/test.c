#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void test1()
{
    const char* str1 = "hello world";
    const char* str2 = "hello world";
    char str3[] = "hello world";
    char str4[] = "hello world";

    printf("str1 = %p : str2 = %p\n",str1,str2);
    printf("str3 = %p : str4 = %p\n",str3,str4);
 
}

void test2()
{
    char str1[] = {'h','e','l','l','o'};
    char str2[] = {'h','e','l','l','o','\0'};
    char str3[] = "hello";

    printf("%ld\n",sizeof(str1));
    printf("%ld\n",sizeof(str2));
    printf("%ld\n",sizeof(str3));

    printf("%zd\n",strlen(str1));
    printf("%zd\n",strlen(str2));
}

int func(int n)
{
    if(n == 1) return 1;
    return n + func(n -1);
}

void show(int(*p)[5],int cow,int col)
{
    for (int i = 0;i < cow ; i++) 
    {
        for (int j = 0; j < col; j++) 
        {
            // printf("%d ",p[i][j]);
            printf("%d ",*(*p + i) + j);
            printf("%d ",*((*(p + i)) + j));
        }
        printf("\n");
    }
}

void test3()
{
    int a[2][5] = {{1,2,3,4,5},{6,7,8,9,10}};
    
    show(a,2,5);
    char* word1 = "hello";
    char* word2 = "world";
    char* word3 = "china";

    char* words[3] = {word1,word2,word3};

    for(int i = 0;i < 3;i++)
    {
        printf("%s\n",words[i]);
    }
}


int main(int argc,char* argv[])
{
    if(argc <= 1)
    {
        printf("Usage: %s <file1> [file2 ...]\n",argv[0]);
        return -1;
    }

    for(int i = 1;i < argc;i++)
    {
        FILE* file = fopen(argv[i],"r");
        if(file == NULL)
        {
            perror("文件不存在!\n");
            return -1;
        }

        char ch;

        while((ch = fgetc(file)) != EOF)
        {
            fputc(ch,stdout);
        }

        fclose(file);
    }

   return 0;
}

void buy(int** q)
{
    *q = (int*)malloc(sizeof(int) * 10);
}

void test4()
{
    int* q = NULL;
    
    buy(&q);
    
    
    for(int i = 0; i< 10;i++)
    {
        q[i] = i;
    }
    
    for(int i = 0; i < 10;i++)
    {
        printf("%d ",q[i]);
    }

}

void swap(int** p1,int *p2)
{
    *p1 = p2;
}

void test5()
{
    // char* names[5] = {"zhang","li","zhao","tian","wang"};
    char* names[5];


    for(int i = 0; i < 5;i++)
    {
        char name[10] = {0};
        // scanf("%s",names[i]);
        scanf("%s",name);
        names[i] = name;
    }
    
    srand(time(NULL));

    printf("%s\n",names[rand() % 5]);

}

void showName(char** names,int size)
{
    for (int i = 0; i < size; i++) 
    {
        printf("%s ",names[i]);
    }
    printf("\n");
}

int test6(int argc,char* argv[])
{
    char* names[5]; 

    for(int i = 0; i < 5;i++)
    {
        char name[25] = {0};
        printf("请输入一个名字:");
        scanf("%s",name);
        names[i] = name;
    }
    
    for(int i = 0;i < 5;i++)
    {
        printf("%s\n",names[i]);
    }

    return 0;
}

void swap_str(char** s1,char** s2)
{
    char* tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

void sort(char** names,int size)
{
    for(int i = 0;i < size - 1;i++)
    {
        for(int j = 0;i < size - 1 -i;i++)
        {
            if(strcmp(names[j],names[j + 1]))
            {
                swap_str(&names[j],&names[j + 1]);
            }
        }
    }

}

int test7()
{
    char* names[5] = {"zhang","li","zhao","tian","wang"}; 
}
