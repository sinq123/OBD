#include "StdAfx.h"
#include "JinBenTengOBD.h"

#include <vector>
#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

CJinBenTengOBD::CJinBenTengOBD(void)
{

	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;
	m_LastOPTime = clock();
	m_nGetRTDataTimeInterval = 1000;

	m_hDll = NULL;
	// 模块名称
	m_strModName = L"JinBenTengOBD";

	// 生成日志文件
	GenLogFile();
}


CJinBenTengOBD::~CJinBenTengOBD(void)
{
	// 对象销毁时,关闭串口资源
	Close();
}

DWORD CJinBenTengOBD::Open(BYTE bPort, const wchar_t* wchPath)
{
	assert(0 != bPort);

	m_bPort = bPort;
	m_strDllPath.Format(L"%s", wchPath);
	//关闭已存在的串口句柄
	Close();
	Sleep(1000);

	m_hDll = LoadLibrary(wchPath);

	CString strLog;
	strLog.Format(L"%s", wchPath);
	if (m_hDll == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::Open", L"加载Dll失败", strLog);
		Close();
		return COM_OPEN_FAIL;
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::Open", L"加载Dll成功", strLog);
		// 判断初始化是否成功
		if (0==JBT_OBDDiagnosisInit())
		{
			// 初始化Comm接口
			JBT_CommInit();
		}
		return COM_OPEN_SUCCEED;
	}

}

DWORD CJinBenTengOBD::Close(void)
{
	if (NULL != m_hDll)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::Close", L"卸载Dll", L"");
	}
	return COBDAdapter::Close();
}

void CJinBenTengOBD::GenLogFile(void)
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

bool CJinBenTengOBD::IsOpen(void) const
{
	if (m_hDll == NULL)
	{
		return false;
	}
	return true;
}

int CJinBenTengOBD::JBT_OBDDiagnosisInit(void)
{
	if (m_hDll == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_OBDDiagnosisInit", L"m_hDll", L"为空");
		Close();
		return 22;
	}

	typedef int (*OBDDiagnosisInit)(char* c);
	OBDDiagnosisInit m_OBDDiagnosisInit = (OBDDiagnosisInit)GetProcAddress(m_hDll, "OBDDiagnosisInit");

	if (m_OBDDiagnosisInit == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_OBDDiagnosisInit", L"OBDDiagnosisInit", L"查询失败！");
		Close();
		return 21;
	}
	// 是否可以为日志目录？暂时给日志目录
	int nRet = m_OBDDiagnosisInit(CNHCommonAPI::UnicodeToANSI(m_strLogFilePath.GetString()));

	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_OBDDiagnosisInit", L"OBDDiagnosisInit", L"初始化诊断接口成功！");
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_OBDDiagnosisInit", L"OBDDiagnosisInit", L"初始化诊断接口失败！");
	}

	return nRet;
}

int CJinBenTengOBD::JBT_CommInit(void)
{
	if (m_hDll == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_CommInit", L"m_hDll", L"为空");
		Close();
		return 22;
	}

	typedef int (*CommInit)(unsigned nCommPortNum);
	CommInit m_CommInit = (CommInit)GetProcAddress(m_hDll, "CommInit");

	if (m_CommInit == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_CommInit", L"m_CommInit", L"查询失败！");
		Close();
		return 21;
	}

	int nRet = m_CommInit(m_bPort);

	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_CommInit", L"m_CommInit", L"初始化Comm接口成功！");
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_CommInit", L"m_CommInit", L"初始化Comm接口失败！");
	}
	return nRet;
}

bool CJinBenTengOBD::SendAndRecvPacket(const std::wstring& strSend, std::wstring& strRecv)
{
	if (!IsOpen())
	{
		return false;
	}

	// 自动锁
	CAutoLock autoLock(GetLogLocker());

	typedef int (*RequestByXMLString)(char * inData, char *outData, int outLen);
	RequestByXMLString m_RequestByXMLString = (RequestByXMLString)GetProcAddress(m_hDll, "RequestByXMLString");

	if (m_RequestByXMLString == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::SendAndRecvPacket", L"RequestByXMLString", L"查询失败！");
		return false;
	}
	char* outData = new char[999990];

	int nRet = m_RequestByXMLString(CNHCommonAPI::UnicodeToANSI(strSend.c_str()), outData, 999990);
	bool bRet;
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::SendAndRecvPacket", L"RequestByXMLString", L"请求执行诊断接口成功！");
		bRet = true;
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::SendAndRecvPacket", L"RequestByXMLString", L"请求执行诊断接口失败！");
		bRet = false;
	}

	strRecv = CNHCommonAPI::ANSIToUnicode(outData);

	// 释放指针资源
	delete [] outData;

	return bRet;
}

