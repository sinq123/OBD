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
	// У���β
	if (wHeader != MAKEWORD('E', 'P'))
	{
		return false;
	}
		
	memcpy(&dwPacketLen, strRecv.data()+nOffset, sizeof(dwPacketLen));
	nOffset += sizeof(dwPacketLen);	
	// У�����ݳ���
	if (dwPacketLen != strRecv.size())
	{
		return false;
	}
		
	std::string strData;
	strData.assign(strRecv.data()+nOffset, dwPacketLen-nMinPacketSize);
	nOffset += dwPacketLen-nMinPacketSize;

	memcpy(&bTail, strRecv.data()+nOffset, sizeof(bTail));
	nOffset += sizeof(bTail);
		
	// У���β
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

	// ��ʼ��Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// ģ������
	m_strModName = L"NanhuaSocketPara";

	// ������־�ļ�
	GenLogFile();

}

CNanhuaSocketPara::~CNanhuaSocketPara(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();
	
	// ж��Winsock
	WSACleanup();

	::DeleteCriticalSection(&m_sCriticalSection);
}

DWORD CNanhuaSocketPara::Open(std::wstring strIP, const int nPort)
{
	assert(0 != nPort);

	// �ر��Ѵ��ڵĴ��ھ��
	Close();
	Sleep(1000);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	int nTimeOut = 3000;
	//����ʱ��
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
	//����ʱ��
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));
	
	CString strLog;
	strLog.Format(L"%s:%d", strIP.c_str(), nPort);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(nPort);
	server.sin_addr.s_addr = inet_addr(TCharToANSI(strIP.c_str()));
	if (connect(m_socket, (struct sockaddr*)&server, sizeof(server)) != SOCKET_ERROR)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::Open", L"���ӳɹ�", strLog);
		return PARA_OPEN_SUCCEED;
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::Open", L"����ʧ��", strLog);
		Close();
		return PARA_OPEN_FAIL;
	}
}

// �رմ���
DWORD CNanhuaSocketPara::Close(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::Close", L"�ر�����", L"");
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
	
	// �����ٽ�����
	::EnterCriticalSection(&m_sCriticalSection);

	std::string strSend = sSend.ToString();

	std::string strRecv;
	if (SendAll(m_socket, (char *)strSend.data(), strSend.size()))
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAndRecvPacket", L"���ͳɹ�", sSend.strXML.c_str());
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
					int nXmlLen = min(max(dwPacketLen-7, 1), 999990);	// ���ư���С��1M����
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
						CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAndRecvPacket", L"����", sRecv.strXML.c_str());
				
						::LeaveCriticalSection(&m_sCriticalSection);
						return true;
					}
				}
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAndRecvPacket", L"����ʧ��", sSend.strXML.c_str());
	}
	::LeaveCriticalSection(&m_sCriticalSection);
	return false;
}

bool CNanhuaSocketPara::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// ѭ������
	int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{  
		int nRet = recv(sockfd, &recvbuf[nIndex], nLeft, 0);
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::RecvAll", L"���ճ�ʱ", L"");
			return false;
		}
		else if (nRet == 0)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::RecvAll", L"������ѶϿ�", L"");
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
	//CNHLogAPI::WriteLogEx(m_strLogFilePath,LOG_MSG, L"��������", strTemp);

	return true;
}

bool CNanhuaSocketPara::SendAll(SOCKET sockfd, char *sendbuf, int len)
{
	//CString strTemp;
	//for (int i=0; i<len; i++)
	//{
	//	strTemp.AppendFormat(L"%02X ", (unsigned char)sendbuf[i]);
	//}
	//CNHLogAPI::WriteLogEx(m_strLogFilePath,LOG_MSG, L"��������", strTemp);

	// ѭ������
	unsigned int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{
		int nRet = send(sockfd, &sendbuf[nIndex], nLeft, 0); 
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CNanhuaSocketPara::SendAll", L"������ѶϿ�", L"");
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
	// �¶�
	if (NULL != pfET)
	{
		(*pfET) = 0;
	}

	// ʪ��
	if (NULL != pfRH)
	{
		(*pfRH) = 0;
	}

	// ����ѹ
	if (NULL != pfAP)
	{
		(*pfAP) = 0;
	}

	// ����
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0;
	}

	// ת��
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}
*/
	// �ж϶˿��Ƿ��
	if (IsOpen())
	{
		float fET(0.0f), fRH(0.0f), fAP(0.0f);
		if (GetEP(fET, fRH, fAP))
		{
			// �¶�
			if (NULL != pfET)
			{
				(*pfET) = fET;
			}
			// ʪ��
			if (NULL != pfRH)
			{
				(*pfRH) = fRH;
			}
			// ����ѹ
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
	// �¶�
	if (NULL != pfET)
	{
		(*pfET) = 0;
	}

	// ʪ��
	if (NULL != pfRH)
	{
		(*pfRH) = 0;
	}

	// ����ѹ
	if (NULL != pfAP)
	{
		(*pfAP) = 0;
	}

	// ����
	if (NULL != pfOilTemp)
	{
		(*pfOilTemp) = 0;
	}

	// ת��
	if (NULL != pusRpm)
	{
		(*pusRpm) = 0;
	}

	// ������
	if (NULL != pwStatus)
	{
		(*pwStatus) = 0;
	}
*/
	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		float fET(0.0f), fRH(0.0f), fAP(0.0f);
		if (GetEP(fET, fRH, fAP))
		{
			// �¶�
			if (NULL != pfET)
			{
				(*pfET) = fET;
			}
			// ʪ��
			if (NULL != pfRH)
			{
				(*pfRH) = fRH;
			}
			// ����ѹ
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
	// �¶�
	if (NULL != pfET)
	{
		(*pfET) = 0;
	}

	// ʪ��
	if (NULL != pfRH)
	{
		(*pfRH) = 0;
	}

	// ����ѹ
	if (NULL != pfAP)
	{
		(*pfAP) = 0;
	}
*/
	// �ж϶˿��Ƿ��
	if(IsOpen())
	{
		float fET(0.0f), fRH(0.0f), fAP(0.0f);
		if (GetEP(fET, fRH, fAP))
		{
			// �¶�
			if (NULL != pfET)
			{
				(*pfET) = fET;
			}
			// ʪ��
			if (NULL != pfRH)
			{
				(*pfRH) = fRH;
			}
			// ����ѹ
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
	strFileName.Format(L"%s_%s.log", odtNow.Format(L"%Y-%m-%d"), m_strModName);
	m_strLogFilePath = wchLogFileFolderPath;
	m_strLogFilePath += strFileName;
}