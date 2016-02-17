#include "tcpConnectImpl.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/epoll.h>

#include "common/hiString.h"
#include "net/hiTcpEPoll.h"
#include "common/hiLog.h"

#include "netThreadCommon.h"

using namespace std;
using namespace std::placeholders;

namespace Hi
{

TcpConnectImpl::TcpConnectImpl(HiTcpEPoll& poll): 
	sock_(-1), 
	poll_(poll),
	state_(CS_NotConnect),
	delay_event_id_(0),
	user_data_(NULL)
{
	FUN_LOG();
}
TcpConnectImpl::~TcpConnectImpl()
{
	FUN_LOG();
	this->close();	
}

bool TcpConnectImpl::connect(const char* ip, 
	unsigned short port,
	const std::function<void(bool,int,void*)>& fun,
	const std::function<void(int,unsigned int)>& on_evt,
	unsigned short time_out, 
	void* user_data)
{
	FUN_LOG();
	if (sock_ != -1)
	{	
		HI_ERROR(("can't connect to server,because connect socket is using"));
		
		return false;
	}	
	if (!poll_.is_running())
	{
		HI_ERROR(("can't connect to server,because epoll is not run"));
		
		return false;
	}
	state_ = CS_NotConnect;
	sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_ < 0)
	{		
		HI_ERROR(("create connect socket failed,error:%d(%s)",
			errno, strerror(errno)));
			
		return false;
	}
	fun_ = fun;
	on_evt_ = on_evt;
	user_data_ = user_data;
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	bzero(&addr.sin_zero, 8);	
	
	int flags =  fcntl(sock_, F_GETFL, 0);
	if (flags < 0)
	{
		::close(sock_);
		sock_ = -1;
		
		HI_ERROR(("fcntl failed,error:%d(%s)",
			errno, strerror(errno)));
			
		return false;
	}
	fcntl(sock_, F_SETFL, flags | O_NONBLOCK);
	errno = 0;
	auto evt = std::bind(&TcpConnectImpl::on_open, this, _1, _2);
	if (!poll_.add(sock_, evt))
	{
		::close(sock_);
		sock_ = -1;
		
		HI_ERROR(("add socket to poll failed"));
		return false;
	}
	if (::connect(sock_, 
		Hi::Net::convert_sock_in(addr), 
		sizeof(addr)) < 0) 
	{      
		state_ = CS_Connecting;    
		int err = errno;
		if (err != EINPROGRESS)
		{
			HI_ERROR(("connect failed,error:%d(%s)",
				errno, strerror(errno)));
				
			this->close();
			return false;
		}
		
		hi_info("connect to (%s)(%d),sock(%d),waiting",
			ip, port, sock_);
		
		auto tm_evt = std::bind(&TcpConnectImpl::on_time_out, this, _1);
		delay_event_id_ = poll_.run_delay(time_out * 1000* 1000, tm_evt);
		
		remote_ip_ = ip;
		remote_port_ = port;
		return true;
	}
	
	state_ = CS_Connected;
	fun_(true, sock_, user_data_);
	return true;
}

void TcpConnectImpl::on_time_out(unsigned short id)
{
	FUN_LOG();
	
	hi_info("connect to (%s)(%d) time out",
		remote_ip_.c_str(), remote_port_);
		
	if (state_ == CS_Connecting)
	{
		delay_event_id_ = 0;
		this->close();
		fun_(false, -1, user_data_);
	}
}

void TcpConnectImpl::on_open(int sock, unsigned int sign)
{
	FUN_LOG();
	if (sign & EPOLLERR)
	{
		this->close();
		fun_(false, -1, user_data_);
		return;
	}
	
	if (sock != sock_)
	{	
		hi_info("get a data,but not this socket");
		
		return;
	}
	
	if (state_ == CS_Connecting)
	{
		int status = 0;
		socklen_t slen = 0;
		if (getsockopt(sock, SOL_SOCKET, 
			SO_ERROR, (void *)&status, &slen)
			< 0)
		{
			HI_ERROR(("connect failed,reason:getsockopt error"));
			
			this->close();
			fun_(false, -1, user_data_);
			return;
		}
		if (status != 0) 
		{
			HI_ERROR(("connect failed,reason:getsockopt error"));
			
			this->close();
			
			fun_(false, -1, user_data_);
			return;
		}
		
		state_ = CS_Connected;
		if (delay_event_id_ != 0)
		{
			poll_.remove_timer(delay_event_id_);
			delay_event_id_ = 0;
		}
		fun_(true, sock_, user_data_);
		return;
	}
	if (state_ == CS_NotConnect)
	{
		hi_info("get a data for socket(%d),but not connected", sock);
	}
	if (state_ == CS_Connected)
	{
		on_evt_(sock, sign);
	}
}

bool TcpConnectImpl::close()	
{
	FUN_LOG();
	if (sock_ != -1)
	{		
		hi_info("will close connect socket(%d)", sock_);
		poll_.remove(sock_);
		if (delay_event_id_ != 0)
		{
			poll_.remove_timer(delay_event_id_);
			delay_event_id_ = 0;
		}
		::close(sock_);
		state_ = CS_NotConnect;
		hi_info("connect socket(%d) is closed", sock_);
		sock_ = -1;
	}
	return true;
}
}