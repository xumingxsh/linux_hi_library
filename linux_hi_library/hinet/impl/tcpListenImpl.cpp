#include "tcpListenImpl.h"

#include <unistd.h>
#include <string.h>

#include "common/hiString.h"
#include "common/hiLog.h"
#include "netThreadCommon.h"

using namespace std;

namespace Hi
{
static void listen_close_fun(){}
static std::function<void()> def_close_fun = std::bind(listen_close_fun);
TcpListenImpl::TcpListenImpl(): 
	sock_(-1), is_opened_(false)
{
	FUN_LOG();
	on_close_ = def_close_fun;
}

TcpListenImpl::~TcpListenImpl()
{
	FUN_LOG();
	this->close();
}

static void* on_thread(void*);

bool TcpListenImpl::set_close_fun(const std::function<void()>& close_fun)
{
	FUN_LOG();
	if (is_opened_)
	{
		HI_ERROR(("listen port is using,so can't set close function"));
		return false;
	}
	on_close_ = close_fun;
	return true;
}

bool TcpListenImpl::open( 
	const std::function<void(int)>& fun, 
	unsigned short port,
	const char* ip)
{
	FUN_LOG();
	if (is_opened_)
	{
		HI_ERROR(("listen port is using"));
		return false;
	}
		
	is_opened_ = true;
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (ip)
	{
		addr.sin_addr.s_addr = inet_addr(ip);
	}
	else
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	bzero(&addr.sin_zero, 8);
	
	sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_ < 0)
	{
		is_opened_ = false;
		HI_ERROR(("create listen socket failed,error:%d(%s)",
			errno, strerror(errno)));
			
		return false;
	}
	on_accept_ = fun;
	
	int ret = bind(sock_, 
		Hi::Net::convert_sock_in(addr), 
		sizeof(sockaddr));
		
	if (ret < 0)
	{
		::close(sock_);
		sock_ = -1;
		is_opened_ = false;
		
		HI_ERROR(("socket bind failed,error:%d(%s)",
			errno, strerror(errno)));
			
		return false;
	}
	ret = listen(sock_, 10);
	if (ret < 0)
	{
		::close(sock_);
		sock_ = -1;
		is_opened_ = false;
		
		HI_ERROR(("socket listen failed,error:%d(%s)",
			errno, strerror(errno)));
			
		return false;
	}
	pthread_create(&thread_, NULL, on_thread, this);
	hi_info("create and listen socket success");
	return true;
}

bool TcpListenImpl::close()
{	
	FUN_LOG();
	if (sock_ >= 0)
	{
		hi_info("will close listen socket");
		::shutdown(sock_, SHUT_RDWR);			
		
		pthread_join(thread_, NULL);
		
		if (sock_ >= 0)
		{
			::close(sock_);
			sock_ = -1;
		}
		if (is_opened_)
		{
			is_opened_ = false;
		}
	}
	hi_info("listen socket is closed");
	return true;
}

bool TcpListenImpl::is_running()
{
	FUN_LOG();
	return sock_ >= 0;
}

void TcpListenImpl::run()
{	
	FUN_LOG();
	hi_info("listen thread start");
	sockaddr_in remote;	
	sockaddr_in local;
	while (true)
	{
		bzero(&remote, sizeof remote);
		bzero(&local, sizeof local);
		
		socklen_t len = sizeof(struct sockaddr);
		int remote_handle = accept(sock_, 
			static_cast<sockaddr*>(implicit_cast<void*>(&remote)), 
			&len);
			
		if (remote_handle < 0)
		{			
			HI_ERROR(("accept failed,error:%d(%s)",
				errno, strerror(errno)));
				
			break;
		}		
		
		on_accept_(remote_handle);
	}
	on_close_();
	if (sock_ >= 0)
	{
		::close(sock_);
		sock_ = -1;
	}
	is_opened_ = false;
	hi_info("listen thread finished");
}

void* on_thread(void* arg)
{
	FUN_LOG();
	TcpListenImpl* impl = static_cast<TcpListenImpl*>(arg);
	impl->run();
	return 0;
}
}