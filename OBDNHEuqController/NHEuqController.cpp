// NHEuqController.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "NHEuqController.h"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\MYLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\MYLib.lib")
#endif

#include "..\OBDNHPadClientLib\NHPadClientLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\OBDNHPadClientLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\OBDNHPadClientLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNHEuqController::CNHEuqController(void)
{
	// 废气分析仪线程
	m_pAnaThread = NULL;
	m_bAnaThreadAttach = false;

	m_pFloThread = NULL;
	m_bFloThreadAttach = false;

	// 氮氧分析仪线程
	m_pNOxThread = NULL;
	m_bNOxThreadAttach = false;

	// 不透光线程
	m_pOpaThread = NULL;

	// OBD
	m_pOBDThread = NULL;

	// 参数采集仪线程
	m_pParaThread = NULL;
}

CNHEuqController::~CNHEuqController(void)
{
}

CNHEuqController& CNHEuqController::GetInstance(void)
{
	static std::auto_ptr<CNHEuqController> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new CNHEuqController());
	}
	return *(auto_ptr_instance.get());
}

void CNHEuqController::Init(void)
{
	// 启动流量计线程
	m_pFloThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	// 启动废气分析仪线程
	m_pAnaThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	// 启动氮氧分析仪线程
	m_pNOxThread = (CAnaThread*)AfxBeginThread(RUNTIME_CLASS(CAnaThread));
	// 启动不透光线程
	m_pOpaThread = (COpaThread*)AfxBeginThread(RUNTIME_CLASS(COpaThread));
	// 启动OBD线程
	m_pOBDThread = (COBDAdapterThread*)AfxBeginThread(RUNTIME_CLASS(COBDAdapterThread));
	// 参数采集仪线程
	m_pParaThread = (CParaThread*)AfxBeginThread(RUNTIME_CLASS(CParaThread));
	
	CString strPath;
	wchar_t chFilePath[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"Equipment.ini", chFilePath))
	{
		// 暂不做任何操作
	}
	strPath = chFilePath;
	CSimpleIni si(strPath);
	BYTE bOpaPort = (BYTE)_wtoi(si.GetString(L"OpaParams", L"Port", L"99"));
	// 不透光型号型号
	CString strOpaProtocol = si.GetString(_T("OpaParams"),_T("Protocol"),_T("NHT1L"));
	// 转化为大写
	strOpaProtocol.MakeUpper();
	COpaThread::InstrumentModel im;
	if(_T("NHT1L") == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT1L;
	}
	else if (L"NHT6" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT6;
	}
	else if (L"NHT610" == strOpaProtocol)
	{
		im = COpaThread::IM_NANHUA_NHT610;
	}
	else if (L"FTY100" == strOpaProtocol)
	{
		im = COpaThread::IM_FOFEN_FTY100;
	}
	else if (L"FLB100" == strOpaProtocol)
	{
		im = COpaThread::IM_FULI_FLB100;
	}
	else if (L"MQY200" == strOpaProtocol)
	{
		im = COpaThread::IM_MINGQUAN_MQY200;
	}
	else
	{
		im = COpaThread::IM_NANHUA_NHT1L;
	}
	m_pOpaThread->Open(bOpaPort, im);
		
	BYTE bAnaPort = (BYTE)_wtoi(si.GetString(L"AnaParams", L"Port", L"99"));
	CString strAnaProtocol = si.GetString(_T("AnaParams"),_T("Protocol"),_T("NHA509"));
	CString strConnectToFlo = si.GetString(_T("AnaParams"),_T("ConnectToFlo"),_T("0"));
	m_bConnectToFlowmeter = (strConnectToFlo == L"1");

	BYTE bFloPort = (BYTE)_wtoi(si.GetString(L"FloParams", L"Port", L"99"));
	CString strFloProtocol = si.GetString(_T("FloParams"),_T("Protocol"),_T("NHA509"));
	strFloProtocol.TrimRight(L"En");	// 流量计不支持加密协议，过滤掉

	//if (bAnaPort == bOpaPort)		// 汽柴混合线不做在同一台设备
	//{
	//	// 分析仪和不透光共享串口
	//	m_pAnaThread->AttachComHandle(m_pOpaThread->GetComHandle(), strAnaProtocol);
	//	m_pAnaThread->AttachCriticalSection(m_pOpaThread->GetCriticalSection());
	//	m_bAnaThreadAttach = true;
	//
	//	if (!m_bConnectToFlowmeter)
	//	{
	//		// 分析仪和流量计独立串口
	//		m_pFloThread->Open(bFloPort, strFloProtocol, true);
	//	}
	//	else
	//	{
	//		// 分析仪和流量计共享串口
	//		m_pFloThread->AttachComHandle(m_pAnaThread->GetComHandle(), strFloProtocol);
	//		m_pFloThread->AttachCriticalSection(m_pAnaThread->GetCriticalSection());
	//		m_bFloThreadAttach = true;
	//	}
	//}
	//else
	{
		m_pFloThread->Open(bFloPort, strFloProtocol, true);
		if (!m_bConnectToFlowmeter)
		{
			// 分析仪和流量计独立串口
			m_pAnaThread->Open(bAnaPort, strAnaProtocol, false);
			m_bAnaThreadAttach = false;
		}
		else
		{
			// 分析仪和流量计共享串口
			m_pAnaThread->AttachComHandle(m_pFloThread->GetComHandle(), strAnaProtocol);
			m_pAnaThread->AttachCriticalSection(m_pFloThread->GetCriticalSection());
			m_bAnaThreadAttach = true;
		}
	}

	BYTE bNOxPort = (BYTE)_wtoi(si.GetString(L"NOxParams", L"Port", L"99"));
	CString strNOxProtocol = si.GetString(_T("NOxParams"),_T("Protocol"),_T("NHA610En"));
	if (bNOxPort == bOpaPort)		// 从NHA610取NOx
	{
		m_pNOxThread->AttachComHandle(m_pOpaThread->GetComHandle(), strNOxProtocol);
		m_pNOxThread->AttachCriticalSection(m_pOpaThread->GetCriticalSection());
		m_bNOxThreadAttach = true;
	}
	//else if (bNOxPort == bAnaPort)	// 从NHA509取NOx			// 汽柴混合线不做在同一台设备
	//{
	//	m_pNOxThread->AttachComHandle(m_pAnaThread->GetComHandle(), strNOxProtocol);
	//	m_pNOxThread->AttachCriticalSection(m_pAnaThread->GetCriticalSection());
	//	m_bNOxThreadAttach = true;
	//}
	else							// 从NHA100取NOx
	{
		m_pNOxThread->Open(bNOxPort, strNOxProtocol, false);
		m_bNOxThreadAttach = false;
	}
	
