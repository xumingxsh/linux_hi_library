#pragma once
#include <functional>

#include "hiCommonLib_Export.h"

namespace Hi
{
	class ThreadPoolImpl;
	class HI_COMMON_LIB_API HiThreadPool
	{
public:
	HiThreadPool();
	virtual ~HiThreadPool();
	public:		
		/**
		*   @brief 启动多线程。
		*   @param[in] threadNum 启动的线程数量
		*   @param[in] run	线程执行逻辑函数
		*   @retval true：成功，false；失败
		*/
		bool start(int threadNum, const std::function<void()>& run, 
		const std::function<void()>& exitFun);	

		/**
		*   @brief 启动多线程。
		*   @param[in] threadNum 启动的线程数量
		*   @param[in] run	线程执行逻辑函数
		*   @retval true：成功，false；失败
		*/
		bool start(int threadNum, const std::function<void()>& run);	
		
		/**
		*   @brief 停止正在处理的线程。
		*   @note 正在执行的线程会逐步退出
		*/
		bool stop();
	
		/**
		*   @brief 是否要退出线程处理逻辑。
		*   @retval true：需要退出，false；不需要退出
		*   @note 超时或取消时会返回true
		*/
		bool is_need_exit();	
	private:
		ThreadPoolImpl* impl_;
	};
}