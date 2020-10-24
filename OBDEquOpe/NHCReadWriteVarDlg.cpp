// NHCReadWriteVarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NHCReadWriteVarDlg.h"


// CNHCReadWriteVarDlg 对话框

IMPLEMENT_DYNAMIC(CNHCReadWriteVarDlg, CDialogZoom)

CNHCReadWriteVarDlg::CNHCReadWriteVarDlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CNHCReadWriteVarDlg::IDD, pParent)
	, m_nDlgFontSize(45)
{
	// 启动测功机线程
	m_pDynThread = (CDynThread*)AfxBeginThread(RUNTIME_CLASS(CDynThread));

	// 针对1280*1024分辨率进行开发
	const int nSM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	// 根据分辨率Y方向进行调整
	m_nDlgFontSize = nSM_CYSCREEN * 30 / 1024;
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
		L"宋体");
}

CNHCReadWriteVarDlg::~CNHCReadWriteVarDlg()
{
	m_fontDlgFont.DeleteObject();

	if (NULL != m_pDynThread)
	{
		m_pDynThread->Close();
		m_pDynThread->QuitThread();
		WaitForSingleObject(m_pDynThread->m_hThread, 5000);
	}
}

void CNHCReadWriteVarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VAR, m_lcVar);
	DDX_Control(pDX, IDC_EDIT_NUMBER_FOR_WRITE, m_edNumberForWrite);
	DDX_Control(pDX, IDC_EDIT_NUMBER_FOR_READ, m_edNumberForRead);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edValue);
	DDX_Control(pDX, IDC_PROGRESS_READ_ALL_VAR, m_pgRealAllVar);
	DDX_Control(pDX, IDC_STATIC_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_shdTitle);
}


BEGIN_MESSAGE_MAP(CNHCReadWriteVarDlg, CDialogZoom)
	ON_BN_CLICKED(IDC_BUTTON_READ_VAR, &CNHCReadWriteVarDlg::OnBnClickedButtonReadVar)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_VAR, &CNHCReadWriteVarDlg::OnBnClickedButtonWriteVar)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CNHCReadWriteVarDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CNHCReadWriteVarDlg 消息处理程序

BOOL CNHCReadWriteVarDlg::OnInitDialog()
{
	CDialogZoom::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitCtrls();

	// 延时打开串口
	SetTimer(1, 500, NULL);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNHCReadWriteVarDlg::SetDlgFont(void)
{
	CWnd *pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		pWnd->SetFont(&m_fontDlgFont);
		pWnd = pWnd->GetNextWindow();
	}
}

void CNHCReadWriteVarDlg::InitCtrls(void)
{
	SetDlgFont();

	m_lbInfo.SetText(_T("正在通讯..."));
	m_lbInfo.SetTitleStyle();
	m_shdTitle.SetTitle(L"测功机读写变量");

	GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(FALSE);

	m_pgRealAllVar.ShowWindow(SW_HIDE);

	// 编辑框只能输入数字
	m_edNumberForRead.ModifyStyle(0, ES_NUMBER);
	m_edNumberForWrite.ModifyStyle(0, ES_NUMBER);
	m_edValue.ModifyStyle(0, ES_NUMBER);

	m_lcVar.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	// 根据1280*1024分辨率进行开发,在其它的分辨率下,要进行调整
	int nSM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
	m_lcVar.InsertColumn(0,  L"变量号",	LVCFMT_LEFT,	150*nSM_CXSCREEN/1280,		0);
	m_lcVar.InsertColumn(1,  L"变量值",	LVCFMT_LEFT,	250*nSM_CXSCREEN/1280,		1);
	m_lcVar.InsertColumn(2,  L"说明",   LVCFMT_LEFT,	550*nSM_CXSCREEN/1280,		2);
	m_lcVar.InsertColumn(3,  L"限值",   LVCFMT_LEFT,	300*nSM_CXSCREEN/1280,		3);
}

void CNHCReadWriteVarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		KillTimer(1);

		OpenSerialPortAndTestCommunication();
	}

	CDialogZoom::OnTimer(nIDEvent);
}

void CNHCReadWriteVarDlg::OpenSerialPortAndTestCommunication(void)
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// 转化为大写
	strDynProtocol.MakeUpper();
	CDynThread::InstrumentModel imDyn;
	if (L"NANHUA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}
	else if (L"MAHA" == strDynProtocol)
	{
		imDyn = CDynThread::IM_MAHA_ASMP;
	}
	//else if (L"VAMAGRSR576" == strDynProtocol)
	//{
	//	imDyn = CDynThread::IM_VAMAG_RSR576;
	//}
	else
	{
		imDyn = CDynThread::IM_NANHUA_NHC03;
	}

	if (DYN_OPEN_SUCCEED == m_pDynThread->Open(bPort, imDyn))
	{
		// 打开串口成功

		// 进行通讯测试
		// 用取状态进行通讯测试
		if (DYN_WR_SUCCEED == m_pDynThread->GetStatus(NULL))
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("读写变量"));
			GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("通讯超时"));
			GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(FALSE);
		}
	}
	else
	{
		// 打开串口失败
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextW(_T("打开通讯端口失败"));
		GetDlgItem(IDC_BUTTON_READ_VAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WRITE_VAR)->EnableWindow(FALSE);
	}
}

