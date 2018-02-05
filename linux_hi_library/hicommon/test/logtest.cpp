#include <gtest/gtest.h>

#include "common/hiLog.h"

#include <string>
#include <sstream>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#include "common/hiLog.h"

/* @notice There has an issue in '## __VA_ARG__', '##' doesn't work.
 *	   I don't known why. 
 *	   I using 'HI_LOG_INFO("HI_LOG_INFO", "")' 
 *	   instead of 'HI_LOG_INFO("HI_LOG_INFO")'
 */

using namespace std;

static string  log_content = "";

static void writeLog(unsigned short logLevel, const char* log);
static void except_judge(const char* fmt, const char* file, const char* function, int line);
static bool isSelected(unsigned short level);
static void addExtendedLogInfo(unsigned short logLevel, const char* file,
                               const char* function, int line, Hi_Extended_LogInfo& info);

static const char* file_performace = "test.txt";
static void writeLog_test_performace(unsigned short logLevel, const char* log);
static void addExtendedLogInfo__test_performace(unsigned short logLevel, const char* file,
                                                const char* function, int line, Hi_Extended_LogInfo& info);

class LogTest : public testing::Test
{

protected:
    void SetUp()
    {
        set_hi_loglevel_info(1);
        set_hi_loglevel_trace(2);
        set_hi_loglevel_debug(3);
        set_hi_loglevel_error(4);
        set_hi_loglevel_critical(5);
        set_hi_loglevel_always_output(6);

        set_hi_log_callback(writeLog);
    }
    void TearDown()
    {
    }
};

#define EXTENDED_FMT  "[%s::%s::line:%d]"

TEST_F(LogTest, Log_Test)
{
    const char* function = __FUNCTION__;
    const char* file = __FILE__;

    EXPECT_FALSE(is_using_hi_log_extended_info());

    // don't using extnended information in log default
    HI_LOG_INFO("HI_LOG_INFO %s %d", "test", 5);
    int line = __LINE__;
    EXPECT_STREQ("level:1HI_LOG_INFO test 5]", log_content.c_str());

    // add extended information in log
    set_hi_log_extended_info_callback(addExtendedLogInfo);

    EXPECT_TRUE(is_using_hi_log_extended_info());

    HI_LOG_INFO("HI_LOG_INFO", "");
    line = __LINE__;
    except_judge("level:1" EXTENDED_FMT "HI_LOG_INFO]", file, function, line);

    HI_LOG_INFO("HI_LOG_INFO %s %d", "test", 5);
    line = __LINE__;
    except_judge("level:1" EXTENDED_FMT "HI_LOG_INFO test 5]", file, function, line);

    HI_LOG_CRITICAL("HI_LOG_CRITICAL", "");
    line = __LINE__;
    except_judge("level:5" EXTENDED_FMT "HI_LOG_CRITICAL]", file, function, line);


    HI_LOG_CRITICAL("HI_LOG_CRITICAL %s %d", "test", 5);
    line = __LINE__;
    except_judge("level:5" EXTENDED_FMT "HI_LOG_CRITICAL test 5]", file, function, line);

    HI_LOG_WRITE(9, "HI_LOG_WRITE", "");
    line = __LINE__;
    except_judge("level:9" EXTENDED_FMT "HI_LOG_WRITE]", file, function, line);

    HI_LOG_WRITE(9, "HI_LOG_WRITE %s %d", "test", 5);
    line = __LINE__;
    except_judge("level:9" EXTENDED_FMT "HI_LOG_WRITE test 5]", file, function, line);

    HI_LOG_ALWAYS_OUTPUT("HI_LOG_ALWAYS_OUTPUT %s %d", "test", 5);
    line = __LINE__;
    except_judge("level:6" EXTENDED_FMT "HI_LOG_ALWAYS_OUTPUT test 5]", file, function, line);

    // using filter in log
    set_hi_log_filter_callback(isSelected);

    log_content = "";

    HI_LOG_INFO("HI_LOG_INFO %s %d", "test", 5);
    line = __LINE__;
    EXPECT_STREQ(log_content.c_str(), "");

    HI_LOG_CRITICAL("HI_LOG_CRITICAL", "");
    line = __LINE__;
    except_judge("level:5" EXTENDED_FMT "HI_LOG_CRITICAL]", file, function, line);

    // don't using filter in log
    set_hi_log_filter_callback(NULL);

    HI_LOG_INFO("HI_LOG_INFO %s %d", "test", 5);
    line = __LINE__;
    except_judge("level:1" EXTENDED_FMT "HI_LOG_INFO test 5]", file, function, line);

    // don't using extended information
    set_hi_log_extended_info_callback(NULL);

    HI_LOG_INFO("HI_LOG_INFO %s %d", "test", 5);
    EXPECT_STREQ("level:1HI_LOG_INFO test 5]", log_content.c_str());

    HI_LOG_CRITICAL("HI_LOG_CRITICAL", "");
    EXPECT_STREQ("level:5HI_LOG_CRITICAL]", log_content.c_str());
}

