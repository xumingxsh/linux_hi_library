#include "common/hiString.h"

#include <stdio.h>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
using namespace std;

#define Hi_String_H_NOT_SUPPORT() \
	stringstream oss;\
	oss<<"function "<<__FUNCTION__<<" not support for linux";\
	string text = oss.str();\
	printf("function %s not support  for linux\n", __FUNCTION__); \
	throw text.c_str()

static bool u82uc(const char* str, wstring& content)
{
#ifdef WINDOWS_
	int textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 ); 
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
	::memset(result,0,(textlen+1)*sizeof(wchar_t)); 
	MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen); 

	content = result;
	free(result);
#endif

#ifdef LINUX_
	Hi_String_H_NOT_SUPPORT();
#endif

	return true; 
}

static bool uc2as(const wchar_t* str, string& content)
{
#ifdef WINDOWS_
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );

	content = result;
	free(result);

#endif

#ifdef LINUX_
	Hi_String_H_NOT_SUPPORT();
#endif
	return true; 
}

static bool u82as(const char* str, string& content)
{
	wstring temp;
	u82uc(str, temp);
	return uc2as(temp.c_str(), content);
}

static bool as2uc(const char* str, wstring& content)
{
#ifdef WINDOWS_
	int textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_ACP, 0, str,-1, NULL,0 ); 
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
	::memset(result,0,(textlen+1)*sizeof(wchar_t)); 
	MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen); 

	content = result;
	free(result);
#endif

#ifdef LINUX_
	Hi_String_H_NOT_SUPPORT();
#endif
	return true; 
}

static bool uc2u8(const wchar_t* str, string& content)
{
#ifdef WINDOWS_
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );

	content = result;
	free(result);
#endif

#ifdef LINUX_
	Hi_String_H_NOT_SUPPORT();
#endif

	return true; 
}

static bool as2u8(const char* str, string& content)
{
	wstring temp;
	as2uc(str, temp);
	return uc2u8(temp.c_str(), content);
}


namespace Hi
{
	HI_COMMON_LIB_API wstring u82uc(const char* str)
	{
		wstring temp;
		if (::u82uc(str, temp))
		{
			return temp;
		}

		return L"";
	}

	HI_COMMON_LIB_API string uc2as(const wchar_t* str)
	{
		string temp;
		if (::uc2as(str, temp))
		{
			return temp;
		}

		return "";
	}

	HI_COMMON_LIB_API string u82as(const char* str)
	{
		string temp;
		if (::u82as(str, temp))
		{
			return temp;
		}

		return "";
	}

	HI_COMMON_LIB_API string as2u8(const char* str)
	{
		string temp;
		if (::as2u8(str, temp))
		{
			return temp;
		}

		return "";
	}

	HI_COMMON_LIB_API wstring as2uc(const char* str)
	{
		wstring temp;
		if (::as2uc(str, temp))
		{
			return temp;
		}

		return L"";
	}

	HI_COMMON_LIB_API string uc2u8(const wchar_t* str)
	{
		string temp;
		if (::uc2u8(str, temp))
		{
			return temp;
		}

		return "";
	}

	struct IPv4
	{
		unsigned char ip3;
		unsigned char ip2;
		unsigned char ip1;
		unsigned char ip0;
	};

	HI_COMMON_LIB_API unsigned int ip2uint(const char* ip)
	{
		IPv4 ipv4;
		memset(&ipv4, 0, sizeof(IPv4));

		string strIP = ip;
		stringstream iss(ip);
		int val = -1;
		iss>>val;
		if (val >= 0)
		{
			ipv4.ip0 = val;
		}
		string::size_type pos = strIP.find('.');
		iss.seekg(pos + 1);
		val = -1;
		iss>>val;
		if (val >= 0)
		{
			ipv4.ip1 = val;
		}
		pos = strIP.find('.', pos + 1);
		iss.seekg(pos + 1);
		val = -1;
		iss>>val;
		if (val >= 0)
		{
			ipv4.ip2 = val;
		}
		pos = strIP.find('.', pos + 1);
		iss.seekg(pos + 1);
		val = -1;
		iss>>val;
		if (val >= 0)
		{
			ipv4.ip3 = val;
		}

		unsigned int ret = 0;
		memcpy(&ret, &ipv4, sizeof(IPv4));
		return ret;
	}

