#include "common/hiString.h"
#include <stdarg.h>
#include "test.h"

using namespace std;

extern void hiString_test();
extern void himetux_test();
extern void hiCondQueue_test();
extern void hiThreadPool_test();
void test_main()
{
	//hiString_test();
	//himetux_test();
	//hiCondQueue_test();
	hiThreadPool_test();
}