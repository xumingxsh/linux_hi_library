#pragma once

#include <deque>

#include "mutex/hiCondition.h"

namespace Hi
{
template<typename T>
class HiCondQueue
{
public:
	HiCondQueue(): isExit_(false)
	{
	}

	bool put(const T& x)
	{
		if (isExit_)
		{
			return false;
		}
		cond_.lock();
		queue_.push_back(x);
		cond_.notify(); // TODO: move outside of lock		
		cond_.unlock();
		return true;
	}

	bool take(T& t)
	{
		cond_.lock();	
		if (!isExit_)
		{
			while (queue_.empty())
			{
				cond_.wait();
				if (isExit_)
				{
					break;
				}
			}
		}
		// always use a while-loop, due to spurious wakeup
		if (queue_.empty())
		{
			cond_.unlock();
			return false;
		}
		t = T(queue_.front());
		queue_.pop_front();
		cond_.unlock();
		return true;
	}

	size_t size()
	{
		cond_.lock();	
		size_t size = queue_.size();
		cond_.unlock();
		return size;
	}

	void quit()
	{
		cond_.lock();	
		if (!isExit_)
		{
			isExit_ = true;
			cond_.notifyAll();
		}
		cond_.unlock();
	}
	
	bool is_quit()
	{
		return isExit_ && queue_.empty();
	}

	bool reset()
	{
		cond_.lock();	
		if (isExit_)
		{
			isExit_ = false;
		}
		cond_.unlock();
		return true;
	}
private:
	HiCondition        cond_;
	std::deque<T>     queue_;
	bool isExit_;
};
}

