#include <iostream>
#include <functional>

using namespace std;

static void simple_test();
static void c11_test();
int main()
{
	//simple_test();
	c11_test();
}

/*
 * @brief 测试make文件是否可用的函数
*/
void simple_test()
{
	cout<<"Hi,Shi Jie!"<<endl;
}
static void run_c11_test(const function<void(const char*)>& fun);
void c11_test()
{
	run_c11_test([&](const char* msg){
		cout<<msg<<endl;
	});
}

/*
 * @brief 测试是否支持C++11的函数
*/
void run_c11_test(const function<void(const char*)>& fun)
{
	fun("Hi,Shi Jie!");
}