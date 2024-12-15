#include "../Server.hpp"


uint64_t conn_id = 0;
std::unordered_map<uint64_t,PtrConnection> _conns;

void OnMessage(const PtrConnection& conn,Buffer* buf)
{
	DBG_LOG("%s",buf->ReadPosition());
	buf->MoveReadOffset(buf->ReadAbleSize());
	std::string msg = "hello world";
	conn->Send(msg.c_str(),msg.size());
	conn->Shutdown(); // 通信一次关闭
}

void ConnectionDestroy(const PtrConnection& conn)
{
	_conns.erase(conn->Id());	
}

void OnConnected(const PtrConnection& conn)
{
	DBG_LOG("new connection:%p,",conn.get());
}

void Acceptor(EventLoop* loop,Channel* listen_channel)
{
	conn_id++;
	int fd = listen_channel->Fd();
	int newFd = accept(fd,NULL,NULL);
	if(newFd < 0){return ;}
	PtrConnection conn(new Connection(loop,conn_id,newFd));

	// conn->SetMessageCallback(std::bind(OnMessage,conn,std::placeholders::_1));
	conn->SetMessageCallback(std::bind(OnMessage,std::placeholders::_1,std::placeholders::_2));
	// bug : 时间到了不关闭
	// conn->SetSrvClosedCallback(std::bind(ConnectionDestroy,conn));
	// conn->SetConnectedCallback(std::bind(OnConnected,conn));
	conn->SetSrvClosedCallback(std::bind(ConnectionDestroy,std::placeholders::_1));
	conn->SetConnectedCallback(std::bind(OnConnected,std::placeholders::_1));

	conn->EnableInactiveRelease(10);
	conn->Established();	// 就绪初始化,添加读监控

	_conns.insert(std::make_pair(conn_id,conn));
}

int main()
{
	srand(time(nullptr));
	EventLoop loop;
	// 监听套接字
	Socket listen_sock;
	listen_sock.CreateServer(8888);
	
	Channel channel(&loop,listen_sock.Fd());
	channel.SetReadCallback(std::bind(Acceptor,&loop,&channel));
	channel.EnableRead();

	while(1)
	{
		loop.Start();	
	}
	listen_sock.Close();

	return 0;
}