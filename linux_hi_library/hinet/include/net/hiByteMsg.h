#pragma once
#include <functional>
#include <memory>

/**
 * @page  修订历史
 * @{
 * @section 网络通信模块
 *
 * @defgroup  网络通信模块
 * @}
 * @brief	二进制消息\n 
 * 			消息格式：
 *			起始标志（1byte），保留字（1byte）,消息长度(2byte)，正文
 * @{
 * - v0.5\n
 * @li 初始版本
 */
namespace Hi
{
/*
* @ brief 消息起始标志
*/
const char HiByteMSG_Begin = 0x68;

/*
* @ brief 消息头长度
*/
const unsigned short HiByteMSG_Head_Len = 4;

/*
* @ brief 消息最大长度
*/
const unsigned short HiByteMSG_Max_Body_Len = 1024;
class HiByteMsgImpl;

/*
* @ brief 二进制消息类
*/
class HiByteMsg final
{
public:
	HiByteMsg();
	
	/*
	* @ brief 构造函数
	* @note 主要用于对传入指针做特殊处理的情况，
	*		主要是HiByteMsgHandle使用，普通用户可忽略
	*/
	HiByteMsg(void* data, unsigned short len, 
		const std::function<void(void*)>& free_fun);
		
	~HiByteMsg();
	HiByteMsg& operator =(const HiByteMsg&) = delete;
    HiByteMsg(const HiByteMsg&) = delete;
public:
	/*
	* @ brief 读取消息中的正文
	*/
	void* data();
	
	/*
	* @ brief 获得消息正文长度
	*/
	unsigned short len();
private:
	HiByteMsgImpl* impl_;
};
typedef std::shared_ptr<Hi::HiByteMsg> HiByteMsg_Ptr;
}/** @}*/ // 二进制消息