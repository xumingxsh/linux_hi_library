#include "tcpEPollImpl.h"
#include <unistd.h>
#include <string.h>
#include <vector>

#include <sys/epoll.h>
#include "common/hiLog.h"
#include "netThreadCommon.h"

using namespace std;
using namespace std::placeholders;
namespace Hi
{
static void on_evt_fun(int, unsigned int){}
static void on_close_fun(){}
static unsigned int default_sign = 
	EPOLLIN | EPOLLPRI | EPOLLRDHUP | EPOLLERR | EPOLLHUP | EPOLLOUT | EPOLLET;
	
TcpEPollImpl::TcpEPollImpl(): 
	poll_(-1), 
	timeout_(100), 
	is_exit_(false), 
	timer_index_(TimerIndex(1, 65530))
{
	FUN_LOG();
	on_event_ = std::bind(on_evt_fun, _1, _2);
	on_close_ = std::bind(on_close_fun);
}
TcpEPollImpl::~TcpEPollImpl()
{
	FUN_LOG();
	if (poll_ != -1)
	{
		::close(poll_);
		poll_ = -1;
	}
}
	
bool TcpEPollImpl::add(int sock)
{
	FUN_LOG();
	return add(sock, default_sign);
}

bool TcpEPollImpl::add(int sock, unsigned int events, 
	const std::function<void(int, unsigned int)>& on_evt)
{
	FUN_LOG();
	funs_.insert(sock, on_evt);
	if (!add(sock, events))
	{
		return false;
	}
	return true;
}

bool TcpEPollImpl::add(int sock, 
	const std::function<void(int, unsigned int)>& on_evt)
{
	FUN_LOG();
	return add(sock, default_sign, on_evt);
}

bool TcpEPollImpl::add(int sock, unsigned int events)
{
	FUN_LOG();
	if (poll_ == -1)
	{
		HI_ERROR(("epoll is not start,so can't add IO object"));
		
		funs_.erase(sock);
		return false;
	}
	struct epoll_event event;
	memset(&event, 0, sizeof(struct epoll_event));
	event.events = events;
	event.data.fd = sock;
	if (::epoll_ctl(poll_, EPOLL_CTL_ADD, sock, &event) < 0)
	{	
		HI_ERROR(("epoll_ctl failed"));
			
		funs_.erase(sock);
		return false;
	}
	return true;
}

unsigned short TcpEPollImpl::run_delay(unsigned int delay_tm,
	const std::function<void(unsigned short)>& fun)
{
	FUN_LOG();
	unsigned short time_id = timer_index_.add(1);
	DelayInfo info;
	info.fun_ = fun;
	info.type_ = DelayInfo::TT_Delay;
	info.set_delay_time(delay_tm);
	timer_funs_.insert(time_id, info);
	return time_id;
}

bool TcpEPollImpl::remove_timer(unsigned short id)
{
	FUN_LOG();
	return timer_funs_.erase(id);
}

bool TcpEPollImpl::remove(int sock)
{
	FUN_LOG();
	if (poll_ == -1)
	{	
		HI_ERROR(("epoll is not start,so can't remove IO object"));
		return false;
	}
	struct epoll_event event;
	memset(&event, 0, sizeof(struct epoll_event));
	event.data.fd = sock;
	if (::epoll_ctl(poll_, EPOLL_CTL_DEL, sock, &event) < 0)
	{
		HI_ERROR(("epoll_ctl failed"));
		return false;
	}
	funs_.erase(sock);
	return true;
}

bool TcpEPollImpl::start(int timeout)
{	
	FUN_LOG();
	if (poll_ != -1)
	{
		HI_ERROR(("epoll is runing,so can't start"));
		return false;
	}
	
	return start(std::bind(on_evt_fun, _1, _2), 
		std::bind(on_close_fun), timeout);
}

bool TcpEPollImpl::start(
	const std::function<void(int, unsigned int)>& fun,
	int timeout)
{	
	FUN_LOG();
	return start(fun, std::bind(on_close_fun), timeout);
}

static void* on_thread(void*);

bool TcpEPollImpl::start(
	const std::function<void(int, unsigned int)>& on_evt, 
	const std::function<void()>& on_close,int timeout)
{
	FUN_LOG();
	if (poll_ != -1)
	{
		HI_ERROR(("epoll is runing,so can't start"));
		return false;
	}
	is_exit_ = false;
	on_event_ = on_evt;
	on_close_ = on_close;
	
	poll_ = epoll_create1(EPOLL_CLOEXEC);
	timeout_ = timeout;
	pthread_create(&thread_, NULL, on_thread, this);
	hi_info("epoll is started");
	return true;
}

void TcpEPollImpl::run()
{
	FUN_LOG();
	hi_info("epoll thread is start");
	vector<struct epoll_event> evts(200);
	while (!is_exit_)
	{
		check_timer();
		int ready = 
			::epoll_wait(poll_, &*evts.begin(),
			   static_cast<int>(evts.size()), timeout_);
			  
		if (ready == 0)
		{	
			continue;
		}
		if (ready < 0)
		{
			if (errno == EINTR || errno == EAGAIN)
			{
				continue;
			}
			else
			{
				HI_ERROR(("epoll_wait failed,error:%d(%s)",
					errno, strerror(errno)));
					
				break;
			}
		}
		
		for (int i = 0; i < ready; i++)
		{
			struct epoll_event& evt = evts[i];
			std::function<void(int, unsigned int)> fun;
			if (funs_.find(evt.data.fd, fun))
			{
				fun(evt.data.fd, evt.events);
			}
			else
			{
				on_event_(evt.data.fd, evt.events);
			}
		}
		
		if (ready == (int)evts.size())
		{
			evts.resize(evts.size()*2);
		}
	}
	
	::close(poll_);
	on_close_();
	funs_.clear();
	timer_funs_.clear();
	poll_ = -1;
	hi_info("epoll thread is finish");
}

bool TcpEPollImpl::finish()
{
	FUN_LOG();
	hi_info("epoll will finish");
	if (poll_ == -1)
	{
		HI_ERROR(("epoll is not running,so can't finish"));
		return false;
	}
	is_exit_ = true;
	
	pthread_join(thread_, NULL);
	
	hi_info("epoll is finished");
	return true;
}

bool TcpEPollImpl::is_running()
{
	FUN_LOG();
	return poll_ != -1;
}

void TcpEPollImpl::check_timer()
{
	FUN_LOG();
	timer_funs_.functionW([&](TimersFuns::Map& myMap){
		auto it = myMap.begin();
		while (it != myMap.end())
		{
			it->second.run();
			if (it->second.is_can_remove())
			{
				myMap.erase(it);
				it = myMap.begin();
				continue;
			}
			++it;
		}
	});
}

void* on_thread(void* arg)
{
	FUN_LOG();
	TcpEPollImpl* impl = static_cast<TcpEPollImpl*>(arg);
	impl->run();
	return 0;
}

TcpEPollImpl::DelayInfo::DelayInfo():
	type_(TcpEPollImpl::DelayInfo::TT_Delay), 
	timer_id_(0)
{
	FUN_LOG();
}

void TcpEPollImpl::DelayInfo::run()
{		
	FUN_LOG();
	if (is_can_run())
	{
		fun_(timer_id_);
	}
}

bool TcpEPollImpl::DelayInfo::is_can_remove()
{	
	FUN_LOG();	
	if (type_ == TT_Delay && run_time_ < Hi::Net::get_cur_time())
	{
		return true;
	}
	return false;
}

void TcpEPollImpl::DelayInfo::set_delay_time(unsigned int delay)
{
	FUN_LOG();
	run_time_ = Hi::Net::get_cur_time() + delay;
}

bool TcpEPollImpl::DelayInfo::is_can_run()
{
	FUN_LOG();
	unsigned long long now = Hi::Net::get_cur_time();
	if (type_ == TT_Delay && run_time_ < now)
	{
		return true;
	}
	return false;
}
}