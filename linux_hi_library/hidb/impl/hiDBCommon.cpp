#include "db/hiDBCommon.h"

#include <sstream>

using namespace std;
namespace Hi
{
HiDBException::HiDBException()
{
	error_id_	= 0;
	db_type_	= HiDBType_MySQL;
}

string HiDBException::to_srting()
{
	ostringstream oss;
	oss<<"Info:HiDBException"
		<<";DBType:"<<db_type_
		<<";Position:"<<position_
		<<";SQL:"<<sql_
		<<";Description:"<<descript_
		<<";Error ID:"<<error_id_;

	return oss.str();
}

const char* HiDBException::what()  const _GLIBCXX_USE_NOEXCEPT
{	
	HiDBException* ex = const_cast<HiDBException*>(this);
	ex->err_ = ex->to_srting();
	return ex->err_.c_str();
}
}