#include "../Server.hpp"

void HandleClose(Channel* channel)
{

	DBG_LOG("close %d",channel->Fd());	
	channel->Remove();
	delete channel;
}

void HandleRead(Channel* channel)
{
	int fd = channel->Fd();
	char buf[1024] = {0};
	int ret = recv(fd,buf,sizeof(buf) -1,0);
	// 这里如果可读事件失败(对方断开连接)会释放.一般真正的服务器要检查缓冲区不会立即释放
	if(ret <=0) // 这里还得是 <= 0 不会退出不会进if成功语句
	{
		HandleClose(channel);
		// 注意这里要return 不然关闭连接了还去访问channel->EnableWrite();
		return;
	}
	channel->EnableWrite();
	// std::cout << buf << std::endl;
	DBG_LOG("%s %d",buf,channel->Fd());	
}

int n = 0;

void HandleWrite(Channel* channel)
{
	int fd = channel->Fd();
	std::string data= "海蒂和爷爷 " + std::to_string(fd) + " : " + std::to_string(++n);

	int ret = send(fd,data.c_str(),data.size(),0);
	// 写失败也会释放
	if(ret < 0)
	{
		HandleClose(channel);
	}
	channel->DisableWrite();
}

void HandleError(Channel* channel)
{
	HandleClose(channel);
}
void HandleEvent(EventLoop* loop,Channel* channel,uint64_t timerid)
{
	std::cout << "get event "<< channel->Fd() << std::endl;
	
	// 如果有任何事件到来,都要刷新活跃度
	loop->TimerRefresh(timerid);
}

void Acceptor(EventLoop* loop,Channel* listen_channel)
{
	int fd = listen_channel->Fd();
	int newFd = accept(fd,NULL,NULL);
	if(newFd < 0){return ;}
	Channel* channel = new Channel(loop,newFd);

	uint64_t timerid = rand() % 10000;
	channel->SetReadCallback(std::bind(HandleRead,channel));
	channel->SetWriteCallback(std::bind(HandleWrite,channel));
	channel->SetCloseCallback(std::bind(HandleClose,channel));
	channel->SetErrorCallback(std::bind(HandleError,channel));
	channel->SetEventCallback(std::bind(HandleEvent,loop,channel,timerid));

	// 非活跃的超时释放,10秒关闭
	// 注意:定时销毁任务必须在都事件之前EnableRead,因为可能启动都事件监控.立即有事件,但是这是还没有任务!
	// int timerid = rand() % 10000;
	loop->TimerAdd(timerid,10,std::bind(HandleClose,channel));

	channel->EnableRead();
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