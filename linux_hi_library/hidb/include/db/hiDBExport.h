#pragma once

/**
*    @defgroup 数据库模块
* @{
*/ 

#ifndef HI_DB_EXPORT
	#ifdef HI_DB_DLL_API
		#define HI_DB_EXPORT
	#else
		#define HI_DB_EXPORT
	#endif	
#endif
/**//** @}*/ // 数据库模块