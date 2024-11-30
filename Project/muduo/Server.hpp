#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <functional>
#include <sys/epoll.h>

#define INF 0
#define DBG 1
#define ERR 2

#define LOG_LEVEL DBG

#define LOG(level,fmt,...)			\
	do 							\
	{							\
		if(level < LOG_LEVEL) break;										\
		fprintf(stdout,"[%s : %d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__);\
	}while(0)					

#define DBG_LOG(fmt,...) LOG(DBG,fmt,##__VA_ARGS__)
#define ERR_LOG(fmt,...) LOG(ERR,fmt,##__VA_ARGS__)

#define BUFFER_DEFALT_SIZE 1024
class Buffer
{
private:
	std::vector<char> _buffer;	 	// 按照字节(char很妙)存储,用户缓冲区
	uint64_t _writer_idx;			// 写偏移
	uint64_t _read_idx;				// 读偏移
public:
	Buffer():_writer_idx(0),_read_idx(0),_buffer(BUFFER_DEFALT_SIZE){}

	// _buffer.begin() 返回迭代器。* 重载了
	char* Begin(){return &*_buffer.begin();}	
	// 起始位置 + 写偏移量
	char* WritePosition() {return Begin() + _writer_idx;}
	// 起始位置 + 读偏移量
	char* ReadPosition() {return Begin() + _read_idx;}
	// 获取缓冲区尾部的空闲空间 : 容器大小 - 写偏移量
	uint64_t TailIdleSize(){return _buffer.size() - _writer_idx;}
	// 获取缓冲区前面的空闲空间 : 读偏移量 
	uint64_t HeadIdleSize(){return _read_idx;}
	// 获取可读数据的大小 = 写偏移 - 读偏移
	uint64_t ReadAbleSize(){return _writer_idx - _read_idx;}
	// 读偏移移动
	void MoveReadOffset(uint64_t len)
	{
		if(len <= 0) {return;}
		assert(len <= ReadAbleSize());
		// 移动
		_read_idx += len;	
	}	
	// 写偏移移动
	void MoveWriteOffset(uint64_t len)
	{
		if(len <= 0) {return;}
		// 要先写入数据必须尾部要空间要够
		assert(len <= TailIdleSize());
		_writer_idx += len;
	}
	// 确认空间是否足够
	void EnsureWriteSpace(uint64_t len)
	{
		if(len <= TailIdleSize()){return;}
		// 如果插入数据的大小 <= 头部 + 尾部空间的大小,移动数据
		if(len <= TailIdleSize() + HeadIdleSize())
		{
			// 使用std::copy函数.将数据移动到起始位置!
			uint64_t  ras = ReadAbleSize();
			// 这样写的位置是0 + _writer_idx,要的是有效数据的结束位置,是 ReadPosition + 有效数据
			// std::copy(ReadPosition(),WritePosition(),Begin());
			std::copy(ReadPosition(),ReadPosition() + ras,Begin());
			_read_idx = 0;
			_writer_idx = ras;
		}
		else
		{
			// 说明不过空间需要扩容
			DBG_LOG("空间不足需要扩容大小为 : %ld\n",_writer_idx + len);
			_buffer.resize(_writer_idx + len/*原来+ len!*/);
		}
	}
	// 写入数据
	void Write(const void* data,uint64_t len)
	{
		// 增 : 一定要注意空间的问题.
		// 将数据拷贝到buffer中 什么类型 ? 都可以! 
		if(len <= 0) {return;}
		EnsureWriteSpace(len);
		// 这里的const ? TODO
		const char* dst = (const char*)data;
		std::copy(dst,dst + len,WritePosition());
	}
	// 写入数据并移动写偏移
	void WriteAndPush(const void* data,uint64_t len)
	{
		Write(data,len);
		MoveWriteOffset(len);
	}
	// 写入字符串
	void WriteString(const std::string& data)
	{
		Write(data.c_str(),data.size());
	}
	void WriteStringAndPush(const std::string& data)
	{
		WriteString(data);
	}
	void WriteBuffer(Buffer& buffer)
	{
		Write(buffer.ReadPosition(),buffer.ReadAbleSize());
	}
	void WriteBufferAndPush(Buffer& buffer)
	{
		WriteBuffer(buffer);
		MoveWriteOffset(buffer.ReadAbleSize());
	}
	// 读取数据
	void Read(void* buf,uint64_t len)
	{
		// 将buffer中的数据拷贝到用户指定的空间中.
		assert(len <= ReadAbleSize());
		std::copy(ReadPosition(),ReadPosition() + len,(char*)buf);
	}
	// 读取数据并移动
	void ReadAndPop(void* buf,uint64_t len)
	{
		Read(buf,len);
		MoveReadOffset(len);
	}
	// 按照字符串的方式读取,返回一个字符串
	std::string ReadAsString(uint64_t len)
	{
		assert(len <= ReadAbleSize());
		std::string ret;
		// 注意开辟空间,另外ret.c_str()是一个const的,空间不能只是可读的!所以&ret[0]
		ret.resize(len);
		Read(&ret[0],len);
		return ret;
	}
	std::string ReadAsStringAndPop(uint64_t len)
	{
		assert(len <= ReadAbleSize());
		std::string ret;
		ret = ReadAsString(len);
		return ret;
	}
	// memchr 是一个C标准库函数，用于在一个内存块中查找指定的字符(这里是\n)第一次出现的位置
	char* FindCRLF()
	{
		char* ret = (char*)memchr(ReadPosition(),'\n',ReadAbleSize());
		return ret;
	}
	std::string GetLine()
	{
		// 想要读取一行通过FindCRLF获取到\n的位置, pos - ReadPosition + 1 . + 1把\n也读进去
		// 如:Hello\nWorld. pos 是 \n 位置. pos - ReadPosition() 不能把\n读进去
		char* pos = FindCRLF();
		if(pos == nullptr){return "";}
		return ReadAsString(pos - ReadPosition() + 1);
	}
	std::string GetLineAndPop()
	{
		std::string ret = GetLine();
		MoveReadOffset(ret.size());
		return ret;
	}
	void Clear()
	{
		_writer_idx = _read_idx = 0;
	}

};

#define MAX_LISTEN 1024
class Socket
{
private:
	int _sockFd;
public:
	Socket():_sockFd(-1){}
	Socket(int fd):_sockFd(fd){}
public:
	int Fd() {return _sockFd;}
	// 创建套接字
	bool Create()
	{
		// int socket(int domain, int type, int protocol)
		// domain 域 : 指定协议. AF_INET IPv4	
		// type : 套接字的类型，决定了套接字提供的是哪种通信服务.SOCK_STREAM.字节流
		// protocol : 使用的特定协议.可以设置为0.IPPROTO_TCP和SOCK_STREAM配套
		// TPPROTO_TCP : 需要引入<arpa/inet.h>  
		_sockFd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(_sockFd < 0)
		{
			ERR_LOG("创建套接字失败!");
			return false;
		}
		return true;
	}
	// 绑定套接字:TODO
	bool Bind(const std::string& ip,uint64_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());

		socklen_t len = sizeof(struct sockaddr_in);
		int ret = bind(_sockFd,(struct sockaddr*)&addr,len);
		if(ret < 0)
		{
			ERR_LOG("绑定IP地址失败!");
			return false;
		}
		return true;
	}
	// 监听
	bool Listen(int backlog = MAX_LISTEN)
	{
		int ret = listen(_sockFd,backlog);
		if(ret == -1)
		{
			ERR_LOG("监听失败!");
			return false;
		}
		return true;
	}
	// connect,客户端向服务器端发送连接
	bool Connect(const std::string& ip,int port)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		addr.sin_port = htons(port);

		socklen_t len = sizeof(struct sockaddr_in);

		int ret = connect(_sockFd,(struct sockaddr*)&addr,len);
		if(ret < 0)
		{
			ERR_LOG("向服务器连接失败!");
			return false;
		}
		return true;
	}
	// accept 服务器获取新连接
	int Accept()
	{
		int newFd = accept(_sockFd,NULL,NULL);
		if(newFd < 0)
		{
			ERR_LOG("获取连接失败!");
			return -1;
		}
		return newFd;
	}
	// 接收数据
	ssize_t Recv(void* buf,size_t n,int flag = 0/*是否为非阻塞*/)
	{
		ssize_t ret = recv(_sockFd/*?*/,buf,n,flag);
		if(ret <= 0)
		{
			if(errno == EAGAIN || errno == EINTR) {return 0;}
			ERR_LOG("读取数据失败!");
			return -1;
		}

		return ret;
	}
	// 非阻塞读取
	ssize_t NonBlockRecv(void* buf,size_t n)
	{
		return Recv(buf,n,MSG_DONTWAIT/*表示非阻塞*/);
	}
	// 发送数据
	ssize_t Send(const void* buf,size_t n,int flag =0)
	{
		ssize_t ret = send(_sockFd,buf,n,flag);
		if(ret <=0)
		{
			if(errno == EAGAIN || errno == EINTR){return 0;}
			ERR_LOG("数据读取失败!");
		}
		return ret;
	}
	ssize_t NonBlockSend(const void* buf,size_t n)
	{
		return Send(buf,n,MSG_DONTWAIT);
	}
	// 关闭套接字
	void Close()
	{
		if(_sockFd != -1)
		{
			close(_sockFd);
			_sockFd = -1;
		}
	}
	// 设置套接字为非阻塞情况
	void NonBlock()
	{
		int falg = fcntl(_sockFd,F_GETFL,0);
		fcntl(_sockFd,F_SETFL,falg | O_NONBLOCK);
	}
	// 设置地址重用
	void ReuseAddress()
	{
		int val = 1;
		setsockopt(_sockFd,SOL_SOCKET,SO_REUSEADDR,(void*)&val,sizeof(int));
		val = 1;
		setsockopt(_sockFd,SOL_SOCKET,SO_REUSEADDR,(void*)&val,sizeof(int));
	}
	// 为了创建简单来创建提供创建服务端和客户端的接口
	bool CreateServer(uint16_t port,const std::string&ip = "0.0.0.0",bool block_falg = false)
	{
		// 1.创建套接字
		if(Create() == false) {return false;}
		// 2.设置非阻塞
		if(block_falg == true) {NonBlock();}
		// 3.绑定套接字
		if(Bind(ip,port) == false) {return false;}
		// 4.开始监听
		if(Listen() == false) {return true;}
		// 5.启动地址重用
		ReuseAddress();
		return true;
	}
	bool CreateClient(uint16_t port,const std::string&ip)
	{
		// 1.获取套接字
		if(Create() == false) return false;
		// 2.向服务器请求连接
		if(Connect(ip,port) == false) return false;
		return true;
	}
};

