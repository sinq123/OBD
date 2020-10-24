#include "StdAfx.h"
#include "NanhuaSocketPara.h"

#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>


EPPacket::EPPacket()
{
	EPPacket(L"");
}

EPPacket::EPPacket(LPCTSTR szXML)
{
	strXML = szXML;

	wHeader = MAKEWORD('E', 'P');
	dwPacketLen = 0x00;
	bTail = '$';
}

std::string EPPacket::ToString()
{
	std::string strData = TCharToUTF8(strXML.c_str());
	dwPacketLen = sizeof(wHeader) + sizeof(dwPacketLen) + strData.size() + sizeof(bTail);

	std::string strRet;
	strRet.append((char *)&wHeader, sizeof(wHeader));
	strRet.append((char *)&dwPacketLen, sizeof(dwPacketLen));	
	strRet.append(strData);
	strRet.append((char *)&bTail, sizeof(bTail));

	return strRet;
}

bool EPPacket::Prase(std::string strRecv)
{
	int nMinPacketSize = sizeof(wHeader)+sizeof(dwPacketLen)+sizeof(bTail);
	if (strRecv.size() < nMinPacketSize)
	{
		return false;
	}

	int nOffset = 0;
	memcpy(&wHeader, strRecv.data()+nOffset, sizeof(wHeader));
	nOffset += sizeof(wHeader);
	// 校验包尾
	if (wHeader != MAKEWORD('E', 'P'))
	{
		return false;
	}
		
	memcpy(&dwPacketLen, strRecv.data()+nOffset, sizeof(dwPacketLen));
	nOffset += sizeof(dwPacketLen);	
	// 校验数据长度
	if (dwPacketLen != strRecv.size())
	{
		return false;
	}
		
	std::string strData;
	strData.assign(strRecv.data()+nOffset, dwPacketLen-nMinPacketSize);
	nOffset += dwPacketLen-nMinPacketSize;

	memcpy(&bTail, strRecv.data()+nOffset, sizeof(bTail));
	nOffset += sizeof(bTail);
		
	// 校验包尾
	if (bTail != '$')
	{
		return false;
	}

	strXML = UTF8ToTChar(strData.c_str());

	return true;
}

CNanhuaSocketPara::CNanhuaSocketPara(void)
: m_socket(INVALID_SOCKET)
{
	::InitializeCriticalSection(&m_sCriticalSection);

	// 初始化Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 模块名称
	m_strModName = L"NanhuaSocketPara";

	// 生成日志文件
	GenLogFile();

}

CNanhuaSocketPara::~CNanhuaSocketPara(void)
{
	// 对象销毁时,关闭串口资源
	Close();
	
	// 卸载Winsock
	WSACleanup();

	::DeleteCriticalSection(&m_sCriticalSection);
}

DWORD CNanhuaSocketPara::Open(std::wstring strIP, const int nPort)
{
	assert(0 != nPort);

	// 关闭已存在的串口句柄
	Close();
	Sleep(1000);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	int nTimeOut = 3000;
	//发送时限
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
	//接收时限
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));
	
	CString strLog;
	strLog.Format(L"%s:%d", strIP.c_str(), nPort);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(nPort);
	server.sin_addr.s_addr = inet_addr(TCharToANSI(strIP.c_str()));
	if (connect(m_socket, (struct sockaddr*)&server, sizeof(server)) != SOCKET_ERROR)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::Open", L"连接成功", strLog);
		return PARA_OPEN_SUCCEED;
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::Open", L"连接失败", strLog);
		Close();
		return PARA_OPEN_FAIL;
	}
}

// 关闭串口
DWORD CNanhuaSocketPara::Close(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::Close", L"关闭连接", L"");
	}
	return PARA_CLOSE_SUCCEED;
}

bool CNanhuaSocketPara::IsOpen(void) const
{
	return m_socket != INVALID_SOCKET;
}

