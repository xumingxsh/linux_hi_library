/**
*    @defgroup 通用模块
* @{
*/ 
#pragma once
#include "hiCommonLib_Export.h"

namespace Hi
{
class CriticalImpl;
/**
* @brief 临界区访问类，主要封装临界区的访问，该类主要在栈中使用，利用局部变量的构造和析构函数出入临界区
*	 @author  徐敏荣
*    @date    2012-06-14
*
* @par 修订历史
*    @version v0.5 \n
*	 @author  徐敏荣
*    @date    2012-06-14
*    @li 初始版本
*
*/
class HiCritical
{
public:

	/**
	*   @brief 构造函数
	*/
	HiCritical();

	/**
	*   @brief 析构函数
	*/
	~HiCritical();

	/**
	*   @brief 进入临界区
	*/
	void lock();
	
	/**
	*   @brief 离开临界区
	*/
	void unlock();
private:
	
	/**
	*   @brief 临界区对象
	*/
	CriticalImpl*	impl_;			/**< 临界区对象 */
};


/**
* @brief 临界区访问管理类，利用构造函数进入临界区，利用西沟函数离开临界区
*		如果向构造函数提供NULL参数，则不使用临界区。
*
*/
class HiCriticalMng
{
public:
	HiCriticalMng(HiCritical& crl): cl(&crl)
	{
		cl->lock();
	}

	HiCriticalMng(HiCritical* crl): cl(crl)
	{
		if (cl)
		{
			cl->lock();
		}
	}

	~HiCriticalMng()
	{
		if (cl)
		{
			cl->unlock();
		}
	}

private:
	HiCritical*   cl;
};
}

/**//** @}*/ // 通用模块