void CNHCReadWriteVarDlg::OnBnClickedButtonReadVar()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginWaitCursor();

	CString str(L"");
	GetDlgItem(IDC_EDIT_NUMBER_FOR_READ)->GetWindowText(str);
	if (L"" == str)
	{
		// 读全部变量

		m_pgRealAllVar.ShowWindow(SW_SHOW);
		m_pgRealAllVar.SetRange(0, 249);

		m_lcVar.SetRedraw(FALSE);
		m_lcVar.DeleteAllItems();
		for (int i=0; i<=249; i++)
		{
			// 跳过备用变量
			if (120 == i)
			{
				i = 200;
			}

			WORD wVarContent;
			if (DYN_WR_SUCCEED == m_pDynThread->ReadVar((BYTE)i, &wVarContent))
			{
				// 读变量成功
				str.Format(L"%d", i);
				int nItemIndex = m_lcVar.InsertItem(i, str);
				str.Format(L"%hu", wVarContent);
				m_lcVar.SetItemText(nItemIndex, 1, str);
			}
			else
			{
				m_lcVar.SetRedraw(TRUE);
				// 读变量异常
				MessageBoxW(L"读变量异常", L"读变量", MB_OK|MB_ICONERROR);
				break;
			}
			m_pgRealAllVar.SetPos(i);
		}
		m_lcVar.SetRedraw(TRUE);
		m_pgRealAllVar.ShowWindow(SW_HIDE);
	}
	else
	{
		// 读特定变量

		// 检查输入
		UINT un = _wtoi(str);
		if (un > 254)
		{
			MessageBoxW(L"请输入正确的变量号(0~254)", L"读变量", MB_OK|MB_ICONERROR);
			GetDlgItem(IDC_EDIT_NUMBER_FOR_READ)->SetFocus();
		}
		else if ((un>119 && un<200) || un>249)
		{
			MessageBoxW(L"此变量备用,暂不能读", L"读变量", MB_OK|MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_NUMBER_FOR_READ)->SetFocus();
		}
		else
		{
			m_lcVar.SetRedraw(FALSE);
			m_lcVar.DeleteAllItems();
			WORD wVarContent;
			if (DYN_WR_SUCCEED == m_pDynThread->ReadVar((BYTE)un, &wVarContent))
			{
				// 读变量成功
				str.Format(L"%u", un);
				int nItemIndex = m_lcVar.InsertItem(un, str);
				str.Format(L"%hu", wVarContent);
				m_lcVar.SetItemText(nItemIndex, 1, str);
			}
			else
			{
				// 读变量异常
				MessageBoxW(L"读变量异常", L"读变量", MB_OK|MB_ICONERROR);
			}
			m_lcVar.SetRedraw(TRUE);
		}
	}

	EndWaitCursor();
}

void CNHCReadWriteVarDlg::OnBnClickedButtonWriteVar()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str(L"");

	// 检查变量号
	GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->GetWindowTextW(str);
	if (L"" == str)
	{
		MessageBoxW(L"变量号不能为空", L"写变量", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->SetFocus();
		return;
	}
	UINT unNumber = _wtoi(str);
	if (unNumber > 254)
	{
		MessageBoxW(L"请输入正确的变量号(0~254)", L"读变量", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->SetFocus();
		return;
	}
	if ((unNumber>119 && unNumber<200) || unNumber>249)
	{
		MessageBoxW(L"此变量备用,暂不能写", L"写变量", MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_NUMBER_FOR_WRITE)->SetFocus();
		return;
	}

	// 检查变量值
	GetDlgItem(IDC_EDIT_VALUE)->GetWindowTextW(str);
	if (L"" == str)
	{
		MessageBoxW(L"变量值不能为空", L"写变量", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_VALUE)->SetFocus();
		return;
	}
	UINT unValue = _wtoi(str);
	if (unValue > 65535)
	{
		MessageBoxW(L"请输入正确的变量值(0~65535)", L"写变量", MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_VALUE)->SetFocus();
		return;
	}

	// 正式写变量
	BeginWaitCursor();
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
	BYTE bPort = (BYTE)_wtoi(si.GetString(L"DynParams", L"Port", L"3"));
	CString strDynProtocol = si.GetString(L"DynParams", L"Protocol", L"NANHUA");
	// 转化为大写
	strDynProtocol.MakeUpper();
	if(strDynProtocol == L"MAHA")
	{
		// 打开写保护,密码9864
		m_pDynThread->WriteVar(0 , 9864);
	}
	if (DYN_WR_SUCCEED == m_pDynThread->WriteVar((BYTE)unNumber, (WORD)unValue))
	{
		// 显示刚刚成功写入的变量
		m_lcVar.SetRedraw(FALSE);
		m_lcVar.DeleteAllItems();
		WORD wVarContent;
		if (DYN_WR_SUCCEED == m_pDynThread->ReadVar((BYTE)unNumber, &wVarContent))
		{
			// 读变量成功
			str.Format(L"%u", unNumber);
			int nItemIndex = m_lcVar.InsertItem(0, str);
			str.Format(L"%hu", wVarContent);
			m_lcVar.SetItemText(nItemIndex, 1, str);
		}
		m_lcVar.SetRedraw(TRUE);

		MessageBoxW(L"写变量成功", L"写变量", MB_OK);
	}
	else
	{
		MessageBoxW(L"写变量异常", L"写变量", MB_OK|MB_ICONERROR);
	}

	if(strDynProtocol == L"MAHA")
	{
		// 关闭写保护
		m_pDynThread->WriteVar(0, 0);
	}
	EndWaitCursor();
}

void CNHCReadWriteVarDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}
