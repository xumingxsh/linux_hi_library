#pragma once

#include <functional>

/**
 * @page  修订历史
 * @{
 * @section 网络通信模块
 *
 * @defgroup  网络通信模块
 * @}
 * @brief	TCP连接模块  
 * @{
 * - v0.5\n
 * @li 初始版本
 */
namespace Hi
{
class TcpConnectImpl;
class HiTcpEPoll;
/*
* @ brief TCP连接类\n
*		  主要实现添加I/O分发机制，超时处理，连接服务器端操作*		  
* @note 1 调用者可以用已有的IO机制进行分发，或者新创建一个EPOLL进行分发
*		2 该类对调用者来说并不需要，在后续版本中会考虑去除（或隐藏）
*/
class HiTcpConnect final
{
public:
	HiTcpConnect();
	HiTcpConnect(HiTcpEPoll& poll);
	~HiTcpConnect();
	HiTcpConnect& operator =(const HiTcpConnect&) = delete;
    HiTcpConnect(const HiTcpConnect&) = delete;
public:		
	/*
	* @brief 连接服务器端
	* @param [in] ip 服务器端IP
	* @param [in] port 服务器端端口
	* @param [in] fun 连接结果事件
	* @param [in] on_evt IO事件，包括接收到消息和断开连接
	* @param [in] time_out_second 连接超时时间
	* @param [in] user_data 用户自定义数据
	* @retval true：成功；false：失败
	*/
	bool connect(const char* ip, 
		unsigned short port, 
		const std::function<void(bool,int,void*)>& fun,
		const std::function<void(int,unsigned int)>& on_evt,
		unsigned short time_out_second, 
		void* user_data = NULL);
		
	/*
	* @brief 关闭连接
	*/
	bool close();
private:
	TcpConnectImpl* impl_;
};
}/** @}*/ // TCP连接模块