//	BYTE bObdPort = (BYTE)_wtoi(si.GetString(L"OBDParams", L"Port", L"99"));
//	std::wstring strServerIP = si.GetString(L"OBDParams", L"ServerIP", L"127.0.0.1");
//	int nServerPort = _wtoi(si.GetString(L"OBDParams", L"ServerPort", L"10055"));
//	CString strObdProtocol = si.GetString(_T("OBDParams"), _T("Protocol"), _T("ZHENGDE"));
//	m_strObdPath = si.GetString(_T("OBDParams"), _T("Path"), _T(""));
//	if (_wcsicmp(strObdProtocol, L"ZENYUAN") == 0)
//	{
//		m_pOBDThread->Open(bObdPort, NULL, strObdProtocol);
//	}
//	else
//	{
//		// 启动exe
//		if (_wcsicmp(strObdProtocol, L"ZHENGDE") == 0)
//		{
//			CString strPara;
//			strPara.Format(L" -com %d -port %d ", bObdPort, nServerPort);
//			if (m_strObdPath != L"")
//			{
//				ShellExecute(NULL, NULL, m_strObdPath, strPara, NULL, SW_MINIMIZE);			
//			}
//		}
//		else if (_wcsicmp(strObdProtocol, L"NHWS") == 0)
//		{
//#ifdef DEBUG
//			PDAServiceLibAPI::GetInstance().SetRunningNumber(L"123456789");
//#endif
//		}
//		m_pOBDThread->Open(strServerIP, nServerPort, strObdProtocol);
//	}
	
	// 获取通讯端口
	BYTE bParaPort = (BYTE)_wtoi(si.GetString(L"ParaParams", L"Port", L"99"));
	std::wstring strServerIPPara = si.GetString(L"ParaParams", L"ServerIP", L"127.0.0.1");
	int nServerPortPara = _wtoi(si.GetString(L"ParaParams", L"ServerPort", L"10061"));
	CString strParaProtocol = si.GetString(L"ParaParams", L"Protocol", L"NHEPCS");
	strParaProtocol.MakeUpper();
	CParaThread::InstrumentModel imPara(CParaThread::IM_NANHUA_EPCS);
	if (strParaProtocol == L"NHP")
	{
		imPara = CParaThread::IM_NANHUA_PARA;
	}
	else if (strParaProtocol == L"NHPS")
	{
		imPara = CParaThread::IM_NANHUA_PARA_Socket;
	}
	else if (strParaProtocol == L"NHEPCS")
	{
		imPara = CParaThread::IM_NANHUA_EPCS;
	}
	else if (strParaProtocol == L"NHEPCS4G")
	{
		imPara = CParaThread::IM_NANHUA_EPCS_4G;
	}
	else if (strParaProtocol == L"DIM")
	{
		imPara = CParaThread::IM_PARA_DIM;
	}
	else if (strParaProtocol == L"DIM_NJRJ")
	{
		imPara = CParaThread::IM_PARA_DIM_NJRJ;
	}
	else if (strParaProtocol == L"SG506")
	{
		imPara = CParaThread::IM_PARA_SG506;
	}

	if (imPara == CParaThread::IM_NANHUA_PARA_Socket)
	{
		m_pParaThread->Open(strServerIPPara, nServerPortPara, imPara);
	}
	else
	{
		m_pParaThread->Open(bParaPort, imPara);
	}
}

