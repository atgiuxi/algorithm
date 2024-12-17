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
#include <memory>
#include <sys/epoll.h>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <condition_variable>
#include <time.h>
#include <signal.h>

#define INF 0
#define DBG 1
#define ERR 2

#define LOG_LEVEL DBG

#define LOG(level,fmt,...)			\
	do 							\
	{							\
		if(level < LOG_LEVEL) break;										\
		time_t t = time(nullptr);\
		struct tm* ltm = localtime(&t);\
		char tmp[32] = {0};\
		strftime(tmp,31,"%H:%M:%S",ltm);\
		fprintf(stdout,"[%p %s %s : %d]" fmt "\n",(void*)pthread_self(),tmp,__FILE__,__LINE__,##__VA_ARGS__);\
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
	}
	// 为了创建简单来创建提供创建服务端和客户端的接口
	bool CreateServer(uint16_t port,const std::string&ip = "0.0.0.0",bool block_falg = false)
	{
		// 1.创建套接字
		if(Create() == false) {return false;}
		// 2.设置非阻塞
		if(block_falg == true) {NonBlock();}
		// 5.启动地址重用
		ReuseAddress();
		// 3.绑定套接字
		if(Bind(ip,port) == false) {return false;}
		// 4.开始监听
		if(Listen() == false) {return true;}
		// // 5.启动地址重用 bug 地址重用要在bind之前.不然server 和client运行.主动关闭server在启动绑定失败
		// ReuseAddress();
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

class EventLoop;
class Poller;
class Channel
{
private:
	int _fd;
	// Poller* _poller;
	EventLoop* _loop;
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
	Channel(/*Poller* poller*/EventLoop* loop,int fd):/*_poller(poller)*/_loop(loop),_fd(fd),_events(0),_revents(0){}
	int Fd(){return _fd;}
	uint32_t Events(){return _events;}
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
		Update();
	}
	// 启动写事件监控
	void EnableWrite()
	{
		_events |= EPOLLOUT;
		Update();
	}
	// 关闭读事件监控
	void DisableRead()
	{
		_events &= ~EPOLLIN;
		Update();
	}
	// 关闭写事件监控
	void DisableWrite()
	{
		_events &= ~EPOLLOUT;
		Update();
	}
	// 关闭所有事件监控
	void DisableAll()
	{
		_events = 0;
		Update();
	}
	// 上面只是声明了_poller这里不能这么写
	// 移除监控
	// void Remove()
	// {
	// 	// 需要调用EventLoop接口来移除监控;TODO
	// 	_poller->RemoveEvent(this);
	// }
	// void Update(){_poller->UpdateEvent(this);}
	// 声明和实现分离
	void Remove();
	void Update();

	// 数据处理,一旦连接触发事件就调用这个函数.触发什么事件通过回调函数来设置处理方法
	void HandleEvent()
	{
		// 要求上层缓冲区处理数据
		if((_revents & EPOLLIN) || (_revents & EPOLLRDHUP)/*对方半关闭的连接*/ || (_revents & EPOLLPRI/*优先带外数据*/))
		{
			// 这个地方有问题,如果对方断开连接,还回去回调_event_callback有问题,因为这里再TestPollerServer.cc
			// 写的时候会关闭channel,再去if(_event_callback){_event_callback();}有问题
			// if(_read_callback) {_read_callback();}
			// 任意函数在之后
			// if(_event_callback){_event_callback();}

			// if(_event_callback){_event_callback();}
			if(_read_callback) {_read_callback();}
		}

		// 下面有可能会出错然后释放连接,就不能继续访问了
		if(_revents & EPOLLOUT)
		{
			// 这里也上面的一样,写事件失败也会关闭channel然后再去访问_event_callback
			// 任意函数在之后,刷新活跃度
			// if(_write_callback) {_write_callback();}
			// if(_event_callback){_event_callback();}

			// if(_event_callback) {_event_callback();}
			if(_write_callback) {_write_callback();}

		}
		else if(_revents & EPOLLERR)
		{
			// 任意处理函数再前,因为出错再去访问任意函数就不合适
			// if(_event_callback){_event_callback();}

			if(_error_callback) {_error_callback();}
		}
		else if(_revents & EPOLLHUP)
		{
			// 任意处理函数再前,因为出错再去访问任意函数就不合适
			// if(_event_callback){_event_callback();}
			if(_error_callback) {_error_callback();}
		}

		// 修改
		if(_event_callback){_event_callback();}
	}
};

#define MAX_EPOLLEVENTS 1024
class Poller
{	
private:
	int _epfd;
	struct epoll_event _evs[MAX_EPOLLEVENTS];
	std::unordered_map<int,Channel*> _channels;
private:
	// 对描述符的直接操作
	void Update(Channel* channel,int op/*用户操作*/)
	{
		int fd = channel->Fd();
		struct epoll_event ev;
		ev.data.fd = fd;
		ev.events = channel->Events();
		int ret = epoll_ctl(_epfd,op,fd,&ev);

		if(ret < 0)
		{
			ERR_LOG("epoll_ctl failed");
		}
		return;
	}

	// 判断一个Channel是否添加了事件监控
	bool HasChannel(Channel* channel)
	{
		auto it = _channels.find(channel->Fd());

		if(it == _channels.end()) {return false;}
		return true;
	}
public:
	// Poller(); // TestPoller.cc:(.text+0x5d3): undefined reference to `Poller::Poller()'
	Poller()
	{
		_epfd = epoll_create(MAX_EPOLLEVENTS);
		if(_epfd < 0)
		{
			ERR_LOG("EPOLL CREATE FAILED");
			abort();
		}
	}

	void UpdateEvent(Channel* channel)
	{
		bool ret = HasChannel(channel);
		if(ret == false)
		{
			// 如果没有被事件监控那么就添加
			_channels.insert(std::make_pair(channel->Fd(),channel));
			Update(channel,EPOLL_CTL_ADD);
			return;
		}
		// 添加了就修改
		Update(channel,EPOLL_CTL_MOD);
		return;
	}

	void RemoveEvent(Channel* channel)
	{
		auto it = _channels.find(channel->Fd());
		if(it != _channels.end())
		{
			_channels.erase(it);
		}
		Update(channel,EPOLL_CTL_DEL);
	}

	void Poll(std::vector<Channel*>* active)
	{
		int nfds = epoll_wait(_epfd,_evs,MAX_EPOLLEVENTS,-1);

		if(nfds < 0)
		{
			if(errno == EINTR) {return;}
			ERR_LOG("EPOLL WAIT ERROR:%s\n",strerror(errno));
			abort();
		}

		for(int i = 0;i < nfds;i++)
		{
			auto it = _channels.find(_evs[i].data.fd);
			assert(it != _channels.end());
			// 设置描述符实际就绪的事件
			it->second->SetREvents(_evs[i].events);
			active->push_back(it->second);
		}
	}
};

using TaskFunc = std::function<void()>;
using ReleaseFunc = std::function<void()>;
class TimerTask
{
private:
	uint64_t _id;			// 定时器任务id
	uint32_t _timeout;		// 定时任务超时时间
	bool _canceled;			// 任务是否被取消。false : 没有被取消. true 被取消
	TaskFunc _task_cb;		// 定时器对象要执行的回调函数
	ReleaseFunc _release;	// 删除任务对象的信息回调函数

public:
	TimerTask(uint64_t id,uint32_t delay,const TaskFunc& cb)
		/*bug : bad_function_call .延时时间10后释放_task_cb没有设置*/
		:_id(id),_timeout(delay),_task_cb(cb),_canceled(false)
	{}

	~TimerTask()
	{
		if(_canceled == false){_task_cb();}		// 如果任务没有被取消,那么执行任务
		_release();		// 无论有没有执行任务都要清理对象
	}

	// 取消任务
	void Cancel(){_canceled = true;}

	// 设置任务清理的回调
	void SetRelease(const ReleaseFunc& cb){_release = cb;}

	// 获取延时时间
	uint32_t DelayTimer(){return _timeout;}
};
class TimerWheel
{
private:
	using WeakTask = std::weak_ptr<TimerTask>;
	using PtrTask = std::shared_ptr<TimerTask>;
	int _tick;		// 时钟秒指针.指向哪个地方就释放,也就是说执行任务
	int _capatity;	// 表盘的数量,即:最大的延迟时间
	std::vector<std::vector<PtrTask>> _wheel;		// 时间轮	
	std::unordered_map<uint64_t,WeakTask> _timers;	// 保存waek_ptr 对象的 

	int _timerfd;	// 定时器描述符
	EventLoop* _loop;
	std::unique_ptr<Channel> _timer_channel;
private:
	// 移除weak_ptr对象
	void RemoveTimer(uint64_t id)
	{
		auto it = _timers.find(id);
		if(it != _timers.end())
		{
			_timers.erase(it);
		}
	}

	// 操作系统中提供的定时器
	static int CreateTimerfd()
	{
		int timerfd = timerfd_create(CLOCK_MONOTONIC,0);
		if(timerfd < 0)
		{
			ERR_LOG("创建定时器失败!");
			abort();
		}

		struct itimerspec itime;
		itime.it_value.tv_sec = 1;
		itime.it_value.tv_nsec = 0;
		itime.it_interval.tv_sec = 1;
		itime.it_interval.tv_nsec = 0;

		timerfd_settime(timerfd,0,&itime,NULL);
		return timerfd;
	}

	// 读取定时器描述符
	int ReadTimefd()
	{
		uint64_t times;
		int ret = read(_timerfd,&times,8); // 8 nbyes 是 timers的大小

		if(ret < 0)
		{
			ERR_LOG("读取定时器描述符失败!");
			abort();
		}
		return times;
	}

	// 该函每秒钟被执行一次.相当于秒针向后走一步
	void RunTimerTask()
	{
		_tick =(_tick + 1) % _capatity;
		// _tick走到哪里哪里的任务就要被执行.数组中保存是shared_ptr的任务列表.都会被释放
		_wheel[_tick].clear();
	}

	// 根据实际的超时次数,执行对应的超时任务
	void OnTime()
	{
		int times = ReadTimefd();
		for(int i = 0;i < times;i++)
		{
			RunTimerTask();
		}
	}	

	void TimerAddInLoop(uint64_t id,uint32_t delay,const TaskFunc& cb)
	{
		PtrTask pt(new TimerTask(id,delay,cb));
		// pt->SetRelease(std::bind(&TimerWheel::RemoveTimer,this,id));
		pt->SetRelease(std::bind(&TimerWheel::RemoveTimer,this,id));

		// 将任务设置到时间轮中
		int pos = (_tick + delay) % _capatity;
		_wheel[pos].push_back(pt);

		// 更加shared_ptr保存一份weak_ptr
		_timers[id] = WeakTask(pt);
	}

	void TimerRefreshInLoop(uint64_t id)
	{
		// 通过保存的weak_ptr构造一个shared_ptr,添加到轮子中.然后延迟任务
		auto it = _timers.find(id);

		if(it == _timers.end()) {return ;}
		
		PtrTask pt = it->second.lock();	// 通过weak_ptr中的lock函数构造sheard_ptr
		// 延迟任务重新将任务添加到时间轮中
		int delay = pt->DelayTimer();
		int pos = (_tick + delay) % _capatity;
		_wheel[pos].push_back(pt);
	}

	void TimerCancelInLoop(uint64_t id)
	{
		auto it = _timers.find(id);
		if(it == _timers.end()) {return;}

		// weak_ptr不能操作管理的对象,需要通过shared_ptr
		PtrTask pt = it->second.lock();
		if(pt) {pt->Cancel();}	// 取消任务
	}
public:
	TimerWheel(EventLoop* loop)
		:_capatity(60),_tick(0),_wheel(_capatity),_loop(loop),_timerfd(CreateTimerfd()),_timer_channel(new Channel(_loop,_timerfd))
	{
		// 设置可读事件回调
		_timer_channel->SetReadCallback(std::bind(&TimerWheel::OnTime,this));
		// 其实读事件监控
		_timer_channel->EnableRead();
	}

	// 定时器中_timer成员是管理weak_ptr对象的.定时器的操作可能是在多线程下进行,需要考虑线程安全问题
	// 如果不加锁,就对对定期的所有操作都放到一个线程中进行
	// 这里是声明 : 
	void TimerAdd(uint64_t id,uint32_t delay,const TaskFunc& cb);
	void TimerRefresh(uint64_t id);
	void TimerCancel(uint64_t id);

	// 这个接口有线程安全问题.严格来说要在对应的EventLoop线程中执行
	bool HasTimer(uint64_t id)
	{
		auto it = _timers.find(id);
		if(it == _timers.end()) {return false;}
		else {return true;}
	}

};

class EventLoop
{
private:
	using Functor = std::function<void()>;
	std::thread::id _thread_id;					// 线程id
	int _event_fd;								// 
	std::unique_ptr<Channel> _event_channel;	// 
	Poller _poller;								// 
	std::vector<Functor> _tasks;				// 任务池
	std::mutex _mutex;
	TimerWheel _timer_wheel;
public:
	// 执行任务池中的所有任务
	void RunAllTask()
	{
		std::vector<Functor> functor;
		// 这里这个作用域 unique_lock<std::mutex> 出了花括号就会销毁
		{
			std::unique_lock<std::mutex> _lock(_mutex);
			// _tasks 的任务从哪里来?
			_tasks.swap(functor);
		}
		// 遍历执行任务
		for(auto& f : functor) {f();}
	}

	static int CreateEventFd()
	{
		// eventfd的作用给是?
		int efd = eventfd(0,EFD_CLOEXEC | EFD_NONBLOCK);

		if(efd < 0)
		{
			ERR_LOG("创建eventfd失败!");
			abort();
		}
		return efd;
	}

	void ReadEventfd()
	{
		uint64_t res = 0;
		// TODO,这就是一个局部的变量没有关心
		int ret = read(_event_fd,&res,sizeof(res));
		if(ret < 0)
		{
			// EINTR : 信号中断.EAGAIN : 无数据可读
			if(errno == EINTR || errno == EAGAIN) {return;}
			ERR_LOG("READ EVENTFD FAILED!");
			abort();
		}
	}
	
	void WeakUpEventFd()
	{
		uint64_t val = 1;
		int ret = write(_event_fd,&val,sizeof(val));
		if(ret < 0)
		{
			if(errno == EINTR) {return;}
			ERR_LOG("WRITE EVENTFD FAILED!");
			abort();
		}
	}	

public:
	EventLoop():_thread_id(std::this_thread::get_id()),
				_event_fd(CreateEventFd()),
				_event_channel(new Channel(this,_event_fd)),
				_timer_wheel(this)
	{
		// 给eventfd添加可读事件回调,读取eventfd事件通知次数
		_event_channel->SetEventCallback(std::bind(&EventLoop::ReadEventfd,this));
		// 启动eventfd的读事件的监控
		_event_channel->EnableRead();
	}

	void Start()
	{
		while(1)
		{
			// 1.事件监控
			std::vector<Channel*> actives;
			// 获取活跃的连接
			_poller.Poll(&actives);
			// 2.事件处理
			for(auto& channel : actives)
			{
				channel->HandleEvent();
			}
			// 3.执行任务
			RunAllTask();
		}
	}

	// 判断当前线程是否在EventLoop对应的线程中
	bool IsInLoop() { return _thread_id == std::this_thread::get_id();}
	void AssertInLoop() {assert(_thread_id == std::this_thread::get_id());}

	// 当前认识是否处于当前线程中,如果是执行,否则压入队列中	
	void RunInLoop(const Functor& cb)
	{
		if(IsInLoop()) {return cb(); /*cb()这样就是执行了并返回执行的结果*/}
		return QueueInLoop(cb);
	}

	// 将任务压入任务池中
	void QueueInLoop(const Functor& cb)
	{
		{
			std::unique_lock<std::mutex> _lock(_mutex);
			_tasks.push_back(cb);
		}
		// TODO
		WeakUpEventFd();
	}

	void UpdateEvent(Channel* channel){return _poller.UpdateEvent(channel);}
	void RemoveEvent(Channel* channel){return _poller.RemoveEvent(channel);}
	void TimerAdd(uint64_t id,uint32_t delay,const TaskFunc&cb) {return _timer_wheel.TimerAdd(id,delay,cb);};
	void TimerRefresh(uint64_t id){return _timer_wheel.TimerRefresh(id);}
	void TimerCannel(uint64_t id){return _timer_wheel.TimerCancel(id);}
	bool HasTimer(uint64_t id) {return _timer_wheel.HasTimer(id);}

};

class Any
{
private:
	class holder 
	{
	public:
		virtual ~holder() {}
		virtual const std::type_info& type() = 0;
		virtual holder *clone() = 0;
	};
	template<class T>
	class placeholder: public holder 
	{
	public:
		placeholder(const T &val): _val(val) {}
		// 获取子类对象保存的数据类型
		virtual const std::type_info& type() { return typeid(T); }
		// 针对当前的对象自身，克隆出一个新的子类对象
		virtual holder *clone() { return new placeholder(_val); }
	public:
		T _val;
	};

	holder *_content;
public:
	Any():_content(NULL) {}
	template<class T>
	Any(const T &val):_content(new placeholder<T>(val)) {}
	Any(const Any &other):_content(other._content ? other._content->clone() : NULL) {}
	~Any() { delete _content; }

	Any &swap(Any &other) 
	{
		std::swap(_content, other._content);
		return *this;
	}

	// 返回子类对象保存的数据的指针
	template<class T>
	T *get() {
		//想要获取的数据类型，必须和保存的数据类型一致
		assert(typeid(T) == _content->type());
		return &((placeholder<T>*)_content)->_val;
	}
	//赋值运算符的重载函数
	template<class T>
	Any& operator=(const T &val) 
	{
		//为val构造一个临时的通用容器，然后与当前容器自身进行指针交换，临时对象释放的时候，原先保存的数据也就被释放
		Any(val).swap(*this);
		return *this;
	}
	Any& operator=(const Any &other) 
	{
		Any(other).swap(*this);
		return *this;
	}
};

class Connection;

typedef enum{
	DISCONNECTED,	// 连接关闭状态
	CONNECTING,		// 连接建立成功,待处理状态
	CONNECTED,		// 连接建立完成,各种设置已完成,可以通信的状态
	DISCONNECTING	// 待关闭状态
} ConnStatu;

using PtrConnection = std::shared_ptr<Connection>;

class Connection : public std::enable_shared_from_this<Connection>
{
private:
	uint64_t _conn_id;		// 连接id,和定时器id共用
	int _sockfd;			// 连接关联的文件描述符
	bool _enable_inactive_release;	// 连接是否启动非活跃销毁的判断标志,默认是false
	EventLoop* _loop;		// 连接关联的EventLoop
	ConnStatu _statu;		// 连接状态
	Socket _socket;			// 套接字操作管理
	Channel _channel;		// 连接事件管理
	Buffer _in_buffer;		// 输入缓冲区
	Buffer _out_buffer;		// 输出缓冲区
	Any _context;			// 上下文

	using ConnectedCallback = std::function<void(const PtrConnection&)>;
	using MessageCallback = std::function<void(const PtrConnection&,Buffer*)>;
	using ClosedCallback = std::function<void(const PtrConnection&)>;
	using AnyEventCallback = std::function<void(const PtrConnection&)>;

	ConnectedCallback _connected_callback;
	MessageCallback _message_callback;
	ClosedCallback _closed_callback;
	AnyEventCallback _event_callback;
	ClosedCallback _server_closed_callback;
private:
	// channel 事件的回调函数
	void HandleRead()
	{
		// 1.接收缓冲区
		char buf[65536];
		ssize_t ret = _socket.NonBlockRecv(buf,sizeof(buf) -1);
		if(ret < 0)
		{
			// 出错了,但不能直接关闭连接
			return ShutdownInLoop();
		}

		// 等于 0 说明没有读取到数据并不是连接断开.-1是连接断开
		// 将数据放入输入缓冲区.
		_in_buffer.WriteAndPush(buf,ret);

		// 2.调用message_callback进行业务处理
		if(_in_buffer.ReadAbleSize() > 0)
		{
			// std::enable_shared_from_this<Connection> 中的shared_from_this ,让对象获取一个自身的shared_ptr
			return _message_callback(shared_from_this(),&_in_buffer);
		}
	}

	void HandleWrite()
	{
		// _out_buffer 中保存的就是要发送的数据
		// bug NonBlockRecv
		ssize_t ret = _socket.NonBlockSend(_out_buffer.ReadPosition(),_out_buffer.ReadAbleSize());
		if(ret < 0)
		{
			// 发送错误关闭连接
			if(_in_buffer.ReadAbleSize() > 0)
			{
				// 要关闭了就要吧缓冲区中的数据读出来
				_message_callback(shared_from_this(),&_in_buffer);
			}
			return Release();	// 实际关闭释放的操作
		}

		_out_buffer.MoveReadOffset(ret);
		if(_out_buffer.ReadAbleSize() == 0)	
		{
			// 没事数据要发送了,就关闭写事件监控
			_channel.DisableWrite();
			// 如果当前连接时待关闭状态,如果有数据就发送完关闭,没有数据直接关闭
			// bug DISCONNECTED
			if(_statu == DISCONNECTING) {return Release();}
		}
	}

	void HandleClose()
	{
		if(_in_buffer.ReadAbleSize() > 0)
		{
			_message_callback(shared_from_this(),&_in_buffer);
		}
		return Release();
	}

	void HandleError()
	{
		return HandleClose();
	}

	// 任意时间被触发1.刷新连接的活跃度 2.调用任意时间回调
	void HandleEvent()
	{
		if(_enable_inactive_release == true) {_loop->TimerRefresh(_conn_id);}
		if(_event_callback) {_event_callback(shared_from_this());}
	}

	// 获取连接,设置状态(启动读监控,调用回调函数)
	void EstablishedInLoop()
	{
		// 当前状态一定是上层处于半连接状态
		assert(_statu == CONNECTING);

		// 如果当前函数执行完毕,就处于连接状态
		_statu = CONNECTED;

		// 读事件监控一旦启动就可能被立即触发.
		_channel.EnableRead();
		if(_connected_callback){_connected_callback(shared_from_this());}
	}

	// 真正释放连接的接口
	void ReleaseInLoop()
	{
		_statu = DISCONNECTED;
		_channel.Remove();
		_socket.Close();
		if(_loop->HasTimer(_conn_id)){CancelInactiveReleaseInLoop();}
		if(_closed_callback) {_closed_callback(shared_from_this());}
		if(_server_closed_callback) {_server_closed_callback(shared_from_this());}
	}

	// 不是实际发送的窗口,而是将数据放到发送缓冲区中,启动可写事件监控
	void SendInLoop(Buffer& buf)
	{
		if(_statu == DISCONNECTED) {return ;}
		_out_buffer.WriteBufferAndPush(buf);
		if(_channel.WriteAble() == false)
		{
			_channel.EnableWrite();
		} 
	}

	void ShutdownInLoop()
	{
		_statu = DISCONNECTING; 	// 设置连接为半关闭的状态
		if(_in_buffer.ReadAbleSize() > 0)
		{
			if(_message_callback) {_message_callback(shared_from_this(),&_in_buffer);}
		}

		// 写入数据的时候出错!没有数据待发送直接关闭
		if(_out_buffer.ReadAbleSize() > 0)
		{
			if(_channel.WriteAble() == false) {_channel.EnableWrite();}
		}
		if(_out_buffer.ReadAbleSize() == 0) {Release();}
	}

	// 启动非活跃连接超时释放规则
	void EnableInactiveReleaseInLoop(int sec)
	{
		_enable_inactive_release = true;

		if(_loop->HasTimer(_conn_id))
		{
			return _loop->TimerRefresh(_conn_id);
		}
		// 如果不存在定时任务,就新增
		_loop->TimerAdd(_conn_id,sec,std::bind(&Connection::Release,this));
	}
	
	void CancelInactiveReleaseInLoop()
	{
		_enable_inactive_release = false;
		if(_loop->HasTimer(_conn_id))
		{
			_loop->TimerCannel(_conn_id);
		}
	}

	void UpgradeInLoop(	const Any &context, 
						const ConnectedCallback &conn, 
						const MessageCallback &msg, 
						const ClosedCallback &closed, 
						const AnyEventCallback &event) 
	{
		_context = context;
		_connected_callback = conn;
		_message_callback = msg;
		_closed_callback = closed;
		_event_callback = event;
	}
public:
	Connection(EventLoop* loop,uint64_t conn_id,int sockfd)
		:_conn_id(conn_id)
		,_sockfd(sockfd)
		,_enable_inactive_release(false)
		,_loop(loop)
		,_statu(CONNECTING)
		,_socket(_sockfd)
		,_channel(loop,_sockfd)
	{
		_channel.SetCloseCallback(std::bind(&Connection::HandleClose,this));
		_channel.SetEventCallback(std::bind(&Connection::HandleEvent,this));
		_channel.SetReadCallback(std::bind(&Connection::HandleRead,this));
		_channel.SetWriteCallback(std::bind(&Connection::HandleWrite,this));
		_channel.SetErrorCallback(std::bind(&Connection::HandleError,this));
	}

	~Connection() {DBG_LOG("RELEASE CONNECTION:%p",this);}
	int Fd() {return _sockfd;}
	int Id() {return _conn_id;}
	bool Connected() {return _statu == CONNECTED;}

	void SetConnectedCallback(const ConnectedCallback& cb) {_connected_callback = cb;}
	void SetMessageCallback(const MessageCallback& cb) {_message_callback = cb;}
	void SetClosedCallback(const ClosedCallback& cb) {_closed_callback = cb;}
	void SetAnyEventCallback(const AnyEventCallback& cb) {_event_callback = cb;}
	void SetSrvClosedCallback(const ClosedCallback& cb) {_server_closed_callback = cb;}
	
	void Established()
	{
		_loop->RunInLoop(std::bind(&Connection::EstablishedInLoop,this));
	}

	void Send(const char* data,size_t len)
	{
		Buffer buf;
		buf.WriteAndPush(data,len);
		_loop->RunInLoop(std::bind(&Connection::SendInLoop,this,std::move(buf)));
	}

	void Shutdown()
	{
		_loop->RunInLoop(std::bind(&Connection::ShutdownInLoop,this));
	}
	
	void Release()
	{
		_loop->QueueInLoop(std::bind(&Connection::ReleaseInLoop,this));
	}

	void EnableInactiveRelease(int sec)
	{
		_loop->RunInLoop(std::bind(&Connection::EnableInactiveReleaseInLoop,this,sec));
	}

	void CannlInactiveRelease()
	{
		_loop->RunInLoop(std::bind(&Connection::CancelInactiveReleaseInLoop,this));
	}

	void Upgrade(const Any &context, const ConnectedCallback &conn, const MessageCallback &msg, 
					const ClosedCallback &closed, const AnyEventCallback &event) 
	{
		_loop->AssertInLoop();
		_loop->RunInLoop(std::bind(&Connection::UpgradeInLoop, this, context, conn, msg, closed, event));
	}
};

class Acceptor
{
private:
	Socket _socket;		// 创建监听套接字
	EventLoop* _loop;	// 对监听套接字进行时间监控
	Channel _channel;	// 事件管理

	using AcceptCallback = std::function<void(int)>;
	AcceptCallback _accept_callback;
private:
	void HandleRead()
	{
		int newfd = _socket.Accept();
		if(newfd < 0) {return ;}

		if(_accept_callback) _accept_callback(newfd);
	}

	int CreateServer(int port)
	{
		bool ret = _socket.CreateServer(port);
		assert(ret == true);
		return _socket.Fd();
	}

public:
	Acceptor(EventLoop* loop,int port) :_socket(CreateServer(port)),_loop(loop),_channel(loop,_socket.Fd())
	{
		_channel.SetReadCallback(std::bind(&Acceptor::HandleRead,this));
	}

	void SetAcceptCallback(const AcceptCallback& cb) {_accept_callback = cb;}
	void Listen() {_channel.EnableRead();}
};

class LoopThread
{
private:
	std::mutex _mutex;
	std::condition_variable _cond;	
	EventLoop* _loop;
	std::thread _thread;
private:
	// 实例化 EventLoop对象.唤醒_cond上有可能阻塞的线程，并且开始运行EventLoop模块的功.
	void ThreadEntry()
	{
		EventLoop loop;
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_loop = &loop;
			_cond.notify_all();
		}
		// 所有唤醒的线程都start?
		loop.Start();
	}
public:
	LoopThread():_loop(nullptr),_thread(std::thread(&LoopThread::ThreadEntry,this)) {}
	EventLoop* GetLoop()
	{
		EventLoop* loop = nullptr;
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cond.wait(lock,[&](){return _loop != NULL;}); // ? TODO 
			// Bug bool IsInLoop() { return _thread_id == std::this_thread::get_id();} _thread_id 为0X0
			loop = _loop;
		}
		return loop;
	}
};

