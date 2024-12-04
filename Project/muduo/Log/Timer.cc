#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <unistd.h>

using OnTimerCallback = std::function<void()>;
using ReleaseCallback = std::function<void()>;

class Timer
{
private:
	int _timeout;		// 当然定时器任务延迟时间
	bool _canceled;		// false : 正常执行 ; true 任务被取消
	uint64_t _timer_id; // 定时器ID

	OnTimerCallback _timer_callback;
	ReleaseCallback _release_callback;
public:
	Timer(uint64_t timer_id,int _timeout):_timeout(_timeout),_timer_id(timer_id),_canceled(false){}

	~Timer()
	{
		if(_timer_callback) _timer_callback();
		if(_release_callback && !_canceled) _release_callback();
	}

	uint64_t Id();
	int DelayTime(){return _timeout;}
	void Canceled(){_canceled = true;}
	void SetOnTimeCallback(const OnTimerCallback& cb) {_timer_callback = cb;}
	void SetReleaseCallback(const OnTimerCallback& cb) {_release_callback = cb;}
};

#define MAX_TIMEOUT 60
class TimerQueue
{
private:
	using WeakTimer = std::weak_ptr<Timer>;
	using PtrTimer = std::shared_ptr<Timer>;
	using Bucket = std::vector<PtrTimer>;
	using BucketList = std::vector<Bucket>;

	int _tick;
	int _capacity;
	BucketList _conns;
	std::unordered_map<uint64_t,WeakTimer> _timers;
public:	
	TimerQueue():_tick(0),_capacity(MAX_TIMEOUT),_conns(_capacity){}

	bool HasTimer(uint64_t id)
	{
		auto it = _timers.find(id);
		if(it != _timers.end()) return true;
		return false;
	}

	void RemoveWeaktimerFromTimerqueue(uint64_t id)
	{
		auto it = _timers.find(id);
		if(it != _timers.end())
		{
			_timers.erase(it);
		}
	}

	void RunOntimeTask()
	{
		_tick = (_tick + 1) % _capacity;
		_conns[_tick].clear();
	}

	void TimerAdd(const OnTimerCallback& cb,int delay,uint64_t id)
	{
		if(delay > _capacity || delay <= 0) return;

		PtrTimer timer(new Timer(id,delay));
		timer->SetOnTimeCallback(cb);
		timer->SetReleaseCallback(std::bind(&TimerQueue::RemoveWeaktimerFromTimerqueue,this,id));

		_timers[id] = WeakTimer(timer);
		_conns[(_tick + delay) % _capacity].push_back(timer);
	} 

	void TimerRefresh(uint64_t id)
	{
		auto it = _timers.find(id);
		assert(it != _timers.end());

		int delay = it->second.lock()->DelayTime();
		_conns[(_tick + delay) % _capacity].push_back(PtrTimer(it->second));
	}
};

class TimerTest
{
private:
	int _data;
public:
	TimerTest(int data):_data(data) {std::cout << "test create" << std::endl;}
	~TimerTest() {std::cout << "test delete\n";}
};

void Del(TimerTest *t)
{
	delete t;
}

int main()
{
	TimerQueue tq;
	TimerTest *t = new TimerTest(10);

	int id = 3;
	tq.TimerAdd(std::bind(Del,t),5,id);

	for(int i= 0;i < 5;i++)
	{
		sleep(1);
		tq.TimerRefresh(id);
		std::cout << "刷新任务!" << std::endl;
	}

	while (1) 
	{	
		sleep(1);
		tq.RunOntimeTask();
		if(tq.HasTimer(id)== false)
		{
			std::cout << "任务完毕" <<std::endl;
			break;
		}
	}

	return 0;
}