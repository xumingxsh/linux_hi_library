#pragma once

#include <functional>

#include "net/hiByteMsg.h"

/**
 * @page  修订历史
 * @{
 * @section 网络通信模块
 *
 * @defgroup  网络通信模块
 * @}
 * @brief	消息处理模块，主要负责接收和分段二进制消息\n 
 * 			消息格式：
 *			起始标志（1byte），保留字（1byte）,消息长度(2byte)，正文
 * @{
 * - v0.5\n
 * @li 初始版本，提供二进制消息的接收和分段功能
 */
namespace Hi
{
class ByteMsgHandleImpl;
/*
* @ brief 消息处理类
*/
class HiByteMsgHandle final
{
public:
	HiByteMsgHandle(int sock, 
		const std::function<void(HiByteMsg_Ptr)>& fun);
	~HiByteMsgHandle();
	HiByteMsgHandle& operator =(const HiByteMsgHandle&) = delete;
    HiByteMsgHandle(const HiByteMsgHandle&) = delete;
public:
	/*
	* @brief 读取消息，接收到的消息以构造函数第二个参数的函数报出
	*/
	bool read();
private:
	ByteMsgHandleImpl* impl_;
};
}/** @}*/ // 消息处理模块