// We want run 1w log, the total time < 1s(1000ms)
// Writing log need open file and close file
// Extended information need include time and thread id
TEST_F(LogTest, performance_test)
{
    timeval start;
    timeval end;
    unsigned int u32S;
    unsigned int u32E;

    set_hi_log_callback(writeLog_test_performace);
    set_hi_log_extended_info_callback(addExtendedLogInfo__test_performace);

    FILE* f = fopen(file_performace, "w");
    if (f != NULL)
    {
        fclose(f);
    }

    gettimeofday(&start, NULL);
    u32S = start.tv_sec * 1000 * 1000 + start.tv_usec;

    for (int i = 0; i < 10000; i++)
    {
        HI_LOG_ERROR("OMCI_LOG_ERROR %s %d \n", "test", 5);
    }
    gettimeofday(&end, NULL);
    u32E = end.tv_sec * 1000 * 1000 + end.tv_usec;
    printf("run 1w log:%d (ms), want < 1000(ms)\n", (u32E-u32S) / 1000);
    EXPECT_GT((unsigned int)1000, (u32E-u32S) / 1000);
}

enum E_TEST
{
    E_1 = 1,
    E_2 = 2,
    E_3 = 3,
    E_256 = 26
};

static void printE_TEST(E_TEST v)
{
    printf("E_TEST:%d\n", v);
}
TEST_F(LogTest, enum_test)
{
    unsigned short val = 2;
    printf("value:%d\n", val);
    E_TEST e = (E_TEST)val;
    printE_TEST(e);
    val = 4;
    printf("value:%d\n", val);
    e = (E_TEST)val;
    printE_TEST(e);
}

static void writeLog(unsigned short logLevel, const char* log)
{
    stringstream oss;
    oss << "level:" << logLevel << log << "]";
    log_content = oss.str();
}

static void except_judge(const char* fmt, const char* file, const char* function, int line)
{
    char item[256] = {0};
    snprintf(item, sizeof(item), fmt, file, function, line - 1);
    EXPECT_STREQ(log_content.c_str(), item);
}

static bool isSelected(unsigned short level)
{
    if (level > 3)
    {
        return true;
    }
    return false;
}

static void addExtendedLogInfo(unsigned short logLevel, const char* file,
                               const char* function, int line, Hi_Extended_LogInfo& info)
{
    snprintf(info.info, sizeof(info.info), EXTENDED_FMT, file, function, line);
}

static void writeLog_test_performace(unsigned short logLevel, const char* log)
{
    FILE* f = fopen(file_performace, "a");
    if (f == NULL)
    {
        return;
    }
    fputs(log, f);
    fclose(f);
}

static void addExtendedLogInfo__test_performace(unsigned short logLevel, const char* file,
                                                const char* function, int line, Hi_Extended_LogInfo& info)
{
    timeval curTime;
    gettimeofday(&curTime, NULL);
    unsigned int msec_r  = curTime.tv_sec * 1000 + curTime.tv_usec  / 1000;
    snprintf(info.info, sizeof(info.info), "[@%.2dm:%.2ds:%.3dms][%s][%s][%d][%u]",
             ((msec_r / 1000) / 60), ((msec_r / 1000) % 60), (msec_r % 1000),
             file, function, line, (unsigned int)pthread_self());
}

