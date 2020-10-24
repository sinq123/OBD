#include "StdAfx.h"
#include "SocketNetLibAPI.h"

#include <winsock2.h> 
#pragma comment(lib, "ws2_32.lib")

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

CSocketNetLibAPI::CSocketNetLibAPI(void)
{
	m_bIsUse = true;
	m_nTimeOut = 1000;
	m_strServerIP = L"192.168.38.105";
	m_nPort = 4999;
		
	// 初始化网络库
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
}

CSocketNetLibAPI::~CSocketNetLibAPI(void)
{
	// 卸载网络库
	CNHLogAPI::WriteLog(L"工况通信断开", L"", L"");
	WSACleanup();
}

bool CSocketNetLibAPI::Connect(LPCTSTR lpServerIP, int nPort)
{
	if (!m_bIsUse)
	{
		return false;
	}
	if(NULL != m_sockfd)
	{
		closesocket(m_sockfd);
		m_sockfd=NULL;
	}
	
	m_strServerIP = lpServerIP;
	m_nPort = nPort;
	CNHLogAPI::WriteLog(L"工况通信连接", L"", L"");

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(nPort);
	server.sin_addr.s_addr = inet_addr(TCharToANSI(lpServerIP)) ;
	m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//发送时限
	setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&m_nTimeOut, sizeof(int));
	//接收时限
	setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_nTimeOut, sizeof(int));

	if (connect(m_sockfd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool CSocketNetLibAPI::DisConnect(void)
{
	if (!m_bIsUse)
	{
		return false;
	}

	if (closesocket(m_sockfd) == 0)
	{
		m_sockfd = 0;
		CNHLogAPI::WriteLog(L"工况通信结束", L"", L"");
		return true;
	}
	else
	{
		return false;
	}
}

bool CSocketNetLibAPI::RecvAll(SOCKET sockfd, char *recvbuf, int len)
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

bool CSocketNetLibAPI::SendAll(SOCKET sockfd, char *sendbuf, int len)
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

bool CSocketNetLibAPI::Snap(LPCTSTR szRunningNumber, int nPosition, 
		LPCTSTR szPlateTypeCode, LPCTSTR szPlateNumber, LPCTSTR szVIN, bool bNewCar)
{
	if (!m_bIsUse)
	{
		return false;
	}
	
	if (Connect(m_strServerIP, m_nPort))
	{
		CString strData;
		strData.AppendFormat(L"%s", szRunningNumber);
		strData.AppendFormat(L"@%02d", nPosition);
		strData.AppendFormat(L"@%s", bNewCar?L"空":szPlateTypeCode);
		strData.AppendFormat(L"@%s", bNewCar?L"空":szPlateNumber);
		strData.AppendFormat(L"@%s", szVIN);
	
		std::string strSend = TCharToANSI(strData);

		// 发送数据内容
		if (SendAll(m_sockfd, (char *)strSend.data(), strSend.size()))
		{
			CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap发送成功", strData);

			char szRecvBuf[1024] = {0};
			if (RecvAll(m_sockfd, szRecvBuf, 1024))
			{
				CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap接收", KANSIToUTF16(szRecvBuf));

				// 等500ms再关闭连接
				Sleep(500);
				DisConnect();
				return true;
			}
			else
			{
				CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap接收", L"失败");
			}

			// 因为此抓拍协议不会有返回，所以发送成功就算拍照成功
			return true;
		}
		else
		{
			CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap发送", L"失败");
		}
		DisConnect();
	}
	else
	{
		CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap", L"连接失败");
	}

	return false;
}

bool CSocketNetLibAPI::SendToServer(const char *const pchSend, const int nLen)
{
	if (!m_bIsUse)
	{
		return false;
	}
	CString str;
	//if (Connect(m_strServerIP, m_nPort))
	//{
		// 发送数据内容
		if (SendAll(m_sockfd, (char *)pchSend, nLen))
		{
			//// 平台无返回
			//char szRecvBuf[1024] = {0};
			//if (RecvAll(m_sockfd, szRecvBuf, 1024))
			//{
			//	str.Format(L"IP:%s,Port:%d",m_strServerIP,m_nPort);
			//	CNHLogAPI::WriteLog( L"工况通信成功,接收成功,返回", KANSIToUTF16(szRecvBuf), str);
			//}
			//else
			//{
			//	str.Format(L"接收失败,IP:%s,Port:%d",m_strServerIP,m_nPort);
			//	CNHLogAPI::WriteLog(L"工况通信失败", L"", str);
			//}
			str.Format(L"IP:%s,Port:%d",m_strServerIP,m_nPort);
			//CNHLogAPI::WriteLog( L"工况通信发送成功", str,  KUTF8ToUTF16(pchSend));
			return true;
		}
		else
		{
			str.Format(L"连接失败,IP:%s,Port:%d",m_strServerIP,m_nPort);
			CNHLogAPI::WriteLog(L"工况通信发送失败",str, KUTF8ToUTF16(pchSend));
			Connect(m_strServerIP,m_nPort);
		}

		return false;
	//}
}
