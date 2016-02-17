#pragma once

#include <functional>

namespace Hi
{
class TcpEPollImpl;

/*
* @brief 使用EPOLL的IO多路复用类
* 		主要方法包括：
*			start： 启动EPOLL
*			finish: 停止EPOLL
*			add：添加监听对象
*			remove： 移除监听对象
* @note 该类类似于ACE中的ACE_Reactor_Impl，作用是完成的是多路事件分离
*		考虑到性能问题，该类暂时未实现定时器功能
*		（定时器功能类似于MFC中的Timer，ACE中的handle_timeout）
*	 @author  徐敏荣
*    @date    2015-01-31
*
* @par 修订历史
*    @version v0.5 \n
*	 @author  徐敏荣
*    @date    2015-01-31
*    @li 初始版本
*    @version v0.6 \n
*	 @author  徐敏荣
*    @date    2015-01-31
*    @li 添加延迟执行函数run_after，目的是为socket connect超时添加，\n
*		 由于类名为HiTcpEPoll，该函数违反了单一职责原则
*/
class HiTcpEPoll
{
public:
	HiTcpEPoll();
	~HiTcpEPoll();
	HiTcpEPoll& operator =(const HiTcpEPoll&) = delete;
    HiTcpEPoll(const HiTcpEPoll&) = delete;
public:
	
	/*
	* @brief 向EPoll中添加一个监听文件
	* @param [in] sock 文件描述符
	* @param [in] events 监听事件
	* @retval true：成功，false；失败，可能监听没有启动
	* @note 允许用户设置events是一个比较鸡肋的方法，虽然给用户一个设置的机会，\n
	*		但是该参数与EPOLL关系密切，要求用户熟悉EPOLL，增加了使用的复杂度
	*/
	bool add(int sock, unsigned int events);
	
	/*
	* @brief 向EPoll中添加一个监听文件
	* @param [in] sock 文件描述符
	* @param [in] on_evt 事件发生时的回调函数
	* @retval true：成功，false；失败，可能监听没有启动
	* @note 添加该方法的最初想法是可以监听\n
	*		HiTcpListen socket，后来考虑到HiTcpListen接口的复杂度，
	* 		而且可能影响性能，在HiTcpListen中暂时没有使用
	*		
	*/
	bool add(int sock, 
		const std::function<void(int, unsigned int)>& on_evt);

	/*
	* @brief 向EPoll中添加一个监听文件
	* @param [in] sock 文件描述符
	* @param [in] events 监听事件
	* @param [in] on_evt 事件发生时的回调函数
	* @retval true：成功，false；失败，可能监听没有启动
	* @note 一个比较全的函数，改函数的存在是为了接口的完整性，\n
	*		events的存在，影响了接口的易用性
	*/
	bool add(int sock, unsigned int events, 
		const std::function<void(int, unsigned int)>& on_evt);
	
	/*
	* @brief 向EPoll中添加一个监听文件
	* @param [in] sock 文件描述符
	* @retval true：成功，false；失败，可能监听没有启动
	* @note 基本接口，默认接收连接的接收，断开消息，设置了EPOLLET标志
	*/
	bool add(int sock);
	
	unsigned short run_delay(unsigned int delay_tm,
		const std::function<void(unsigned short)>& fun);
		
	bool remove_timer(unsigned short id);
	
	/*
	* @brief 移除EPoll中的监听文件
	* @param [in] sock 文件描述符
	* @retval true：成功，false；失败
	*/
	bool remove(int sock);
	
	/*
	* @brief 启动EPoll
	* @retval true：成功，false；失败
	* @note 本方法下，通过add添加相关监听事件
	*/
	bool start();
	
	/*
	* @brief 启动EPoll
	* @param [in] fun 默认的事件回调函数
	* @retval true：成功，false；失败
	*/
	bool start(const std::function<void(int, unsigned int)>& fun);
	
	/*
	* @brief 启动EPoll
	* @param [in] fun 默认的事件回调函数
	* @param [in] on_close EPOLL停止时的回调函数
	* @retval true：成功，false；失败
	*/
	bool start(
		const std::function<void(int, unsigned int)>& on_evt, 
		const std::function<void()>& on_close);	
	
	/*
	* @brief 停止EPoll
	*/
	bool finish();
	
	/*
	* @brief 是否正在运行
	*/
	bool is_running();
private:
	TcpEPollImpl* impl_;
};
}