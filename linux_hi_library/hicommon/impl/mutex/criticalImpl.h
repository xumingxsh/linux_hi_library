#pragma once

#ifdef WINDOWS_
#include "pthread/pthread.h"
#endif
#ifdef LINUX_
#include "pthread.h"
#endif

namespace Hi
{
class CriticalImpl
{
public:
	CriticalImpl()
	{
		pthread_mutex_init(&mutex_, NULL);
	}

	~CriticalImpl()
	{
		pthread_mutex_destroy(&mutex_);
	}

	void lock()
	{
		pthread_mutex_lock(&mutex_);
	}

	void unlock()
	{
		pthread_mutex_unlock(&mutex_);
	}
private:
	pthread_mutex_t	mutex_;			/**< 临界区对象 */
};
}
