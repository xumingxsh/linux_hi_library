#include "test.h"
#include "common/hiString.h"
#include "common/hiScopeGuard.h"
#include <iostream>

using namespace std;
static void test_time2string();
static void test_ip2uint();
static void test_uf2uc();
static void test_scope_guard();

void hiString_test()
{
	TEST_FUN("time2string", test_time2string);
	TEST_FUN("ip2uint", test_ip2uint);
	TEST_FUN("u82uc", test_uf2uc);
	TEST_FUN("hiScopeGuard", test_scope_guard);
}

void test_time2string()
{
	string time_str = Hi::time2string();
	cout<<"currect time is:"<<time_str.c_str()<<endl;
	__time32_t t = time(NULL);
	time_str = Hi::time2string(t);
	cout<<"currect time is:"<<time_str.c_str()<<endl;
	
	const char* date = "2008-08-06 09:07:06";
	__time32_t tt = Hi::string2time(date);
	cout<<date<<" time_t is:"<<tt<<endl;
	cout<<tt<<" date is:"<<Hi::time2string(tt)<<endl;
	
	ASSERT_TEST((Hi::time2string(tt) == date, 
		"Hi::time2string or Hi::string2time is error"));
		
	date = "2008-8-6 9:7:6";
	cout<<"want 2008-8-6 9:7:6 time_t is "<<tt<<endl;
	__time32_t ttt = Hi::string2time(date);
	cout<<"2008-8-6 9:7:6 time_t is "<<ttt<<" to string is "<<Hi::time2string(ttt)<<endl;
	ASSERT_TEST((Hi::time2string(ttt) == "2008-08-06 09:07:06", 
		"Hi::string2time is error,can't anlyaze like 2008-8-6 9:7:6"));
		
	ASSERT_TEST((ttt == tt, 
		"Hi::string2time is error,can't anlyaze like 2008-8-6 9:7:6"));
}

void test_ip2uint()
{
	const char* ip = "172.16.5.154";
	unsigned int ipInt = Hi::ip2uint(ip);
	cout<<ip<<" int is:"<<ipInt<<endl;
	cout<<ipInt<<" ip is:"<<Hi::uint2ip(ipInt)<<endl;
	ASSERT_TEST((Hi::uint2ip(ipInt) == ip, "Hi::ip2uint or Hi::uint2ip is error"));
}

void test_uf2uc()
{
	const char* chr = "this is a test";
	try
	{
		wstring wstr = Hi::u82uc(chr);
	} 
	catch(const char* ex)
	{
		cout<<"get a throw by Hi::u82uc::"<<ex<<endl;
	}
}

void test_scope_guard()
{
	{
		cout<<"want print:ON_SCOPE_EXIT leave"<<endl;
		ON_SCOPE_EXIT([&]() {
			cout<<"ON_SCOPE_EXIT leave"<<endl;
		});
	}
	{
		HiScopeGuard guard([&]() {
			cout<<"HiScopeGuard leave"<<endl;
		});
		cout<<"want print:HiScopeGuard leave"<<endl;
	}
	{
		HiScopeGuard guard([&]() {
			cout<<"HiScopeGuard leave"<<endl;
		});
		cout<<"want not print before HiScopeGuard end"<<endl;
		guard.dismissed();
	}
	cout<<"HiScopeGuard end"<<endl;
}