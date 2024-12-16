#include "../Server.hpp"

int main()
{
	Socket client_sock;
	
	int ret = client_sock.CreateClient(8888,"127.0.0.1");
	if(ret == false)
	{
		DBG_LOG("创建客户端失败!");
		return -1;
	}
	for(;;)
	{
		std::string str = "echo : hello world";
		client_sock.Send(str.c_str(),str.size());
		char buf[1024] = {0};
		int ret = client_sock.Recv(buf,sizeof(buf) -1);

		DBG_LOG("%s",buf);
		sleep(1);
	}

	return 0;
}