#include "StdAfx.h"
#include "ZhengDeOBD.h"

#include <vector>
#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

#define CMD_TIMEOUT			5000
#define CMD_INIT_TIMEOUT	60000

CZhengDeOBD::CZhengDeOBD(void)
{
	srand(unsigned int(time(NULL)));

	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;
	m_LastOPTime = clock();
	m_nGetRTDataTimeInterval = 1000;
	m_nFuelType = 1;

	m_socket = INVALID_SOCKET;

	// 初始化Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 模块名称
	m_strModName = L"ZhengDeOBD";

	// 生成日志文件
	GenLogFile();
}

CZhengDeOBD::~CZhengDeOBD(void)
{
	// 对象销毁时,关闭串口资源
	Close();

	// 卸载Winsock
	WSACleanup();
}

//DWORD CZhengDeOBD::Open(BYTE bPort, const int nBaudRate)
DWORD CZhengDeOBD::Open(std::wstring strIP, const int nPort)
{
	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;

	assert(0 != nPort);

	// 关闭已存在的串口句柄
	Close();
	Sleep(1000);

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	int nTimeOut = CMD_TIMEOUT;
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
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::Open", L"连接成功", strLog);
		return COM_OPEN_SUCCEED;
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::Open", L"连接失败", strLog);
		Close();
		return COM_OPEN_FAIL;
	}
}

DWORD CZhengDeOBD::Close(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::Close", L"关闭连接", L"");
	}
	return COBDAdapter::Close();
}

bool CZhengDeOBD::IsOpen(void) const
{
	return m_socket != INVALID_SOCKET;
}

bool CZhengDeOBD::SendAndRecvPacket(ZDPacket sZDPSend, ZDPacket &sZDPRecv)
{
	if (!IsOpen())
	{
		return false;
	}

	// 自动锁
	CAutoLock autoLock(GetLogLocker());
	
	std::string strSend = sZDPSend.ToString();	
	std::wstring strLog = UTF8ToTChar(strSend.c_str());

	std::string strRecv;
	if (SendAll(m_socket, (char *)strSend.data(), strSend.size()))
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::SendAndRecvPacket", L"发送成功", strLog.c_str());
		char szBuf[MAX_PATH] = {0};

		memset(szBuf, 0, MAX_PATH);
		if (RecvAll(m_socket, szBuf, 2))
		{
			strRecv.append(szBuf, 2);

			memset(szBuf, 0, MAX_PATH);
			if (RecvAll(m_socket, szBuf, 6))
			{
				strRecv.append(szBuf, 6);
				int nPacketLen = atoi(szBuf);

				if (nPacketLen > 0)
				{
					int nXmlLen = min(max(nPacketLen-9, 1), 999990);
					char *recvBuf = new char[nXmlLen];
					memset(recvBuf, 0, nXmlLen);
					RecvAll(m_socket, recvBuf, nXmlLen);
					strRecv.append(recvBuf, nXmlLen);
					delete recvBuf;

					memset(szBuf, 0, MAX_PATH);
					RecvAll(m_socket, szBuf, 1);
					strRecv.append(szBuf, 1);

					if (sZDPRecv.Prase(strRecv))
					{
						strLog = UTF8ToTChar(sZDPRecv.ToString().c_str());
						CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::SendAndRecvPacket", L"接收", strLog.c_str());
						return true;
					}
				}
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::SendAndRecvPacket", L"发送失败", strLog.c_str());
	}
	return false;
}

bool CZhengDeOBD::RecvAll(SOCKET sockfd, char *recvbuf, int len)
{
	// 循环接收
	int nLeft = len;
	int nIndex = 0;
	while (nLeft > 0)
	{  
		int nRet = recv(sockfd, &recvbuf[nIndex], nLeft, 0);
		if (nRet == SOCKET_ERROR)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::RecvAll", L"接收超时", L"");
			return false;
		}
		else if (nRet == 0)
		{
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::RecvAll", L"服务端已断开", L"");
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

bool CZhengDeOBD::SendAll(SOCKET sockfd, char *sendbuf, int len)
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
			CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CZhengDeOBD::SendAll", L"服务端已断开", L"");
			Close();
			return false;
		}
		nLeft -= nRet;
		nIndex += nRet;
	}
	return true;
}

