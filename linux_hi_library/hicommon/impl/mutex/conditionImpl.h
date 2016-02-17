#pragma once
#ifdef LINUX_
#include "pthread.h"
#endif
namespace Hi
{
class ConditionImpl
{
public:
	explicit ConditionImpl()
	{
		pthread_mutex_init(&mutex_, NULL);
		pthread_cond_init(&pcond_, NULL);
	}

	~ConditionImpl()
	{
		pthread_cond_destroy(&pcond_);
		pthread_mutex_destroy(&mutex_);
	}

	void wait()
	{
		pthread_cond_wait(&pcond_, &mutex_);
	}

	// returns true if time out, false otherwise.
	bool waitForSeconds(int seconds);

	void notify()
	{
		pthread_cond_signal(&pcond_);
	}

	void notifyAll()
	{
		pthread_cond_broadcast(&pcond_);
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
	pthread_mutex_t mutex_;
	pthread_cond_t pcond_;
};
}
