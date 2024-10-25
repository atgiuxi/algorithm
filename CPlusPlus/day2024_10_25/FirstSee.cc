#include <stdio.h>
#include <iostream>
// #include "Base.c" // 不要这样去引用，使用extern外部符号

using namespace std;
enum SEX
{
    MALE = 10,
    FEMALE,
    SECRET
};

// 一.定义常量的几种方式
void test1()
{
    // 1.字面常量
    3.14;

    // 2.const修饰的常变量，本质还是变量，不能被修改
    const float pai = 3.14f;
    printf("the pia value is %f\n",pai);  

    // 3.#define的标识符常量
    #define MAX 1024
    printf("the MAX value is %d\n",MAX);

    // 4.枚举常量
    // 默认从0开始，往后依次累加，也可以指定值
    printf("the value of male is %d\n",MALE);
    printf("the value of female is %d\n",FEMALE);
    printf("the value of secret is %d\n",SECRET);
}

class Num
{
public:
    int Max(const int a,const int b)
    {
        cout << "this function is not const " <<endl;
        if(a > b) return a;
        else return b;
    }

    int Max(const int a,const int b) const 
    {
        cout << "this function is const " <<endl;
        if(a > b) return a;
        else return b;
    }
};

// 二、const关键字的作用
void test2()
{
    // 1.const 修饰变量（常变量）表示该变量的值不能被修改
    const int num = 5;
    // num = 10; // not can change

    // 2.const修饰指针，分为指针常量和常量指针
    int value = 10;
    
    // 常量指针（记忆：常变量）: 不能修改它所指向的数据
    const int *ptr1 = &value;
    ptr1 = &num;
    // 指针常量：指针在初始化后，不能去修改它的指向，修改这个指向的数据
    int* const ptr2 = &value;
    *ptr2 = 5;

    printf("the value of ptr1 is %d\n",*ptr1);
    printf("the value of ptr2 is %d\n",*ptr2);

    // 3.const 修饰成员函数与非const同名函数构成重载，const 修饰成员函数 const对象和非const 对象都可以调用
    // 两个函数同时由还是会调用，非const调用最匹配的，调用非const版本的MAX
    const Num n1;
    Num n2;
    n1.Max(5,10); 
    n2.Max(10,5);
}
// 三、指针常量与常量指针区别
// 四、static关键字的几个用处
extern int g_val;
extern int static_g_val;

extern void Hello();
extern void StaticHello();
// 注意这样编译：$ g++ FirstSee.cc Base.c 
void test4()
{
    // 1.修饰局部变量
    {
        static int num = 5;
        cout << "num the value is " << num << endl;
    }
    // not can access the num 
    // cout << "num the value is " << num << endl;

    // 2、创建base.cc文件验证。修饰全局变量、修饰函数
    cout << g_val << endl;
    // /usr/bin/ld: FirstSee.cc:(.text+0x1d7): undefined reference to `static_g_val'
    // 上面声明不会报错，访问才会报错
    // cout << static_g_val << endl;

    Hello();
    // StaticHello();
}

// 五、指针的理解
void test5()
{
    int num = 5;

    // 1.一级指针：用来存储一个变量的地址。
    int* ptr1 = &num;

    // 2.二级指针：
}

int main()
{

    return 0;
}