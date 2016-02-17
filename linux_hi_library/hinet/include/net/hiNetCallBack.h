#pragma once
#include <functional>

/**
 * @page  修订历史
 * @{
 * @section 网络通信模块
 *
 * @defgroup  网络通信模块
 * @}
 * @brief	网络通信事件
 * @{
 * - v0.5\n
 * @li 初始版本
 */
namespace Hi
{
/*
* @ brief TCP客户端响应事件
*/
class TcpClientEvt
{
public:	
	typedef std::function<void(bool,int,void*)> OpenFun;
public:
	TcpClientEvt();	
	TcpClientEvt(const OpenFun& on_open,
		const std::function<void(int)>& on_receive,
		const std::function<void(int)>& on_close);
public:
	
	/*
	* @ brief 接收完成事件
	*/
	OpenFun						on_open_;
	
	/*
	* @ brief 接收到数据事件
	*/
	std::function<void(int)> 	on_receive_;
	
	/*
	* @ brief 连接关闭事件
	*/
	std::function<void(int)> 	on_close_;
};
/*
* @ brief TCP服务端响应事件
*/
class TcpEvent
{
public:
	TcpEvent();
public:
	/*
	* @ brief 客户端连接事件
	*/
	std::function<void(int)> on_open_;
	
	/*
	* @ brief 客户端连接关闭事件
	*/
	std::function<void(int)> on_close_;
	
	/*
	* @ brief 接收到客户端数据事件
	*/
	std::function<void(int)> on_receive_;
};
}/** @}*/ // 网络通信事件