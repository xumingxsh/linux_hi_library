#include "tcpClientImpl.h"
#include <sys/epoll.h>
#include "net/hiTcpEPoll.h"
#include "net/hiTcpConnect.h"
#include "common/hiLog.h"

using namespace std;
using namespace std::placeholders;
namespace Hi
{
TcpClientImpl::TcpClientImpl(): poll_(new HiTcpEPoll()), 
	connect_(new HiTcpConnect(*poll_)),
	poll_is_mine_(true)
{		
	FUN_LOG();
	poll_->start();
}

TcpClientImpl::TcpClientImpl(HiTcpEPoll& poll): 
	poll_(&poll), 
	connect_(new HiTcpConnect(*poll_)),
	poll_is_mine_(false)
{
	FUN_LOG();
}

TcpClientImpl::~TcpClientImpl()
{
	FUN_LOG();
	if (connect_)
	{
		connect_->close();
		delete connect_;
		connect_ = NULL;
	}
	if (poll_is_mine_ && poll_)
	{
		poll_->finish();
		delete poll_;
		poll_ = NULL;
	}
}

bool TcpClientImpl::connect(const char* ip, 
	unsigned short port, 
	const TcpClientEvt& evt,
	unsigned short time_out_second, 
	void* user_data)
{
	FUN_LOG();
	if (!poll_ || !connect_)
	{
		HI_ERROR(("epoll or connect socket not start,so can't connect"));
		return false;
	}
	auto on_event = std::bind(
		&TcpClientImpl::on_poll_event,
		this, _1, _2);
	evt_= evt;
	return connect_->connect(ip, port, 
		evt.on_open_, on_event, 
		time_out_second, user_data);
}
		
bool TcpClientImpl::close()
{
	FUN_LOG();
	if (!poll_ || !connect_)
	{
		HI_ERROR(("epoll or connect socket not start,so can't close client"));
		return false;
	}
	return connect_->close();
}

void TcpClientImpl::on_poll_event(int fd, unsigned int evt)
{
	FUN_LOG();
	if ((evt & EPOLLRDHUP) || (evt & EPOLLHUP))
	{
		hi_info("in TcpClientImpl:sock will close");
		
		if (poll_)
		{
			poll_->remove(fd);
		}
		
		evt_.on_close_(fd);
		if (connect_)
		{
			connect_->close();
		}
		return;
	}
	if ((evt & EPOLLIN) || (evt & EPOLLPRI))
	{
		hi_info("receive data in TcpClientImpl");
		evt_.on_receive_(fd);
	}
}

static void on_open_fun(bool,int,void*){}
static void on_receive_fun(int){}
static void on_close_fun(int){}

TcpClientEvt::TcpClientEvt()
{
	FUN_LOG();
	on_open_ = std::bind(&on_open_fun, _1, _2, _3);
	on_receive_ = std::bind(&on_receive_fun, _1);
	on_close_ = std::bind(&on_close_fun, _1);
}
TcpClientEvt::TcpClientEvt(const OpenFun& on_open,
	const std::function<void(int)>& on_receive,
	const std::function<void(int)>& on_close):
	on_open_(on_open),
	on_receive_(on_receive),
	on_close_(on_close)
{
	FUN_LOG();
}
}