bool CZhengDeOBD::Xml2Map(LPCTSTR szXML,  
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

bool CZhengDeOBD::Init(DWORD &dwProtocol, const int nFuelType)
{
	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;

	m_nFuelType = nFuelType;

	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"StartTest");
	strXML.AppendFormat(L"<data>");
	strXML.AppendFormat(L"<Protocol>%02d</Protocol>", dwProtocol);
	strXML.AppendFormat(L"<FuelType>%d</FuelType>", m_nFuelType);
	strXML.AppendFormat(L"</data>");
	strXML.AppendFormat(L"</request>");
	
	ZDPacket sZDPSend((LPCTSTR)strXML);
	ZDPacket sZDPRecv;
	
	int nTimeOut = CMD_INIT_TIMEOUT;
	//发送时限
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
	//接收时限
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));

	if (SendAndRecvPacket(sZDPSend, sZDPRecv))
	{
		nTimeOut = CMD_TIMEOUT;
		//发送时限
		setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
		//接收时限
		setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));

		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"StartTest")
			{
				if (strCode == L"0")
				{
					dwProtocol = _wtoi(mapData[L"Protocol"].c_str());
					//switch (dwProtocol)
					//{
					//case 1:	m_nGetRTDataTimeInterval = 1500;	break;	//KWP2000 FastInit
					//case 2:	m_nGetRTDataTimeInterval = 1500;	break;	//KWP2000 5baudInit
					//case 3:	m_nGetRTDataTimeInterval = 1500;	break;	//ISO9141-2 5baudInit
					//case 4:	m_nGetRTDataTimeInterval = 1000;	break;	//标准CAN ISO15765
					//case 5:	m_nGetRTDataTimeInterval = 1000;	break;	//扩展CAN ISO15765
					//case 6:	m_nGetRTDataTimeInterval = 2000;	break;	//VPW
					//case 7:	m_nGetRTDataTimeInterval = 2000;	break;	//PWM
					//case 8:	m_nGetRTDataTimeInterval = 1000;	break;	//J1939
					//case 9:	m_nGetRTDataTimeInterval = 1500;	break;	//KWP1281
					//default:m_nGetRTDataTimeInterval = 1500;	break;
					//}
					return true;
				}
			}
		}
	}
	else
	{
		nTimeOut = CMD_TIMEOUT;
		//发送时限
		setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(int));
		//接收时限
		setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(int));
	}

	return false;
}


