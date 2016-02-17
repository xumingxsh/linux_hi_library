#include "hiDBImpl.h"

#include "mutex/hiCritical.h"

namespace Hi
{
static void defaultFun(HiDBLogType type,const char* ex)
{
}

HiDBImpl::HiDBImpl(bool isUsingLock): critical_(NULL)
{
	if (isUsingLock)
	{
		critical_ = new HiCritical();
	}

	log_fun_ = defaultFun;
}

HiDBImpl::~HiDBImpl()
{
	if (this->critical_)
	{
		delete critical_;
		critical_ = NULL;
	}
}
void HiDBImpl::set_log_function(const HiDBLogFun& fun)
{
	log_fun_ = fun;
}
}