class LoopThreadPool
{
private:
	int _thread_count;
	int _next_idx;
	EventLoop* _baseloop;
	std::vector<LoopThread*> _threads;
	std::vector<EventLoop* > _loops;
public:
	LoopThreadPool(EventLoop* baseloop):_thread_count(0),_next_idx(0),_baseloop(baseloop){}
	void SetThreadCount(int count) {_thread_count = count;}

	void Create()
	{
		if(_thread_count > 0)
		{
			_threads.resize(_thread_count);
			_loops.resize(_thread_count);

			for(int i = 0;i < _thread_count;i++)
			{
				_threads[i] = new LoopThread();
				_loops[i] = _threads[i]->GetLoop();
			}
		}
	}

	EventLoop* NextLoop()
	{
		if(_thread_count == 0)
		{
			return _baseloop;
		}

		_next_idx = (_next_idx + 1) % _thread_count;
		return _loops[_next_idx];
	}
};

class TcpServer
{
private:
	uint64_t _next_id;		// 这是一个自动增长的连接ID
	int _port;	
	int _timeout;			// 任务多少秒结束
	bool _enable_inactive_realease;	// 是否启动了非活跃超时销毁标志
	EventLoop _baseloop;	// 主线程的EventLoop对象
	Acceptor _acceptor;		// 这是监听套接字管理对象
	LoopThreadPool _pool;	// 从属线程EventLoop线程池
	std::unordered_map<uint64_t,PtrConnection> _conns;	// 保存管理所有连接对应的shared_ptr对象。

