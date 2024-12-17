#include "../Server.hpp"

void OnMessage(const PtrConnection& conn,Buffer* buf)
{
	DBG_LOG("%s",buf->ReadPosition());
	buf->MoveReadOffset(buf->ReadAbleSize());
	std::string msg = "hello world";
	conn->Send(msg.c_str(),msg.size());

	// 这里添加NetWork解决shutdown 之后send 客户端进程退出的问题
	// conn->Shutdown(); // 通信一次关闭
}

void OnClosed(const PtrConnection& conn)
{
	DBG_LOG("closed connection:%p,",conn.get());
}

void OnConnected(const PtrConnection& conn)
{
	DBG_LOG("new connection:%p,",conn.get());
}


int main()
{
	TcpServer server(8888);
	server.SetThreadCount(2);
	server.EnableInactiveRelease(10);

	server.SetClosedCallback(OnClosed);
	server.SetConnectedCallback(OnConnected);
	server.SetMessageCallback(OnMessage);

	server.Start();

	return 0;
}