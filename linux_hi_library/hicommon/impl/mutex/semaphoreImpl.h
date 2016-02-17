#pragma once
#include <functional>
#include "mutex/hiCondition.h"

namespace Hi
{
class SemaphoreImpl
{
public:
	SemaphoreImpl(int max);
	~SemaphoreImpl();

	bool request(const std::function<bool()>& fun);
	bool release(const std::function<bool()>& fun);
	void exit();
private:
	SemaphoreImpl* impl;
private:
	int handle_count_;
	bool isExit_;
	HiCondition isNotFull_;
};
}