	HI_COMMON_LIB_API string uint2ip(unsigned int ip)
	{
		IPv4 ipv4; 
		memcpy(&ipv4, &ip, sizeof(IPv4));
		stringstream oss;
		oss<<(int)ipv4.ip0<<"."<<(int)ipv4.ip1<<"."<<(int)ipv4.ip2<<"."<<(int)ipv4.ip3;
		return oss.str();
	}

	static bool replace_if_pr_2(char itor, char old)
	{
		if (itor == old)
		{
			return true;
		}
		return false;
	}

	HI_COMMON_LIB_API bool replace(string& source, const char* old, const char* newStr)
	{
		if (::strlen(old) == 1 &&  ::strlen(newStr) == 1)
		{
			replace_if(source.begin(), source.end(), 
				bind2nd(ptr_fun(replace_if_pr_2), *old), *newStr);

			return true;
		}

		string s1(old);
		string s2(newStr);

		if (s1 == s2)
		{
			return true;
		}
		string::size_type pos = source.find(s1);
		while (pos != string::npos)
		{
			source.replace(pos, s1.size(), s2);
			pos = source.find(old);
		}

		return true;
	}

	HI_COMMON_LIB_API void toLower(string& source)
	{
		transform(source.begin(), source.end(), source.begin(), ::tolower);  
	}

	HI_COMMON_LIB_API void toUpper(string& source)
	{
		transform(source.begin(), source.end(), source.begin(), ::toupper);  
	}

	HI_COMMON_LIB_API string toLower(const char* source)
	{
		string dst = source;
		toUpper(dst);  
		return dst.c_str();
	}

	HI_COMMON_LIB_API string toUpper(const char* source)
	{
		string dst = source;
		toUpper(dst);  
		return dst.c_str();
	}

	HI_COMMON_LIB_API void split(const char* ch, const char* token, vector<string>& dest)
	{
		int nend=0;   
		int nbegin=0;   
		string src = ch;
		string temp;
		while(nend != -1)   
		{   
			nend = src.find_first_of(token, nbegin);   
			if(nend == -1)   
			{
				temp = src.substr(nbegin, src.length() - nbegin);
				if (!temp.empty())
				{
					dest.push_back(temp.c_str());  
				}
			}
			else  
			{
				temp = src.substr(nbegin, nend - nbegin);
				dest.push_back(temp.c_str());   
			}
			nbegin = nend + 1;   
		}
	}
	
	HI_COMMON_LIB_API std::string time2string(__time32_t t)
	{		
		if (t == 0)
		{
			t = time(NULL);	
		}
		
		struct tm* st = localtime(&t);
				
		char arr[24];
		memset(arr, 0, sizeof(arr));
		
		strftime(arr, 24, "%2Y-%2m-%2d %2H:%2M:%2S", st);
		return arr;
	}

	HI_COMMON_LIB_API __time32_t string2time(const char* t)
	{
		if (!t || ::strlen(t) > 19 || ::strlen(t) < 14)
		{
			return 0;
		}

		struct tm tm1;  
		__time32_t time1; 

		sscanf(t, "%d-%d-%d %d:%d:%d",      
			&tm1.tm_year,   
			&tm1.tm_mon,   
			&tm1.tm_mday,   
			&tm1.tm_hour,   
			&tm1.tm_min,  
			&tm1.tm_sec);  

		tm1.tm_year -= 1900;  
		tm1.tm_mon --;  


		tm1.tm_isdst=-1;  

#ifdef WINDOWS_
		time1 = _mktime32(&tm1);  
#endif
#ifdef LINUX_
		time1 = mktime(&tm1);  
#endif
		return time1;  
	}
}
