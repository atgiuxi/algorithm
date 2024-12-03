#include <iostream>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <limits.h>
#include <algorithm>
#include <array>
#include <memory>

using namespace std;
int  findit(vector<int>  v,int num, int tofind )
{
	for(int i = 0; i < num;i++)
	{
		if(v[i] == tofind) return i;	
	}
	return -1;
}

string tegoter(vector<string> v)
{
	string ret;

	size_t len = INT_MAX;

	for(auto& e : v)
	{
		len = std::min(len,e.size());
	}

	for(int i = 0;i < len;i++)
	{
		char ch = v[0][i];
		for(int j = 0;j < v.size(); j++)
		{
			if(ch != v[j][i]) return ret;
		}
		ret +=ch;
	}
	return ret;
}

int test2()
{
	vector<string> v = {"flower","flow","flight"};
	string ret = tegoter(v);

	cout << ret << endl;
	return 0;
}

void Func(int x)
{
	cout << typeid(x).name() << endl;
}

void Func(void *x)
{
	cout << typeid(x).name() << endl;
}

template<class T>
void Foo(T t)
{
	cout << typeid(t).name() << endl;
}

int test1()
{
	Foo(0);
	Foo(NULL);
	Foo(nullptr);
	return 0;
}


int test3()
{
	array<int, 5> arr = {1,2,3,6,4};

	return 0;
}


int test4()
{
	int a = 9;
	auto fun1 = [=]()mutable{return ++a;};
	cout << fun1() << endl;
	cout << a << endl;

	auto fun2 = [=]{return a + 1;};
	auto fun3 = [&]{return a + 1;};
	cout << fun2() << endl;
	cout << fun3() << endl;
	++a;
	
	cout << fun2() << endl;
	cout << fun3() << endl;
	cout << fun2() << endl;
	cout << fun1() << endl;
	return 0;
}

int test5()
{
	const int a = 10;
	const int *b = &a;
	int *res = const_cast<int*>(b);
	*res = 20;

	cout << &a << endl;
	cout << b << endl;
	cout << res << *res << endl;

	
	return 0;
}
class A
{
public:
	virtual void f(){cout << "class A" << endl;}
};

class B : public A
{
public:
	void f(){cout << "class B" << endl;}
};

int test6()
{
	A a;
	B b;

	A* pa = &a;
	B* pb = &b;



	return 0;
}

int test7()
{
	char str[] = "glad to test something";
	char *p = str;
	p++;
	cout << p << endl;
	int *p1 = reinterpret_cast<int*>(p);
	p1++;
	p = reinterpret_cast<char*>(p1);
	cout << p << endl;
	return 0;
}

template<class T>
class MangerPointer
{
private:	
	T* p;
public:
	MangerPointer(T* p):p(p)
	{
		// p = new T;
	}

	~MangerPointer()
	{
		delete p;
	}
};

class Person
{
public:
	Person()
	{
		cout << "Person" << endl;
	}
	~Person()
	{
		cout << "~Person" << endl;
	}
};

int test8()
{
	// Person p;
	// MangerPointer<Person> m(&p);

	Person *p = new Person();
	MangerPointer<Person> m(p);
	return 0;
}


class Test
{
	string _name;
public:
	Test(string name):_name(name)
	{
		cout << "Test() : " << _name << endl;
	}
	~Test()
	{
		cout << "~Test() : " <<_name << endl;
	}
};

int main()
{
	shared_ptr<Test> t1(new Test("a"));
	shared_ptr<Test> t2(new Test("b"));
	shared_ptr<Test> t3(new Test("c"));

	cout << t1.use_count() << endl;
	t1 = t2;
	cout << t1.use_count() << endl;

	
	return 0;
}