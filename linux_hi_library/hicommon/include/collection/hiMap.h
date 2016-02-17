#pragma once

#include <map>
#include <functional>

#include "mutex/hiCritical.h"

namespace Hi
{
	template<typename K, typename T>
	class HiMap
	{
	public:
		typedef std::map<K, T> Map;
	public:
		bool update(K k, const T& t)
		{
			lock.lock();
			myMap[k] = t;
			lock.unlock();
			return true;
		}

		bool insert(K k, const T& t)
		{
			lock.lock();
			auto it = myMap.find(k);
			if (it != myMap.end())
			{
				lock.unlock();
				return false;
			}
			myMap.insert(std::pair<K, T>(k, t));			
			lock.unlock();
			return true;
		}		

		bool insert_val(K k, T t)
		{
			lock.lock();
			auto it = myMap.find(k);
			if (it != myMap.end())
			{
				lock.unlock();
				return false;
			}
			myMap.insert(std::pair<K, T>(k, t));	
			lock.unlock();
			return true;
		}		

		bool erase(K k)
		{
			lock.lock();
			auto it = myMap.find(k);
			if (it == myMap.end())
			{
				lock.unlock();
				return false;
			}
			myMap.erase(it);
			lock.unlock();
			return true;
		}		

		bool erase(K k, T& t)
		{
			lock.lock();
			auto it = myMap.find(k);
			if (it == myMap.end())
			{
				lock.unlock();
				return false;
			}
			t = it->second;
			myMap.erase(it);
			lock.unlock();
			return true;
		}

		bool erase(const std::function<bool(T&)>& fun)
		{
			HiCriticalMng mng(&lock);
			for (auto it = myMap.begin(); it != myMap.end(); ++it)
			{
				if (fun(it->second))
				{
					myMap.erase(it);
					return true;
				}
			}
			return false;
		}

		bool find(K k, T& t)
		{
			lock.lock();
			auto it = myMap.find(k);
			if (it == myMap.end())
			{
				lock.unlock();
				return false;
			}
			t = it->second;
			lock.unlock();
			return true;
		}
		
		bool exist(const std::function<bool(T&)>& fun)
		{
			HiCriticalMng mng(&lock);
			for (auto it = myMap.begin(); it != myMap.end(); ++it)
			{
				if (fun(it->second))
				{
					return true;
				}
			}
			return false;
		}

		bool exist(K k)
		{
			lock.lock();
			auto it = myMap.find(k);
			if (it == myMap.end())
			{
				lock.unlock();
				return false;
			}
			lock.unlock();
			return true;
		}

		bool find(T& t, const std::function<bool(T&)>& fun)
		{
			lock.lock();
			for (auto it = myMap.begin(); it != myMap.end(); ++it)
			{
				if (fun(it->second))
				{
					t = it->second;
					lock.unlock();
					return true;
				}
			}
			lock.unlock();
			return false;
		}

		void clear()
		{
			lock.lock();
			myMap.clear();
			lock.unlock();
		}

		void clear(const std::function<void(T&)>& fun)
		{
			HiCriticalMng mng(&lock);
			for (auto it = myMap.begin(); it != myMap.end(); ++it)
			{
				fun(it->second);
				myMap.erase(it);
			}
		}

		void functionR(const std::function<void(const Map&)>& fun)
		{
			HiCriticalMng mng(&lock);
			fun(myMap);
		}
		void functionW(std::function<void(Map&)> fun)
		{
			HiCriticalMng mng(&lock);
			fun(myMap);
		}

		bool findDo(K k, const std::function<bool(T&)>& fun)
		{
			HiCriticalMng mng(&lock);
			auto it = myMap.find(k);
			if (it == myMap.end())
			{
				return false;
			}
			return fun(it->second);
		}
		void findDoNoReturn(K k, const std::function<void(T&)>& fun)
		{
			HiCriticalMng mng(&lock);
			auto it = myMap.find(k);
			if (it == myMap.end())
			{
				return;
			}
			fun(it->second);
		}

		void foreach(const std::function<void(T&)>& fun)
		{
			lock.lock();
			for (auto it = myMap.begin(); it != myMap.end(); ++it)
			{
				fun(it->second);
			}
			lock.unlock();
		}

		bool empty()
		{
			lock.lock();
			bool result = myMap.empty();
			lock.unlock();
			return result;
		}

		unsigned short size()
		{
			lock.lock();
			unsigned short size = (unsigned short)myMap.size();
			lock.unlock();
			return size;
		}
	private:
		Map myMap;
		HiCritical lock;	
	};
}