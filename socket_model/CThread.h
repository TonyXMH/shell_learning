/*************************************************************************
	> File Name: CThread.h
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月26日 星期三 11时42分22秒
 ************************************************************************/
#ifndef __NCE_CTHREAD_H
#define __NCE_CTHREAD_H

#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#define MSG_SIZE 1024
namespace lce
{
	class CThread
	{
		public:
			CThread();
			virtual ~CThread();
			int start();
			int stop();
			int detach();
			int join();

			pthread_t getId()
			{
				return m_iId;
			}
			pid_t getPid()
			{
				if(m_iPid == 0)
					m_iPid = syscall(SYS_gettid);
				return m_iPid;
			}
			bool isStopped() 
			{
				return (1 == m_iStop);
			}
			char * getErrMsg()
			{
				return m_szErrMsg;
			}

			virtual int run() = 0;
		private:
			static void * procThread(void * pParam)
			{
				CThread * pCThread = (CThread *)pParam;
				pCThread->run();
				return 0;
			}
		protected:
			char m_szErrMsg[MSG_SIZE];
		private:
			pid_t m_iPid;
			pthread_t m_iId;
			int m_iStop;
	};
};
#endif
