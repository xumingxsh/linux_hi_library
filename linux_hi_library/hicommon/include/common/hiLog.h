#pragma once
#include <string>
#include <sys/time.h>

namespace Hi
{
enum Hi_Log_Type
{
	HLT_FunLog,	/// < 函数的进入，离开时间
	HLT_Info,	/// < 普通日志
	HTL_Warn,	/// < 警告日志
	HTL_Error	/// < 错误日志
};
typedef void (*HiLogCallBack)(Hi_Log_Type type, const char* script);
class FunctionLog
{
public:
	FunctionLog(const char* name);
	~FunctionLog();
private:
	std::string name_;
	std::string param_;
	timeval begin_;
};

void set_log_callback(HiLogCallBack callback);
void set_log_evel(Hi_Log_Type type);
void hi_log(Hi_Log_Type type, const char* fmt,...);
void hi_info(const char* fmt,...);
void hi_warn(const char* fmt,...);
void hi_error(const char* fmt,...);

class HiErrorInfo
{
public:
	HiErrorInfo(const char* fun);
	void log(const char* fmt, ...);
private:
	std::string function_;
};
}
#ifdef USE_FUN_LOG
#ifndef FUN_LOG
#define FUN_LOG() Hi::FunctionLog fun_log_##__LINE__(__FUNCTION__)
#endif
#else
#ifndef FUN_LOG
#define FUN_LOG() 
#endif
#endif


#ifndef HI_ERROR
#define HI_ERROR(x)  \
	{\
		Hi::HiErrorInfo(__FUNCTION__).log x;\
	}while(0)
#endif



	
	