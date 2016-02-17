/**
*    @defgroup 通用模块
* @{
*/ 
#pragma once
#include "mutex/hiCritical.h"

namespace Hi
{
template<typename T>
class HiAtomic
{
public:
	HiAtomic(T t, T max): t_(t), max_(max), min_(t)
	{
	}
	
	T add(T t)
	{		
		T tmp;
		critical_.lock();
		t_ += t;
		if (t_ >= max_)
		{
			t_ = min_;
		}
		tmp = t;
		critical_.unlock();
		return tmp;
	}
private:
	T	t_;		
	T 	max_;
	T 	min_;
	HiCritical critical_;
};
}

/**//** @}*/ // 通用模块