	using ConnectedCallback = std::function<void(const PtrConnection&)>;
	using MessageCallback = std::function<void(const PtrConnection&,Buffer*)>;
	using ClosedCallback = std::function<void(const PtrConnection&)>;
	using AnyEventCallback = std::function<void(const PtrConnection&)>;

	using Functor = std::function<void()>;

	ConnectedCallback _connected_callback;
	MessageCallback _message_callback;
	ClosedCallback _closed_callback;
	AnyEventCallback _event_callback;
private:
	void RunAfterInLoop(const Functor & task,int delay)
	{
		_next_id++;
		_baseloop.TimerAdd(_next_id,delay,task);
	}	

	void NewConnection(int fd)
	{
		_next_id++;
		PtrConnection conn(new Connection(_pool.NextLoop(),_next_id,fd));
		conn->SetMessageCallback(_message_callback);
		conn->SetClosedCallback(_closed_callback);
		conn->SetConnectedCallback(_connected_callback);
		conn->SetAnyEventCallback(_event_callback);
		conn->SetSrvClosedCallback(std::bind(&TcpServer::RemoveConnection,this,std::placeholders::_1));

		if(_enable_inactive_realease) conn->EnableInactiveRelease(_timeout);
		conn->Established();
		_conns.insert(std::make_pair(_next_id,conn));
	}

