#include "StdAfx.h"
#include "NSSocket.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

CNSSocket::CNSSocket(void)
: m_socket(INVALID_SOCKET)
{
	// ��ʼ��Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

CNSSocket::~CNSSocket(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();
	
	// ж��Winsock
	WSACleanup();
}

bool CNSSocket::Open(LPCTSTR szIP/*=NULL*/, const int nPort/*=NULL*/)
{
	// �ر��Ѵ��ڵĴ��ھ��
	Close();
	Sleep(500);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	int nTimeOut = 5000;
	//����ʱ��
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
	//����ʱ��
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));
	
	if (szIP != NULL)
	{
		m_strIP = szIP;
	}
	if (nPort != NULL)
	{
		m_nPort= nPort;
	}
	CString strLog;
	strLog.Format(L"%s:%d", m_strIP.c_str(), m_nPort);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(m_nPort);
	server.sin_addr.s_addr = inet_addr(TCharToANSI(m_strIP.c_str()));
	if (connect(m_socket, (struct sockaddr*)&server, sizeof(server)) != SOCKET_ERROR)
	{
		CNHLogAPI::WriteLog(L"CNSSocket::Open", L"���ӳɹ�", strLog);
		return true;
	}
	else
	{
		CNHLogAPI::WriteLog(L"CNSSocket::Open", L"����ʧ��", strLog);
		Close();
		return false;
	}
}

// �رմ���
bool CNSSocket::Close(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	
		CNHLogAPI::WriteLog(L"CNSSocket::Close", L"�ر�����", L"");
	}
	return true;
}

bool CNSSocket::IsOpen(void) const
{
	return m_socket != INVALID_SOCKET;
}

bool CNSSocket::SendAndRecvPacket(LPCTSTR szSend, std::wstring &strRecv)
{
	if (!IsOpen())
	{
		return false;
	}

	std::string strSendData = TCharToANSI(szSend);

	std::string strRecvData;
	if (SendAll(m_socket, (char *)strSendData.data(), strSendData.size()))
	{
		CNHLogAPI::WriteLog(L"CNSSocket::SendAndRecvPacket", L"���ͳɹ�", szSend);
		
		char szBuf[MAX_PATH] = {0};
		while (RecvAll(m_socket, szBuf, 1))
		{
			strRecvData.append(szBuf, 1);
			if (strRecvData.find("</Message>") != -1)
			{
				// ���յ�������XML
				break;
			}
			if (strRecvData.size() > 1024*1024)
			{
				// �����������ݳ���
				break;
			}
		}
		
		
		CString strTemp = ANSIToTChar(strRecvData.c_str());
		strTemp.Replace(L"\r\r", L"\r");
		strRecv = (LPCTSTR)strTemp;

		CXmlReader xmlReader;
		if (xmlReader.Parse(strRecv.c_str()))
		{
			CNHLogAPI::WriteLog(L"CNSSocket::SendAndRecvPacket", L"���ճɹ�", strRecv.c_str());
				
			return true;
		}
		else
		{
			CNHLogAPI::WriteLog(L"CNSSocket::SendAndRecvPacket", L"����ʧ��", strRecv.c_str());
				
			return false;
		}
	}
	else
	{
		CNHLogAPI::WriteLog(L"CNSSocket::SendAndRecvPacket", L"����ʧ��", szSend);
	}
	return false;
}

bool CNSSocket::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// ѭ������
	int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{  
		int nRet = recv(sockfd, &recvbuf[nIndex], nLeft, 0);
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLog(L"CNSSocket::RecvAll", L"���ճ�ʱ", L"");
			return false;
		}
		else if (nRet == 0)
		{
			CNHLogAPI::WriteLog(L"CNSSocket::RecvAll", L"������ѶϿ�", L"");
			Close();
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}

	//CString strTemp;
	//for (int i=0; i<len; i++)
	//{
	//	strTemp.AppendFormat(L"%02X ", (unsigned char)recvbuf[i]);
	//}
	//CNHLogAPI::WriteLog(LOG_MSG, L"��������", strTemp);

	return true;
}

bool CNSSocket::SendAll(SOCKET sockfd, char *sendbuf, int len)
{
	//CString strTemp;
	//for (int i=0; i<len; i++)
	//{
	//	strTemp.AppendFormat(L"%02X ", (unsigned char)sendbuf[i]);
	//}
	//CNHLogAPI::WriteLog(LOG_MSG, L"��������", strTemp);

	// ѭ������
	unsigned int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{
		int nRet = send(sockfd, &sendbuf[nIndex], nLeft, 0); 
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLog(L"CNSSocket::SendAll", L"������ѶϿ�", L"");
			Close();
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}
	return true;
}

