#include "test.h"
#include "collection/hiCondQueue.h"
#include "common/hiString.h"
#include <iostream>
#include <unistd.h>
#ifdef LINUX_
#include "pthread.h"
#endif

using namespace std;
static void test_hiCondQueue();

void hiCondQueue_test()
{
	TEST_FUN("hiCondQueue", test_hiCondQueue);
}

static void* hiCondThread_test(void *arg);
static void* hiCondThread_test2(void *arg);

static  Hi::HiCondQueue<int> myQueue;
static bool s_queue_isExit = false;
void test_hiCondQueue()
{
	static pthread_t t_hiCondQueue1;
	static pthread_t t_hiCondQueue2;
	
	pthread_create(&t_hiCondQueue1, NULL, hiCondThread_test, NULL);
	pthread_create(&t_hiCondQueue2, NULL, hiCondThread_test2, NULL);
		
		
	sleep(30);
	myQueue.quit();
	cout<<"quit my queue"<<endl;
		
	
	pthread_join(t_hiCondQueue1, NULL);
	pthread_join(t_hiCondQueue2, NULL);
}

void* hiCondThread_test(void *arg)
{
	int index = 0;
	while (index < 50)
	{
		if (myQueue.put(index))
		{
			cout<<Hi::time2string()<<" create a value:"<<index<<endl;
		}
		else
		{
			cout<<Hi::time2string()<<" put a value false, because queue is quitted"<<endl;
			break;
		}
		sleep(index % 5 + 2);
		index++;
	}
	
	myQueue.quit();
	s_queue_isExit = true;
	return NULL;
}
void* hiCondThread_test2(void *arg)
{
	while (true)
	{
		int value = 0;
		if (!myQueue.take(value))
		{
			cout<<"my queue is empty and quit"<<endl;
			break;
		}
		
		cout<<Hi::time2string()<<" take a value:"<<value<<endl;
	}
	return NULL;
}