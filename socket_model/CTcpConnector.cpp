/*************************************************************************
	> File Name: CTcpConnector.cpp
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月27日 星期四 19时27分09秒
 ************************************************************************/

#include "CTcpConnector.h"
#include <sys/types.h>
#include <socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>

namespace lce
{
	bool CTcpConnector::connect(const std::string &sSvrIp, const unsigned short wSvrPort, const time_t dwTimeout)
	{
		m_sSvrIp = sSvrIp;
		m_wSvrPort = wSvrPort;
		m_dwTimeout = dwTimeout;
		return __imp_connect();
	}
	bool CTcpConnector::__imp_connect()
	{
		if(m_iFd != -1)
		{
			close();
		}
		m_iFd = socket(PF_INET, SOCK_STREAM, 0);
		//设置本地IP和端口可以复用
		int reuse = 1;
		setsockopt(m_iFd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(int));
		if(-1 == m_iFd)
		{
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Connect: socket error(%d):%s", errno, strerr(errno));
			return false;
		}

		struct sockaddr_in sockSvrAddr;
		memset(&sockSvrAddr, 0, sizeof(struct sockaddr_in));
		sockSvrAddr.sin_family = AF_INET;
		sockSvrAddr.sin_addr.s_addr = inet_addr(m_sSvrIp.cstr());
		sockSvrAddr.sin_port = htons(m_wSvrPort);

		//设置socket为非阻塞
		int flag;
		flag = fcntl(m_iFd, F_GETFL, 0);
		flag |= O_NONBLOCK;
		fcntl(m_iFd, F_SETFL, flag);

		int ret = -1;
		//由于有重名函数用作用域符限制
		ret = ::connect(m_iFd, (struct sockaddr *)&sockSvrAddr, sizeof(struct sockaddr));
		//非阻塞的connect在返回-1的时候也有可能保持连接。
		if(ret == 0)
		{
			//连接成功将socket设置为阻塞
			flag &= ~O_NONBLOCK;
			fcntl(m_iFd, F_SETFL, flag);
			m_bConnect = true;
			return true;
		}
		if(errno != EINPROGRESS)
		{
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Connect:connect error(%d): %s", errno, strerr(errno));
			close();
			return false;
		}
		else
		{//若errno位为EINPROGRESS,需要用select进行写测试
			struct timeval tv;
			tv.tv_sec = dwTimeout / 1000;		//秒
			tv.tv_usec = (dwTimeout % 1000) * 1000;

			fd_set set, rset;
			FD_ZERO(set);
			FD_ZERO(rset);
			FD_SET(m_iFd, &set);
			FD_SET(m_iFd, &rset);

			ret = select(m_iFd + 1, &rset, &set, NULL, &tv);
			if(ret < 0)
			{
				snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Connect: select error(%d):%s", errno, strerr(errno));
				close();
				return false;
			}
			else if(ret == 0)
			{
				snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Connect: time out");
				close();
				return false;
			}
			else
			{
				socklen_t ilen = sizeof(int);
				int error = -1;
				getsockopt(m_iFd, SOL_SOCKET, SO_ERROR, &error, &ilen);
				if(error == 0)
				{
					flag &= ~O_NONBLOCK;
					fcntl(m_iFd, F_SETFL, flag);
					m_bConnect = true;
					return true;
				}
				else
				{
					snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Connect fail");
					close();
					return false;
				}
			}
		}
		snprintf(m_szErrMsg, sizeof(m_szErrMsg), "connect unprocess error");
		return false;
	}
	
	int CTcpConnector::write(char *pData, const int iSize, const time_t dwTimeout)
	{
		if(!m_bConnect)
			__imp_connect();
		//设置写定时
		struct timeval tv;
		tv.tv_sec = dwTimeout / 1000;
		tv.tv_usec = (dwTimeout % 1000) * 1000;
		setsockopt(m_iFd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizof(tv));
		
		int iSendLen = -1;
		iSendLen = ::send(m_iFd, pData, iSize, 0);
		if(iSendLen < 0)
		{
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Write: send error: %s", strerr(errno));
			close();
			return -1;
		}
		return iSendLen;
	}
	//更安全的读
	int CTcpConnector::readn(char *pBuf, const int iSize, const time_t dwTimeout)
	{
		if(!m_bConnect)
		{
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "no create Connect");
			return -1;
		}
		//设置读超时
		struct timeval tv;
		tv.tv_sec = dwTimeout / 1000;
		tv.tv_usec = (dwTimeout % 1000) * 1000;
		setsockopt(m_iFd, SOL_SOCKET, SO_REVTIMEO, &tv, sizeof(tv));
		//保证完整的读取iSize个字节
		int iReadSize = 0;
		while(iReadSize < iSize)
		{
			int n = ::recv(m_iFd, pBuf + iReadSize, iSize - iReadSize, 0);
			if(n < 0)
			{
				snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Read: rev error(%d): %s", errno, strerr(errno));
				close();
				return -1;
			}
			else
			{
				if(0 == n)				//读到EOF
				{
					this->close();
					return iReadSize;
				}
				iReadSize += n;
			}
		}
		return iReadSize;
	}

	int CTcpConnector::read(char *pBuf, const int iSize, const time_t dwTimeout)
	{
		if(!m_szErrMsg)
		{
			snprintf(m_szErrMsg, sizeof(m_szErrMSg), "no create connect");
			return -1;
		}

		struct timeval tv;
		tv.tv_sec = dwTimeout / 1000;
		tv.tv_usec = (dwTimeout % 1000) * 1000;
		setsockopt(m_iFd, SOL_SOCKET, SO_REVTIMEO, &tv, sizeof(tv));

		int iReadSize = ::recv(m_iFd, pBuf, iSize, 0);
		if(iReadSize < 0)
		{
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "read: recv error(%d): %s", errno, strerr(errno));
			colse();
			return -1;
		}
		else
		{
			if(0 == iReadSize)
			{
				snprintf(m_szErrMsg, sizeof(m_szErrMsg), "read: recv error(%d): %s", errno, strerr(errno));
				this->close();
			}

		}
		return iReadSize;

	}
	void CTcpConnector::close()
	{
		::close(m_iFd);
		m_iFd = -1;
		m_bConnect = false;
	}

}

