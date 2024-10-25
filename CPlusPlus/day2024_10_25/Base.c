#include <stdio.h>


int g_val = 5;
static int static_g_val = 10;

void Hello()
{
    printf("void Hello()\n");
}

static void  StaticHello()
{
    printf("static void  StaticHello()\n");
}