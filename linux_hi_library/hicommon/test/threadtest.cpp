#include "test.h"
#include "thread/hiThreadPool.h"
#include <iostream>
#include <unistd.h>

using namespace std;
static void test_hiThreadPool();
static void test_pthread_join_self();

void hiThreadPool_test()
{
	//TEST_FUN("hiThreadPool", test_hiThreadPool);
	TEST_FUN("test_pthread_join_self", test_pthread_join_self);
}

static Hi::HiThreadPool pool1;
static Hi::HiThreadPool pool2;

void test_hiThreadPool()
{
	pool1.start(4, [&](){
		pthread_t mySign = pthread_self();
		int index = 0;
		cout<<"pool1 thread:"<<mySign<<" start"<<endl;
		while (!pool1.is_need_exit())
		{
			index++;
			cout<<"pool1 thread:"<<mySign<<" get value:"<<index<<endl;
			usleep(600000);
		}
		cout<<"pool1 thread:"<<mySign<<" finished"<<endl;
		return 0;
	}, [&](){
	});
	pool2.start(4, [&](){
		pthread_t mySign = pthread_self();
		int index = 0;
		cout<<"pool2 thread:"<<mySign<<" start"<<endl;
		while (!pool2.is_need_exit())
		{
			index++;
			cout<<"pool2 thread:"<<mySign<<" get value:"<<index<<endl;
			usleep(800000);
		}
		cout<<"pool2 thread:"<<mySign<<" finished"<<endl;
		return 0;
	}, [&](){
		cout<<"pool2 threads exit"<<endl;
	});
	sleep(3);
	cout<<"call pool1 stop function"<<endl;
	pool1.stop();
	cout<<"call pool2 stop function"<<endl;
	pool2.stop();
}

static void* pthread_join_test(void *arg);
static pthread_t t_pthread_join;
void test_pthread_join_self()
{
	pthread_create(&t_pthread_join, NULL, pthread_join_test, NULL);
	sleep(30);
}

void* pthread_join_test(void *arg)
{
	cout<<"start"<<endl;
	int index = 0;
	while (index < 20)
	{
		cout<<"pthread_join_test:"<<index<<endl;
		sleep(1);
		index++;
	}
	pthread_join(t_pthread_join, NULL);
	cout<<"pthread_join_test exit"<<endl;
	return NULL;
}