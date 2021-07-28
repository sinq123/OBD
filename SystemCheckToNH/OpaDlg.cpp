// OpaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpaDlg.h"

// COpaDlg 对话框

IMPLEMENT_DYNAMIC(COpaDlg, CDialogZoom)

// 标定是否Pass
bool COpaDlg::m_bDemarcationPass = false;

COpaDlg::COpaDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(COpaDlg::IDD, pParent)
{
	// 针对1280*1024分辨率进行开发
	int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 25 / 1024;
	m_fontDlgFont.CreateFont(m_nDlgFontSize,
		0, 
		0, 
		0, 
		FW_BOLD,
		FALSE, 
		FALSE, 
		FALSE, 
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_MODERN,
		_T("宋体"));
}

COpaDlg::~COpaDlg()
{
	m_fontDlgFont.DeleteObject();
}

void COpaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(COpaDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_OPA, &COpaDlg::OnBnClickedButtonOpa)
END_MESSAGE_MAP()


// COpaDlg 消息处理程序
BOOL COpaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void COpaDlg::InitCtrls()
{
	// 初始化字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"不透光度计");
}

void COpaDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}


void COpaDlg::OnBnClickedButtonOpa()
{
	// TODO: 在此添加控件通知处理程序代码	
	// 删除原有设备校准检查信息文件
	DeleteEquCalChkInfoFile();

	ShowNHT6CalChkDlg(ProcessCtrlCallBack);
}

void COpaDlg::DeleteEquCalChkInfoFile(void)
{
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// 文件不存在
		return;
	}
	DeleteFile(wchPath);
}

DWORD COpaDlg::CheckEquCalChkInfoFile(void)
{
	// 检查文件是否存在

	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"EquCalChkInfo.ini", wchPath, false))
	{
		// 文件不存在
		return 0x01;
	}

	// 检查操作是否完成

	CSimpleIni si(wchPath);
	CString str = si.GetString(L"EquCalChkInfo", L"IsOperationFinishedAtLeaseOnce", L"0");
	if (L"0" == str)
	{
		// 操作未完成
		return 0x02;
	}
	else
	{
		// 记录标定是否Pass(后续有用)
		str = si.GetString(L"EquCalChkInfo", L"Judgement", L"0");
		m_bDemarcationPass = (L"通过"==str)?true:false;

		return 0x00;
	}
}

DWORD COpaDlg::WriteEquCalChkInfo(void)
{
	DWORD dwReturn(0);

	// 写EquCalChkInfo表
	EQUCALCHKINFO sEquCalChkInfo;
	ZeroMemory(&sEquCalChkInfo, sizeof(EQUCALCHKINFO));
	GetIniEquCalChkInfo(&sEquCalChkInfo);

	dwReturn = SetDboEquCalChkInfo(sEquCalChkInfo);
	//CNHModeClientDB_SQLite::SetDboEquCalChkInfo(sEquCalChkInfo);
	return dwReturn;
}

DWORD COpaDlg::WriteDemarcationLog(void)
{
	DWORD dwReturn(0);

	// 写DemarcationLog表
	DEMARCATIONLOG sDemarcationLog;
	ZeroMemory(&sDemarcationLog, sizeof(DEMARCATIONLOG));
	GetIniDemarcationLog(&sDemarcationLog);

	dwReturn = SetDboDemarcationLog(sDemarcationLog);
	return dwReturn;
}

DWORD COpaDlg::WriteDemarcationResultOfOpaCalChk(void)
{
	DWORD dwReturn(0);

	// 写DemarcationResultOfOpaCalChk表
	DEMARCATIONRESULTOFOPACALCHK sResultOfOpaCalChk;
	ZeroMemory(&sResultOfOpaCalChk, sizeof(DEMARCATIONRESULTOFOPACALCHK));
	GetIniResultOfOpaCalChk(&sResultOfOpaCalChk);

	dwReturn = SetDboDemarcationResultOfOpaCalChk(sResultOfOpaCalChk);
	return dwReturn;
}

