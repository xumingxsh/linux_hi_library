#include "net/hiTcpConnect.h"
#include "tcpConnectImpl.h"
#include "common/hiLog.h"

using namespace std;

namespace Hi
{
HiTcpConnect::HiTcpConnect(HiTcpEPoll& poll): 
	impl_(new TcpConnectImpl(poll))
{
	FUN_LOG();
}

HiTcpConnect::~HiTcpConnect()
{
	FUN_LOG();
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}

bool HiTcpConnect::connect(const char* ip, 
	unsigned short port,
	const  std::function<void(bool,int,void*)>& fun,
	const std::function<void(int,unsigned int)>& on_evt,
	unsigned short time_out, 
		void* user_data)
{
	FUN_LOG();
	return impl_->connect(ip, port, fun, on_evt, time_out, user_data);
}

bool HiTcpConnect::close()
{
	FUN_LOG();
	return impl_->close();
}
}