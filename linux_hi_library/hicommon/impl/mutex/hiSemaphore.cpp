#include "mutex/hiSemaphore.h"
#include "semaphoreImpl.h"

namespace Hi
{

HiSemaphore::HiSemaphore(int max): impl_(new SemaphoreImpl(max))
{
}

HiSemaphore::~HiSemaphore()
{
	delete impl_;
}

bool HiSemaphore::request(const std::function<bool()>& fun)
{
	return impl_->request(fun);
}
bool HiSemaphore::release(const std::function<bool()>& fun)
{
	return impl_->release(fun);
}

void HiSemaphore::exit()
{
	impl_->exit();
}
}