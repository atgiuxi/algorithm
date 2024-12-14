#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void funcThread(int n)
{
	n += 10;
}

void funcThreadRef(int& n)
{
	n += 10;
}

int main()
{
	cout << "main begin ..." << endl;
	int n = 5;
	thread t(funcThread,n);
	t.join();
	cout << "n the value is " << n << endl;

	int ref_n = 5;
	thread t_ref(funcThreadRef,std::ref(ref_n));
	t_ref.join();
	cout << "n_ref the value is " << ref_n << endl;

	cout << "main end ..." << endl;

	return 0;
}



