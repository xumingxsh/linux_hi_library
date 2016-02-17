#include "common/hiString.h"
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include "test.h"

using namespace std;

static void test_main();
int main()
{
	ofstream ferr("cerr.log");
	streambuf* buf = cerr.rdbuf(ferr.rdbuf());
	test_main();
	cerr.rdbuf(buf);
	ferr.close();
}

extern void hidb_test();
void test_main()
{
	hidb_test();
}

AssertInfo::AssertInfo(const char* file, const char* fun, int line): 
	file_(file), function_(fun), line_(line)
{
}

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
		cerr<<file_<<" "<<function_<<" "<<line_<<" "<<chArr<<endl;
	}
	return isOK;
}