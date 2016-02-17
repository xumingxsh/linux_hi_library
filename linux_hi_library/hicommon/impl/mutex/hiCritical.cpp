#include "mutex/hiCritical.h"
#include "criticalImpl.h"
namespace Hi
{
class CriticalImpl;

HiCritical::HiCritical(): impl_(new CriticalImpl())
{
}

HiCritical::~HiCritical()
{
	delete impl_;
}

void HiCritical::lock()
{
	impl_->lock();
}

void HiCritical::unlock()
{
	impl_->unlock();
}
}