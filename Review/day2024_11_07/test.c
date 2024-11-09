#include <stdio.h>
#include <string.h>
#include <assert.h>

size_t my_strlen1(const char *str)
{
    size_t count = 0;

    while (*str != '\0')
    {
        count++;
        str++;
    }
    return count;
}

size_t my_strlen2(const char *str)
{
    const char *start = str;

    while (*str != '\0')
    {
        str++;
    }
    return str - start;
}

size_t my_strlen3(const char *str)
{
    if (*str == '\0')
        return 0;
    return 1 + my_strlen3(str + 1);
}

void test1()
{
    const char *str1 = "hello world";
    char str2[] = "hello";

    // %u 是用于格式化输出无符号整数，%zu 输出size_t
    printf("%u %u\n", my_strlen3(str1), my_strlen3(str2));
}

void brk_my_strcpy(char *distination, const char *sourse)
{
    while (sourse != '\0')
    {
        *(distination++) = *(sourse++);
    }
}

void my_strcpy(char *distination, const char *sourse)
{
    const char *ret = sourse;    // 好习惯
    assert(distination != NULL); // 断言，false就进入函数就报错，错就是错，对就不报错
    assert(sourse != NULL);

    while (*(distination++) = *(ret++))
    {
        ;
    }
}

void test2()
{
    char distination[128];
    const char *sourse = "hello";

    my_strcpy(distination, sourse);

    printf("%s\n", distination);
}

char *my_strcat(char *destination, const char *source)
{
    char *ret = destination;

    assert(destination != NULL);
    assert(source != NULL);

    while (*destination != '\0')
    {
        destination++;
    }

    while (*(destination++) = *(source++))
    {
    }

    return ret;
}

void test3()
{
    char arr[128] = "hello";
    my_strcat(arr, " world");

    printf("%s\n", arr);
}

/*
int my_strcmp(const char* str1,const char* str2)
{
    assert(str1 != null);
    assert(str2 != NULL);

    while(* str1 != '\0' && *str2 != '\0')
    {
        if(*str1 > * str2) {return 1;}
        else if(*str1 < *str2) {return -1;}
        else
        {
            str1++;
            str2++;
        }
    }
    if(str1 == '\0' && str2 =='\0') return 0;
    else if(str1 == '\0' && str2 !='\0') return -1;
    else return 1;
}
*/

int my_strcmp(const char *src, const char *dst)
{
    int ret = 0;
    assert(src != NULL);
    assert(dst != NULL);

    while (!(ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
        ++src, ++dst;
    if (ret < 0)
        ret = -1;
    else if (ret > 0)
        ret = 1;
    return ret;
}

void test4()
{
    const char *str1 = "aaaa";
    const char *str2 = "aaa";

    printf("%d\n", my_strcmp(str1, str2));
}

char *my_strstr(const char *str1, const char *str2)
{
    char *ret = (char *)str1;

    if (*str2 == '\0')
        return ret;

    char *s1, *s2;

    while (*ret != '\0')
    {
        // 给第二层循环使用
        s1 = ret;
        s2 = (char *)str2;

        while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
        {
            s1++;
            s2++;
        }

        // s2都结尾了，说明已经找到了
        if (*s2 == '\0')
        {
            return ret;
        }
        ret++;
    }

    return NULL;
}

void test5()
{
    const char *str1 = "take it easy,the all things will be ok!";
    const char *str2 = "easy";

    if (my_strstr(str1, str2) != NULL)
    {
        printf("存在\n");
    }
    else
    {
        printf("不存在\n");
    }
}

void *my_memcpy(void *destination, const void *source, size_t num)
{
    void *ret = destination;

    while (num--)
    {
        *(char *)destination = *(char *)source;
        destination++;
        source++;
    }
    return ret;
}

struct person
{
    char name[128];
    int age;
} person;

void test6()
{
    const char *name = "i am lihua";
    my_memcpy(&person.name, name, strlen(name) + 1);

    int age = 0;
    my_memcpy(&person.age, &age, sizeof(int));

    printf("name = %s : age = %d\n", person.name, person.age);
}

void *my_memmove(void *destination, void *sourse, size_t num)
{
    void *ret = destination;

    if (destination <= sourse || destination >= sourse + num)
    {
        while (num--)
        {
            *(char *)destination = *(char *)sourse;
            destination++;
            sourse++;
        }
    }
    else
    {
        destination = destination + num - 1;
        sourse = sourse + num - 1;

        while (num--)
        {
            *(char *)destination = *(char *)sourse;
            destination--;
            sourse--;
        }
    }
    
    return ret;
}

void test7()
{
    char str[] = "memmove can allow memory cover";

    char *ret = (char *)my_memmove(str + 20, str + 15, 10);

    puts(ret);
}


int do_dup(int a[],int N)
{
	int temp;
	
	while (a[0]!=a[a[0]])
	{
		temp=a[0];
		a[0]=a[temp];
		a[temp]=temp;
	}
	return a[0];
}


void test8()
{
    int arr[] = {1,2,3,4,5,3};

    int ret = do_dup(arr,6);

    printf("%d\n",ret);
}

int main()
{
    int arr[] = {1,2,3,4,5,3};

    int ret = do_dup(arr,6);

    printf("%d\n",ret);

    return 0;
}


 