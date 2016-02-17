#pragma once

#include <functional>

#include "net/hiNetCallBack.h"

namespace Hi
{
class HiTcpEPoll;
class HiTcpConnect;
class TcpClientImpl
{
public:
	TcpClientImpl();
	TcpClientImpl(HiTcpEPoll& poll);
	~TcpClientImpl();
public:
	bool connect(const char* ip, 
		unsigned short port, 
		const TcpClientEvt& evt,
		unsigned short time_out_second, 
		void* user_data = NULL);
		
	bool close();
private:
	void on_poll_event(int, unsigned int);
private:
	HiTcpEPoll* poll_;
	HiTcpConnect* connect_;
	bool poll_is_mine_;
	TcpClientEvt evt_;
};
}