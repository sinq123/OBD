// ParameterEnvParamsCalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParameterEnvParamsCalibrationDlg.h"
#include "UpTachometerDlg.h"

// CParameterEnvParamsCalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CParameterEnvParamsCalibrationDlg, CDialogZoom)

CParameterEnvParamsCalibrationDlg::CParameterEnvParamsCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CParameterEnvParamsCalibrationDlg::IDD, pParent)
	, m_pParaThread(NULL)
	, m_bRefreshDataSign(true)
	, m_pfProcessCtrl(NULL)
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

	//m_pParaThread = (CParaThread*)AfxBeginThread(RUNTIME_CLASS(CParaThread));
	m_pParaThread = CNHEuqController::GetInstance().m_pParaThread;
}

CParameterEnvParamsCalibrationDlg::~CParameterEnvParamsCalibrationDlg()
{
	m_fontDlgFont.DeleteObject();
	
	//if (NULL != m_pParaThread)
	//{
	//	m_pParaThread->Close();
	//	m_pParaThread->QuitThread();
	//	WaitForSingleObject(m_pParaThread->m_hThread, 5000);
	//}
}

void CParameterEnvParamsCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_lbTitle);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);

	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbLabel1);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbLabel4);

	DDX_Control(pDX, IDC_EDIT_ET_MEA, m_edETMea);
	DDX_Control(pDX, IDC_EDIT_RH_MEA, m_edRHMea);
	DDX_Control(pDX, IDC_EDIT_AP_MEA, m_edAPMea);
	DDX_Control(pDX, IDC_EDIT_OT_MEA, m_edOTMea);

	DDX_Control(pDX, IDC_EDIT_ET_STD, m_neETStd);
	DDX_Control(pDX, IDC_EDIT_RH_STD, m_neRHStd);
	DDX_Control(pDX, IDC_EDIT_AP_STD, m_neAPStd);
	DDX_Control(pDX, IDC_EDIT_OT_STD, m_neOTStd);
}


BEGIN_MESSAGE_MAP(CParameterEnvParamsCalibrationDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_ET_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonEtCal)
	ON_BN_CLICKED(IDC_BUTTON_RH_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonRhCal)
	ON_BN_CLICKED(IDC_BUTTON_AP_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonApCal)
	ON_BN_CLICKED(IDC_BUTTON_OT_CAL, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonOtCal)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_CONFIG, &CParameterEnvParamsCalibrationDlg::OnBnClickedButtonDefaultConfig)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_UP_ENV_PARA, &CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpEnvPara)
	ON_BN_CLICKED(IDC_BTN_UP_TACHOMETER, &CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpTachometer)
END_MESSAGE_MAP()


// CParameterEnvParamsCalibrationDlg 消息处理程序

BOOL CParameterEnvParamsCalibrationDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_odtbegin = COleDateTime::GetCurrentTime();
	// 初始化控件
	InitCtrls();

	// 最大化显示窗口
	ShowWindow(SW_MAXIMIZE);

	// 延时打开串口
	SetTimer(2, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CParameterEnvParamsCalibrationDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CParameterEnvParamsCalibrationDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbTitle.SetTitle(L"参数测量仪环境参数校准");

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();

	// 设置标签颜色
	m_lbLabel1.SetTextColor(RGB(0, 102, 204));
	m_lbLabel2.SetTextColor(RGB(0, 102, 204));
	m_lbLabel3.SetTextColor(RGB(0, 102, 204));
	m_lbLabel4.SetTextColor(RGB(0, 102, 204));

	// 测量值编辑框
	m_edETMea.SetReadOnly(FALSE);
	m_edRHMea.SetReadOnly(FALSE);
	m_edAPMea.SetReadOnly(FALSE);
	m_edOTMea.SetReadOnly(FALSE);

	// 校准值编辑框
	m_neETStd.AllowNegative(FALSE);
	m_neETStd.SetMaxDecimalPlaces(1);
	m_neETStd.AllowNegative(1);
	m_neRHStd.AllowNegative(FALSE);
	m_neRHStd.SetMaxDecimalPlaces(1);
	m_neAPStd.AllowNegative(FALSE);
	m_neAPStd.SetMaxDecimalPlaces(1);
	m_neOTStd.AllowNegative(FALSE);
	m_neOTStd.SetMaxDecimalPlaces(1);

	GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonEtCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdET;
	GetDlgItem(IDC_EDIT_ET_STD)->GetWindowTextW(strStdET);
	if (L"" == strStdET)
	{
		MessageBox(L"请输入环境温度标准值", L"环境温度校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdET = static_cast<float>(_wtof(strStdET));
	if (fStdET<-50.0f || fStdET>200.0f)
	{
		MessageBox(L"环境温度标准值不在合理范围\r\n请输入合理的环境参数标准值", L"环境温度校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pParaThread->EnvironmentTemperatureSlopeCalibration(&bResult, fStdET);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"环境温度标准成功", L"环境温度校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"环境温度标准失败", L"环境温度校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonRhCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdRH;
	GetDlgItem(IDC_EDIT_RH_STD)->GetWindowTextW(strStdRH);
	if (L"" == strStdRH)
	{
		MessageBox(L"请输入相对湿度标准值", L"相对湿度校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdRH = static_cast<float>(_wtof(strStdRH));
	if (fStdRH<0.0f || fStdRH>100.0f)
	{
		MessageBox(L"相对湿度标准值不在合理范围\r\n请输入合理的相对湿度标准值", L"相对湿度校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pParaThread->RelativeHumiditySlopeCalibration(&bResult, fStdRH);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"相对湿度标准成功", L"相对湿度校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"相对湿度标准失败", L"相对湿度校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonApCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdAP;
	GetDlgItem(IDC_EDIT_AP_STD)->GetWindowTextW(strStdAP);
	if (L"" == strStdAP)
	{
		MessageBox(L"请输入大气压力标准值", L"大气压力校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdAP = static_cast<float>(_wtof(strStdAP));
	if (fStdAP<0.0f || fStdAP>200.0f)
	{
		MessageBox(L"大气压力标准值不在合理范围\r\n请输入合理的大气压力标准值", L"大气压力校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pParaThread->AirPressureSlopeCalibration(&bResult, fStdAP);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"大气压力标准成功", L"大气压力校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"大气压力标准失败", L"大气压力校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonOtCal()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检验标准值合理性
	CString strStdOT;
	GetDlgItem(IDC_EDIT_OT_STD)->GetWindowTextW(strStdOT);
	if (L"" == strStdOT)
	{
		MessageBox(L"请输入油温标准值", L"油温校准", MB_OK|MB_ICONWARNING);
		return;
	}
	float fStdOT = static_cast<float>(_wtof(strStdOT));
	if (fStdOT<-50.0f || fStdOT>200.0f)
	{
		MessageBox(L"油温标准值不在合理范围\r\n请输入合理的油温标准值", L"油温校准", MB_OK|MB_ICONWARNING);
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pParaThread->OilTemperatureColdSideSlopeCalibration(&bResult, fStdOT);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"油温标准成功", L"油温校准", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"油温标准失败", L"油温校准", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}

void CParameterEnvParamsCalibrationDlg::OpenSerialPortAndTestCommunication(void)
{
	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// 配置文件路径
	strPath = strPath + L"\\Config\\Equipment.ini";

	// 去除配置文件只读属性
	DWORD dwAttributes(0);
	dwAttributes = ::GetFileAttributes(strPath);
	dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
	::SetFileAttributes(strPath, dwAttributes);

	CSimpleIni si(strPath);
	// 获取通讯端口
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"ParaParams", L"Port", L"7"));
	// 获取分析仪通讯协议类型,默认为503
	CString strParaProtocol = si.GetString(L"ParaParams", L"Protocol", L"NHP");
	// 转化为大写
	strParaProtocol.MakeUpper();
	//if (PARA_OPEN_SUCCEED == m_pParaThread->Open(bPort, CParaThread::IM_NANHUA_PARA))
	if (m_pParaThread->IsOpen())
	{
		if (PARA_WR_SUCCEED == m_pParaThread->GetEntireData())
		{
			// 就绪
			m_lbInfo.SetText(_T("参数测量仪环境参数校准"));
			// 位于实时通讯界面
			GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DEFAULT_CONFIG)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);

			// 开始刷新数据
			EnableRefreshData(true);
		}
		else
		{
			m_lbInfo.SetText(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DEFAULT_CONFIG)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
		}
	}
	else
	{
		m_lbInfo.SetText(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_ET_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RH_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AP_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OT_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEFAULT_CONFIG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(TRUE);
	}
}

void CParameterEnvParamsCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case 1:
		{
			//RefreshData();
		}
		break;
	case 2:
		{
			KillTimer(2);
			OpenSerialPortAndTestCommunication();
		}
		break;
	default:
		{}
		break;
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CParameterEnvParamsCalibrationDlg::RefreshData(void)
{
	// 刷新数据
	float fET(0.0f);
	float fRH(0.0f);
	float fAP(0.0f);
	float fOT(0.0f);
	USHORT usRev(0);

	m_pParaThread->GetEntireData(&fET, &fRH, &fAP, &fOT, &usRev);

	CString str(L"");
	
	str.Format(L"%.1f", fET);
	m_edETMea.SetWindowTextW(str);

	str.Format(L"%.1f", fRH);
	m_edRHMea.SetWindowTextW(str);

	str.Format(L"%.1f", fAP);
	m_edAPMea.SetWindowTextW(str);

	str.Format(L"%.1f", fOT);
	m_edOTMea.SetWindowTextW(str);
}

void CParameterEnvParamsCalibrationDlg::EnableRefreshData(bool bEnable)
{
	if (bEnable)
	{
		SetTimer(1, 1000, NULL);
	}
	else
	{
		KillTimer(1);
	}
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedButtonDefaultConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK != MessageBoxW(L"确定现在恢复参数测量仪校准参数默认设置？", L"参数测量仪校准设置", MB_OKCANCEL|MB_ICONWARNING))
	{
		return;
	}

	BeginWaitCursor();
	EnableRefreshData(false);

	BYTE bResult(0);
	DWORD dwResult = m_pParaThread->RecoverCalibrationParameters(&bResult, CPara::AllParameters);

	EnableRefreshData(true);
	EndWaitCursor();

	if (PARA_WR_SUCCEED==dwResult && 0x31==bResult)
	{
		MessageBox(L"恢复参数测量仪校准参数默认设置成功", L"参数测量仪校准设置", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(L"恢复参数测量仪校准参数默认设置失败", L"参数测量仪校准设置", MB_OK|MB_ICONINFORMATION);
	}
}

void CParameterEnvParamsCalibrationDlg::SetProcessCtrlCallBack(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus))
{
	m_pfProcessCtrl = pfProcessCtrl;
}

void CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpEnvPara()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str[6];
	for (int i = 0; i < 6; i++)
	{
		str[i] = L"";
	}

	//获取全部是参数
	// 温度（测量）
	m_edETMea.GetWindowTextW(str[0]);
	m_neETStd.GetWindowTextW(str[1]);
	// 湿度
	m_edRHMea.GetWindowTextW(str[2]);
	m_neRHStd.GetWindowTextW(str[3]);
	// 大气压
	m_edAPMea.GetWindowTextW(str[4]);
	m_neAPStd.GetWindowTextW(str[5]);

	bool bEmp(false);
	for (int i = 0; i < 6; i++)
	{
		if (str[i].IsEmpty())
		{
			bEmp = true;
			break;
		}
	}
	if (bEmp)
	{
		MessageBox(L"环境参数有空值");
		return;
	}

	// 先删除之前的文档
	// 获取文件路径
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 == CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, false))
	{
		DeleteFile(wchPath);
	}
	
	CString strTemp;
	CString strResult;

	strResult.AppendFormat(L"\r\n[EnvParams]");

	// 开始时间
	strTemp.Format(L"\r\nStartTime=%s", m_odtbegin.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	strResult += strTemp;

	// 结束时间
	strTemp.Format(L"\r\nEndTime=%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S").GetString());
	strResult += strTemp;

	// 温度测量
	strTemp.Format(L"\r\nETMea=%s", str[0]);
	strResult += strTemp;
	// 温度标准
	strTemp.Format(L"\r\nETStd=%s", str[1]);
	strResult += strTemp;
	// 湿度测量
	strTemp.Format(L"\r\nRHMea=%s", str[2]);
	strResult += strTemp;
	// 湿度标准
	strTemp.Format(L"\r\nRHStd=%s", str[3]);
	strResult += strTemp;
	// 大气压测量
	strTemp.Format(L"\r\nAPMea=%s", str[4]);
	strResult += strTemp;
	// 大气压标准
	strTemp.Format(L"\r\nAPStd=%s", str[5]);
	strResult += strTemp;

	ZeroMemory(wchPath, sizeof(wchPath));
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, true);
	CStdioFileEx sfe;
	sfe.Open(wchPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CStdioFileEx::modeWriteUnicode);
	// 追加写入
	sfe.SeekToEnd();
	sfe.WriteString(strResult);
	sfe.Close();

	Sleep(200);
	// 回调输出信息
	if (NULL != m_pfProcessCtrl)
	{
		m_pfProcessCtrl(0x01);
	}
}


void CParameterEnvParamsCalibrationDlg::OnBnClickedBtnUpTachometer()
{
	// TODO: 在此添加控件通知处理程序代码

	CUpTachometerDlg dlg;

	if (IDOK == dlg.DoModal())
	{
		Sleep(200);
		// 回调输出信息
		if (NULL != m_pfProcessCtrl)
		{
			m_pfProcessCtrl(0x02);
		}
	}
}