bool CJinBenTengOBD::Xml2Map(LPCTSTR szXML,  
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
			// 版本号
			if (strCmd == L"GetSoftwareVersion")
			{
				std::wstring strContent;
				xmlReader.GetNodeContent(strContent);
				mapData[L"SoftwareVersion"] = strContent;
			}
			else
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
		}
		return true;
	}
	return false;
}

bool CJinBenTengOBD::IsAdapterOnline(void)
{
	// 3.3.3、取 VCI 连接状态
	// 判断是否可以启动
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetVCIState");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetVCIState")
			{
				if (strCode == L"0")
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CJinBenTengOBD::Init(DWORD &dwProtocol, const int nFuelType)
{
	// 3.3.1、开始 OBD 检测
	m_nFuelType = nFuelType;

	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"StartTest");
	strXML.AppendFormat(L"<data>");
	strXML.AppendFormat(L"<Protocol>%02d</Protocol>", dwProtocol);
	strXML.AppendFormat(L"<FuelType>%d</FuelType>", m_nFuelType);
	strXML.AppendFormat(L"</data>");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"StartTest")
			{
				if (strCode == L"0")
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CJinBenTengOBD::GetScanStartTest(std::wstring& strProgress, DWORD &dwProtocol)
{
	// 3.3.2、取车辆扫描进度状态

	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"ScanStartTest");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"ScanStartTest")
			{
				if (strCode == L"0")
				{
					// 判断是否有key
					if (mapData.count(L"progress") >= 1)
					{
						strProgress = mapData[L"progress"].c_str();
					}
					if (mapData.count(L"Protocol") >= 1)
					{
						dwProtocol = _wtoi(mapData[L"Protocol"].c_str());
					}
					return true;
				}
			}
		}
	}

	return false;
}

bool CJinBenTengOBD::GetInitState(OBDInitState &enState, DWORD &dwProtocol)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetVCIState");
	strXML.AppendFormat(L"</request>");


	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetVCIState")
			{
				if (strCode == L"0")
				{
					// 判断是否有key
					if (mapData.count(L"SoftwareVersion") >= 1)
					{
						dwProtocol = _wtoi(mapData[L"Protocol"].c_str());
						enState = OBD_InitSucceed;
					}
					else if (strCode == L"1")
					{
						enState = OBD_InitFailed;
					}
					else if (strCode == L"2")
					{
						enState = OBD_Initing;
					}
					return true;
				}
			}
		}
	}

	return false;
}

