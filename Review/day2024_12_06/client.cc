#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int fd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in addr;
	addr.sin_family  = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	socklen_t len = sizeof(addr);
	int ret = connect(fd,(struct sockaddr*)&addr,len);
	
	while(1)
	{
		char buf[128] = {0};
		sprintf(buf,"msg : hello wordl");
		send(fd,buf,sizeof(buf),false);
		sleep(1);
	}
	close(fd);
	return 0;	
}