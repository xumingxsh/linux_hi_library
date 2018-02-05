#include "common/hiLog.h"
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static Hi_Log_CallBack writeLogCallback = NULL;
static Hi_Log_Filter_Callback filterCallBack = NULL;
static Hi_Extended_LogInfo_Callback extendedInfoCallback = NULL;

const static int MAX_BUF_SIZE = 2048;

static unsigned short logInfoLevel          = 0;
static unsigned short logTraceLevel         = 0;
static unsigned short logDebugLevel         = 0;
static unsigned short logErrorLevel         = 0;
static unsigned short logCriticalLevel      = 0;
static unsigned short logAlwaysOutputLevel  = 0;

static void writeLogWithCallback(unsigned short logType, const char* file,
                                 const char* function, int line, const char* fmt, va_list args);

static bool isSelected(unsigned short level);

void set_hi_log_callback(Hi_Log_CallBack callback)
{
    writeLogCallback = callback;
}

void set_hi_log_filter_callback(Hi_Log_Filter_Callback callback)
{
    filterCallBack = callback;
}

void set_hi_log_extended_info_callback(Hi_Extended_LogInfo_Callback callback)
{
    extendedInfoCallback = callback;
}

void set_hi_loglevel_info(unsigned short level)
{
    logInfoLevel = level;
}

void set_hi_loglevel_debug(unsigned short level)
{
    logDebugLevel = level;
}

void set_hi_loglevel_critical(unsigned short level)
{
    logCriticalLevel = level;
}

void set_hi_loglevel_trace(unsigned short level)
{
    logTraceLevel = level;
}

void set_hi_loglevel_error(unsigned short level)
{
    logErrorLevel = level;
}

void set_hi_loglevel_always_output(unsigned short level)
{
    logAlwaysOutputLevel = level;
}

bool is_using_hi_log_extended_info()
{
    return extendedInfoCallback != NULL;
}

#define WRITE_LOG(level) \
	if (!isSelected(level)) \
    { \
        return;\
    }\
    va_list arguments;\
    va_start(arguments, fmt);\
    writeLogWithCallback(level, file, function, line, fmt, arguments);\
    va_end(arguments)

void hilog_writeLog(const char* file, const char* function, int line, unsigned short logLevel, const char* fmt, ...)
{
	WRITE_LOG(logLevel);
}

void hilog_writeInfoLog(const char* file, const char* function, int line, const char* fmt, ...)
{
	WRITE_LOG(logInfoLevel);
}

void hilog_writeTraceLog(const char* file, const char* function, int line, const char* fmt, ...)
{
	WRITE_LOG(logTraceLevel);
}

void hilog_writeDebugLog(const char* file, const char* function, int line, const char* fmt, ...)
{
	WRITE_LOG(logDebugLevel);
}

void hilog_writeCriticalLog(const char* file, const char* function, int line, const char* fmt, ...)
{
	WRITE_LOG(logCriticalLevel);
}

void hilog_writeErrorLog(const char* file, const char* function, int line, const char* fmt, ...)
{
	WRITE_LOG(logErrorLevel);
}

void hilog_writeAlwaysOutputLog(const char* file, const char* function, int line, const char* fmt, ...)
{
	WRITE_LOG(logAlwaysOutputLevel);
}

static void writeLogWithCallback(unsigned short logLevel, const char* file,
                                 const char* function, int line, const char* fmt, va_list args)
{
    char chArr[MAX_BUF_SIZE];
    memset(chArr, 0, sizeof(chArr));

    vsnprintf(chArr, MAX_BUF_SIZE, fmt, args);


    if (NULL == extendedInfoCallback)
    {
        writeLogCallback(logLevel, chArr);
        return;
    }

    Hi_Extended_LogInfo info;
    memset(&info, 0, sizeof(Hi_Extended_LogInfo));
    extendedInfoCallback(logLevel, file, function, line, info);

    info.info[HI_MAX_LOG_FILE_NAME_LINE_LENGTH - 1] = '\0';

    char item[MAX_BUF_SIZE + HI_MAX_LOG_FILE_NAME_LINE_LENGTH] = {0};

    snprintf(item, sizeof(item), "%s%s", info.info, chArr);
    writeLogCallback(logLevel, item);
}

static bool isSelected(unsigned short level)
{
    if (NULL == writeLogCallback)
    {
        return false;
    }

    if (NULL == filterCallBack)
    {
        return true;
    }
    return filterCallBack(level);
}
