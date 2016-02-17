#pragma once

#include <functional>

/**
 * @page  修订历史
 * @{
 * @section 网络通信模块
 *
 * @defgroup  网络通信模块
 * - v0.5\n
 * @li 初始版本，提供监听客户端tcp socket连接功能
 * @}
 * @brief	客户端监听模块  
 * @{
 */
namespace Hi
{
class TcpListenImpl;

/**
* @brief 客户端tcp socket监听类
*		 主要接口为：
*		 1) open/close： 打开监听/关闭监听
*		 2) 客户端连接成功后的回调void(int,SocketChannel&) 
* @note 存在的问题（疑问）：\n
*		1) 接口看上去有点多，光open就有4个，我曾经想把4个接口合成一个，\n
*			这样做其实也很简单，但是那样这一个接口就有点复杂，\n
*			所以没有那样处理\n
*			在下一个版本中，我打算对接口做一下修改：\n
*				添加set_close_fun接口：\n
*					bool set_close_fun(const std::function<void()>&)\n
*				将open减少到一个，样式为：
*					bool open(const std::function<void(int,SocketChannel&)>&\n
*						unsigned short port,const char* ip = NULL);\n
*		2) 最好本类能禁止用户继承，就像Java中的final，C#中的sealed？\n
*			暂时没想到方法实现
*/
class HiTcpListen
{
public:
	HiTcpListen();
	virtual ~HiTcpListen();
	
	// 不允许赋值
	HiTcpListen& operator =(const HiTcpListen&) = delete;
	
	// 不允许拷贝
    HiTcpListen(const HiTcpListen&) = delete;
public:		
	/*
	* @brief 打开监听
	* @param [in] fun accept成功函数
	* @param [in] port 监听端口
	* @param [in] ip 监听IP，如果该参数为空，则监听本机所有IP
	* @retval true：打开成功；false：打开失败
	*/
	bool open(
		const std::function<void(int)>& fun, 
		unsigned short port, 
		const char* ip = NULL);
	
	/*
	* @brief 关闭监听时的回调函数
	* @param [in] close_fun 监听关闭前的回调函数
	* @retval true：成功；false：失败（启动监听之后再设置）
	* @note 启动之前设置有效
	*/
	bool set_close_fun(const std::function<void()>& close_fun);
	
	/*
	* @brief 关闭监听
	* @retval true：成功；false：失败
	*/
	bool close();	
	
	/*
	* @brief 是否正在监听
	*/
	bool is_running();
private:
	TcpListenImpl* impl_;
};
}
/** @}*/ // 客户端监听模块