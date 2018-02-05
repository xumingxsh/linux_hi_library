#include <gtest/gtest.h>
#include "collection/hiCondQueue.h"
#include "common/hiString.h"
#include <unistd.h>
#include "pthread.h"

using namespace std;

static void* hiCondThread_test(void *arg);
static void* hiCondThread_test2(void *arg);

static  Hi::HiCondQueue<int> myQueue;
TEST(mutexttest, test_hiCondQueue)
{
    static pthread_t t_hiCondQueue1;
    static pthread_t t_hiCondQueue2;

    pthread_create(&t_hiCondQueue1, NULL, hiCondThread_test, NULL);
    pthread_create(&t_hiCondQueue2, NULL, hiCondThread_test2, NULL);


    pthread_join(t_hiCondQueue1, NULL);
    pthread_join(t_hiCondQueue2, NULL);
}

void* hiCondThread_test(void *arg)
{
    int index = 0;
    while (index < 50)
    {
        myQueue.put(index);
        index++;
    }

    myQueue.quit();

    return NULL;
}
void* hiCondThread_test2(void *arg)
{
    int getValue = -1;
    while (true)
    {
        int value = 0;
        if (!myQueue.take(value))
        {
            getValue = -2;
            break;
        }
        EXPECT_LT(getValue, value);
        getValue = value;

    }
    EXPECT_EQ(getValue, -2);
    return NULL;
}