bool CZhengDeOBD::GetInitState(OBDInitState &enState, DWORD &dwProtocol)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetVCIState");
	strXML.AppendFormat(L"</request>");
	
	ZDPacket sZDPSend((LPCTSTR)strXML);
	ZDPacket sZDPRecv;

	if (SendAndRecvPacket(sZDPSend, sZDPRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetVCIState")
			{
				if (strCode == L"0")
				{
					dwProtocol = _wtoi(mapData[L"Protocol"].c_str());
					enState = OBD_InitSucceed;
					return true;
				}
				else if (strCode == L"1")
				{
					enState = OBD_InitFailed;
					return true;
				}
				else if (strCode == L"2")
				{
					enState = OBD_Initing;
					return true;
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::IsAdapterOnline(void)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetVCIState");
	strXML.AppendFormat(L"</request>");
	
	ZDPacket sZDPSend((LPCTSTR)strXML);
	ZDPacket sZDPRecv;

	if (SendAndRecvPacket(sZDPSend, sZDPRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetVCIState")
			{
				if (strCode == L"0")
				{
					return true;
				}
				else if (strCode == L"1")
				{
					return false;
				}
				else if (strCode == L"2")
				{
					return false;
				}
				else if (strCode == L"3")
				{
					return false;
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetDTC(std::vector<SDTCData> &vtDTC)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetDTC");
		strXML.AppendFormat(L"</request>");
	
		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetDTC")
				{
					if (strCode == L"0")
					{
						int nCount = _wtoi(mapData[L"Count"].c_str());
						for (int i=0; i<nCount; i++)
						{
							CString strKey;
							SDTCData sDTCData;

							strKey.Format(L"DTC%d", i+1);
							sDTCData.strCode = mapData[(LPCTSTR)strKey];

							strKey.Format(L"Detail%d", i+1);
							sDTCData.strComment = mapData[(LPCTSTR)strKey];

							vtDTC.push_back(sDTCData);
						}
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetLampState(bool &bIsOn)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetLampState");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetLampState")
				{
					if (strCode == L"0")
					{
						bIsOn = (_wtoi(mapData[L"LampState"].c_str())==1);
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetSystemCheckState");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetSystemCheckState")
				{
					if (strCode == L"0")
					{
						sSystemCheckItem.nMFT = _wtoi(mapData[L"MFT"].c_str());
						sSystemCheckItem.nFST = _wtoi(mapData[L"FST"].c_str());
						sSystemCheckItem.nCC = _wtoi(mapData[L"CC"].c_str());
						sSystemCheckItem.nCCH = _wtoi(mapData[L"CCH"].c_str());
						sSystemCheckItem.nOS = _wtoi(mapData[L"OS"].c_str());
						sSystemCheckItem.nOSH = _wtoi(mapData[L"OSH"].c_str());
						sSystemCheckItem.nEECS = _wtoi(mapData[L"EECS"].c_str());
						sSystemCheckItem.nEGR = _wtoi(mapData[L"EGR"].c_str());
						sSystemCheckItem.nSAIS = _wtoi(mapData[L"SAIS"].c_str());
						sSystemCheckItem.nICM = _wtoi(mapData[L"ICM"].c_str());
						sSystemCheckItem.nDOC = _wtoi(mapData[L"DOC"].c_str());
						sSystemCheckItem.nSCR = _wtoi(mapData[L"SCR"].c_str());
						sSystemCheckItem.nDPF = _wtoi(mapData[L"DPF"].c_str());
						sSystemCheckItem.nASC = _wtoi(mapData[L"ASC"].c_str());
						sSystemCheckItem.nPOC = _wtoi(mapData[L"POC"].c_str());
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::ClearDTC(void)
{
	return false;
}

bool CZhengDeOBD::GetRTData(int &nRev, float &fV, float &fTemp)
{
	if (clock()-m_LastOPTime > m_nGetRTDataTimeInterval)	// 强制在底层限制采样命令的发送间隔
	{
		m_LastOPTime = clock();

		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;
		
		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetRTData")
				{
					if (strCode == L"0")
					{
						m_fV = (float)_wtof(mapData[L"Velocity"].c_str());
						m_fTemp = (float)_wtof(mapData[L"CoolantTemp"].c_str());
						m_nRev = _wtoi(mapData[L"EngineRev"].c_str());

						nRev = m_nRev;
						fV = m_fV;
						fTemp = m_fTemp;
						return true;
					}
					else if (strCode == L"3")
					{
						nRev = m_nRev;
						fV = m_fV;
						fTemp = m_fTemp;
					}
				}
			}
		}
	}
	else
	{
		nRev = m_nRev;
		fV = m_fV;
		fTemp = m_fTemp;
		return true;
	}
	return false;
}

bool CZhengDeOBD::GetRTData(std::map<std::wstring, std::wstring> &mapData)
{
	if (clock()-m_LastOPTime > m_nGetRTDataTimeInterval)	// 强制在底层限制采样命令的发送间隔
	{
		m_LastOPTime = clock();

		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
		strXML.AppendFormat(L"<data>");
		strXML.AppendFormat(L"<FuelType>%d</FuelType>", m_nFuelType);
		strXML.AppendFormat(L"</data>");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;
		
		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			mapData.clear();
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetRTData")
				{
					if (strCode == L"0")
					{
						m_fV = (float)_wtof(mapData[L"Velocity"].c_str());
						m_fTemp = (float)_wtof(mapData[L"CoolantTemp"].c_str());
						m_nRev = _wtoi(mapData[L"EngineRev"].c_str());
						m_mapRTData = mapData;
						return true;
					}
					else if (strCode == L"3")
					{
						mapData = m_mapRTData;
					}
				}
			}
		}
	}
	else
	{
		mapData = m_mapRTData;
		return true;
	}
	return false;
}

bool CZhengDeOBD::GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetIUPR");
		strXML.AppendFormat(L"<data>");
		strXML.AppendFormat(L"<FuelType>%d</FuelType>", m_nFuelType);
		strXML.AppendFormat(L"</data>");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			mapData.clear();
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetIUPR")
				{
					if (pnCode != NULL)
					{
						*pnCode = _wtoi(strCode.c_str());
					}
					if (strCode == L"0")
					{
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetDTCMileage(DWORD &dwMileage)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetDTCMileage");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetDTCMileage")
				{
					if (strCode == L"0")
					{
						dwMileage = _wtoi(mapData[L"Mileage"].c_str());
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetFreezeData(std::wstring &strData)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetFreezeData");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetFreezeData")
				{
					if (strCode == L"0")
					{
						strData = mapData[L"FreezeData"];
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode/*=NULL*/)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetVIN");
		strXML.AppendFormat(L"</request>");
	
		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetVIN")
				{
					if (pnCode != NULL)
					{
						*pnCode = _wtoi(strCode.c_str());
					}
					strVIN = mapData[L"VIN"];
					strVINHex = mapData[L"VINHex"];

					if (strCode == L"0")
					{
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetOBDType(std::wstring &strType)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetOBDType");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetOBDType")
				{
					if (strCode == L"0")
					{
						strType = mapData[L"Type"];
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetMileage(DWORD &dwMileage)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetMileage");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetMileage")
				{
					if (strCode == L"0")
					{
						dwMileage = _wtoi(mapData[L"Mileage"].c_str());
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetECUInfo");
		strXML.AppendFormat(L"</request>");
	
		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetECUInfo")
				{
					if (pnCode != NULL)
					{
						*pnCode = _wtoi(strCode.c_str());
					}

					if (strCode == L"0")
					{
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::GetSoftwareVer(std::wstring &strVer)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetSoftwareVersion");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetSoftwareVersion")
				{
					if (strCode == L"0")
					{
						strVer = mapData[L"SoftwareVersion"];
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

bool CZhengDeOBD::UnInit(void)
{
	for (int i=0; i<3; i++)
	{
		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"StopTest");
		strXML.AppendFormat(L"</request>");

		ZDPacket sZDPSend((LPCTSTR)strXML);
		ZDPacket sZDPRecv;

		if (SendAndRecvPacket(sZDPSend, sZDPRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(sZDPRecv.strXML.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"StopTest")
				{
					if (strCode == L"0")
					{
						return true;
					}
					else if (strCode == L"3")
					{
						Sleep(1000+rand()%200);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return false;
}

std::wstring CZhengDeOBD::GetOBDProtocolName(DWORD dwProtocol)
{
	switch (dwProtocol)
	{
	case 1:		return L"KWP2000 FastInit";
	case 2:		return L"KWP2000 5baudInit";
	case 3:		return L"ISO9141-2 5baudInit";
	case 4:		return L"标准CAN ISO15765";
	case 5:		return L"扩展CAN ISO15765";
	case 6:		return L"VPW";
	case 7:		return L"PWM";
	case 8:		return L"J1939";
	case 9:		return L"KWP1281";
	case 10:	return L"Audi CAN";
	default:	return L"未知协议";
	}
}

void CZhengDeOBD::GenLogFile(void)
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