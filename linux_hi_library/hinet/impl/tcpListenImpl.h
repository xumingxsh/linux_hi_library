#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <functional>

#include "pthread.h"

namespace Hi
{
class TcpListenImpl
{
public:
	TcpListenImpl();
	~TcpListenImpl();
public:
	bool open(
		const std::function<void(int)>& fun, 
		unsigned short port, 
		const char* ip);
		
	bool set_close_fun(const std::function<void()>& close_fun);
	bool close();
	void run();
	bool is_running();
private:
	int sock_;
	std::string ip_;
	unsigned short port_;
	bool is_opened_;
	
	pthread_t thread_;
	std::function<void(int)> on_accept_;
	std::function<void()> on_close_;
};
}