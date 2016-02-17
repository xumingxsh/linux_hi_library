#include "common/hiLog.h"
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/epoll.h>

#include <iostream>

//#include "mutex/hiCritical.h"

using namespace std;

#if !defined(HILOG_ON_VARLIST)
#define HILOG_ON_VARLIST(x) \
	char chArr[2048];\
	memset(chArr, 0 , sizeof(chArr));\
	va_list arguments;\
	va_start(arguments, x);\
	vsnprintf(chArr, 2047, x, arguments);	\
	va_end(arguments) ; 
#endif

namespace Hi
{
static HiLogCallBack s_callback = NULL;
static Hi_Log_Type s_log_type = HLT_Info;
//static HiCritical s_lock;	
static pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}
FunctionLog::FunctionLog(const char* name)
{
	if ((int)s_log_type > (int)HLT_FunLog)
	{
		return;
	}
	name_ = name;
	gettimeofday(&begin_, NULL);
	hi_log(HLT_FunLog, 
		"[%s %s ][function:%s][thread:%d][enter]",
		__DATE__, __TIME__, name, gettid());
}
FunctionLog::~FunctionLog()
{
	if ((int)s_log_type > (int)HLT_FunLog)
	{
		return;
	}
	timeval now;
	gettimeofday(&now, NULL);
	unsigned long long distance = 
		(now.tv_sec * 1000 * 1000 + now.tv_usec) - 
		(begin_.tv_sec * 1000 * 1000 + begin_.tv_usec);		
	
	hi_log(HLT_FunLog, 
		"[%s %s ][function:%s][thread:%d]"
		"[leave,leave,time long(usecond):%d]",
		__DATE__, __TIME__, name_.c_str(), 
		gettid(), distance);
}

void set_log_evel(Hi_Log_Type type)
{
	s_log_type = type;
}

void set_log_callback(HiLogCallBack callback)
{
	s_callback = callback;
}

void hi_log(Hi_Log_Type type, const char* fmt, ...)
{
	if ((int)s_log_type > (int)type)
	{
		return;
	}
	
	HILOG_ON_VARLIST(fmt);
	
	if (s_callback != NULL)
	{
		//s_lock.lock();
		s_callback(type, chArr);
		//s_lock.unlock();
	}
	else
	{
		//pid_t pid = gettid();
		//s_lock.lock();
		/*
		cout<<"[log type:"<<type<<"]"<<"[thread id:"<<pid<<
			"]"<<"["<<__DATE__<<" "<<__TIME__<<"]["<<chArr<<"]"<<endl;*/
		cout<<"["<<__DATE__<<" "<<__TIME__<<"]["<<chArr<<"]"<<endl;
		//s_lock.unlock();
	}
}
void hi_info(const char* fmt,...)
{
	HILOG_ON_VARLIST(fmt);
	hi_log(HLT_Info, chArr);
}
void hi_warn(const char* fmt,...)
{
	HILOG_ON_VARLIST(fmt);
	hi_log(HTL_Warn, chArr);
}
void hi_error(const char* fmt,...)
{
	HILOG_ON_VARLIST(fmt);
	hi_log(HTL_Error, chArr);
}
void hi_error(const char* fun, const char* fmt,...)
{
	HILOG_ON_VARLIST(fmt);
	hi_log(HTL_Error, "%s %s", fun, chArr);
}

HiErrorInfo::HiErrorInfo(const char* fun):function_(fun)
{
}

void HiErrorInfo::log(const char* fmt, ...)
{
	HILOG_ON_VARLIST(fmt);
	hi_error("%s %s", function_.c_str(), chArr);
}

}


	
	