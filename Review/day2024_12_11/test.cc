#include <iostream>
#include <stdio.h>

using namespace std;

template<typename T>
T add(T a,T b){return a + b;}

int test1()
{
	add(5,5);

	return 0;
}

// class Base
// {
// public:
// 	virtual void show(){cout << "base class show" << endl;}
// };
// class Derived : public Base
// {
// public:
// 	virtual void show(){cout << "derived class show" << endl;}
// };
class Base 
{
public:
	virtual ~Base() { std::cout << "base destructor" << std::endl; }
};

class Derived : public Base 
{
public:
	~Derived() { std::cout << "berived destructor" << std::endl; }
};

int main()
{
	Base* b = new Derived;
	delete b;

	getc(stdin);
	return 0;
}