#include "Http.hpp"
#include <iostream>

using namespace std;

void TestSplit()
{
	vector<string> v;
	string src = "hello world     ";
	Util::Split(src," ",&v);

	for(auto& e : v)
	{
		cout << e << endl;
	}
}

void TestReadFile()
{
	string dst;
	TestSplit();
	string filename = "../1.TestUtil.cc";
	Util::ReadFile(filename,&dst);

	cout << dst << endl;	
}

void TestWriteFile()
{
	string filename = "a.txt";
	string src = "hello world";
	Util::WriteFile(filename,src);
}

void Test()
{
	string url = "http://098ac ++ ";
	string res = Util::UrlEncode(url,true);
	cout << res << endl;

	cout << Util::UrlDecode(res,true) << endl;

	cout << Util::StatuDesc(404) << endl;

	cout << Util::IsDirectory("../build") << endl;
	cout << Util::IsRegular("a.txt") << endl;
	cout << Util::ValidPath("/../login.html") << endl;
}

int main()
{
	Test();
	return 0;
}