bool CJinBenTengOBD::GetDTC(std::vector<SDTCData> &vtDTC)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetDTC");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetDTC")
			{
				if (strCode == L"0")
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CJinBenTengOBD::GetLampState(bool &bIsOn)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
	strXML.AppendFormat(L"</request>");


	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetRTData")
			{
				if (strCode == L"0")
				{
					bIsOn = (_wtoi(mapData[L"LampState"].c_str())==1);
					return true;
				}
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem)
{
	// 3.3.9、取系统监测/就绪状态
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetSystemCheckState");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
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
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::ClearDTC(void)
{
	return false;
}

bool CJinBenTengOBD::GetRTData(int &nRev, float &fV, float &fTemp)
{
	if (clock()-m_LastOPTime > m_nGetRTDataTimeInterval)	// 强制在底层限制采样命令的发送间隔
	{
		m_LastOPTime = clock();

		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
		strXML.AppendFormat(L"</request>");

		std::wstring strRecv;
		if (SendAndRecvPacket(strXML.GetString(), strRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			std::map<std::wstring, std::wstring> mapData;
			if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetRTData")
				{
					if (strCode == L"0")
					{
						m_fV = (float)_wtof(mapData[L"speed"].c_str());
						m_fTemp = (float)_wtof(mapData[L"fdjlqywd"].c_str());
						m_nRev = _wtoi(mapData[L"zhuansu"].c_str());

						nRev = m_nRev;
						fV = m_fV;
						fTemp = m_fTemp;
						return true;
					}
					else
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

bool CJinBenTengOBD::GetRTData(std::map<std::wstring, std::wstring> &mapData)
{
	if (clock()-m_LastOPTime > m_nGetRTDataTimeInterval)	// 强制在底层限制采样命令的发送间隔
	{
		m_LastOPTime = clock();

		m_LastOPTime = clock();

		CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		strXML.AppendFormat(L"<request>");
		strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
		strXML.AppendFormat(L"</request>");

		std::wstring strRecv;
		if (SendAndRecvPacket(strXML.GetString(), strRecv))
		{
			std::wstring strCmd, strCode, strMsg;
			mapData.clear();
			if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
			{
				if (strCmd == L"GetRTData")
				{
					if (strCode == L"0")
					{
						m_fV = (float)_wtof(mapData[L"speed"].c_str());
						m_fTemp = (float)_wtof(mapData[L"fdjlqywd"].c_str());
						m_nRev = _wtoi(mapData[L"zhuansu"].c_str());
						m_mapRTData = mapData;
						return true;
					}
					else
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

bool CJinBenTengOBD::GetDTCMileage(DWORD &dwMileage)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
	strXML.AppendFormat(L"</request>");


	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetRTData")
			{
				if (strCode == L"0")
				{
					dwMileage = (_wtoi(mapData[L"Mileage"].c_str())==1);
					return true;
				}
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::GetFreezeData(std::wstring &strData)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetFreezeData");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetFreezeData")
			{
				if (strCode == L"0")
				{
					// 判断是否有key
					if (mapData.count(L"dzdjzgzm") >= 1)
					{
						strData = mapData[L"dzdjzgzm"].c_str();
					}
					return true;
				}
			}
		}
	}

	return false;
}

bool CJinBenTengOBD::GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode/*=NULL*/)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetECUInfo");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetECUInfo")
			{
				if (pnCode != NULL)
				{
					*pnCode = _wtoi(strCode.c_str());
				}
				strVIN = mapData[L"VIN"];
				strVINHex = L"17";

				if (strCode == L"0")
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::GetOBDType(std::wstring &strType)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
	strXML.AppendFormat(L"</request>");


	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetRTData")
			{
				if (strCode == L"0")
				{
					strType =mapData[L"obdtype"].c_str();
					return true;
				}
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::GetMileage(DWORD &dwMileage)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetRTData");
	strXML.AppendFormat(L"</request>");


	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetRTData")
			{
				if (strCode == L"0")
				{
					dwMileage =_wtoi(mapData[L"mileage"].c_str());
					return true;
				}
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetECUInfo");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetECUInfo")
			{
				if (pnCode != NULL)
				{
					*pnCode = _wtoi(strCode.c_str());
				}
				// 删除VIN
				mapData.erase(L"VIN");
				if (strCode == L"0")
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::GetSoftwareVer(std::wstring &strVer)
{
	// 3.3.4、取软件版本号
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetSoftwareVersion");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"GetSoftwareVersion")
			{
				if (strCode == L"0")
				{
					// 判断是否有key
					if (mapData.count(L"SoftwareVersion") >= 1)
					{
						strVer = mapData[L"SoftwareVersion"].c_str();
					}
					return true;
				}
			}
		}
	}

	return false;
}

bool CJinBenTengOBD::GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"GetIUPR");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		mapData.clear();
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
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
			}
		}
	}
	return false;
}

bool CJinBenTengOBD::UnInit(void)
{
	bool bRet(false);
	CString strXML = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	strXML.AppendFormat(L"<request>");
	strXML.AppendFormat(L"<cmd>%s</cmd>", L"StopTest");
	strXML.AppendFormat(L"</request>");

	std::wstring strRecv;
	if (SendAndRecvPacket(strXML.GetString(), strRecv))
	{
		std::wstring strCmd, strCode, strMsg;
		std::map<std::wstring, std::wstring> mapData;
		if (Xml2Map(strRecv.c_str(), strCmd, strCode, strMsg, mapData))
		{
			if (strCmd == L"StopTest")
			{
				if (strCode == L"0")
				{
					bRet =  true;
				}
			}
		}
	}

	// 停止OBD后，不管是否成功都释放资源
	JBT_OBDDiagnosisRelease();

	return bRet;
}

int CJinBenTengOBD::JBT_OBDDiagnosisRelease(void)
{
	
	if (m_hDll == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_OBDDiagnosisRelease", L"m_hDll", L"为空");
		Close();
		return 22;
	}

	typedef int (*OBDDiagnosisRelease)(void);
	OBDDiagnosisRelease m_OBDDiagnosisRelease = (OBDDiagnosisRelease)GetProcAddress(m_hDll, "OBDDiagnosisRelease");

	if (m_OBDDiagnosisRelease == NULL)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_OBDDiagnosisRelease", L"m_OBDDiagnosisRelease", L"查询失败！");
		Close();
		return 21;
	}

	int nRet = m_OBDDiagnosisRelease();

	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_OBDDiagnosisRelease", L"m_OBDDiagnosisRelease", L"释放加载的资源接口成功！");
	}
	else
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath,L"CJinBenTengOBD::JBT_CommInit", L"m_OBDDiagnosisRelease", L"释放加载的资源接口失败！");
	}
	return nRet;
}

