#include <stdarg.h>
#include <string.h>
#include "db/hiDB.h"
#include "hiDBMySQL.h"

using namespace std;
using namespace std::placeholders;

#if !defined(HISDB_ON_VARLIST)
#define HISDB_ON_VARLIST(x, y) \
	char chArr[2048];\
	memset(chArr, 0 , sizeof(chArr));\
	va_list arguments;\
	va_start(arguments, y);\
	vsnprintf(chArr, 2047, x, arguments);	\
	va_end(arguments) ; \
	log_fun_(HiDLT_SQL, chArr);
#endif

namespace Hi
{
static void defaultFun(HiDBLogType type,const char* ex)
{
}

static bool IsImplOK(HiDBImpl* db)
{
	if (!db)
	{
		return false;
	}
	return true;
}

// 构造函数
HiDB::HiDB(HiDBType type, bool isUsingLock):impl_(NULL)
{
	if (type == HiDBType_MySQL)
	{
		impl_ = new HiDBMySQL(isUsingLock);
	}

	log_fun_ = defaultFun;
}

// 析构函数
HiDB::~HiDB()
{
	if (impl_)
	{
		delete impl_;
		impl_ = NULL;
	}
}

// 打开数据库连接
bool HiDB::open(const char* conn)
{
	if (!impl_)
	{
		
		log_fun_(HiDLT_Exception, "in open function:impl is null");
		return false;
	}

	return impl_->open(conn);
}

bool HiDB::is_open()
{
	if (!impl_)
	{
		return false;
	}

	return impl_->is_open();
}

void HiDB::close(void)
{
	if (!IsImplOK(impl_))
	{
		return;
	}

	return impl_->close();
}

bool HiDB::execute_no_query(const char* sql, ...)
{
	if (!IsImplOK(impl_))
	{
		return false;
	}

	HISDB_ON_VARLIST(sql, sql);

	return impl_->execute_no_query(chArr);
}

string HiDB::execute_scalar(const char* sql, ...)
{
	if (!IsImplOK(impl_))
	{
		return "";
	}

	HISDB_ON_VARLIST(sql, sql);
	
	return impl_->execute_scalar(chArr);
}

std::shared_ptr<HiDBTable> HiDB::execute_query(const char* sql, ...)
{
	if (!IsImplOK(impl_))
	{
		return NULL;
	}
	HISDB_ON_VARLIST(sql, sql);

	return impl_->execute_query(chArr);
}	

void HiDB::on_transaction(const std::function<void()>& fun)
{
	if (!IsImplOK(impl_))
	{
		HiDBHelperOnError_void("HiDB::OnTransaction", 
			"HiDB is not impl", "", 0);
	}
	return impl_->on_transaction(fun);
}

void HiDB::set_log_function(const HiDBLogFun& fun)
{
	log_fun_ = fun;
	if (impl_)
	{
		impl_->set_log_function(fun);
	}
}

void HiDB::set_log_function(HiDBLogCallBack fun)
{
	Hi::HiDBLogFun logfun = std::bind(fun, _1, _2);
	set_log_function(logfun);
}
}