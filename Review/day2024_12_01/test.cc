#include<iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

int add(int a,int b)
{
	return a + b;
}

template<class T>
using vec = std::vector<T>; 

// 回调函数
int callback(int(*call)(int,int),int a,int b) 
{
	return call(a,b);
}

int test1()
{
	int(*func)(int,int) = add;
	int ret = add(5,5);	// 像函数一样使用函数指针
	printf("%d\n",ret);
	printf("%d\n",callback(func,5,5));

	vec<int> v;
	return 0;
}

struct Functor
{
	void operator()()
	{
		std::cout << "hello world" << std::endl;
	}
};

int test2()
{
	Functor functor;
	functor();

	return 0;
}

struct Goods
{
	string _name;
	double _price;

	Goods(string name,double price):_name(name),_price(price){}
};

int test3()
{
	vector<Goods> v = {{"苹果",5.5},{"香蕉",8.8},{"梨",6.6}};
	sort(v.begin(),v.end(),[](const Goods& g1,const Goods& g2){return g1._name < g2._name;});
	sort(v.begin(),v.end(),[](const Goods& g1,const Goods& g2){return g1._price < g2._price;});

	for(auto& good : v)
	{
		cout << good._name << endl;
	}
	return 0;
}


int test4()
{
	std::function<int(int,int)> func = add;
	cout << func(5,5) << endl;

	return 0;
}

using Callback = std::function<void()>;

void show(Callback cb)
{
	cb();
}

void showNum(int a,int b)
{	
	cout << "a = " << a << " b = " << b << endl;
}

int test5()
{
	show(std::bind(showNum,5,5));
	auto b = bind(showNum,5,std::placeholders::_1);
	b(10);

	return 0;
}



int main()
{
	


	return 0;
}