	void RemoveConnnetionInLoop(const PtrConnection& conn)
	{
		int id = conn->Id();
		auto it = _conns.find(id);
		if(it != _conns.end())
		{
			_conns.erase(it);
		}
	}

	void RemoveConnection(const PtrConnection& conn)
	{
		_baseloop.RunInLoop(std::bind(&TcpServer::RemoveConnnetionInLoop,this,conn));
	}
public:
	TcpServer(int port):
		_port(port),
		_next_id(0),
		_enable_inactive_realease(false),
		_acceptor(&_baseloop,port),
		_pool(&_baseloop)
	{
		_acceptor.SetAcceptCallback(std::bind(&TcpServer::NewConnection,this,std::placeholders::_1));
		_acceptor.Listen();
	}
	void SetThreadCount(int count) { return _pool.SetThreadCount(count); }
	void SetConnectedCallback(const ConnectedCallback&cb) { _connected_callback = cb; }
	void SetMessageCallback(const MessageCallback&cb) { _message_callback = cb; }
	void SetClosedCallback(const ClosedCallback&cb) { _closed_callback = cb; }
	void SetAnyEventCallback(const AnyEventCallback&cb) { _event_callback = cb; }
	void EnableInactiveRelease(int timeout) { _timeout = timeout,_enable_inactive_realease = true; }
	//用于添加一个定时任务
	void RunAfter(const Functor &task, int delay) {
		_baseloop.RunInLoop(std::bind(&TcpServer::RunAfterInLoop, this, task, delay));
	}
	void Start() { _pool.Create();/*不在构造函数创建，thread_count = 0N*/  _baseloop.Start(); }
};

// void Channel::Remove()
// {
// 	_poller->RemoveEvent(this);
// }
// void Channel::Update(){_poller->UpdateEvent(this);}

void Channel::Remove()
{
	_loop->RemoveEvent(this);
}
void Channel::Update()
{
	_loop->UpdateEvent(this);
}

void TimerWheel::TimerAdd(uint64_t id,uint32_t delay,const TaskFunc& cb)
{
	_loop->RunInLoop(std::bind(&TimerWheel::TimerAddInLoop,this,id,delay,cb));	
}
void TimerWheel::TimerRefresh(uint64_t id)
{
	_loop->RunInLoop(std::bind(&TimerWheel::TimerRefreshInLoop,this,id));	
}
void TimerWheel::TimerCancel(uint64_t id)
{
	_loop->RunInLoop(std::bind(&TimerWheel::TimerCancelInLoop,this,id));	
}

class NetWork
{
public:
	NetWork()
	{
		DBG_LOG("SIGPIE INTI");
		// 断开连接继续Send发送数据，会导致客户端收到信号进程退出，这里忽略
		signal(SIGPIPE,SIG_IGN);
	}
};

static NetWork nw;