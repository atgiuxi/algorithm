#include "Logic.hpp"

using namespace std;


int main()
{
	vector<Player> player = {{"张三"},{"李四"},{"王五"}};

	Logic l(player);
	l.Run();


	
	return 0;
}