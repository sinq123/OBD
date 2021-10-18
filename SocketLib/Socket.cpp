#include "StdAfx.h"
#include "Socket.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

CSocket::CSocket(void)
: m_socket(INVALID_SOCKET)
{
	// ��ʼ��Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetHLDFilePath(L"LOG", L"", wchLogFileFolderPath, true);

	// ɾ��lDays��ǰ�������죩����־�ļ�
	COleDateTime odtNow(COleDateTime::GetCurrentTime());
	const long lDays(30);
	COleDateTime odtDelLog = odtNow - COleDateTimeSpan(lDays, 0, 0, 0);
	SYSTEMTIME stDelLog;
	ZeroMemory(&stDelLog, sizeof(SYSTEMTIME));
	stDelLog.wYear = (WORD)odtDelLog.GetYear();
	stDelLog.wMonth = (WORD)odtDelLog.GetMonth();
	stDelLog.wDay = (WORD)odtDelLog.GetDay();
	stDelLog.wHour = 23;
	stDelLog.wMinute = 59;
	stDelLog.wSecond = 59;
	CNHLogAPI::DeleteLogEx(wchLogFileFolderPath, stDelLog);

	// ���ɽ�����־�ļ�·��
	// ��������־�ļ�
	CStringW strFileName;
	strFileName.Format(L"%s_Socket����.log", odtNow.Format(L"%Y-%m-%d"));
	m_strLogFilePath = wchLogFileFolderPath;
	m_strLogFilePath += strFileName;
}

CSocket::~CSocket(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();
	
	// ж��Winsock
	WSACleanup();
}

bool CSocket::Open(LPCTSTR szIP/*=NULL*/, const int nPort/*=NULL*/)
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
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::Open", L"���ӳɹ�", strLog);
		return true;
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::Open", L"����ʧ��", strLog);
		Close();
		return false;
	}
}

// �رմ���
bool CSocket::Close(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::Close", L"�ر�����", L"");
	}
	return true;
}

bool CSocket::IsOpen(void) const
{
	return m_socket != INVALID_SOCKET;
}

bool CSocket::SendAndRecvPacket(LPCTSTR szSend, std::wstring &strRecv)
{
	if (!IsOpen())
	{
		return false;
	}

	std::string strSendData = TCharToANSI(szSend);

	std::string strRecvData;
	if (SendAll(m_socket, (char *)strSendData.data(), strSendData.size()))
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"���ͳɹ�", szSend);
		
		char szBuf[MAX_PATH] = {0};
		while (RecvAll(m_socket, szBuf, 1))
		{
			strRecvData.append(szBuf, 1);
			if (strRecvData.find("####") != -1)
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
		std::wstring strData;
		if (strRecvData.empty())
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"����ʧ��", L"NULL");				
			return false;
		}
		else
		{
			strData = ANSIToTChar(strRecvData.c_str());
		
			int nIntPos = strData.find(L"@@@")+3;
			int nTekPos = strData.find(L"tek");
			strRecv = strData.substr(nIntPos, nTekPos-nIntPos);
			//strRecv = strData;
			if (strData.find(L"@@@0") == -1)
			{
				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"���ճɹ�", strData.c_str());				
				return true;
			}
			else
			{
				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"����ʧ��", strData.c_str());				
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"����ʧ��", szSend);
	}
	return false;
}

bool CSocket::RecvPacket(std::wstring &strRecv)
{
	if (!IsOpen())
	{
		return false;
	}
	
	std::string strRecvData;
	char szBuf[MAX_PATH] = {0};
	while (RecvAll(m_socket, szBuf, 1))
	{
		strRecvData.append(szBuf, 1);
		if (strRecvData.find("####") != -1)
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

	std::wstring strData;
	if(strRecvData == "")
	{
	}
	else
	{
		strData = ANSIToTChar(strRecvData.c_str());
	}
	int nIntPos = strData.find(L"@@@")+3;
	int nTekPos = strData.find(L"tek");
	strRecv = strData.substr(nIntPos, nTekPos-nIntPos);

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"���ճɹ�", strData.c_str());
	return true;
}

bool CSocket::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// ѭ������
	int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{  
		int nRet = recv(sockfd, &recvbuf[nIndex], nLeft, 0);
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::RecvAll", L"���ճ�ʱ", L"");
			return false;
		}
		else if (nRet == 0)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::RecvAll", L"������ѶϿ�", L"");
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
	//CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"��������", strTemp);

	return true;
}

bool CSocket::SendAll(SOCKET sockfd, char *sendbuf, int len)
{
	//CString strTemp;
	//for (int i=0; i<len; i++)
	//{
	//	strTemp.AppendFormat(L"%02X ", (unsigned char)sendbuf[i]);
	//}
	//CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"��������", strTemp);

	// ѭ������
	unsigned int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{
		int nRet = send(sockfd, &sendbuf[nIndex], nLeft, 0); 
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAll", L"������ѶϿ�", L"");
			Close();
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}
	return true;
}

