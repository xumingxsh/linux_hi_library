#include "thread/hiThreadPool.h"
#include "threadPoolImpl.h"
using namespace std;

namespace Hi
{
HiThreadPool::HiThreadPool(): impl_(new ThreadPoolImpl())
{
}

HiThreadPool::~HiThreadPool()
{
	delete impl_;
	impl_ = NULL;
}

bool HiThreadPool::stop()
{
	return impl_->stop();
}

bool HiThreadPool::start(int threadNum,
	const std::function<void()>& run,
	const function<void()>& exitFun)
{
	return impl_->start(threadNum, run, exitFun);
}
bool HiThreadPool::start(int threadNum,const std::function<void()>& run)
{
	return impl_->start(threadNum, run, [&](){});
}

bool HiThreadPool::is_need_exit()
{
	return impl_->is_need_exit();
}
}