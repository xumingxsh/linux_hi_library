#pragma once

#include <functional>

#include "net/hiByteMsg.h"

namespace Hi
{
class ByteMsgHandleImpl
{
public:
	ByteMsgHandleImpl(int sock, 
		const std::function<void(HiByteMsg_Ptr)>& fun);
		
	~ByteMsgHandleImpl();
public:
	bool read();
private:
	bool read_head();
	bool read_i();
private:
	int sock_;
	std::function<void(HiByteMsg_Ptr)> on_complete_;
	void* buf_;
	unsigned short buf_pos_;	
	unsigned short msg_len_;
	
	
	char head_[4];
	unsigned short head_pos_;
};
}