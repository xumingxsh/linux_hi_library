#include "byteMsgHandleImpl.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include "netThreadCommon.h"

#include "common/hiLog.h"

using namespace std;

namespace Hi
{
ByteMsgHandleImpl::ByteMsgHandleImpl(int sock, 
	const std::function<void(HiByteMsg_Ptr)>& fun): 
	sock_(sock), 
	on_complete_(fun), 
	buf_(NULL), 
	buf_pos_(0),
	msg_len_(0),
	head_pos_(0)
{
	FUN_LOG();
	//memset(head_,0,sizeof(char)*4);
}

ByteMsgHandleImpl::~ByteMsgHandleImpl()
{
	FUN_LOG();
	if (buf_ != NULL)
	{
		free(buf_);
		buf_ = NULL;
	}
}

static int recv_msg(int sock, void* buf, int len);
static bool on_recv_result(int result);
bool ByteMsgHandleImpl::read_i()
{
	FUN_LOG();
	if (!read_head())
	{
		return false;
	}
	if (msg_len_ == 0)
	{
		return true;
	}	
	
	// 如果上一次有数据未接收完，本次完成接收
	int size = recv_msg(sock_, 
		((char*)buf_) + buf_pos_, 
		msg_len_ - buf_pos_);
		
	if (!on_recv_result(size))
	{
		return false;
	}
	
	buf_pos_ += size;
	
	// 如果数据接收完毕，则将接收相关属性还原为初始状态
	if (buf_pos_ == msg_len_)
	{
		cout<<"a message is completed"<<endl;
		shared_ptr<HiByteMsg> msg(new HiByteMsg(buf_, msg_len_,[&](void* dt){
			if (dt != NULL)
			{
				free(dt);
			}
		}));
		on_complete_(msg);
		buf_ = NULL;
		msg_len_ = 0;
		buf_pos_ = 0;
		
		head_pos_ = 0;
	}
	return true;
}

bool ByteMsgHandleImpl::read()
{
	FUN_LOG();
	while (true)
	{
		if (!read_i())
		{
			break;
		}
	}
	if(errno == EAGAIN)
	{
		cout<<"EAGAIN"<<endl;
		return true;
	}
	return false;
}
struct Head_Struct
{
	unsigned char a1;
	unsigned char a2;
	unsigned short len;
};
bool ByteMsgHandleImpl::read_head()
{
	FUN_LOG();
	// 还有未接收完的消息体
	if (msg_len_ != 0)
	{
		return true;
	}
	
	// 还有未接收完的消息头		
	int size = recv_msg(sock_, head_ + head_pos_, 4 - head_pos_);	
	if (!on_recv_result(size))
	{
		return false;
	}
	head_pos_ += size;
	
	// 如果接收完成，则设置消息长度
	if (head_pos_ == 4)
	{
		unsigned short len = 16;
		Head_Struct* h_s = 
			static_cast<Head_Struct*>(Hi::Net::implicit_cast<void*>(&head_));
			
		len = h_s->len;
		msg_len_ = len;
		if (msg_len_ > HiByteMSG_Max_Body_Len)
		{
			cout<<"msg len error,len:"<<msg_len_<<endl;
			msg_len_ =  0;
			head_pos_ = 0;
			return true;
		}
		buf_pos_ = 0;
		memset(head_, 0, 4);
		head_pos_ = 0;
		buf_ = malloc(HiByteMSG_Max_Body_Len);
		cout<<"parase head,len:"<<msg_len_<<endl;
		return true;
	}
	
	// 如果起始位数不是标志位，则抛弃该字段
	if ((char)head_[0] != HiByteMSG_Begin)
	{
		cout<<"not find head start char in msg. byte is:"<<hex<<head_<<endl;
		memset(head_, 0, 4);
		head_pos_ = 0;
		return true;
	}
	return true;
}
bool on_recv_result(int result)
{
	FUN_LOG();
	if (result > 0)
	{
		return true;
	}
	cout<<"recv error,reason:"<<strerror(errno)<<endl;
	return false;
}
int recv_msg(int sock, void* buf, int len)
{
	FUN_LOG();
	return recv(sock, buf, len, 0);	
}
}