#include "semaphoreImpl.h"
#include "common/hiScopeGuard.h"
using namespace std;

namespace Hi
{
SemaphoreImpl::SemaphoreImpl(int max): handle_count_(max), isExit_(false)
{
}
SemaphoreImpl::~SemaphoreImpl()
{
}

bool SemaphoreImpl::request(const function<bool()>& fun)
{	
	if (isExit_)
	{
		fun();
		return false;
	}
	isNotFull_.lock();
	ON_SCOPE_EXIT([&](){
		isNotFull_.unlock();
		});
	while (handle_count_ < 1)
	{		
		if (isExit_)
		{	
			fun();
			return false;
		}
		isNotFull_.wait();
		if (isExit_)
		{
			fun();
			return false;
		}
	}

	handle_count_--;
	bool ret = fun();
	if (!ret)
	{
		handle_count_++;
		isNotFull_.notify();
	}
	return ret;
}
bool SemaphoreImpl::release(const std::function<bool()>& fun)
{	
	isNotFull_.lock();
	ON_SCOPE_EXIT([&](){
		isNotFull_.unlock();
		});
	bool ret = fun();
	if (ret)
	{
		handle_count_++;
		isNotFull_.notify();
	}
	return ret;
}
void SemaphoreImpl::exit()
{
	isExit_ = true;
	isNotFull_.notifyAll();
}
}