bool CNanhuaSocketPara::SendAndRecvPacket(EPPacket sSend, EPPacket &sRecv)
{
	if (!IsOpen())
	{
		return false;
	}
	
	// 进入临界区域
	::EnterCriticalSection(&m_sCriticalSection);

	std::string strSend = sSend.ToString();

	std::string strRecv;
	if (SendAll(m_socket, (char *)strSend.data(), strSend.size()))
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAndRecvPacket", L"发送成功", sSend.strXML.c_str());
		char szBuf[MAX_PATH] = {0};

		memset(szBuf, 0, MAX_PATH);
		if (RecvAll(m_socket, szBuf, 2))
		{
			strRecv.append(szBuf, 2);

			memset(szBuf, 0, MAX_PATH);
			if (RecvAll(m_socket, szBuf, 4))
			{
				strRecv.append(szBuf, 4);
				DWORD dwPacketLen = 0x00;
				memcpy(&dwPacketLen, szBuf, 4);

				if (dwPacketLen > 0)
				{
					int nXmlLen = min(max(dwPacketLen-7, 1), 999990);	// 限制包大小，1M左右
					char *recvBuf = new char[nXmlLen];
					memset(recvBuf, 0, nXmlLen);
					RecvAll(m_socket, recvBuf, nXmlLen);
					strRecv.append(recvBuf, nXmlLen);
					delete recvBuf;

					memset(szBuf, 0, MAX_PATH);
					RecvAll(m_socket, szBuf, 1);
					strRecv.append(szBuf, 1);

					if (sRecv.Prase(strRecv))
					{
						CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAndRecvPacket", L"接收", sRecv.strXML.c_str());
				
						::LeaveCriticalSection(&m_sCriticalSection);
						return true;
					}
				}
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAndRecvPacket", L"发送失败", sSend.strXML.c_str());
	}
	::LeaveCriticalSection(&m_sCriticalSection);
	return false;
}

bool CNanhuaSocketPara::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// 循环接收
	int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{  
		int nRet = recv(sockfd, &recvbuf[nIndex], nLeft, 0);
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::RecvAll", L"接收超时", L"");
			return false;
		}
		else if (nRet == 0)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::RecvAll", L"服务端已断开", L"");
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
	//CNHLogAPI::WriteLogEx(m_strLogFilePath,LOG_MSG, L"联网接收", strTemp);

	return true;
}

bool CNanhuaSocketPara::SendAll(SOCKET sockfd, char *sendbuf, int len)
{
	//CString strTemp;
	//for (int i=0; i<len; i++)
	//{
	//	strTemp.AppendFormat(L"%02X ", (unsigned char)sendbuf[i]);
	//}
	//CNHLogAPI::WriteLogEx(m_strLogFilePath,LOG_MSG, L"联网发送", strTemp);

	// 循环发送
	unsigned int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{
		int nRet = send(sockfd, &sendbuf[nIndex], nLeft, 0); 
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAll", L"服务端已断开", L"");
			Close();
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}
	return true;
}

bool CNanhuaSocketPara::Xml2Map(LPCTSTR szXML,  
		std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, 
		std::map<std::wstring, std::wstring> &mapData)
{
	CXmlReader xmlReader;
	if (xmlReader.Parse(szXML))
	{
		if (xmlReader.OpenNode(L"response/cmd"))
		{
			xmlReader.GetNodeContent(strCmd);
		}
		if (xmlReader.OpenNode(L"response/code"))
		{
			xmlReader.GetNodeContent(strCode);
		}
		if (xmlReader.OpenNode(L"response/msg"))
		{
			xmlReader.GetNodeContent(strMsg);
		}
		if (xmlReader.OpenNode(L"response/data"))
		{
			xmlReader.EnterNode();
			do
			{
				std::wstring strName, strContent;
				xmlReader.GetNodeName(strName);
				xmlReader.GetNodeContent(strContent);
				mapData[strName] = strContent;
			}
			while (xmlReader.NextSibling());
		}
		return true;
	}
	return false;
}

bool CNanhuaSocketPara::GetEP(float &fET, float &fRH, float &fAP)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetEP");
	strXML.AppendFormat(L"</request>");

	EPPacket sSend((LPCTSTR)strXML);
	EPPacket sRecv;

	if (SendAndRecvPacket(sSend, sRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(sRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetEP")
			{
				if (strCode == L"0")
				{
					fET = (float)_wtof(mapData[L"ET"].c_str());
					fRH = (float)_wtof(mapData[L"RH"].c_str());
					fAP = (float)_wtof(mapData[L"AP"].c_str());

					return true;
				}
			}
		}
	}
	return false;
}

