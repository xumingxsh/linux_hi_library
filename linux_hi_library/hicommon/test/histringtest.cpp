#include <gtest/gtest.h>
#include "common/hiString.h"
#include "common/hiScopeGuard.h"
#include <iostream>

using namespace std;

TEST(StringTest, test_time2string)
{
    string time_str = Hi::time2string();
    __time32_t t = time(NULL);
    time_str = Hi::time2string(t);

    const char* date = "2008-08-06 09:07:06";
    __time32_t tt = Hi::string2time(date);

    EXPECT_STREQ(Hi::time2string(tt).c_str(), date);

    __time32_t ttt = Hi::string2time(date);
    EXPECT_STREQ(Hi::time2string(ttt).c_str(), "2008-08-06 09:07:06");

    EXPECT_EQ(ttt, tt);
}

TEST(StringTest, test_ip2uint)
{
    const char* ip = "172.16.5.154";
    unsigned int ipInt = Hi::ip2uint(ip);
    EXPECT_STREQ(Hi::uint2ip(ipInt).c_str(), ip);
}

TEST(StringTest, test_uf2u)
{
    EXPECT_NO_THROW(Hi::u82uc("this is a test"));
}

TEST(StringTest, test_scope_guard)
{
    int value = 0;
    {
        ON_SCOPE_EXIT([&]() {
            value = 7;
        });
    }
    EXPECT_EQ(value, 7);
    {
        HiScopeGuard guard([&]() {
            value = 5;
        });
    }
    EXPECT_EQ(value, 5);
    {
        HiScopeGuard guard([&]() {
            value = 3;
        });
        guard.dismissed();
    }
    EXPECT_EQ(value, 5);
}
