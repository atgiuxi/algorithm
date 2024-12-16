#include "../Server.hpp"


uint64_t conn_id = 0;
std::unordered_map<uint64_t,PtrConnection> _conns;
EventLoop base_loop;
std::vector<LoopThread> threads(2);
int next_loop = 0;

void OnMessage(const PtrConnection& conn,Buffer* buf)
{
	DBG_LOG("%s",buf->ReadPosition());
	buf->MoveReadOffset(buf->ReadAbleSize());
	std::string msg = "hello world";
	conn->Send(msg.c_str(),msg.size());
	// conn->Shutdown(); // 通信一次关闭
}

void ConnectionDestroy(const PtrConnection& conn)
{
	_conns.erase(conn->Id());	
}

void OnConnected(const PtrConnection& conn)
{
	DBG_LOG("new connection:%p,",conn.get());
}

void NewConnection(int fd)
{
	conn_id++;
	next_loop = (next_loop + 1) % 2;
	PtrConnection conn(new Connection(threads[next_loop].GetLoop(),conn_id,fd));

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

	DBG_LOG("NEW Child thread---------------------");
}

int main()
{
	srand(time(nullptr));

	Acceptor acceptor(&base_loop,8888);
	// acceptor.Listen();
	
	acceptor.SetAcceptCallback(std::bind(NewConnection,std::placeholders::_1));
	// 	void Listen() {_channel.EnableRead();} 启动都事件监控要在SetAccpetCallback设置回调函数之后
	acceptor.Listen();
	while(1)
	{
		base_loop.Start();	
	}

	return 0;
}