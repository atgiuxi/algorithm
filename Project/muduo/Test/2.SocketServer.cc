#include "../Server.hpp"

/*测试Socket功能*/
int main()
{
	// 监听套接字
	Socket listen_sock;
	listen_sock.CreateServer(8888);

	while(1)
	{
		// accept 获取已连接套接字(通信套接字)
		int newfd = listen_sock.Accept();

		if(newfd < 0)
		{
			continue;
		}
		DBG_LOG("获取新连接成功!");

		Socket client_sock(newfd);
		char buf[1024] = {0};
		// 读取数据
		int n = client_sock.Recv(buf,sizeof(buf) -1);
		if(n < 0)
		{
			client_sock.Close();
			continue;
		}	
		// 这里将数据回显给客户端
		client_sock.Send(buf,n);
		client_sock.Close();
	}
	listen_sock.Close();

	return 0;
}