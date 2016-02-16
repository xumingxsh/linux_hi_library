#pragma once

/**
*    @defgroup �ַ���������
*    @brief    ��Ҫ����Ը��ֱ����ַ���֮�����ת����ע�⣺���汾��������Ϊdll�ĵ�������ʹ�ã���Ϊ�õ���string����Ҫ��ָMT����ʱ��dll����
*	 @author  ������
*    @date    2012-07-17
*
* @par �޶���ʷ
*    @version v0.5 \n
*	 @author  ������
*    @date    2012-07-17
*    @li ��ʼ�汾
* @{
*/ 

#include <string.h>

#include <string>
#include <sstream>
#include <vector>

#include "hiCommonLib_Export.h"

namespace Hi
{
	/// @fn	std::string time2string(__time32_t t)
	///
	/// @brief	����32λʱ����yyyy-MM-dd HH:mm:ss���ַ���.
	///
	/// @author	Xuminrong
	/// @date	2014-12-25
	///
	/// @param [in]	t	Ҫת����ʱ�䣬����0ʱ��ʾ��õ�ǰʱ��.
	///
	/// @return	yyyy-MM-dd HH:mm:ss���ַ���
	HI_COMMON_LIB_API std::string time2string(__time32_t t = 0);

	HI_COMMON_LIB_API __time32_t string2time(const char* t);
}

namespace Hi
{
	/// @fn	template<class T> string number2string(T value)
	///
	/// @brief	����ת�����ַ���.
	///
	/// @author	Xuminrong
	/// @date	2014-8-22
	///
	/// @exception	FormatException	Thrown when the format of the ? is incorrect.
	///
	/// @typeparam	T	ת��ǰ������.
	/// @param	value	��Ҫת��������.
	///
	/// @return	ת�����ַ���.
	template<class T>
	std::string n2string(T value)
	{
		std::stringstream oss;
		oss << value;
		if (oss.fail())
		{
			return "";
		}
		return oss.str();
	}
	
	template<size_t N>
	void mem_cpy(char (&dest)[N], const std::string source)
	{
		int len = N > source.size() ? source.size() : N;
		::memcpy(dest, source.c_str(), len);
	}

	template<size_t N, size_t L>
	void mem_cpy_arr(char (&dest)[N], const char (&src)[L])
	{
		int len = N > L ? L : N;
		::memcpy(dest, src, len);
	}
}

/**//** @}*/ // �ַ���������