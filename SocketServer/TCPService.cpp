#include "StdAfx.h"
#include "TCPService.h"
#include "Packet.h"

CTCPService::CTCPService(void)
{
	::InitializeCriticalSection(&m_csVS);

	// 初始化网络库
	WSADATA wsdata;
	//启动SOCKET库，版本为2.0
	WSAStartup(0x0202, &wsdata);

	m_sListen = INVALID_SOCKET;
	m_nPort = 10060;
}

CTCPService::~CTCPService(void)
{
	// 释放网络库
	WSACleanup();

	::DeleteCriticalSection(&m_csVS);
}

bool CTCPService::Listen(void)
{
	m_sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if (m_sListen == INVALID_SOCKET)  
	{
		return false;
	}

	sockaddr_in  service;  
	service.sin_family = AF_INET;  
	service.sin_addr.s_addr = INADDR_ANY;  
	service.sin_port = htons(m_nPort);
	if (bind(m_sListen, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) 
	{
		return false;
	}

	if (listen(m_sListen, 5) == SOCKET_ERROR) 
	{
		return false;
	}

	return true;
}

SOCKET CTCPService::Accept(void)
{
	sockaddr_storage clientaddr;
	int addrlen = sizeof(clientaddr);
	SOCKET sClient = accept(m_sListen, (sockaddr*)&clientaddr, &addrlen);
	if (sClient != SOCKET_ERROR)
	{
		m_vtClient.push_back(sClient);
	}
	return sClient;
}

CString CTCPService::GetPeerInfo(SOCKET sockfd)
{
	CString strPeer;

	struct sockaddr_in sa;
	int len = sizeof(sa);
	if (!getpeername(sockfd, (struct sockaddr *)&sa, &len))
	{
		strPeer.Format(L"%s:%d", CStringW(inet_ntoa(sa.sin_addr)), ntohs(sa.sin_port));
	}
	return strPeer;
}

void CTCPService::DisConnect(void)
{
	closesocket(m_sListen);
	
	::EnterCriticalSection(&m_csVS);
	for (UINT i=0; i<m_vtClient.size(); i++)
	{
		closesocket(m_vtClient[i]);
	}
	m_vtClient.clear();
	::LeaveCriticalSection(&m_csVS);
}

void CTCPService::ClostClient(SOCKET sockfd)
{
	::EnterCriticalSection(&m_csVS);
	for (UINT i=0; i<m_vtClient.size(); i++)
	{
		if (sockfd == m_vtClient[i])
		{
			closesocket(m_vtClient[i]);
			m_vtClient.erase(m_vtClient.begin()+i);
			break;
		}
	}
	::LeaveCriticalSection(&m_csVS);
}

bool CTCPService::RecvAll(SOCKET sockfd, char *recvbuf, int len)
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

bool CTCPService::SendAll(SOCKET sockfd, char *sendbuf, int len)
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

bool CTCPService::RecvReqPacket(SOCKET sockfd, ZDPacket &sPacketReq)
{
	std::string strRecv;
	char szBuf[MAX_PATH] = {0};

	memset(szBuf, 0, MAX_PATH);

	if (RecvAll(sockfd, szBuf, 2))
	{
		strRecv.append(szBuf, 2);
		if (memcmp(&sPacketReq.wHeader, szBuf, 2) != 0)	// 快速验证包头
		{
			return false;
		}

		memset(szBuf, 0, MAX_PATH);
		if (RecvAll(sockfd, szBuf, 6))
		{
			strRecv.append(szBuf, 6);
			int nPacketLen = atoi(szBuf);

			if (nPacketLen > 0)
			{
				int nXmlLen = min(max(nPacketLen-9, 1), 999990);

				char *recvBuf = new char[nXmlLen];
				memset(recvBuf, 0, nXmlLen);
				RecvAll(sockfd, recvBuf, nXmlLen);
				strRecv.append(recvBuf, nXmlLen);
				delete recvBuf;

				memset(szBuf, 0, MAX_PATH);
				RecvAll(sockfd, szBuf, 1);
				strRecv.append(szBuf, 1);

				if (sPacketReq.Prase(strRecv))
				{
					return true;
				}
			}
		}
	}
	return false;
}


bool CTCPService::Response(SOCKET sockfd, ZDPacket& sPacketRes)
{
	std::string strSend = sPacketRes.ToString();
	if (SendAll(sockfd, (char *)strSend.data(), strSend.size()))
	{
		return true;
	}
	return false;
}