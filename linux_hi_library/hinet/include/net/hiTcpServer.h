#pragma once

#include <functional>

#include "net/hiNetCallBack.h"

/**
 * @page  修订历史
 * @{
 * @section 网络通信模块
 *
 * @defgroup  网络通信模块
 * @}
 * @brief	TCP服务端模块  
 * @{
 * - v0.5\n
 * @li 初始版本，提供TCP服务端功能
 * - v0.6\n
 * @li 将open，receive，close函数抽象为TcpEvent
 * - v0.7\n
 * @li 添加is_running函数，添加拷贝，赋值函数
 * - v0.7\n
 * @li 将3个open函数修改为1个函数
 * - v0.8\n
 * @li 将remote，local地址从TcpEvent的open_fun_中移除
 */
namespace Hi
{
class TcpServerImpl;
/*
* @ brief TCP服务端
*/
class HiTcpServer final
{
public:
	HiTcpServer();
	~HiTcpServer();
	
	// 防止被拷贝和赋值
	HiTcpServer& operator =(const HiTcpServer&) = delete;
    HiTcpServer(const HiTcpServer&) = delete;
public:
	/*
	* @brief 启动监听
	* @param [in] evt 通知对象
	* @param [in] port 监听端口
	* @param [in] ip 监听IP，如果为空时，表示监听本机所有的IP
	* @retval true：成功；false：失败
	*/
	bool open(const TcpEvent& evt, 
		unsigned short port, 
		const char* ip = NULL);
		
	/*
	* @brief 关闭监听
	* @retval true：成功；false：失败
	* @note 监听没有启动时，关闭会失败
	*/
	bool close();
	
	/*
	* @brief 是否正在监听
	*/
	bool is_running();
private:
	TcpServerImpl* impl_;
};
} /** @}*/ // TCP服务端模块