DWORD COpaDlg::WriteOpaCalChkData(void)
{
	// 预检查EquCalChkinfo.ini文件
	// 检查项目：
	// 1)文件是否存在
	// 2)操作是否完成
	if (0x00 != CheckEquCalChkInfoFile())
	{
		// 检查不通过
		return 0xff;
	}

	// 写EquCalChkInfo表
	WriteEquCalChkInfo();
	
	// 写DemarcationLog表
	WriteDemarcationLog();
	
	// 写DemarcationResultOfOpaCalChk表
	WriteDemarcationResultOfOpaCalChk();

	if (m_bDemarcationPass)
	{
		// 标定Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		// 更新sLineInfo不透光相关日期
		COleDateTime odtNow = COleDateTime::GetCurrentTime();
		CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		// 校准日期
		ZeroMemory(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate));
		wcscpy_s(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate)/sizeof(wchar_t), str);
		// 检查日期
		ZeroMemory(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate));
		wcscpy_s(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate)/sizeof(wchar_t), str);

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}
	else
	{
		// 标定 not Pass
		LINEINFO sLineInfo;
		ZeroMemory(&sLineInfo, sizeof(sLineInfo));

		// 从数据库更新检测线信息成员变量sLineInfo
		ReadLineInfo(sLineInfo);

		//// 更新sLineInfo不透光相关日期
		//COleDateTime odtNow = COleDateTime::GetCurrentTime();
		//CStringW str = odtNow.Format(L"%Y-%m-%d %H:%M:%S");
		//// 校准日期
		//ZeroMemory(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate));
		//wcscpy_s(sLineInfo.wchOpacimeterChannelCalibrationDate, sizeof(sLineInfo.wchOpacimeterChannelCalibrationDate)/sizeof(wchar_t), str);
		//// 检查日期
		//ZeroMemory(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate));
		//wcscpy_s(sLineInfo.wchOpacimeterChannelCheckDate, sizeof(sLineInfo.wchOpacimeterChannelCheckDate)/sizeof(wchar_t), str);
		wcscpy_s(sLineInfo.wchOpacimeterChannelCalibrationDate, L"2000-01-01");
		wcscpy_s(sLineInfo.wchOpacimeterChannelCheckDate, L"2000-01-01");

		// 将成员变量sLineInfo写入LineInfo表
		WriteLineInfo(sLineInfo);
		// 将成员变量sLineInfo写入LineInfo.ini文件
		WriteLineInfoFile(sLineInfo);
	}


	// 本次EquCalChkInfo.ini文件使用完毕，需要删除
	DeleteEquCalChkInfoFile();
	return 0x00;
}

DWORD COpaDlg::ReadLineInfo(LINEINFO &sLineInfo)
{
	NHCLIENT sNHClient;
	ZeroMemory(&sNHClient, sizeof(NHCLIENT));
	GetIniNHClient(&sNHClient);

	CString strSql;
	strSql.Format(L"select * from LineInfo where LineNumber = '%s'", sNHClient.wchLineNumber);

	DWORD dwReturn = GetDboLineInfo(strSql, &sLineInfo);

	return dwReturn;
}

DWORD COpaDlg::WriteLineInfo(LINEINFO &sLineInfo)
{
	DWORD dwReturn(0);

	// 基本检查，以防数据被篡改(有限度检查)
	// 判断检测线编号是否为空
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// 检测线编号为空
		// 不写LineInfo
		dwReturn = 0xff;
	}
	else
	{
		// 检测线编号不为空
		// 写LineInfo
		dwReturn = SetDboLineInfo(sLineInfo);
	}

	return dwReturn;
}

DWORD COpaDlg::WriteLineInfoFile(LINEINFO &sLineInfo)
{
	DWORD dwReturn(0);

	// 基本检查，以防数据被篡改(有限度检查)
	// 判断检测线编号是否为空
	if (0 == wcscmp(L"", sLineInfo.wchLineNumber))
	{
		// 检测线编号为空
		// 不写LineInfo.ini
		dwReturn = 0xff;
	}
	else
	{
		SetIniLineInfo(sLineInfo);
		dwReturn = 0x00;
	}

	return dwReturn;
}

DWORD  COpaDlg::ProcessCtrlCallBack(const DWORD dwStatus, const void* pBuf/*=NULL*/, const LONG lBufLen/*=0*/)
{
	switch (dwStatus)
	{
	case NH_OPA_CAL_CHK_START:
	case NH_OPA_CAL_CHK_STOP:
		{
			wchar_t *pMsg = new wchar_t[lBufLen+1]();
			memcpy_s(pMsg, lBufLen+1, pBuf, lBufLen);
			OutputDebugStringW(pMsg);
			TRACE("\n");
			delete [] pMsg;
		}
		break;
	case NH_OPA_CAL_CHK_FINISH:
		{
			WriteOpaCalChkData();
		}
		break;
	default:
		{
			return 0xff;
		}
		break;
	}
	return 0x00;
}
