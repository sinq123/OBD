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
		
	// ��ʼ�������
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
}

CSocketNetLibAPI::~CSocketNetLibAPI(void)
{
	// ж�������
	CNHLogAPI::WriteLog(L"����ͨ�ŶϿ�", L"", L"");
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
	CNHLogAPI::WriteLog(L"����ͨ������", L"", L"");

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(nPort);
	server.sin_addr.s_addr = inet_addr(TCharToANSI(lpServerIP)) ;
	m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//����ʱ��
	setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&m_nTimeOut, sizeof(int));
	//����ʱ��
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
		CNHLogAPI::WriteLog(L"����ͨ�Ž���", L"", L"");
		return true;
	}
	else
	{
		return false;
	}
}

bool CSocketNetLibAPI::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// ѭ������
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
	// ѭ������
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
		strData.AppendFormat(L"@%s", bNewCar?L"��":szPlateTypeCode);
		strData.AppendFormat(L"@%s", bNewCar?L"��":szPlateNumber);
		strData.AppendFormat(L"@%s", szVIN);
	
		std::string strSend = TCharToANSI(strData);

		// ������������
		if (SendAll(m_sockfd, (char *)strSend.data(), strSend.size()))
		{
			CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap���ͳɹ�", strData);

			char szRecvBuf[1024] = {0};
			if (RecvAll(m_sockfd, szRecvBuf, 1024))
			{
				CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap����", KANSIToUTF16(szRecvBuf));

				// ��500ms�ٹر�����
				Sleep(500);
				DisConnect();
				return true;
			}
			else
			{
				CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap����", L"ʧ��");
			}

			// ��Ϊ��ץ��Э�鲻���з��أ����Է��ͳɹ��������ճɹ�
			return true;
		}
		else
		{
			CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap����", L"ʧ��");
		}
		DisConnect();
	}
	else
	{
		CNHLogAPI::WriteLog(LOG_MSG, L"CSocketNetLibAPI::Snap", L"����ʧ��");
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
		// ������������
		if (SendAll(m_sockfd, (char *)pchSend, nLen))
		{
			//// ƽ̨�޷���
			//char szRecvBuf[1024] = {0};
			//if (RecvAll(m_sockfd, szRecvBuf, 1024))
			//{
			//	str.Format(L"IP:%s,Port:%d",m_strServerIP,m_nPort);
			//	CNHLogAPI::WriteLog( L"����ͨ�ųɹ�,���ճɹ�,����", KANSIToUTF16(szRecvBuf), str);
			//}
			//else
			//{
			//	str.Format(L"����ʧ��,IP:%s,Port:%d",m_strServerIP,m_nPort);
			//	CNHLogAPI::WriteLog(L"����ͨ��ʧ��", L"", str);
			//}
			str.Format(L"IP:%s,Port:%d",m_strServerIP,m_nPort);
			//CNHLogAPI::WriteLog( L"����ͨ�ŷ��ͳɹ�", str,  KUTF8ToUTF16(pchSend));
			return true;
		}
		else
		{
			str.Format(L"����ʧ��,IP:%s,Port:%d",m_strServerIP,m_nPort);
			CNHLogAPI::WriteLog(L"����ͨ�ŷ���ʧ��",str, KUTF8ToUTF16(pchSend));
			Connect(m_strServerIP,m_nPort);
		}

		return false;
	//}
}
