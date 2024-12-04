#include <iostream>
#include <memory>
#include <string>
#include <time.h>

using namespace std;

int test1()
{
	unique_ptr<string> u1(new string("hello world"));
	unique_ptr<string> u2;

	u2 = std::move(u1);
	cout << *u2 << endl;
	return 0;
}

int test2()
{
	shared_ptr<string> s1(new string("hello"));
	shared_ptr<string> s2(new string("world"));
	
	// 上面可不是管理一个对象
	cout << s1.use_count() << " : " << s2.use_count() << endl; 

	// 交换管理的对象
	swap(s1,s2);
	cout << "s1  " << *s1 << " : " << "s2 " << *s2 << endl;

	// 方式1:
	string *str = new string("hello world");
	shared_ptr<string> s3(str);
	shared_ptr<string> s4(s3);
	cout << s3.use_count() << " : " << s4.use_count() << endl;	
	// 方式2:
	shared_ptr<string> s5 = s4;
	cout << s3.use_count() << " : " << s4.use_count() << " : " << s5.use_count() << endl;	

	// 放弃管理权
	s5.reset();

	return 0;
}
class B;	// 注意声明
class A
{
public:
	weak_ptr<B> pb_;
	~A() { cout << "A delete\n";}
};

class B
{
public:
	shared_ptr<A> pa_;
	~B() { cout << "B delete\n";}
};

void fun()
{
	shared_ptr<A> pa(new A());
	shared_ptr<B> pb(new B());
	cout << pa.use_count() << " : " << pb.use_count() << endl;	// 输出1
	pa->pb_ = pb;
	pb->pa_ = pa;
	cout << pa.use_count() << " : " << pb.use_count() << endl;	// 输出2
}

class Developer
{
public:
	virtual void develop(){cout << "开发者开发" << endl;}
};

class Manitor : public Developer
{
public:
	void develop() { cout << "大神开发" << endl;}
};

class SmallBird : public Developer
{
public:
	void develop() { cout << "菜鸟开发" << endl;}
};


class Commpany
{
public:
	Commpany(){srand(time(NULL));}	
	Developer* recruit()
	{
		int num = rand() % 2;
		Developer* s;
		if(num == 0)
		{
			s = new SmallBird;
		}
		else { s = new Manitor;}
		return s;
	}

	void work(Developer* p)
	{
		p->develop();
	}
};

int main()
{
	Commpany c;
	Developer* d = c.recruit();
	c.work(d);
	return 0;
}