std::wstring CJinBenTengOBD::GetOBDProtocolName(DWORD dwProtocol)
{
	// <!--燃料类型  0：汽油(默认) 1：柴油-->
	if (m_nFuelType == 0)
	{
		switch (dwProtocol)
		{
		case 51:		return L"GAS_CAN_Frame";
		case 4:			return L"15765_11_500K";
		case 44:		return L"15765_29_500K";
		case 45:		return L"15765_11_250K";
		case 46:		return L"15765_29_250K";
		case 47:		return L"27145_11_500K";
		case 48:		return L"27145_29_500K";
		case 49:		return L"27145_11_250K";
		case 50:		return L"27145_29_250K";
		case 1:			return L"14230";
		case 2:			return L"14230_Add";
		case 7:			return L"J1850_PWM";
		case 6:			return L"J1850_VPW";
		case 3:			return L"9141";
		case 31:		return L"J1939";
		case 33:		return L"15765_AT";
		case 32:		return L"14230_ELSE";
		default:		return L"未知协议";
		}
	}
	else if (m_nFuelType == 1)
	{
		switch (dwProtocol)
		{
		case 1:			return L"ISO_14230_4_KWP_FAST";
		case 29:		return L"ISO_CAN_Frame";
		case 8:			return L"SAE_J1939";
		case 30:		return L"ISO_15765_ELSE";
		case 25:		return L"ISO_27145_29BIT_250K";
		case 23:		return L"ISO_15765_29BIT_250K";
		case 27:		return L"ISO_27145_11BIT_250K";
		case 21:		return L"ISO_15765_11BIT_250K";
		case 26:		return L"ISO_27145_29BIT_500K";
		case 24:		return L"ISO_15765_29BIT_500K";
		case 28:		return L"ISO_27145_11BIT_500K";
		case 22:		return L"ISO_15765_11BIT_500K";
		case 43:		return L"ISO_CAN_Frame_3";
		case 35:		return L"ISO_27145_29BIT_250K_3";
		case 36:		return L"ISO_15765_29BIT_250K_3";
		case 37:		return L"ISO_27145_11BIT_250K_3";
		case 38:		return L"ISO_15765_11BIT_250K_3";
		case 39:		return L"ISO_27145_29BIT_500K_3";
		case 40:		return L"ISO_15765_29BIT_500K_3";
		case 41:		return L"ISO_27145_11BIT_500K_3";
		case 42:		return L"ISO_15765_11BIT_500K_3";
		case 34:		return L"SAE_J1939_REQ";
		case 2:			return L"ISO_14230_4_KWP_Add";
		case 7:			return L"SAE_J1850_PWM";
		case 6:			return L"SAE_J1850_VPW";
		case 3:			return L"ISO_9141_2_686A";
		default:		return L"未知协议";
		}
	}
	return L"";
}