#include "common.h"

int main()
{
	pid_t ret = fork();
	if(ret < 0)
	{
		perror("fork fail");
		return -1;
	}
	else if(ret == 0)
	{
		setsid();
		chdir("/home/xiyan/space/algorithm/Review/day2024_12_01");
		umask(0);

		int num = getdtablesize();
		for(int i =0;i < num;i++)
		{
			close(i);
		}

		int fd = open("log.txt",O_WRONLY | O_CREAT);
		if(fd < 0)
		{
			perror("open failed");
			return -1;
		}

		char buf[128] = {0};
		while(1)
		{
			time_t t = time(NULL);
			sprintf(buf,"%s",ctime(&t));
			write(fd,buf,strlen(buf));
			memset(buf,0,sizeof(buf));
			sleep(100);
		}
	}
	else if(ret > 0)
	{
		exit(0);
	}

	return 0;
}