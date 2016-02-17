#include "hiDBMySQL.h"
#include <string.h>
#include "mutex/hiCritical.h"
#include "common/hiScopeGuard.h"
#include <sstream>
using namespace std;
namespace Hi
{
// 构造函数
HiDBMySQL::HiDBMySQL(bool isUsingLock): HiDBImpl(isUsingLock), p_sql_data_(NULL)
{
}

// 析构函数
HiDBMySQL::~HiDBMySQL()
{
	if(p_sql_data_)
	{
		::mysql_close(p_sql_data_);
		p_sql_data_ = NULL;
	}
}

// 打开数据库连接
bool HiDBMySQL::open(const char* conn)
{
	if (critical_)
	{
		critical_->lock();
	}
	ON_SCOPE_EXIT([&](){
		if (critical_)
		{
			critical_->unlock();
		}
	});

	if(p_sql_data_)
	{
		log_fun_(HiDLT_Normal, "p_sql_data_ is not null, so need not open connect");
		return true;
	}

	if (::strlen(conn) == 0)
	{
		log_fun_(HiDLT_Exception, "open function param conn is empty");
		p_sql_data_ = NULL;
		return false;
	}

	char host[40]	= {0};
	char dbname[40] = {0};
	long port		= 0;
	char user[40]	= {0};
	char pwd[40]	= {0};
	char chrSet[40]	= {0};

	::sscanf(conn, 
		("host=%[^;];port=%ld;dbname=%[^;];user=%[^;];pwd=%[^;];charset=%[^;];"),
		host, &port, dbname,user, pwd, chrSet);

	stringstream oss;
	oss<<"open function conn param: host:"<<host<<" port:"<<port<<" dbname:"<<dbname<<" user:"<<user
		<<" pwd:"<<pwd<<" chrSet:"<<chrSet;
	log_fun_(HiDLT_Normal, oss.str().c_str());
	::mysql_init(&sql_data_);

	if(::mysql_real_connect(&sql_data_, 
		host, user, pwd, dbname, port, NULL, 0))
	{
		p_sql_data_ = &sql_data_;

		ostringstream oss;
		oss<<"set names "<<chrSet;

		::mysql_query(p_sql_data_, oss.str().c_str());

		return true;
	}
	else
	{
		if(p_sql_data_)
		{
			long id = mysql_errno(p_sql_data_);
			const char* err = mysql_error(p_sql_data_);
			//关闭连接
			::mysql_close(p_sql_data_);

			p_sql_data_ = NULL;
			HiDBHelperOnError("Open(const char*)", err, "", id);
		}
		else
		{
			log_fun_(HiDLT_Exception, "in open function:p_sql_data_ is null");
			p_sql_data_ = &sql_data_;
			long id = mysql_errno(p_sql_data_);
			const char* err = mysql_error(p_sql_data_);
			
			p_sql_data_ = NULL;
			HiDBHelperOnError("Open(const char*)", err, "", id);
		}
		
			
		return false;
	}
}

void HiDBMySQL::close(void)
{
	if (critical_)
	{
		critical_->lock();
	}
	ON_SCOPE_EXIT([&](){
		if (critical_)
		{
			critical_->unlock();
		}
	});

	if(p_sql_data_)
	{
		::mysql_close(p_sql_data_);
		p_sql_data_ = NULL;
	}
}

bool HiDBMySQL::execute_no_query(const char* sql)
{
	if (critical_)
	{
		critical_->lock();
	}
	ON_SCOPE_EXIT([&](){
		if (critical_)
		{
			critical_->unlock();
		}
	});

	if (!is_open())
	{
		//HiDBHelperOnError("ExecuteNoQuery(const char*)", "Database is not open", sql, 0);
		return false;
	}

	long error = ::mysql_query(p_sql_data_, sql);
	bool result = (error == 0)? true:false;
	if (result)
	{
		MYSQL_RES *pResult = ::mysql_store_result(p_sql_data_);
		if(pResult)
		{
			::mysql_free_result(pResult);	//释放数据集
		}
		return true;
	}

	unsigned int errId = mysql_errno(p_sql_data_);
	if (errId == 0)
	{
		MYSQL_RES *pResult = ::mysql_store_result(p_sql_data_);
		if(pResult)
		{
			::mysql_free_result(pResult);	//释放数据集
		}
		return false;
	}
	HiDBHelperOnError("ExecuteNoQuery(const char*)", 
		mysql_error(p_sql_data_), sql, mysql_errno(p_sql_data_));
}

string HiDBMySQL::execute_scalar(const char* sql)
{
	if (critical_)
	{
		critical_->lock();
	}
	ON_SCOPE_EXIT([&](){
		if (critical_)
		{
			critical_->unlock();
		}
	});

	if (!is_open())
	{
		//HiDBHelperOnError("ExecuteNoQuery(const char*)", "Database is not open", sql, 0);
		return "";
	}

	long error = ::mysql_query(p_sql_data_, sql);
	if(error != 0)//执行SQL语句
	{
		HiDBHelperOnError("HiDBMySQL::ExecuteScalar(const char*, HiDBValue&)", 
			::mysql_error(p_sql_data_), sql, error);
	}

	MYSQL_RES* pResult= ::mysql_store_result(p_sql_data_);	//获取数据集
	if(!pResult)
	{
		HiDBHelperOnError("HiDBMySQL::ExecuteScalar(const char*, HiDBValue&)", 
			::mysql_error(p_sql_data_), sql, mysql_errno(p_sql_data_));
	}
	if (pResult->row_count == 0)
	{
		::mysql_free_result(pResult);	//释放数据集
		return "";
	}

	//MYSQL_FIELD* pFields = ::mysql_fetch_field(pResult);	
	::mysql_data_seek(pResult, 0);

	MYSQL_ROW curRow = ::mysql_fetch_row(pResult);

	string ret = curRow[0];

	::mysql_free_result(pResult);	//释放数据集
	return ret;
}

std::shared_ptr<HiDBTable> HiDBMySQL::execute_query(const char* sql)
{
	if (critical_)
	{
		critical_->lock();
	}
	ON_SCOPE_EXIT([&](){
		if (critical_)
		{
			critical_->unlock();
		}
	});

	if (!is_open())
	{
		//HiDBHelperOnError("ExecuteNoQuery(const char*)", "Database is not open", sql, 0);
		return NULL;
	}

	long error = ::mysql_query(p_sql_data_, sql);
	if(error != 0)//执行SQL语句
	{
		HiDBHelperOnError("HiDBMySQL::ExecuteQuery(const char*)", 
			::mysql_error(p_sql_data_), sql, error);
	}

	MYSQL_RES* pResult= ::mysql_store_result(p_sql_data_);	//获取数据集
	if(!pResult)
	{
		HiDBHelperOnError("HiDBMySQL::ExecuteQuery(const char*)", 
			::mysql_error(p_sql_data_), sql, mysql_errno(p_sql_data_));
	}

	std::shared_ptr<HiDBTable> tb(new HiDBTable());

	if (pResult->row_count == 0)
	{
		::mysql_free_result(pResult);	//释放数据集
		return tb;
	}

	MYSQL_FIELD* pFields = ::mysql_fetch_field(pResult);	

	for (unsigned int i = 0; i < pResult->row_count; i++)
	{
		::mysql_data_seek(pResult, i);
		MYSQL_ROW curRow = ::mysql_fetch_row(pResult);

		map<string, string> list;
		tb->push_back(list);
		map<string, string>& list2 = *tb->rbegin();
		for (int j = 0; j < (long)pResult->field_count; j++)
		{
			string val = "";
			if (curRow[j])
			{
				val = curRow[j];
			}
			list2[pFields[j].name] = val;
		}
	}

	::mysql_free_result(pResult);	//释放数据集

	return tb;
}	

void HiDBMySQL::on_transaction(const std::function<void()>& fun)
{
	if (critical_)
	{
		critical_->lock();
	}
	ON_SCOPE_EXIT([&](){
		if (critical_)
		{
			critical_->unlock();
		}
	});

	mysql_autocommit(p_sql_data_, 0);

	try
	{
		fun();

		if (::mysql_commit(p_sql_data_) == 0)
		{
			mysql_autocommit(p_sql_data_, 1);
			return;
		}

		if (::mysql_rollback(p_sql_data_) == 0)
		{
			mysql_autocommit(p_sql_data_, 1);
		}

		HiDBHelperOnError_void("HiDBMySQL::OnTransaction", 
			"execute transaction sucess,but commit failed", "", 0);
	}
	catch(HiDBException& e)
	{
		if (::mysql_rollback(p_sql_data_) == 0)
		{
			mysql_autocommit(p_sql_data_, 1);
		}
		HiDBHelperOnError_void("HiDBMySQL::RollbackTransaction", 
			e.descript_, e.sql_, e.error_id_);
	}
	catch (...)
	{
		if (::mysql_rollback(p_sql_data_) == 0)
		{
			mysql_autocommit(p_sql_data_, 1);
		}
		HiDBHelperOnError_void("HiDBMySQL::RollbackTransaction", 
			::mysql_error(p_sql_data_), "", mysql_errno(p_sql_data_));
	}
}

bool HiDBMySQL::is_open()
{
	return p_sql_data_ == NULL ? false : true;
}
}