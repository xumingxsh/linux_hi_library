#pragma once

#include <functional>

#include "net/hiNetCallBack.h"
#include "net/hiTcpConnect.h"

namespace Hi
{
class TcpConnectImpl
{
private:
	enum CS_State
	{
		CS_NotConnect,
		CS_Connecting,
		CS_Connected
	};
public:
	TcpConnectImpl(HiTcpEPoll& poll);
	~TcpConnectImpl();
public:
	bool connect(const char* ip, 
		unsigned short port, 
		const  std::function<void(bool,int,void*)>& fun,
		const std::function<void(int,unsigned int)>& on_evt,
		unsigned short time_out, 
		void* user_data = NULL);
		
	bool close();
private:
	void on_open(int sock, unsigned int sign);
	void on_time_out(unsigned short);
private:
	 std::function<void(bool,int,void*)> fun_;
	 std::function<void(int,unsigned int)> on_evt_;
	int sock_;
	HiTcpEPoll& poll_;
	CS_State state_;
	unsigned short delay_event_id_;	
	std::string remote_ip_;
	unsigned short remote_port_;
	void* user_data_;
};
}