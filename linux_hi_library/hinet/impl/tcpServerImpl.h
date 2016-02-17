#pragma once

#include <map>

#include <net/hiTcpListen.h>
#include <net/hiTcpEPoll.h>
#include "net/hiNetCallBack.h"

namespace Hi
{
class TcpEvent;

class TcpServerImpl
{
public:
	TcpServerImpl();
	~TcpServerImpl();
public:
	bool open(const TcpEvent& evt, 
		unsigned short port, 
		const char* ip);
		
	bool close();
	bool is_running();
private:
	void on_accept(int sock);
		
	void on_listen_close();
	
	void on_poll_event(int sock, unsigned int events);
private:
	HiTcpListen listen_;
	HiTcpEPoll	poll_;
	
	std::map<int, int>	socks_;
	TcpEvent* evt_;
};
}