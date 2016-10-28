/*************************************************************************
	> File Name: CEvent.h
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月28日 星期五 18时37分08秒
 ************************************************************************/

#ifndef __NCE_CEVENT_H
#define __NCE_CEVENT_H

#include <sys/epoll.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <queue>
#include <set>
#include <unordered_map>
#include <memory>
#include <functional>
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include "OptTime.h"
using namespace std;

namespace lce
{
	typedef function<void (int, void*)> HandlerEvent;
	const int EPOLL_MAX_EVENT = 1024;
	const int EPOLL_WAIT_TIMEOUT = 3000;
	class CEvent
	{
		public:
			unordered_map<int, uint64_t> MAP_TIME_INDEX;
			enum EventType
			{
				EV_DONE = 0,
				EV_READ = 1,
				EV_WRITE = 2,
			};
			typedef struct 
			{
				int iEPollFd;
				struct epoll_event stEvents[EPOLL_MAX_EVENT];
			}SEPollState;
			typedef struct
			{
				int iEventType;
				HandlerEvent onRead;
				HandlerEvent onWrite;
				void *pClientRData;
				void *pClientWData;
			}SFdEvent;
			typedef struct
			{
				int iTimerId;
				uint64_t ddwMillSecs;
				HandlerEvent onTimer;
				void *pClientData;
			}STimeEvent;
			typedef struct
			{
				int iMsgType;
				HandlerEvent onMessage;
				void *pClientData;
			}SMsgEvent;
			struct cmpTimer
			{
				bool operator()(const STimeEvent &stTimeEvent1, const STimeEvent &stTimeEvent2)
				{
					return stTimeEvent1.ddwMillSecs < stTimeEvent2.ddwMillSecs;
				}
			};
		public:
			CEvent();
			~CEvent();
			int init(uint32_t dwMasFdNum = 10000);
			int addFdEvent(int iWatchFd, int iEventType, const HandlerEvent &onEvent, void *pClientData);
			int delFdEvent(itn iWatchFd, int iEventType);
			int addTimer(int iTimerId, uint32_t dwExpire, const HandlerEvent &onEvent, void *pClientData);
			int delTimer(int iTimerId);
			int addMessage(int iMsgType, const HandlerEvent &onEvent, void *pClientData);

			int run();
			int stop();
			const char *getErrorMsg()
			{
				return m_szErrMsg;
			}
		private:
			static inline uint64_t getMillSecsNow()
			{
				struct timeval stNow;
				gettimeofday(&stNow, NULL);
				return (stNow.tv_sec * 1000 + stNow.tv_usec / 1000);
			}
		private:
			int m_iMsgFd[2];
			SFdEvent *m_stFdEvents;
			SEPollState m_stEPollState;
			queue<SMsgEvent> m_queMsgEvents;
			multiset<StimeEvent, cmpTimer> m_setSTimerEvents;
			MAP_TIME_INDEX m_mapTimeEventIndexs;
			char m_szErrMsg[1024];
			bool m_bRun;
			bool m_bInit;
			uint32_t m_dwMaxFdNum;
			pthread_mutex_t m_lock;
	};
};
#endif
