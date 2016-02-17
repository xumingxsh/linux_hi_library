#include "net/hiTcpServer.h"
#include "tcpServerImpl.h"
#include "common/hiLog.h"

using namespace std;
using namespace std::placeholders;

namespace Hi
{
static void on_open_fun(int){}
static void on_close_fun(int){}
static void on_receive_fun(int){}
TcpEvent::TcpEvent()
{
	FUN_LOG();
	on_open_ = std::bind(on_open_fun, _1);
	on_close_ = std::bind(on_close_fun, _1);
	on_receive_ = std::bind(on_receive_fun, _1);
}

HiTcpServer::HiTcpServer(): impl_(new TcpServerImpl())
{
	FUN_LOG();
}

HiTcpServer::~HiTcpServer()
{
	FUN_LOG();
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}

bool HiTcpServer::open(const TcpEvent& evt, 
	unsigned short port, 
	const char* ip)
{
	FUN_LOG();
	return impl_->open(evt, port, ip);
}

bool HiTcpServer::close()
{
	FUN_LOG();
	return impl_->close();
}

bool HiTcpServer::is_running()
{
	FUN_LOG();
	return impl_->is_running();
}
}