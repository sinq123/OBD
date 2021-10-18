#include "StdAfx.h"
#include "Socket.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

CSocket::CSocket(void)
: m_socket(INVALID_SOCKET)
{
	// 初始化Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 日志文件所在文件夹路径
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetHLDFilePath(L"LOG", L"", wchLogFileFolderPath, true);

	// 删除lDays天前（含当天）的日志文件
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

	// 生成今天日志文件路径
	// 主程序日志文件
	CStringW strFileName;
	strFileName.Format(L"%s_Socket联网.log", odtNow.Format(L"%Y-%m-%d"));
	m_strLogFilePath = wchLogFileFolderPath;
	m_strLogFilePath += strFileName;
}

CSocket::~CSocket(void)
{
	// 对象销毁时,关闭串口资源
	Close();
	
	// 卸载Winsock
	WSACleanup();
}

bool CSocket::Open(LPCTSTR szIP/*=NULL*/, const int nPort/*=NULL*/)
{
	// 关闭已存在的串口句柄
	Close();
	Sleep(500);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	int nTimeOut = 5000;
	//发送时限
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
	//接收时限
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
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::Open", L"连接成功", strLog);
		return true;
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::Open", L"连接失败", strLog);
		Close();
		return false;
	}
}

// 关闭串口
bool CSocket::Close(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::Close", L"关闭连接", L"");
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
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"发送成功", szSend);
		
		char szBuf[MAX_PATH] = {0};
		while (RecvAll(m_socket, szBuf, 1))
		{
			strRecvData.append(szBuf, 1);
			if (strRecvData.find("####") != -1)
			{
				// 接收到完整的XML
				break;
			}
			if (strRecvData.size() > 1024*1024)
			{
				// 超过正常数据长度
				break;
			}
		}
		std::wstring strData;
		if (strRecvData.empty())
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"接收失败", L"NULL");				
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
				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"接收成功", strData.c_str());				
				return true;
			}
			else
			{
				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"接收失败", strData.c_str());				
				return false;
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"发送失败", szSend);
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
			// 接收到完整的XML
			break;
		}
		if (strRecvData.size() > 1024*1024)
		{
			// 超过正常数据长度
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

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAndRecvPacket", L"接收成功", strData.c_str());
	return true;
}

bool CSocket::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// 循环接收
	int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{  
		int nRet = recv(sockfd, &recvbuf[nIndex], nLeft, 0);
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::RecvAll", L"接收超时", L"");
			return false;
		}
		else if (nRet == 0)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::RecvAll", L"服务端已断开", L"");
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
	//CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"联网接收", strTemp);

	return true;
}

bool CSocket::SendAll(SOCKET sockfd, char *sendbuf, int len)
{
	//CString strTemp;
	//for (int i=0; i<len; i++)
	//{
	//	strTemp.AppendFormat(L"%02X ", (unsigned char)sendbuf[i]);
	//}
	//CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"联网发送", strTemp);

	// 循环发送
	unsigned int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{
		int nRet = send(sockfd, &sendbuf[nIndex], nLeft, 0); 
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath, L"CSocket::SendAll", L"服务端已断开", L"");
			Close();
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}
	return true;
}

