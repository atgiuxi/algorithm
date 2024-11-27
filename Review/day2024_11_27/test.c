#include <stdio.h>

int test1(int argc,const char* argv[])
{
    if(argc != 3)
    {
        printf("参数错误!\n");
        return -1;
    }

    FILE *fpr = fopen(argv[1],"r");
    FILE *fwr = fopen(argv[2],"w");

    int read_size = 0;
    char buf[1024] = {0};
    while((read_size = fread(buf,1,1023,fpr)) > 0)
    {
        fwrite(buf,1,read_size,fwr);
    }

    fclose(fpr);
    fclose(fwr);

    return 0;
}	

int main(int argc,const char* argv[])
{
    if(argc != 2)
    {
        printf("参数错误!\n");
        return -1;
    }

    FILE *fpr = fopen(argv[1],"r");
    
    int count = 0;
    char buf[128] = {0};
    while(1)
    {
        char* ret = fgets(buf,sizeof(buf),fpr);
        if(ret == NULL)
        {
            break;
        }
        count++;
    }
    printf("%d\n",count);

    return 0;
}