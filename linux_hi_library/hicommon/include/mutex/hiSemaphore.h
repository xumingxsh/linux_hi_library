#pragma once
#include <functional>

#include "hiCommonLib_Export.h"

namespace Hi
{
class SemaphoreImpl;
class HI_COMMON_LIB_API HiSemaphore
{
public:
	HiSemaphore(int max);
	~HiSemaphore();

	bool request(const std::function<bool()>& fun);
	bool release(const std::function<bool()>& fun);
	void exit();
private:
	SemaphoreImpl* impl_;
};
}