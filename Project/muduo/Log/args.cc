#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <iostream>

using  namespace std;

#define _GNU_SOURC

// 不定参宏函数
// fmt 表示格式化字符串,...表示不定参数
// 类似printf("%s","hello")
// "[%s : %d]" fmt "\n" 字符串有自动连接功能
#define LOG_BRK(fmt, ...) printf("[%s : %d]" fmt "\n",__FILE__,__LINE__, __VA_ARGS__)
#define LOG(fmt, ...) printf("[%s : %d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)


int test1()
{
	LOG_BRK("%s","hello");
	// 这是不行的, __VA_ARGS__宏替换的时候多了一个,编译通过
	// LOG_BRK("hello");

	// ,##__VA_ARGS__ 中的##表示没有参数把,去掉
	LOG("hello");
	return 0;
}

// C语言中函数中 man va_start
// 参数压栈，参数是连续的！
// va_start 是宏
void printNum(int count,...)
{
	va_list ap;
	va_start(ap,count); // 让ap指向count参数后的第一个参数的起始地址
	for(int i = 0; i < count;i++)
	{
		// 按照int类型,不同类型是不行的,要直到类型，所用使用者要传入格式化字符串%d、%s.来告诉编译器按照什么类型取，取多大
		int num = va_arg(ap,int);	// ap自动向后偏移！
		printf("param[%d] : %d\n",i,num);
	}

	va_end(ap);	// 将ap指针置空
}

// man vasprintf
void PrintC(const char* fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	char* res;
	int ret = vasprintf(&res,fmt,ap);

	if(ret != -1)
	{
		// #define _GNU_SOURC避免警告
		printf("%s",res);
		free(res);
	}
	va_end(ap);
}

int test2()
{
	printNum(5,1,2,3,4,5,6); // 5个参数.6个行不行?试一试,不输出！

	PrintC("%s\n","hello");
	return 0;
}

void PrintfPlus()
{
	printf("\n");
}

// 不定参参数包
// 递归的思想
template<typename T,typename ...Args>
void PrintfPlus(T& value,Args &&...args)
{
	std::cout << value << " ";

	// 获取参数的个数
	if((sizeof ...(args)) > 0)
	{
		// 使用右值完美转发保证，参数的原先的类型属性。
		PrintfPlus(std::forward<Args>(args)...);	
	}
	else { PrintfPlus();}
}


int main()
{
	PrintfPlus("hello"," world");
	return 0;
}