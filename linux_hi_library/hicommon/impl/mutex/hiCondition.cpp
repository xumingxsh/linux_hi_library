#include "mutex/hiCondition.h"
#include "conditionImpl.h"

namespace Hi
{	
HiCondition::HiCondition(): impl_(new ConditionImpl())
{
}

HiCondition::~HiCondition()
{
	delete impl_;
}

void HiCondition::wait()
{
	impl_->wait();
}

// returns true if time out, false otherwise.
bool HiCondition::waitForSeconds(int seconds)
{
	return false;
}

void HiCondition::notify()
{
	impl_->notify();
}

void HiCondition::notifyAll()
{
	impl_->notifyAll();
}

void HiCondition::lock()
{
	impl_->lock();
}

void HiCondition::unlock()
{
	impl_->unlock();
}
}