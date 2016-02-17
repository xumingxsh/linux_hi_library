#pragma once

/**
*    @defgroup 数据库模块
* @{
*/ 
#include <string>
#include <vector>
#include <map>
#include <sstream>
namespace Hi
{
/** @brief 数据库类型 */
enum HiDBType
{
	HiDBType_Invail,	/**<  无效类型 */
	HiDBType_MySQL,	/**<  MySQL */
};

#ifndef HiDBTable

typedef std::map<std::string, std::string> HiDBMap;

/** @brief 查询结果 */
typedef std::vector<HiDBMap> HiDBTable; /**<  查询结果 */
#endif

/** @brief 数据库操作异常 */
class HiDBException: public std::exception
{
public:
	HiDBException();
public:
	std::string to_srting();
	const char* what()  const _GLIBCXX_USE_NOEXCEPT;
public:
	std::string		sql_;			/**<  本次操作的SQL语句 */
	std::string		descript_;		/**<  异常描述 */
	std::string		position_;		/**<  异常位置 */
	long			error_id_;		/**<  异常编号 */
	HiDBType		db_type_;		/**<  数据库类型 */
private:
	std::string err_;
};

/** @brief 异常语句宏 */
#define HiDBHelperOnError_void(ps, script,sql, id) \
	HiDBException exception;\
	exception.position_ = ps;\
	exception.descript_ = script;\
	exception.sql_ = sql;\
	exception.error_id_ = id;\
	log_fun_(HiDLT_Exception, exception.what());\
	throw exception;

/** @brief 异常语句宏 */
#define HiDBHelperOnError(ps, script,sql, id) \
	HiDBException exception;\
	exception.position_ = ps;\
	exception.descript_ = script;\
	exception.sql_ = sql;\
	exception.error_id_ = id;\
	log_fun_(HiDLT_Exception, exception.what());\
	throw exception;
	//return false;
}
/**//** @}*/ // 数据库模块