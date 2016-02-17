#pragma once
#include <string>

/**
 * @page  修订历史
 * @{
 * @section 测试模块修订历史
 *
 * - v0.5\n
 * @li 初始版本，提供测试的主函数及框架的实现\n
 *		在本版本中，有两个问题未能处理\n
 *		1) 如何隐藏AssertInfo类的结构\n
 *		2) 如何获得所在函数的参数（ASSERT_TEST宏中的问题）\n
 *		3）可变参数大于某个值（2048）时，该如何处理（见write_log的实现）\n 
 *		4）本来打算在这里提供用16进制打印二进制数据的函数，从位置来说，\n
 *		   放置在这里比较合适，从单一职责原则看，放置在这儿比较怪异，所以没有添加
 * @}
 * @note 1：在windows中， 我曾经专门写过一个HiTest的MFC程序来专门处理单元测试\n
 *		 	现在忽然发现，以当前这种方式写测试代码更方便，更直接\n
 *		 2：在Java中有JUnit，C#中有NUnit，不知道C++有没有相似的单元测试工具？\n
 *		 3：对单元测试代码来说，添加这个test.h已经够复杂，为了运行一段测试代码，\n
 *			要引入test.h，的确麻烦了点
 * @defgroup  测试模块
 * @brief	测试模块  
 * @{
 */

/**
* @brief 测试函数启动宏，在启动测试函数前后，添加日志
*/
#define TEST_FUN(name, fun) \
	write_log("start test function:%s", name);\
	write_log("------------------------------------");\
	fun();\
	write_log("------------------------------------");\
	write_log("finished test function:%s",name)
	
/**
* @brief 断言日志类，当出发断言时，会打印相应的错误日志
* @note 一般用于判断测试结果是否与期望一致时使用\n
*		用户不直接使用该类，该类在ASSERT_TEST宏中被使用\n
*		添加该类的目的主要是为能在宏中使用可变参数，该功能由\n
*		ACE中借鉴而来
* @note AssertInfo对用户来说，是一个不需要了解的类，该如何隐藏该类\n
*		到目前为止，我还没有思路
*/
class AssertInfo
{
public:
	AssertInfo(const char* file, const char* fun, int line);
	bool log(bool isOK, const char* fmt, ...);
private:
	std::string file_;
	std::string function_;
	int line_;
};

/**
* @brief 写测试日志
* @note 将写日志提取为一个函数后，可以方便的对给功能进行处理\n
*		例如加锁，例如切换输出位置
*/
void write_log(const char* fmt, ...);

/**
* @brief 测试断言宏，断言测试结果是否正确
* @note 注意使用方式(两个括号)：
*		ASSERT_TEST((true, "this is a %d", "test"))
* @note 该宏存在的目的，除了实现断言外，\n
*		还是为了能够获取file,line和function的宏\n
*		如何获得function的参数，我到现在也还没有思路
*/
#define ASSERT_TEST(x) \
	{\
		AssertInfo(__FILE__,__FUNCTION__, __LINE__).log x;\
	}while(0)

/**
* @brief 将要由用户实现的测试函数，被main函数调用
* @note 我的想法：
*		在本程序中，试图尽量避免使用继承机制实现功能的扩展\n
*		试图上程序看上去更直接
*/
void test_main();

/** @}*/ // 测试模块
	