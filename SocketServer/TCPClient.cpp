#include "StdAfx.h"
#include "TCPClient.h"


#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

CTCPClient::CTCPClient(void)
{
	// 初始化网络库
	WSADATA wsdata;
	//启动SOCKET库，版本为2.0
	WSAStartup(0x0202, &wsdata);

	m_sClient = INVALID_SOCKET;
	m_nPort = 10060;
	m_strIP = L"127.0.0.1";
}

CTCPClient::~CTCPClient(void)
{
	// 释放网络库
	WSACleanup();

}

bool CTCPClient::Listen(void)
{
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)  
	{
		return false;
	}

	int nTimeOut = 5000;
	//发送时限
	setsockopt(m_sClient, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
	//接收时限
	setsockopt(m_sClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(m_nPort);
	server.sin_addr.s_addr = inet_addr(TCharToANSI(m_strIP.GetString()));

	if (connect(m_sClient, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

void CTCPClient::DisConnect(void)
{
	if (m_sClient != INVALID_SOCKET)
	{
		closesocket(m_sClient);
		m_sClient = INVALID_SOCKET;
	}
	
}

bool CTCPClient::IsOpen(void) const
{
	return m_sClient != INVALID_SOCKET;
}

bool CTCPClient::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// 循环接收
	int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{  
		int nRet = recv(sockfd, &recvbuf[nIndex], nLeft, 0);
		if (nRet == SOCKET_ERROR)
		{
			return false;
		}
		else if (nRet == 0)
		{
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}

	return true;
}

bool CTCPClient::SendAll(SOCKET sockfd, char *sendbuf, int len)
{
	// 循环发送
	unsigned int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{
		int nRet = send(sockfd, &sendbuf[nIndex], nLeft, 0); 
		if (nRet == SOCKET_ERROR)
		{
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}
	return true;
}

bool CTCPClient::SendAndRecvPacket(ZDPacket sZDPSend, ZDPacket &sZDPRecv)
{
	if (!IsOpen())
	{
		return false;
	}

	std::string strSend = sZDPSend.ToString();

	if (SendAll(m_sClient, (char *)strSend.data(), strSend.size()))
	{
		std::string strRecv;
		char szBuf[MAX_PATH] = {0};
		memset(szBuf, 0, MAX_PATH);
		if (RecvAll(m_sClient, szBuf, 2))
		{
			strRecv.append(szBuf, 2);

			memset(szBuf, 0, MAX_PATH);
			if (RecvAll(m_sClient, szBuf, 6))
			{
				strRecv.append(szBuf, 6);
				int nPacketLen = atoi(szBuf);

				if (nPacketLen > 0)
				{
					int nXmlLen = min(max(nPacketLen-9, 1), 999990);
					char *recvBuf = new char[nXmlLen];
					memset(recvBuf, 0, nXmlLen);
					RecvAll(m_sClient, recvBuf, nXmlLen);
					strRecv.append(recvBuf, nXmlLen);
					delete recvBuf;

					memset(szBuf, 0, MAX_PATH);
					RecvAll(m_sClient, szBuf, 1);
					strRecv.append(szBuf, 1);

					if (sZDPRecv.Prase(strRecv))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}