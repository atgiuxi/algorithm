#include "list.hpp"

using namespace xiYan;

int main()
{
	list<int> li;

	li.push_back(1);
	li.push_back(2);
	li.push_back(3);

	li.print();
	li.pop_back();
	li.pop_back();
	li.pop_back();

	li.print();
	return 0;
}