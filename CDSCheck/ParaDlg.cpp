// ParaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParaDlg.h"

// CParaDlg 对话框

IMPLEMENT_DYNAMIC(CParaDlg, CDialogZoom)

CParaDlg::CParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParaDlg::IDD, pParent)
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

CParaDlg::~CParaDlg()
{
	m_fontDlgFont.DeleteObject();
}

void CParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
}


BEGIN_MESSAGE_MAP(CParaDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ENV_PARAMS_CAL, &CParaDlg::OnBnClickedButtonEnvParamsCal)
END_MESSAGE_MAP()


// CParaDlg 消息处理程序
BOOL CParaDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CParaDlg::InitCtrls()
{
	// 初始化字体
	SetDlgFont();

	m_lbTitle.SetTitle(L"参数测量仪");
}

void CParaDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}


void CParaDlg::OnBnClickedButtonEnvParamsCal()
{
	// TODO: 在此添加控件通知处理程序代码	

	ShowLoadParameterEnvParamsCalibrationDlg(UpEnvParamsAndTachometerProcessCtrlCallBack);

}


void CParaDlg::UpEnv(void)
{
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, false))
	{
		// 暂时不做其它操作
	}
	CSimpleIni si(wchPath);

	CString strTemp;

	SEnvPara sEnvPara;
	sEnvPara.JYY = theApp.m_sUserInfo.wchName;
	sEnvPara.JYYSFZH = L"123456200001010101";
	strTemp = si.GetString(L"EnvParams", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sEnvPara.odtbegin.ParseDateTime(strTemp))
	{
		sEnvPara.odtbegin = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"EnvParams", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sEnvPara.odtEnd.ParseDateTime(strTemp))
	{
		sEnvPara.odtEnd = COleDateTime::GetCurrentTime();
	}
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"ETMea", L"")));
	sEnvPara.scwd = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"ETStd", L"")));
	sEnvPara.hjwd = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"RHMea", L"")));
	sEnvPara.scsd = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"RHStd", L"")));
	sEnvPara.hjsd = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"APMea", L"")));
	sEnvPara.scdqy = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"EnvParams", L"APStd", L"")));
	sEnvPara.hjdqy = strTemp.GetString();


	SHBMsg sHBMsg;
	CZYHttp_PAI::GetInstance().SetParaCheck(sEnvPara, sHBMsg);
	
	if (sHBMsg.code == L"200")
	{
		AfxMessageBox(L"上传成功");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"上传失败：%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}
}

void CParaDlg::UpTac(void)
{
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfTachometer.ini", wchPath, false))
	{
		// 暂时不做其它操作
	}
	CSimpleIni si(wchPath);

	CString strTemp;

	STachometer sTachometer;
	sTachometer.JYY = theApp.m_sUserInfo.wchName;
	sTachometer.JYYSFZH = L"123456200001010101";
	strTemp = si.GetString(L"Tachometer", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sTachometer.odtbegin.ParseDateTime(strTemp))
	{
		sTachometer.odtbegin = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"Tachometer", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!sTachometer.odtEnd.ParseDateTime(strTemp))
	{
		sTachometer.odtEnd = COleDateTime::GetCurrentTime();
	}

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs1", L"")));
	sTachometer.bzzs1 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc1", L"")));
	sTachometer.sc1 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs2", L"")));
	sTachometer.bzzs2 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc2", L"")));
	sTachometer.sc2 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs3", L"")));
	sTachometer.bzzs3 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc3", L"")));
	sTachometer.sc3 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs4", L"")));
	sTachometer.bzzs4 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc4", L"")));
	sTachometer.sc4 = strTemp.GetString();

	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"bzzs5", L"")));
	sTachometer.bzzs5 = strTemp.GetString();
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"Tachometer", L"sc5", L"")));
	sTachometer.sc5 = strTemp.GetString();

	SHBMsg sHBMsg;
	CZYHttp_PAI::GetInstance().SetSpeCheck(sTachometer, sHBMsg);
	
	if (sHBMsg.code == L"200")
	{
		AfxMessageBox(L"上传成功");
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"上传失败：%s-%s", sHBMsg.code.c_str(), sHBMsg.msg.c_str());
		AfxMessageBox(strMsg);
	}
}


DWORD CParaDlg::UpEnvParamsAndTachometerProcessCtrlCallBack(const DWORD dwStatus)
{
	// 上传环境参数
	switch (dwStatus)
	{
	case 0x01:
		{
			// 环境参数
			UpEnv();
		}
		break;
	case 0x02:
		{
			// 转速
			UpTac();
		}
		break;
	}

	return 0x00;
}