void CNHEuqController::UnInit(void)
{
	// 停泵
	if (m_pNOxThread->IsOpen())
	{
		m_pNOxThread->StopPump();
	}
	if (m_pAnaThread->IsOpen())
	{
		m_pAnaThread->StopPump();
	}
	if (m_pFloThread->IsOpen())
	{
		m_pFloThread->RunFan_Flowmeter(false);
	}
	
	if (m_bNOxThreadAttach)
	{
		m_pNOxThread->DetachCriticalSection();
		m_pNOxThread->DetachComHandle();
		m_bNOxThreadAttach = false;
	}

	if (m_bAnaThreadAttach)
	{
		m_pAnaThread->DetachCriticalSection();
		m_pAnaThread->DetachComHandle();
		m_bAnaThreadAttach = false;
	}

	if (m_bFloThreadAttach)
	{
		m_pFloThread->DetachCriticalSection();
		m_pFloThread->DetachComHandle();
		m_bFloThreadAttach = false;
	}

	// 退出分析仪线程
	if (NULL != m_pNOxThread)
	{
		m_pNOxThread->Close();
		m_pNOxThread->QuitThread();
		WaitForSingleObject(m_pNOxThread->m_hThread, 5000);
	}
	if (NULL != m_pAnaThread)
	{
		m_pAnaThread->Close();
		m_pAnaThread->QuitThread();
		WaitForSingleObject(m_pAnaThread->m_hThread, 5000);
	}
	if (NULL != m_pFloThread)
	{
		m_pFloThread->Close();
		m_pFloThread->QuitThread();
		WaitForSingleObject(m_pFloThread->m_hThread, 5000);
	}

	// 退出不透光线程
	if (NULL != m_pOpaThread)
	{
		m_pOpaThread->Close();
		m_pOpaThread->QuitThread();
		WaitForSingleObject(m_pOpaThread->m_hThread, 5000);
	}

	// 退出OBD线程
	if (NULL != m_pOBDThread)
	{
		if (m_pOBDThread->GetInstrumentModel() == COBDAdapterThread::IM_ZHENGDE)
		{
			// 退出exe
			if (m_strObdPath != L"")
			{
				ShellExecute(NULL, NULL, m_strObdPath, L" -quit", NULL, SW_MINIMIZE);
			}
		}

		m_pOBDThread->Close();
		m_pOBDThread->QuitThread();
		WaitForSingleObject(m_pOBDThread->m_hThread, 5000);		
	}

	// 参数采集仪线程
	if (NULL != m_pParaThread)
	{
		m_pParaThread->Close();
		m_pParaThread->QuitThread();
		WaitForSingleObject(m_pParaThread->m_hThread, 5000);
	}
}