class Channel
{
private:
	int _fd;
	uint32_t _events;	// 当前需要监控的事件
	uint32_t _revents;	// 当前连接触发的事件
	using EventCallback = std::function<void()>;
	EventCallback _read_callback;	// 可读事件被触发的函数
	EventCallback _write_callback;	// 可写事件被触发的函数
	EventCallback _error_callback;	// 错误事件被触发的函数
	EventCallback _close_callback;	// 连接断开事件被触发的函数
	EventCallback _event_callback;	// 任意事件被触发的函数
public:
	// 对描述符事件管理类
	Channel(int fd):_fd(fd),_events(0),_revents(0){}
	int Fd(){return _fd;}
	// 设置就绪的事件
	void SetREvents(uint32_t evnets){_revents = evnets;}
	void SetReadCallback(const EventCallback& cb){_read_callback = cb;}
	void SetWriteCallback(const EventCallback& cb){_write_callback = cb;}
	void SetErrorCallback(const EventCallback& cb){_error_callback = cb;}
	void SetCloseCallback(const EventCallback& cb){_close_callback = cb;}
	void SetEventCallback(const EventCallback& cb){_event_callback = cb;}
	// 当前是否监控了可读
	bool ReadAble()
	{
		return (_events & EPOLLIN);
	}
	// 当前是否监控了可写
	bool WriteAble()
	{
		return (_events & EPOLLOUT);
	}
	// 启动读事件监控
	void EnableRead()
	{
		// 后续会添加到Eventloop的事件监控中! epollctl设置
		_events |= EPOLLIN;
	}
	// 启动写事件监控
	void EnableWrite()
	{
		_events |= EPOLLOUT;
	}
	// 关闭读事件监控
	void DisableRead()
	{
		_events &= ~EPOLLIN;
	}
	// 关闭写事件监控
	void DisableWrite()
	{
		_events &= ~EPOLLOUT;
	}
	// 关闭所有事件监控
	void DisableAll()
	{
		_events = 0;
	}
	// 移除监控
	void Remove()
	{
		// 需要调用EventLoop接口来移除监控;TODO
	}
	// 数据处理,一旦连接触发事件就调用这个函数.触发什么事件通过回调函数来设置处理方法
	void HandleEvent()
	{
		// 要求上层缓冲区处理数据
		if((_revents & EPOLLIN) || (_revents & EPOLLRDHUP)/*对方半关闭的连接*/ || (_revents & EPOLLPRI/*优先带外数据*/))
		{
			if(_read_callback) {_read_callback();}
			// 任意函数在之后
			if(_event_callback){_event_callback();}
		}

		// 下面有可能会出错然后释放连接,就不能继续访问了
		if(_revents & EPOLLOUT)
		{
			// 任意函数在之后,刷新活跃度
			if(_write_callback) {_write_callback();}
			if(_event_callback){_event_callback();}
		}
		else if(_revents & EPOLLERR)
		{
			// 任意处理函数再前,因为出错再去访问任意函数就不合适
			if(_event_callback){_event_callback();}
			if(_error_callback) {_error_callback();}
		}
		else if(_revents & EPOLLHUP)
		{
			// 任意处理函数再前,因为出错再去访问任意函数就不合适
			if(_event_callback){_event_callback();}
			if(_error_callback) {_error_callback();}
		}
	}
};