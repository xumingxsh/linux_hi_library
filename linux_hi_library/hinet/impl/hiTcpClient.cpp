#include "net/hiTcpClient.h"
#include "tcpClientImpl.h"
#include "common/hiLog.h"

namespace Hi
{
HiTcpClient::HiTcpClient(HiTcpEPoll& poll):
	impl_(new TcpClientImpl(poll))
{
	FUN_LOG();
}
HiTcpClient::HiTcpClient(): 
	impl_(new TcpClientImpl())
{
	FUN_LOG();
}
HiTcpClient::~HiTcpClient()
{
	FUN_LOG();
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}

bool HiTcpClient::connect(const char* ip, 
	unsigned short port, 
	const TcpClientEvt& evt,
	unsigned short time_out_second, 
		void* user_data)
{
	FUN_LOG();
	return impl_->connect(ip, port, evt, time_out_second, user_data);
}
	
bool HiTcpClient::close()
{
	FUN_LOG();
	return impl_->close();
}
}