#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int test1()
{
	fputs("world",stdout);
	fputs("hello",stderr);
	
	while(1)
	{
		sleep(1);
	}
	return 0;
}

int test2()
{
	int ret = open("a.txt",O_RDONLY | O_CREAT,0666);

	if(ret == -1)
	{
		perror("opne failed");
		return -1;
	}
	printf("%d\n",ret);

	close(ret);

	return 0;
}


int test3()
{
	
	int fd = open("a.txt",O_RDONLY | O_CREAT,0666);
	if(fd == -1)
	{
		perror("opne failed");
		return -1;
	}

	char buf[1024] = {0};	

	int ret = read(fd,buf,sizeof(buf) -1);	

	printf("%s \n",buf);
	
	return 0;
}

int test4(int argc,const char* argv[])
{
#if 0
	int fd = open(argv[1],O_RDONLY);
	if(fd == -1)
	{
		return -1;
	}
	printf("%ld\n",lseek(fd,0,SEEK_END));
	write(1,"hello\n",strlen("hello\n"));
	write(2,"hello",sizeof("hello"));
#endif
#if 0
	struct stat s;
	int ret = stat("test.c",&s);

	if(ret == -1)
	{
		return -1;
	}

#endif
	DIR* dptr = opendir("../day2024_11_28");

	if(dptr == NULL)
	{
		return -1;	
	}
	struct dirent * ren = readdir(dptr);



	return 0;
}


int test5()
{
	int fd = open("a.txt",O_RDONLY | O_RDWR | O_CREAT, 0775);
	if(fd == -1)
	{
		printf("open fail\n");
		return -1;
	}

	char str[] = "hello world";

	write(fd,str,sizeof(str));
	printf("%ld\n",lseek(fd,0,SEEK_END));


	return 0;
}

int test6(int argc,const char* argv[])
{
    if(argc != 2)
    {
        printf("参数错误!\n");
        return -1;
    }

    FILE *fpr = fopen(argv[1],"r");
    
    int count = 1;
	char ch = 0;
    while((ch = fgetc(fpr)) != EOF)
    {
		if(ch == '\n')count++;
    }
    printf("%d\n",count);

    return 0;
}


int test7(int argc,const char* argv[])
{
    if(argc != 3)
    {
        printf("参数错误!\n");
        return -1;
    }

    FILE *fpr = fopen(argv[1],"r");
    FILE *fpw = fopen(argv[2],"w");
	
	char ch;
    while((ch = fgetc(fpr)) != EOF)
    {
		fputc(ch,fpw);
    }

    return 0;
}

int test8(int argc,const char* argv[])
{
    if(argc != 3)
    {
        printf("参数错误!\n");
        return -1;
    }

    FILE *fpr = fopen(argv[1],"r");
    FILE *fpw = fopen(argv[2],"w");
	
	char buf[128];
	while((fgets(buf,sizeof(buf) -1,fpr)) != NULL)
	{
		fputs(buf,fpw);
	}

    return 0;
}

int test9(int argc,const char* argv[])
{
    if(argc != 3)
    {
        printf("参数错误!\n");
        return -1;
    }

    FILE *fpr = fopen(argv[1],"r");
    FILE *fpw = fopen(argv[2],"w");
	
	char buf[128] = {0};
	int n = 0;
	while((n = fread(buf,1,sizeof(buf) - 1,fpr)) > 0)
	{
		fwrite(buf,1,n,fpw);
	}

    return 0;
}


int test10()
{
	int fdr = open("test.c",O_RDONLY);
	int fdw = open("a.c",O_WRONLY | O_CREAT,0755);

	char buf[128] = {0};
	int n = 0;
	while((n = read(fdr,buf,sizeof(buf) -1)) > 0)
	{
		write(fdw,buf,n);
	}

	return 0;
}

int test12()
{
	FILE* fp = fopen("a.txt","r");
	if(fp == NULL)
	{
		perror("fopen fail");
		return -1;
	}

	fseek(fp,0,SEEK_END);
	int size = ftell(fp);

	printf("文件的大小是 : %d\n",size);

	return 0;
}

