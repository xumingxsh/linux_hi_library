#ifndef HI_LOG_HEAD_H
#define HI_LOG_HEAD_H

#define HI_LOG_INFO(format, ...) \
    hilog_writeInfoLog(__FILE__,__FUNCTION__, __LINE__, format, __VA_ARGS__)

#define HI_LOG_TRACE(format, ...) \
    hilog_writeTraceLog(__FILE__,__FUNCTION__, __LINE__, format, __VA_ARGS__)

#define HI_LOG_DEBUG(format, ...) \
    hilog_writeDebugLog(__FILE__,__FUNCTION__, __LINE__, format, __VA_ARGS__)

#define HI_LOG_CRITICAL(format, ...) \
    hilog_writeCriticalLog(__FILE__,__FUNCTION__, __LINE__, format, __VA_ARGS__)

#define HI_LOG_ERROR(format, ...) \
    hilog_writeErrorLog(__FILE__,__FUNCTION__, __LINE__, format, __VA_ARGS__)

#define HI_LOG_ALWAYS_OUTPUT(format, ...) \
    hilog_writeAlwaysOutputLog(__FILE__,__FUNCTION__, __LINE__, format, __VA_ARGS__)

/*
 *  * @brief The macro of write log.
 *   * @note This macro is not recommanded.
 *    *        It is only used when you log level is not in info/trace/debug/critical/error.
 *    */
#define OMCI_LOG_WRITE(logLevel, format, ...) \
     hilog_writeLog(__FILE__,__FUNCTION__, __LINE__, logLevel, format, __VA_ARGS__)
	
extern "C"
{
/*
 *  * @brief The callback of write log
 *   * @param[in] logLevel log level
 *    * @param[in] log log content
 *    */
typedef void (*Hi_Log_CallBack)(unsigned short log_level, const char* log);

/*
 *  * @brief Set the callback of write log
 *   * @param[in] callback
 *    * @note User need provide the implement.
 *     *       It will add function name,file, line number  and so on Automatically.
 *      *       If user not call this function or callback is NULL, then it will output nothing
 *      */
void set_hi_log_callback(Hi_Log_CallBack callback);

/*
 *  * @brief Set user log level value, it will used by HI_LOG_INFO
 *   * @param[in] level  log level value of info
 *    * @note If this function not called, the HI_LOG_INFO will use zero as it's loglevel
 *    */
void set_hi_info_loglevel(unsigned short level);

/*
 *  * @brief Set user log level value, it will used by HI_LOG_TRACE
 *   * @param[in] level  log level value of trace
 *    * @note If this function not called, the HI_LOG_TRACE will use zero as it's loglevel
 *    */
void set_hi_trace_loglevel(unsigned short level);

/*
 *  * @brief Set user log level value, it will used by HI_LOG_DEBUG
 *   * @param[in] level  log level value of debug
 *    * @note If this function not called, the HI_LOG_DEBUG will use zero as it's loglevel
 *    */
void set_hi_debug_loglevel(unsigned short level);

/*
 *  * @brief Set user log level value, it will used by HI_LOG_CRITICAL
 *   * @param[in] level  log level value of critical
 *    * @note If this function not called, the HI_LOG_CRITICAL will use zero as it's loglevel
 *    */
void set_hi_critical_loglevel(unsigned short level);

/*
 *  * @brief Set user log level value, it will used by HI_LOG_ERROR
 *   * @param[in] level log level value of error
 *    * @note If this function not called, the HI_LOG_ERROR will use zero as it's loglevel
 *    */
void set_hi_error_loglevel(unsigned short level);

/*
 *  * @brief The level which always output.
 *   * @param[in] level Wether always output depended on your implement.
 *    * @note In many cases, Developer using error ot cirtical level to write log,
 *     *       just to output the log to screen or file， Those log not error, also not cirtical.
 *      *         so we provider this level for this.
 *       *       Wether always output, depend on the log which user provider.
 *        *       Default this level is 0, maybe the log not output
 *        */
void set_hi_always_output_loglevel(unsigned short level);


/*
 *  * @brief The callback of log filter
 *   * @param[in] level log level
 *    * @retval true: the log will output in this level; false: the log will not ouput in this level
 *    */
typedef bool (*Hi_Log_Filter_Callback)(unsigned short level);

/*
 *  * @brief Set the callback of log filter
 *   * @param[in] callback
 *    * @note If user not call this function or callback is NULL,
 *     *       all log will output
 *     */
void  set_hi_log_filter_callback(Hi_Log_Filter_Callback callback);


#define HI_MAX_LOG_FILE_NAME_LINE_LENGTH 256

/*
 *  * @brief The log extended information
 *   * @note We want the extended information include file, function, position and so on.
 *   */
typedef struct Hi_Extended_LogInfo
{
    char info[HI_MAX_LOG_FILE_NAME_LINE_LENGTH];
} Hi_Extended_LogInfo;

/*
 *  * @brief The callback of write HI log extended information.
 *   * @param[in] log level May be user want add flag of some level flag in extended information
 *    * @param[in] file
 *     * @param[in] function
 *      * @param[in] line
 *       * @param[in/out] info this parameter is memset to '0' when it inputed
 *        * @note we will set last bit of Hi_Extended_LogInfo.info to '\0'
 *        */
typedef void (*Hi_Extended_LogInfo_Callback)(unsigned short level, const char* file,
                                        const char* function, int line, Hi_Extended_LogInfo& info);

/*
 *  * @brief User can use this function to set his extended information of log.
 *   * @param[in] callback
 *    * @note If callback is NULL, don't add extended information to log.
 *     *       Default is NULL
 *     */
void set_hi_extended_log_info_callback(Hi_Extended_LogInfo_Callback callback);

bool is_using_extended_info();
}

extern "C"
{	
void hilog_writeLog(const char* file, const char* function, int line, unsigned short logLevel, const char* fmt, ...);
void hilog_writeInfoLog(const char* file, const char* function, int line, const char* fmt, ...);
void hilog_writeTraceLog(const char* file, const char* function, int line, const char* fmt, ...);
void hilog_writeDebugLog(const char* file, const char* function, int line, const char* fmt, ...);
void hilog_writeCriticalLog(const char* file, const char* function, int line, const char* fmt, ...);
void hilog_writeErrorLog(const char* file, const char* function, int line, const char* fmt, ...);
void hilog_writeAlwaysOutputLog(const char* file, const char* function, int line, const char* fmt, ...);
}

#endif // HI_LOG_HEAD_H
