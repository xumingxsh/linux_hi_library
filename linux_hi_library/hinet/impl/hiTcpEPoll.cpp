#include "net/hiTcpEPoll.h"
#include "tcpEPollImpl.h"
#include "common/hiLog.h"

using namespace std;

namespace Hi
{
HiTcpEPoll::HiTcpEPoll(): impl_(new TcpEPollImpl())
{
	FUN_LOG();
}
HiTcpEPoll::~HiTcpEPoll()
{
	FUN_LOG();
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}
bool HiTcpEPoll::add(int sock, unsigned int events, 
	const std::function<void(int, unsigned int)>& on_evt)
{
	FUN_LOG();
	return impl_->add(sock, events, on_evt);
}

bool HiTcpEPoll::add(int sock, 
	const std::function<void(int, unsigned int)>& on_evt)
{
	FUN_LOG();
	return impl_->add(sock, on_evt);
}

bool HiTcpEPoll::add(int sock, unsigned int events)
{
	FUN_LOG();
	return impl_->add(sock, events);
}

bool HiTcpEPoll::add(int sock)
{
	FUN_LOG();
	return impl_->add(sock);
}

unsigned short HiTcpEPoll::run_delay(
	unsigned int delay_tm,
	const std::function<void(unsigned short)>& fun)
{
	FUN_LOG();
	return impl_->run_delay(delay_tm, fun);
}

bool HiTcpEPoll::remove_timer(unsigned short id)
{
	FUN_LOG();
	return impl_->remove_timer(id);
}

bool HiTcpEPoll::remove(int sock)
{
	FUN_LOG();
	return impl_->remove(sock);
}

bool HiTcpEPoll::start()
{
	FUN_LOG();
	return impl_->start();
}

bool HiTcpEPoll::start(
	const std::function<void(int, unsigned int)>& fun)
{
	FUN_LOG();
	return impl_->start(fun, 100);
}

bool HiTcpEPoll::start(
	const std::function<void(int, unsigned int)>& on_evt, 
	const std::function<void()>& on_close)
{
	FUN_LOG();
	return impl_->start(on_evt, on_close, 100);
}

bool HiTcpEPoll::finish()
{
	FUN_LOG();
	return impl_->finish();
}

bool HiTcpEPoll::is_running()
{
	FUN_LOG();
	return impl_->is_running();
}
}