int test13()
{
	int fd = open("a.txt",O_RDONLY);
	if(fd < 0)
	{
		perror("open fail");
		return -1;
	}
	
	off_t size = lseek(fd,0,SEEK_END);

	printf("文件的大小是 : %ld\n",size);
	return 0;
}

int test14(int argc,const char* argv[])
{
	if(argc != 2)
	{
		printf("参数传递错误!");
		return -1;
	}

	DIR * dir = opendir(argv[1]);	

	if(dir == NULL)
	{
		printf("opendir fail\n");
		return -1;
	}

	struct dirent* rent;

	while((rent = readdir(dir)) != NULL)
	{
		if(rent->d_name[0] == '.') {continue;}
		printf("%s\n",rent->d_name);
	}

	closedir(dir);
	return 0;
}

int getLine()
{
	char ch;
	int n = 1;
	FILE* fp = fopen("test.txt","r");
	while ((ch = fgetc (fp)) != EOF)
	{
		if (ch == '\n')
			n++;
	}
	fclose(fp);
	return n;
}

int test15() 
{
	// 打开就到末尾
	FILE* fp = fopen("test.txt","a");
	

	if(fp == NULL)
	{
		perror("fopen faild");
		return -1;
	}

	time_t now; 
	char strTime[32] = {0};

	struct tm* localTime;

	char ch = 0;
	int n = getLine();
	while(1)
	{
		now = time(NULL);
		localTime = localtime(&now);
		fprintf(fp,"%d : %d-%d-%d %d:%d:%d\n",n,localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
		fflush(fp);
		sleep(1);

		n++;
	}

	return 0;
}

typedef struct 
{
    char ID3v1TAG[3];     	// TAG
    char MUSIC_NAME[30];  	// 存储标题信息
    char Artist[30];      	// 存储歌手信息
    char Album[30];       	// 存储专辑信息
    char YEAR[4];        	// 存储年代信息
    char Comments[28];    	// 存储备注信息,28个字节
    char Genre;          	// 音乐风格信息保留位，1个字节
    char Reserved;       	// 保留位，1个字节
    char Track;          	// 音轨(曲号)保留位，1个字节
} MUSIC_TITLE_INFO;

void ReadMp3(const char* fileName,MUSIC_TITLE_INFO *song)
{
	FILE* fp = fopen(fileName,"rb");
	if(fp == NULL)
	{
		printf("fopen file");
		return;
	}

	// 从文件的末尾移动128字节
	fseek(fp,-sizeof(MUSIC_TITLE_INFO),SEEK_END);
	// 将歌曲信息读取到song中
	fread(song,1,sizeof(MUSIC_TITLE_INFO),fp);

	fclose(fp);
}

void Printf(const MUSIC_TITLE_INFO *info) 
{
    printf("歌曲名 : %s\n", info->MUSIC_NAME);
    printf("歌手 : %s\n", info->Artist);
    printf("专辑 : %s\n", info->Album);
    printf("年代: %s\n", info->YEAR);
}

void ChangeArtist(MUSIC_TITLE_INFO *info, const char *name) 
{
    strncpy(info->Artist, name, sizeof(info->Artist) - 1);
    info->Artist[sizeof(info->Artist) - 1] = '\0'; 
}

void Update(MUSIC_TITLE_INFO* info,char* fileName)
{
	FILE* fp = fopen(fileName,"rb+");	
	if(fp == NULL)
	{
		return;
	}

	fseek(fp,-sizeof(MUSIC_TITLE_INFO),SEEK_END);
	fwrite(info,sizeof(MUSIC_TITLE_INFO),1,fp);

	fclose(fp);
}

int test16(int argc, char *argv[]) 
{
    if (argc < 2) 
	{
        printf("Usage: %s <mp3 file>\n", argv[0]);
        return 1;
    }

    MUSIC_TITLE_INFO song;
	ReadMp3(argv[1],&song);
	Printf(&song);

	char name[24];
	printf("请输入修改歌手的名字:");
	scanf("%s",name);
	ChangeArtist(&song,name);

	Update(&song,argv[1]);

	Printf(&song);

   return 0;
}
