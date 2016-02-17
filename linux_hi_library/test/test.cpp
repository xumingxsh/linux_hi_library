#include <fstream>
#include <iostream>
#include <string.h>
#include <stdarg.h>
#include "test.h"

using namespace std;

// mian函数，意图实现的目标：
// 1: 提供程序的入口
// 2：提供输出流的控制
int main()
{
	ofstream ferr("cerr.log");
	streambuf* buf = cerr.rdbuf(ferr.rdbuf());
	test_main();
	cerr.rdbuf(buf);
	ferr.close();
}

AssertInfo::AssertInfo(const char* file, const char* fun, int line): 
	file_(file), function_(fun), line_(line)
{
}

// 如果断言失败，则向cerr中写入错误日志
bool AssertInfo::log(bool isOK, const char* fmt, ...)
{
	if (!isOK || isOK)
	{
		char chArr[2048];
		memset(chArr, 0 , sizeof(chArr));
		va_list arguments;
		va_start(arguments, fmt);
		vsnprintf(chArr, 2047, fmt, arguments);	
		va_end(arguments) ;
		cerr<<__DATE__<<" "<<__TIME__<<" "<<file_<<
			" "<<function_<<" "<<line_<<" "<<chArr<<endl;
	}
	return isOK;
}

void write_log(const char* fmt, ...)
{
	char chArr[2048];
	memset(chArr, 0 , sizeof(chArr));
	va_list arguments;
	va_start(arguments, fmt);
	vsnprintf(chArr, 2047, fmt, arguments);	
	va_end(arguments);
	cout<<__DATE__<<" "<<__TIME__<<" "<<chArr<<endl;
}