#include "net/hiByteMsgHandle.h"
#include "byteMsgHandleImpl.h"
#include "common/hiLog.h"

using namespace std;

namespace Hi
{
HiByteMsgHandle::HiByteMsgHandle(int sock, 
	const std::function<void(HiByteMsg_Ptr)>& fun): 
	impl_(new ByteMsgHandleImpl(sock, fun))
{
	FUN_LOG();
}
HiByteMsgHandle::~HiByteMsgHandle()
{
	FUN_LOG();
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}

bool HiByteMsgHandle::read()
{
	FUN_LOG();
	return impl_->read();
}
}