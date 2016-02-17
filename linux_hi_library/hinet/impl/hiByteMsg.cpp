#include "net/hiByteMsg.h"
#include "common/hiLog.h"

using namespace std;

namespace Hi
{
class HiByteMsgImpl
{
public:
	HiByteMsgImpl(void* data, unsigned short len, 
		const std::function<void(void*)>& free): 
		data_(data),len_(len), free_fun_(free)
	{
	}
	~HiByteMsgImpl()
	{
		free_fun_(data_);
	}
public:
	void* data()
	{
		return data_;
	}
	unsigned short len()
	{
		return len_;
	}
private:
	void* data_;
	unsigned short len_;
	std::function<void(void*)> free_fun_;
};
HiByteMsg::HiByteMsg()
{
	FUN_LOG();
	impl_ = NULL;
}
HiByteMsg::HiByteMsg(void* data, unsigned short len,
	const std::function<void(void*)>& free_fun):
	impl_(new HiByteMsgImpl(data, len, free_fun))
{
	FUN_LOG();
}
HiByteMsg::~HiByteMsg()
{
	FUN_LOG();
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}
void* HiByteMsg::data()
{
	FUN_LOG();
	if (impl_)
	{
		return impl_->data();
	}
	return NULL;
}
unsigned short HiByteMsg::len()
{
	FUN_LOG();
	if (impl_)
	{
		return impl_->len();
	}
	return 0;
}
}