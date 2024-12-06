#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
	int fd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in addr;
	addr.sin_family  = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	socklen_t len = sizeof(addr);
	int ret = connect(fd,(struct sockaddr*)&addr,len);
	
	const char buf[] = "hello world";

	send(fd,buf,sizeof(buf),false);

	close(fd);
	return 0;	
}