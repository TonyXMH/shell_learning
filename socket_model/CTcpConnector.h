/*************************************************************************
	> File Name: CTcpConnector.h
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月26日 星期三 19时38分26秒
 ************************************************************************/

#ifndef __TCP_CONNECTOR_H__
#define __TCP_CONNECTOR_H__
#include<string>
#define MSG_SIZE 1024
#define FD_INIT -1
namespace lce
{
	class CTcpConnector
	{
		public:
			CTcpConnector():m_iFd(FD_INIT), m_bConnect(false){}
			~CTcpConnector()
			{
				close();
			}
			bool connect(const std::string &sSvrIp, const short wSvrPort, const time_t dwTimeout = 3000);
			int write(const char *pData, const int iSize, const time_t dwTimeout = 3000);
			int read(char *pBuf, const int iBufSize, const time_t dwTimeout=3000);
			int read(std::string &sBuf, const time_t dwTimeout = 3000);
			int readn(char *pBuf, const int iSize, const time_t dwTimeout=3000);
			void close();
			const char* getErrMsg() const
			{
				return m_szErrMsg;
			}

	
		private:
			boo _imp_connect();
		private://屏蔽复制构造函数和赋值函数
			CTcpConnector(const CTcpConnector &rhs);
			CTcpConnector& operator=(const CTcpConnector &rhs);
		private:
			char m_szErrMsg[MSG_SIZE];
			int m_iFd;
			bool m_bConnect;
			std::string m_sSvrIp;			//服务器IP地址点分十进制
			unsigned short m_wSvrPort;		//端口
			time_t m_dwTimeout;				//超时时间
	};
};