DWORD CNanhuaSocketPara::GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);
	//assert(NULL != pfOilTemp);
	//assert(NULL != pusRpm);
/*	
	// 温度
	if (NULL != pfET)
	{
		(*pfET) = 0;
	}

	// 湿度
	if (NULL != pfRH)
	{
		(*pfRH) = 0;
	}

	// 大气压
	if (NULL != pfAP)
	{
		(*pfAP) = 0;
	}

	// 油温
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0;
	}

	// 转速
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}
*/
	// 判断端口是否打开
	if (IsOpen())
	{
		float fET(0.0f), fRH(0.0f), fAP(0.0f);
		if (GetEP(fET, fRH, fAP))
		{
			// 温度
			if (NULL != pfET)
			{
				(*pfET) = fET;
			}
			// 湿度
			if (NULL != pfRH)
			{
				(*pfRH) = fRH;
			}
			// 大气压
			if (NULL != pfAP)
			{
				(*pfAP) = fAP;
			}
			return PARA_WR_SUCCEED;
		}
		else
		{
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaSocketPara::GetEntireData(float *const pfET, 
		float *const pfRH, 
		float *const pfAP, 
		float *const pfOilTemp, 
		USHORT *const pusRpm, 
		WORD *const pwStatus
		)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);
	//assert(NULL != pfOilTemp);
	//assert(NULL != pusRpm);
/*	
	// 温度
	if (NULL != pfET)
	{
		(*pfET) = 0;
	}

	// 湿度
	if (NULL != pfRH)
	{
		(*pfRH) = 0;
	}

	// 大气压
	if (NULL != pfAP)
	{
		(*pfAP) = 0;
	}

	// 油温
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0;
	}

	// 转速
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}

	// 传感器
	if (NULL != pwStatus)
	{
		(*pwStatus) = 0;
	}
*/
	// 判断端口是否打开
	if(IsOpen())
	{
		float fET(0.0f), fRH(0.0f), fAP(0.0f);
		if (GetEP(fET, fRH, fAP))
		{
			// 温度
			if (NULL != pfET)
			{
				(*pfET) = fET;
			}
			// 湿度
			if (NULL != pfRH)
			{
				(*pfRH) = fRH;
			}
			// 大气压
			if (NULL != pfAP)
			{
				(*pfAP) = fAP;
			}
			return PARA_WR_SUCCEED;
		}
		else
		{
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaSocketPara::GetEnvironmentParameters(float *const pfET, 
		float *const pfRH, 
		float *const pfAP)
{
	//assert(NULL != pfET);
	//assert(NULL != pfRH);
	//assert(NULL != pfAP);
/*	
	// 温度
	if (NULL != pfET)
	{
		(*pfET) = 0;
	}

	// 湿度
	if (NULL != pfRH)
	{
		(*pfRH) = 0;
	}

	// 大气压
	if (NULL != pfAP)
	{
		(*pfAP) = 0;
	}
*/
	// 判断端口是否打开
	if(IsOpen())
	{
		float fET(0.0f), fRH(0.0f), fAP(0.0f);
		if (GetEP(fET, fRH, fAP))
		{
			// 温度
			if (NULL != pfET)
			{
				(*pfET) = fET;
			}
			// 湿度
			if (NULL != pfRH)
			{
				(*pfRH) = fRH;
			}
			// 大气压
			if (NULL != pfAP)
			{
				(*pfAP) = fAP;
			}
			return PARA_WR_SUCCEED;
		}
		else
		{
			return PARA_WR_WRITE_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		return PARA_WR_PORT_NOT_OPEN;
	}
}

DWORD CNanhuaSocketPara::DataTrans(const BYTE * pbDataBuf, const UINT nDatalen)
{
	return PARA_WR_UNKNOWN_ERROR;
}

DWORD CNanhuaSocketPara::GetIMEI_ICCID(char* chIMEI, char* chICCID)
{
	return PARA_WR_UNKNOWN_ERROR;
}


void CNanhuaSocketPara::GenLogFile(void)
{
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
	strFileName.Format(L"%s_%s.log", odtNow.Format(L"%Y-%m-%d"), m_strModName);
	m_strLogFilePath = wchLogFileFolderPath;
	m_strLogFilePath += strFileName;
}