#include "tcpServerImpl.h"
#include <unistd.h>
#include <sys/epoll.h>

#include "common/hiLog.h"
#include "net/hiTcpServer.h"

using namespace std;
using namespace std::placeholders;

namespace Hi
{
TcpServerImpl::TcpServerImpl(): evt_(new TcpEvent())
{
	FUN_LOG();
}
	
TcpServerImpl::~TcpServerImpl()
{
	FUN_LOG();
	listen_.close();
	poll_.finish();
	if (evt_)
	{
		delete evt_;
		evt_ = NULL;
	}
}

static bool start_epoll(HiTcpEPoll& poll, 
	const std::function<void(int, unsigned int)>& fun);

bool TcpServerImpl::open(const TcpEvent& evt, 
	unsigned short port, const char* ip)
{
	FUN_LOG();
	auto poll_event = std::bind(
		&Hi::TcpServerImpl::on_poll_event, this, _1, _2);
		
	if (!start_epoll(poll_, poll_event))
	{
		HI_ERROR(("start epoll failed"));
		return false;
	}
	int index = 0;
	int result = false;	
	*evt_ = evt;
	
	auto on_open = std::bind(&TcpServerImpl::on_accept, this, _1);
	auto on_close = std::bind(&TcpServerImpl::on_listen_close, this);
	
	while (!result && index < 10)
	{
		listen_.set_close_fun(on_close);
		result = listen_.open(on_open, port, ip);
		if (!result)
		{
			sleep(5);
			index++;
			if (index < 10)
			{				
				hi_info(
					"listen (%d) failed,,will open again for later", 
					port);		
			}
		}
	}
	return result;
}

bool TcpServerImpl::close()
{
	FUN_LOG();
	listen_.close();
	poll_.finish();
	return true;
}

bool TcpServerImpl::is_running()
{
	FUN_LOG();
	return listen_.is_running() && poll_.is_running();
}

void TcpServerImpl::on_accept(int sock)
{
	FUN_LOG();
	auto it = socks_.find(sock);
	if (it == socks_.end())
	{
		socks_[sock] = sock;
	}
	evt_->on_open_(sock);
	poll_.add(sock);
}
	
void TcpServerImpl::on_listen_close()
{
	FUN_LOG();
	for (auto it = socks_.begin(); it != socks_.end(); ++it)
	{
		evt_->on_close_(it->first);
		::close(it->first);
	}
	sleep(10);
	socks_.clear();
}

void TcpServerImpl::on_poll_event(int fd, unsigned int evt)
{	
	FUN_LOG();
	if ((evt & EPOLLRDHUP) || (evt & EPOLLHUP))
	{
		poll_.remove(fd);
		
		auto it = socks_.find(fd);
		if (it != socks_.end())
		{
			evt_->on_close_(fd);
			::close(fd);
			socks_.erase(it);			
		}
		return;
	}
	if ((evt & EPOLLIN) || (evt & EPOLLPRI))
	{
		evt_->on_receive_(fd);
	}
}

bool start_epoll(HiTcpEPoll& poll, 
	const std::function<void(int, unsigned int)>& fun)
{
	FUN_LOG();
	int index = 0;
	bool result = false;
	while (!result && index < 10)
	{
		result = poll.start(fun, [&](){
			hi_info("epoll is closed");
		});
		if (!result)
		{
			sleep(1);
			index++;
			if (index < 10)
			{				
				hi_info("start epoll failed,will start again for later");
			}
		}
	}
	return result;
}
}