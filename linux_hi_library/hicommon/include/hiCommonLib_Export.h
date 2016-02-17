#pragma once

#ifdef HiCommon_Lib_DLL_EXPORT
#define HI_COMMON_LIB_API
#else
#define HI_COMMON_LIB_API
#endif
#include <time.h> 
typedef time_t __time32_t;