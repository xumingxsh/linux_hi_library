#pragma once

#include <functional>

#include "net/hiNetCallBack.h"
#include "net/hiTcpEPoll.h"

/**
 * @page  修订历史
 * @{
 * @section 网络通信模块
 *
 * @defgroup  网络通信模块
 * @}
 * @brief	TCP客户端模块  
 * @{
 * - v0.5\n
 * @li 初始版本
 */
namespace Hi
{
class TcpClientImpl;

/*
* @ brief TCP客户端
*/
class HiTcpClient final
{
public:
	HiTcpClient();
	HiTcpClient(HiTcpEPoll& poll);
	~HiTcpClient();
	HiTcpClient& operator =(const HiTcpClient&) = delete;
    HiTcpClient(const HiTcpClient&) = delete;
public:
	/*
	* @brief 连接服务器端
	* @param [in] ip 服务器端IP
	* @param [in] port 服务器端端口
	* @param [in] evt 客户端事件对象
	* @param [in] time_out_second 连接超时时间
	* @param [in] user_data 用户自定义数据
	* @retval true：成功；false：失败
	*/
	bool connect(const char* ip, 
		unsigned short port, 
		const TcpClientEvt& evt,
		unsigned short time_out_second, 
		void* user_data = NULL);
		
	/*
	* @brief 断开连接
	* @retval true：成功；false：失败
	*/
	bool close();
private:
	TcpClientImpl* impl_;
};
}/** @}*/ // TCP客户端模块