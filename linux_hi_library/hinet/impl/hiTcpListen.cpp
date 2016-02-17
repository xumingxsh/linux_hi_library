#include "net/hiTcpListen.h"
#include "tcpListenImpl.h"
#include "common/hiLog.h"

namespace Hi
{
HiTcpListen::HiTcpListen(): impl_(new TcpListenImpl())
{
	FUN_LOG();
}

HiTcpListen::~HiTcpListen()
{
	FUN_LOG();
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}

bool HiTcpListen::open(
	const std::function<void(int)>& fun, 
	unsigned short port, 
	const char* ip)
{
	FUN_LOG();
	return impl_->open(fun, port, ip);
}

bool HiTcpListen::set_close_fun(const std::function<void()>& close_fun)
{
	FUN_LOG();
	return impl_->set_close_fun(close_fun);
}

bool HiTcpListen::close()
{
	FUN_LOG();
	return impl_->close();
}

bool HiTcpListen::is_running()
{
	FUN_LOG();
	return impl_->is_running();
}
}