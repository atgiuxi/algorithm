#include <stdio.h>

void test1()
{
    int a = 5;
    char c = 'a';

    int* ptra = &a;
    char* ptrc = &c; 

    printf("prta : before = %p : after = %p\n",ptra,ptra + 1);
    printf("prtc : before = %p : after = %p\n",ptrc,ptrc + 1);


}


int main()
{
    int* ptra;

    printf("*prta : before = %d : after = %d\n",*ptra,*(ptra + 1));

    return 0;
}