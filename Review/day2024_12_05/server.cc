#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	// 获取套接字
	int fd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	socklen_t len = sizeof(addr);

	// 绑定ip地址和端口
	int ret = bind(fd,(sockaddr*)&addr,len);	

	// 监听套接字
	ret = listen(fd,1024);

	// 获取新连接	
	while(1)
	{
		// 获取通信套接字
		int newfd = accept(fd,nullptr,nullptr);

		// 通信操作
		char buf[128] = {0};
		recv(newfd,buf,sizeof(buf),false);	
		printf("%s\n",buf);

		// 关闭
		close(newfd);
	}

	// 关闭连接
	close(fd);

	return 0;
}