#include "../Server.hpp"


uint64_t conn_id = 0;
std::unordered_map<uint64_t,PtrConnection> _conns;
EventLoop base_loop;
LoopThreadPool *loop_pool;

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
	PtrConnection conn(new Connection(loop_pool->NextLoop(),conn_id,fd));

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
	loop_pool = new LoopThreadPool(&base_loop);
	loop_pool->SetThreadCount(2);
	loop_pool->Create();

	Acceptor acceptor(&base_loop,8888);
	// acceptor.Listen();
	
	acceptor.SetAcceptCallback(std::bind(NewConnection,std::placeholders::_1));
	// 	void Listen() {_channel.EnableRead();} 启动都事件监控要在SetAccpetCallback设置回调函数之后
	acceptor.Listen();
	base_loop.Start();	// 在Server.hpp中Start就是死循环了 EventLoop 中

	return 0;
}