#include "SeqList.h"

int main()
{
	SeqList seq;

	Init(&seq);

	Insert(&seq,0,1);
	Insert(&seq,0,2);
	Insert(&seq,0,3);
	
	Print(&seq);

	Erase(&seq,0);
	Erase(&seq,0);
	Erase(&seq,0);
	Print(&seq);

	Destroy(&seq);

	return 0;
}