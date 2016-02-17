#pragma once
#include "hiCommonLib_Export.h"

namespace Hi
{
class ConditionImpl;
class HiCondition
{
public:
	HiCondition();
	~HiCondition();
public:
	void wait();

	// returns true if time out, false otherwise.
	bool waitForSeconds(int seconds);

	void notify();

	void notifyAll();

	void lock();

	void unlock();
private:
	ConditionImpl* impl_;
};
}