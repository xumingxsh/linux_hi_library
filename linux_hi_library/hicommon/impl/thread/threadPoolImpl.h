#pragma once
#include <functional>
#include <vector>

#ifdef LINUX_
#include "pthread.h"
#endif

#ifdef WINDOWS_
#include "windows.h"
#endif

namespace Hi
{
class ThreadPoolImpl
{
public:
	ThreadPoolImpl();
	virtual ~ThreadPoolImpl();
public:
	/**
	*   @brief �������̡߳�
	*   @param[in] threadNum �������߳�����
	*   @param[in] run	�߳�ִ���߼�����
	*   @retval true���ɹ���false��ʧ��
	*/
	bool start(int threadNum, const std::function<void()>& run,
		const std::function<void()>& exitFun);	
		
	/**
	*   @brief ֹͣ���ڴ�����̡߳�
	*   @note ����ִ�е��̻߳����˳�
	*/
	bool stop();
	
	/**
	*   @brief �Ƿ�Ҫ�˳��̴߳����߼���
	*   @retval true����Ҫ�˳���false������Ҫ�˳�
	*   @note ��ʱ��ȡ��ʱ�᷵��true
	*/
	bool is_need_exit();	
private:
	std::vector<pthread_t> threads;	// �̼߳���
	bool is_exit;					// �Ƿ��˳��߳�
	std::function<void()>	exit_fun;
};
}
