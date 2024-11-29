#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int test1()
{
	int ret = fork();

	if(ret == -1)
	{
		perror("fork fail");
		return -1;
	}
	else if(ret == 0)
	{
		execl("/usr/bin/ls","ls","-l",NULL);
		printf("pid = %d : ppid = %d\n",getpid(),getppid());
	}
	else if(ret > 0)
	{
		printf("pid = %d : ppid = %d\n",getpid(),getppid());
		wait(NULL);
	}
	
	return 0;
}


int test2(int argc,const char* argv[])
{
	int ret = fork();
	if(argc != 3)
	{
		printf("usege : 命令 参数 参数\n");
		return -1;
	}

	if(ret < 0)
	{
		perror("fork fail");
		return -1;
	}
	else if(ret == 0)
	{
		execlp("cp","cp",argv[1],argv[2],NULL);
	}
	else if(ret > 0)
	{
		wait(NULL);
	
	}

	return 0;
}


int test3(int argc,const char* argv[])
{
	int ret = fork();
	if(ret < 0)
	{
		perror("fork fail");
		return -1;
	}
	else if(ret == 0)
	{
		execlp("bash","bash","test.sh",NULL);
	}
	else if(ret > 0)
	{
		wait(NULL);
	}

	return 0;
}

int main()
{
	int ret = fork();

	if(ret < 0) 
	{
		return -1;
	}
	else if(ret == 0)
	{
		setsid();

		int num = getdtablesize();
		for(int i= 0;i < num;i++)
		{
			close(1);
		}
	}
	else if(ret > 0)
	{
		
	}

	return 0;
}
