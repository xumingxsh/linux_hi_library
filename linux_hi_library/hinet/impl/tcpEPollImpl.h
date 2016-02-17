#pragma once

#include <sys/time.h>

#include <functional>
#include <map>

#include "pthread.h"

#include "collection/hiMap.h"
#include "mutex/hiAtomic.h"

namespace Hi
{
class FdsArr;
class TcpEPollImpl
{
public:
	TcpEPollImpl();
	~TcpEPollImpl();
public:
	bool add(int sock, unsigned int events, const std::function<void(int, unsigned int)>& on_evt);
	bool add(int sock, const std::function<void(int, unsigned int)>& on_evt);
	bool add(int sock, unsigned int events);
	bool add(int sock);
	bool remove(int sock);
	bool start(int timeout = 100);
	bool start(const std::function<void(int, unsigned int)>& fun, int timeout = 100);
	bool start(const std::function<void(int, unsigned int)>& on_evt, const std::function<void()>& on_close,int timeout = 100);
	bool finish();
	bool is_running();
	unsigned short run_delay(unsigned int delay_tm,const std::function<void(unsigned short)>& fun);
	bool remove_timer(unsigned short id);
public:	
	void run();
private:
	void check_timer();
private:
	pthread_t thread_;
	int poll_;
	int timeout_;
	bool is_exit_;
	std::function<void(int, unsigned int)> on_event_;
	std::function<void()> on_close_;
	HiMap<int, std::function<void(int, unsigned int)>> funs_;
	class DelayInfo
	{
	public:
		enum T_Type
		{
			TT_Delay,
			TT_Timer
		};
	public:
		DelayInfo();
		void run();
		bool is_can_remove();
		void set_delay_time(unsigned int delay);
		bool is_running();
	private:
		bool is_can_run();		
	public:
		std::function<void(unsigned short)> fun_;
		T_Type type_;
		unsigned short timer_id_;
		unsigned long long run_time_;
	};
	typedef HiMap<int, DelayInfo> TimersFuns;
	TimersFuns timer_funs_;
	typedef HiAtomic<unsigned short> TimerIndex;
	TimerIndex timer_index_;
};
}