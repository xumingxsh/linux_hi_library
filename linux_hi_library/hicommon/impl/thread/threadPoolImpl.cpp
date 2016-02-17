#include "threadPoolImpl.h"
#include <time.h> 
#include <vector>

using namespace std;
namespace Hi
{
ThreadPoolImpl::ThreadPoolImpl(): is_exit(false)
{
}

ThreadPoolImpl::~ThreadPoolImpl()
{
}

bool ThreadPoolImpl::stop()
{
	if (is_exit)
	{
		return false;
	}
	is_exit = true;
	for (auto it = threads.begin(); it != threads.end(); ++it)
	{
		pthread_join(*it, NULL);
	}
	threads.clear();
	exit_fun();
	return true;
}

class ThreadFun
{
public:
	function<void()> run;
};

static void* run_fun(void*);
bool ThreadPoolImpl::start(int threadNum,const function<void()>& run, 
	const std::function<void()>& exitFun)
{
	if (!threads.empty())
	{
		return false;
	}
	exit_fun = exitFun;
	for (int i = 0; i < threadNum; i++)
	{
		ThreadFun* tf = new ThreadFun;
		tf->run = run;
		pthread_t pt;
		pthread_create(&pt, NULL,  run_fun, tf);
		threads.push_back(pt);
	}
	return true;
}

void* run_fun(void* arg)
{
	ThreadFun* tf = static_cast<ThreadFun*>(arg);
	function<void()> run = tf->run;
	delete tf;
	run();
	return NULL;
}

bool ThreadPoolImpl::is_need_exit()
{
	return is_exit;
}
}