#pragma once

/**
*    @defgroup 数据库模块
* @{
*/ 

#include <memory>
#include <functional>

#include "hiDBCommon.h"

namespace Hi
{
class HiDBImpl;

/// @enum	HiDBLogType
///
/// @brief	日志类型.
enum HiDBLogType
{
	HiDLT_Normal,		/// @brief	普通日志. 
	HiDLT_Exception,	/// @brief	异常日志. 
	HiDLT_SQL			/// @brief	SQL语句日志，主要调试时使用. 
};

/// @typedef	std::function<void(HiDBLogType,const char*)> HiDBLogFun
///
/// @brief	日志回调函数.
typedef std::function<void(HiDBLogType,const char*)> HiDBLogFun;

typedef void (*HiDBLogCallBack)(HiDBLogType,const char*);

/**
* @brief 数据库操作类，封装数据库的通用操作，本类使用策略模式实现
*	 @author  徐敏荣
*    @date    2012-06-14
*
* @par 修订历史
*    @version v0.5 \n
*	 @author  徐敏荣
*    @date    2012-06-14
*    @li 初始版本
*    @version v0.6 \n
*	 @author  徐敏荣
*    @date    2014-08-04
*    @li 简化程序
*    @date    2014-08-04
*    @li 添加错误信息的报出，扩展异常为继承自std::exception
*    @date    2014-09-11
*    @li 修复多次close时崩溃的问题
*
*/
class HiDB
{
public:

	/**
	*   @brief 构造函数
	*	@param[in] type			数据库类型
	*	@param[in] isUsingLock	是否需要使用互斥锁
	*/
	HiDB(HiDBType type = HiDBType_MySQL, bool isUsingLock = false);

	/**
	*   @brief 析构函数
	*/
	~HiDB();

public:
	
	/**
	*   @brief 打开数据库连接
	*	@param[in] conn			数据库连接字符串
	*   @retval true：成功，false；失败
	*   @par 实例：
	*   @code
	*	HiDB db;
	*	if (db.open("host=127.0.0.1;port=3306;dbname=test;user=root;pwd=root;charset=gbk;"))
	*	{
	*		// 打开成功
	*	}
	*	else
	*	{
	*		// 打开失败
	*	}
	*   @endcode
	*/
	bool open(const char* conn);
	
	/**
	*   @brief 关闭据库连接
	*/
	void close(void);
	
	/**
	*   @brief 数据库连接是否打开
	*/
	bool is_open();

public:
	
	/**
	*   @brief 执行SQL语句，并不返回结果
	*	@param[in] conn			SQL语句
	*   @retval true：成功，false；失败
	*   @par 实例：
	*   @code
	*	HiDB db;
	*	if (db.executeNoQuery("UPDATE table SET Paramer1='%s' 
	*		and Paramer2='%s' OR Paramer3=%d", "test1", "test2", 3))
	*	{
	*		// 执行成功
	*	}
	*	else
	*	{
	*		// 执行失败
	*	}
	*   @endcode
	*/
	bool execute_no_query(const char* sql, ...);
	
public:

	/**
	*   @brief 执行SQL语句，返回一个结果
	*	@param[in] sql			SQL语句
	*   @retval 获得的数据，如果为空，则失败
	*/
	std::string execute_scalar(const char* sql, ...);
	
public:

	/**
	*   @brief 执行SQL语句，返回一个结果集合
	*	@param[in] sql			SQL语句
	*   @retval 存储查询记录的智能指针
	*/
	std::shared_ptr<HiDBTable> execute_query(const char* sql, ...);
	
public:	

	/**
	*   @brief 在事务中执行处理
	*	@param[in] fun	处理函数
	*/
	void on_transaction(const std::function<void()>& fun);

public:
	void set_log_function(const HiDBLogFun& fun);
	void set_log_function(HiDBLogCallBack fun);

private:
	HiDBImpl*	impl_;		/**< 数据库操作实现指针 */
	HiDBLogFun	log_fun_;	/**< 日志回调函数 */
};
}
/**//** @}*/ // 数据库模块