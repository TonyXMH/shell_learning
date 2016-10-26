/*************************************************************************
	> File Name: CThread.cpp
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月26日 星期三 13时18分32秒
 ************************************************************************/

#include "CThread.h"
namespace lce
{
	CThread::CThread()
	{
		m_iId = 0;			//线程ID
		m_iPid = 0;			//进程ID
		m_iStop = 0;
	}
	CThread::~CThread()
	{
		detach();
		stop();
	}

	int CThread::start()
	{
		//设置忽略SIGPIPE信号。
		sigset_t signal_mask;
		sigemptyset(&signal_mask);
		sigaddset(&signal_mask, SIGPIPE);
		pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);

		if(pthread_create(&m_iId, 0, CThread::procThread, this) != 0)
		{
			snprintf(m_szErrMsg, sizof(m_szErrMsg), "file: %s, line: %d, errno = %d, error = %s", __FILE__, __LINE__, errno, strerror(errno));
			return -1;
		}
		m_iStop = 0;
		return 0;
	}

	int CThread::detach()	//异步退出不阻塞调用者
	{
		if(m_iId > 0)
			return pthread_detach(m_iId);
	}

	int CThread::join()
	{
		if(m_iId > 0)
			return pthread_join(m_iId);
	}

	int CThread::stop()
	{
		if(m_iStop)
			return 0;
		m_iStop = 1;
		if(m_iId > 0)
			return pthread_cancel(m_iId);
		return 0;
	}
};
