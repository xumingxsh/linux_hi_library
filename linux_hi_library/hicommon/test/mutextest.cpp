#include "test.h"
#include "common/hiString.h"
#include "mutex/hiCritical.h"
#include "mutex/hiCondition.h"
#include "mutex/hiSemaphore.h"
#include <iostream>
#include <unistd.h>
#ifdef LINUX_
#include "pthread.h"
#endif

using namespace std;
static void test_hicritical();
static void test_hicondition();
static void test_hiSemaphore();

void himetux_test()
{
	TEST_FUN("hiCritical", test_hicritical);
	TEST_FUN("hiCondition", test_hicondition);
	TEST_FUN("hiSemaphore", test_hiSemaphore);
}

static int hicritical_val = 0;
void* hicritical_test(void *arg);
void* hicritical_test2(void *arg);
void test_hicritical()
{
	static pthread_t t_hicritical1;
	static pthread_t t_hicritical2;
	static pthread_t t_hicritical3;
	static pthread_t t_hicritical4;
	
	cout<<"want 1234567890 in order"<<endl;
	pthread_create(&t_hicritical1, NULL, hicritical_test, NULL);
	pthread_create(&t_hicritical2, NULL, hicritical_test, NULL);
	pthread_create(&t_hicritical3, NULL, hicritical_test, NULL);
	pthread_create(&t_hicritical4, NULL, hicritical_test, NULL);
	
	sleep(3);
	
	pthread_join(t_hicritical1, NULL);
	pthread_join(t_hicritical2, NULL);
	pthread_join(t_hicritical3, NULL);
	pthread_join(t_hicritical4, NULL);
	
	hicritical_val = 0;
	
	cout<<"want 1234567890 not in order"<<endl;
	
	pthread_create(&t_hicritical1, NULL, hicritical_test2, NULL);
	pthread_create(&t_hicritical2, NULL, hicritical_test2, NULL);
	pthread_create(&t_hicritical3, NULL, hicritical_test2, NULL);
	pthread_create(&t_hicritical4, NULL, hicritical_test2, NULL);
	
	sleep(3);
	
	pthread_join(t_hicritical1, NULL);
	pthread_join(t_hicritical2, NULL);
	pthread_join(t_hicritical3, NULL);
	pthread_join(t_hicritical4, NULL);
}
static void* hicondition_product(void* arg);
static void* hicondition_consum(void* arg);
static bool s_condition_isExit = false;
void test_hicondition()
{
	static pthread_t t_hicondition1;
	static pthread_t t_hicondition2;
	static pthread_t t_hicondition3;
	static pthread_t t_hicondition4;
	
	pthread_create(&t_hicondition1, NULL, hicondition_product, NULL);
	
	static int t2 = 2;
	static int t3 = 3;
	static int t4 = 4;
	pthread_create(&t_hicondition2, NULL, hicondition_consum, &t2);
	pthread_create(&t_hicondition3, NULL, hicondition_consum, &t3);
	pthread_create(&t_hicondition4, NULL, hicondition_consum, &t4);
	
	while (!s_condition_isExit)
	{
		sleep(3);
	}
	
	pthread_join(t_hicondition1, NULL);
	pthread_join(t_hicondition2, NULL);
	pthread_join(t_hicondition3, NULL);
	pthread_join(t_hicondition4, NULL);
}

static Hi::HiCritical hicritical;
void* hicritical_test(void *arg)
{
	while (hicritical_val < 25)
	{
		hicritical.lock();
		hicritical_val++;
		for (int i = 1; i < 10; i++)
		{
			cout<<i;
			usleep(20);
		}
		cout<<0<<"|";
		hicritical.unlock();	
	}
	hicritical.lock();
	cout<<endl;
	hicritical.unlock();	
	return NULL;
}
void* hicritical_test2(void *arg)
{
	while (hicritical_val < 25)
	{
		hicritical_val++;
		for (int i = 1; i < 10; i++)
		{
			cout<<i;
			usleep(20);
		}
		cout<<0<<"|";
	}
	cout<<endl;
	return NULL;
}
static int s_testVal =  0;
static Hi::HiCondition s_condition;
void* hicondition_product(void* arg)
{
	int index = 0;
	while (index < 3)
	{
		s_condition.lock();
		if (s_testVal != 0)
		{
			cout<<"error "<<endl;
		}
		cout<<"produce thread create a number"<<endl;
		s_testVal++;
		s_condition.notify();
		s_condition.unlock();
			
		sleep(1);
		index++;
	}
	s_condition_isExit = true;
	s_condition.notifyAll();
	return NULL;
}
void* hicondition_consum(void* arg)
{
	int tid = *static_cast<int*>(arg);
	while (!s_condition_isExit)
	{
		s_condition.lock();
		while (s_testVal == 0)
		{
			s_condition.wait();
			if (s_condition_isExit)
			{
				s_condition.unlock();
				return NULL;
			}
		}
		cout<<"tid:"<<tid<<" consumer this number, s_testVal:"<<s_testVal<<endl;
		s_testVal--;
		s_condition.unlock();
	}
	return NULL;
}

static Hi::HiSemaphore s_Semaphore(2);
static void* Semaphore_test(void* arg);
void test_hiSemaphore()
{
	static int t2 = 2;
	static int t3 = 3;
	static int t4 = 4;
	
	static pthread_t t_Semaphore2;
	static pthread_t t_Semaphore3;
	static pthread_t t_Semaphore4;
		
	pthread_create(&t_Semaphore2, NULL, Semaphore_test, &t2);
	pthread_create(&t_Semaphore3, NULL, Semaphore_test, &t3);
	pthread_create(&t_Semaphore4, NULL, Semaphore_test, &t4);
		
	sleep(5);
	cout<<"exit Semaphore"<<endl;
	s_Semaphore.exit();
	
	pthread_join(t_Semaphore2, NULL);
	pthread_join(t_Semaphore3, NULL);
	pthread_join(t_Semaphore4, NULL);
}

void* Semaphore_test(void* arg)
{
	int tid = *static_cast<int*>(arg);
	cout<<"start thread:"<<tid<<endl;
	int index = 0;
	int request_time = 0;
	int want_release_time = 0;
	int release_time = 0;
	int index_count = 5;
	while (index < index_count)
	{
		bool requestResult = false;
		bool isOK = s_Semaphore.request([&]()->bool{
			request_time++;
			if (index % 2 == 0)
			{
				requestResult = true;
				want_release_time++;
			}
			cout<<Hi::time2string()<<" tid:"<<tid<<" get a request,result:"<<requestResult<<endl;
			return requestResult;
		});
		cout<<Hi::time2string()<<" tid:"<<tid<<" request result:"<<isOK<<endl;
		index++;
				
		if (requestResult)
		{
			sleep(tid);
			
			s_Semaphore.release([&]()->bool{
				cout<<"tid:"<<tid<<" release a request"<<endl;
				release_time++;
				return true;
			});
		}
	}
	cout<<"want request time "<<index_count<<",real:"<<request_time<<endl;
	ASSERT_TEST((request_time == index_count, "Hi::HiSemaphore is error"));
	cout<<"want release time "<<want_release_time<<",real:"<<release_time<<endl;
	ASSERT_TEST((want_release_time == release_time, "Hi::HiSemaphore is error"));
	return NULL;
}
