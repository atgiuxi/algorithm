#include "list.hpp"

using namespace xiYan;


#define LOG(...) printf(__VA_ARGS__)
#define RELOG(format,...) printf(format,##__VA_ARGS__)


int main() {
  list<int> li;

  LOG("hello\n");
  RELOG("This is a test %d, %d, %d \n", 1, 2, 3);
  RELOG("The value is %d\n", 42); 

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