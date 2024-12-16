#include "../Server.hpp"
#include <iostream>

int main()
{
	Buffer buf;
	std::string input = "hello\nworld";

	buf.WriteAndPush(input.c_str(),input.size());

	std::cout << buf.ReadAbleSize() << std::endl;
	std::cout << buf.ReadAsStringAndPop(input.size()) << std::endl;
	std::cout << buf.GetLine() << std::endl;	
	return 0;
}