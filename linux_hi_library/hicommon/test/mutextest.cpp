#include <gtest/gtest.h>
#include "common/hiString.h"
#include "mutex/hiCritical.h"
#include "mutex/hiCondition.h"
#include "mutex/hiSemaphore.h"
#include <iostream>
#include <unistd.h>
#include "pthread.h"

using namespace std;

static int hicritical_val = 0;
static void* hicritical_test(void *arg);
static void* hicritical_test2(void *arg);

static stringstream output_str;
TEST(Hicritical, test_hicritical)
{
    static pthread_t t_hicritical1;
    static pthread_t t_hicritical2;
    static pthread_t t_hicritical3;
    static pthread_t t_hicritical4;

    output_str.str("");
    pthread_create(&t_hicritical1, NULL, hicritical_test, NULL);
    pthread_create(&t_hicritical2, NULL, hicritical_test, NULL);
    pthread_create(&t_hicritical3, NULL, hicritical_test, NULL);
    pthread_create(&t_hicritical4, NULL, hicritical_test, NULL);


    pthread_join(t_hicritical1, NULL);
    pthread_join(t_hicritical2, NULL);
    pthread_join(t_hicritical3, NULL);
    pthread_join(t_hicritical4, NULL);

    vector<string> arr;
    Hi::split(output_str.str().c_str(), "|", arr);
    bool is_in_order = true;
    for (auto str : arr)
    {
        if (str != "1234567890")
        {
            is_in_order = false;
        }
    }
    EXPECT_TRUE(is_in_order);

    hicritical_val = 0;
    output_str.str("");

    pthread_create(&t_hicritical1, NULL, hicritical_test2, NULL);
    pthread_create(&t_hicritical2, NULL, hicritical_test2, NULL);
    pthread_create(&t_hicritical3, NULL, hicritical_test2, NULL);
    pthread_create(&t_hicritical4, NULL, hicritical_test2, NULL);

    pthread_join(t_hicritical1, NULL);
    pthread_join(t_hicritical2, NULL);
    pthread_join(t_hicritical3, NULL);
    pthread_join(t_hicritical4, NULL);

    arr.clear();
    Hi::split(output_str.str().c_str(), "|", arr);
    is_in_order = true;
    for (auto str : arr)
    {
        if (str != "1234567890")
        {
            is_in_order = false;
        }
    }
    EXPECT_FALSE(is_in_order);
}
static Hi::HiCritical hicritical;
static void* hicritical_test(void *arg)
{
    while (hicritical_val < 25)
    {
        hicritical.lock();
        hicritical_val++;
        for (int i = 1; i < 10; i++)
        {
            output_str<<i;
            usleep(20);
        }
        output_str<<0<<"|";
        hicritical.unlock();
    }
    hicritical.lock();
    cout<<endl;
    hicritical.unlock();
    return NULL;
}

static void* hicritical_test2(void *arg)
{
    while (hicritical_val < 25)
    {
        hicritical_val++;
        for (int i = 1; i < 10; i++)
        {
            output_str<<i;
            usleep(20);
        }
        output_str<<0<<"|";
    }
    cout<<endl;
    return NULL;
}

static void* hicondition_product(void* arg);
static void* hicondition_consum(void* arg);
static bool s_condition_isExit = false;
TEST(HiCondition_test,  test_hicondition)
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
        sleep(1);
    }

    pthread_join(t_hicondition1, NULL);
    pthread_join(t_hicondition2, NULL);
    pthread_join(t_hicondition3, NULL);
    pthread_join(t_hicondition4, NULL);
}

static int s_testVal =  0;
static Hi::HiCondition s_condition;
static void* hicondition_product(void* arg)
{
    int index = 0;
    while (index < 10)
    {
        s_condition.lock();
        EXPECT_EQ(s_testVal, 0);
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
static void* hicondition_consum(void* arg)
{
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
        s_testVal--;
        s_condition.unlock();
    }
    return NULL;
}

static Hi::HiSemaphore s_Semaphore(2);
static void* Semaphore_test(void* arg);
TEST(HiSemaphore_test, test_hiSemaphore)
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

    usleep(100);
    s_Semaphore.exit();

    pthread_join(t_Semaphore2, NULL);
    pthread_join(t_Semaphore3, NULL);
    pthread_join(t_Semaphore4, NULL);
}

static void* Semaphore_test(void* arg)
{
    int tid = *static_cast<int*>(arg);
    int index = 0;
    int request_time = 0;
    int want_release_time = 0;
    int release_time = 0;
    int index_count = 5;
    while (index < index_count)
    {
        bool requestResult = false;
        bool isOK = s_Semaphore.request([&]()->bool {
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
            usleep(10);

            s_Semaphore.release([&]()->bool {
                cout<<"tid:"<<tid<<" release a request"<<endl;
                release_time++;
                return true;
            });
        }
    }
    EXPECT_EQ(request_time, index_count);
    EXPECT_EQ(want_release_time, release_time);
    return NULL;
}
