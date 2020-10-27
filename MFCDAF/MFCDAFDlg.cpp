
// MFCDAFDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDAF.h"
#include "MFCDAFDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDAFDlg 对话框




CMFCDAFDlg::CMFCDAFDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDAFDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMFCDAFDlg::~CMFCDAFDlg()
{
}

void CMFCDAFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_FILEPATH, m_edFilePath);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_reLog);
}

BEGIN_MESSAGE_MAP(CMFCDAFDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDC_ED_FILEPATH, &CMFCDAFDlg::OnEnChangeEdFilepath)
END_MESSAGE_MAP()


// CMFCDAFDlg 消息处理程序

BOOL CMFCDAFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_reLog.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PICTURE)->ShowWindow(SW_HIDE);
	// 允许拖动文件
	DragAcceptFiles(true);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDAFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDialogEx::UpdateWindow();
		if (m_img.data)
		{
			DrawMat(m_img, IDC_PICTURE);
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDAFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCDAFDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT count;
	TCHAR filePath[MAX_PATH] = { 0 };
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//从成功的拖放操作中检索文件的名称。并取代被拖拽文件的数目

	CString strPath;
	if (count == 1)//如果只拖拽一个文件夹
	{
		DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));//获得拖拽的文件名
		strPath.Format(L"%s,",filePath);
		UpdateData(FALSE);
		DragFinish(hDropInfo);//拖放成功后，释放内存
	}
	else//如果拖拽多个文件夹
	{
		for (UINT i = 0; i<count; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			strPath.AppendFormat(L"%s,",filePath);
		}

		UpdateData(FALSE);
		DragFinish(hDropInfo);
	}
	strPath.Delete(strPath.GetLength()-1, 1);
	m_reLog.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PICTURE)->ShowWindow(SW_HIDE);
	m_edFilePath.SetWindowTextW(strPath);
	CDialogEx::OnDropFiles(hDropInfo);
}

bool ToUnicode(CString& strTemp)
{
	bool bRet(false);
	int nlength = strTemp.GetLength();
	for (int i=0; i < nlength; i++)
	{
		int num = strTemp.GetAt(i);
		if ((num >= 'a' && num <='z') ||
			(num >= 'A' && num <= 'Z') ||
			(num >= '0' && num <= '9')
			)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}

void CMFCDAFDlg::OnEnChangeEdFilepath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strPath(L"");
	m_img = cv::Mat();
	m_edFilePath.GetWindowTextW(strPath);

	CString strMsg;
	if (strPath.GetLength() >= 3 && strPath.Find(L".cer") != std::string::npos)
	{
		m_reLog.ShowWindow(SW_SHOW);
		bool bRet(false);
		strMsg.AppendFormat(L"文件路径:%s\r\n\r\n", strPath);
		byte * pbEnData(NULL);
		UINT unDataLen(0);
		try
		{
			CFile pFile;
			if (pFile.Open(strPath, CFile::modeRead|CFile::shareDenyNone))
			{
				unDataLen = (UINT)pFile.GetLength();
				pbEnData = new byte[unDataLen]();
				pFile.Read(pbEnData, unDataLen);

				bRet = true;
			}
			else
			{
				strMsg.AppendFormat(L"问题:%s\r\n", L"打开证书失败");
			}
			pFile.Close();
		}
		catch (CFileException ex)
		{
			if (NULL != pbEnData)
			{
				delete[] pbEnData;
				pbEnData = NULL;
			}
			strMsg.AppendFormat(L"问题:%s\r\n", L"缺少证书(文件异常)");
		}


		if (bRet)
		{
			byte * pbDeData = new byte[unDataLen+1]();

			// 解密
			CAes Aes;
			Aes.Decryption(pbEnData, pbDeData, unDataLen);
			CString strCerData = KANSIToUTF16((char*)pbDeData);

			if (NULL != pbDeData)
			{
				delete[] pbDeData;
				pbDeData = NULL;
			}

			CString str(L"");
			AfxExtractSubString(str, strCerData, 0, L'\n');
			strMsg.AppendFormat(L"证书号:%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 1, L'\n');
			strMsg.AppendFormat(L"制作证书PC:%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 2, L'\n');
			strMsg.AppendFormat(L"制作证书日期:%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 3, L'\n');
			strMsg.AppendFormat(L"制作证书到期日期(未用):%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 4, L'\n');
			strMsg.AppendFormat(L"登录密码:%s\r\n\r\n", str);

			str.Empty();
			CString strTemp;
			AfxExtractSubString(str, strCerData, 5, L'\n');
			if (str.Find(L"1") != -1)
			{
				strTemp.AppendFormat(L"OBD检测，");
			}
			if (str.Find(L"2") != -1)
			{
				strTemp.AppendFormat(L"自检上传，");
			}

			if (strTemp.IsEmpty())
			{
				strMsg.AppendFormat(L"使用类型:%s\r\n\r\n", str);
			}
			else
			{
				// 删除最后一个字符
				strTemp.Delete(strTemp.GetLength()-1, 1);
				strMsg.AppendFormat(L"使用类型:%s\r\n\r\n", strTemp);
			}
		}

		if (NULL != pbEnData)
		{
			delete[] pbEnData;
			pbEnData = NULL;
		}
	}
	else if (strPath.GetLength() >= 3 && 
		(strPath.Find(L".ini") != std::string::npos || strPath.Find(L".txt") != std::string::npos || strPath.Find(L".log") != std::string::npos)
		)
	{
		m_reLog.ShowWindow(SW_SHOW);
		CString strTemp;
		bool bRet(false);
		strMsg.AppendFormat(L"文件路径:%s\r\n\r\n", strPath);
		byte * pbEnData(NULL);
		UINT unDataLen(0);
		try
		{
			CFile pFile;
			if (pFile.Open(strPath, CFile::modeRead|CFile::shareDenyNone))
			{
				unDataLen = (UINT)pFile.GetLength();
				pbEnData = new byte[unDataLen]();
				pFile.Read(pbEnData, unDataLen);
				//pFile.Read(strTemp.GetBuffer(pFile.GetLength()),pFile.GetLength());
				bRet = true;
			}
			else
			{
				strMsg.AppendFormat(L"问题:%s\r\n\r\n", L"打开文件失败");
			}
			pFile.Close();
		}
		catch (CFileException& ex)
		{
			if (NULL != pbEnData)
			{
				delete[] pbEnData;
				pbEnData = NULL;
			}
			strMsg.AppendFormat(L"问题:%s\r\n", L"缺少证书(文件异常)");
		}

		if (bRet)
		{
			strTemp.Format(_T("%s"), pbEnData);
			if (!ToUnicode(strTemp))
			{
				CString strCerData = KANSIToUTF16((char*)pbEnData);
				strTemp.Format(L"%s", strCerData);
			}
			strMsg.AppendFormat(L"内容:\r\n%s", strTemp);
		}

		if (NULL != pbEnData)
		{
			delete[] pbEnData;
			pbEnData = NULL;
		}
	}
	else if (strPath.GetLength() >= 3 && 
		(strPath.Find(L".jpg") != std::string::npos || strPath.Find(L".png") != std::string::npos || 
		strPath.Find(L".gif") != std::string::npos || strPath.Find(L".bmp") != std::string::npos ||
		strPath.Find(L".JPG") != std::string::npos)
		)
	{
		
		GetDlgItem(IDC_PICTURE)->ShowWindow(SW_SHOW);

		std::string sPath(CW2A(strPath.GetString()));
		m_img = cv::imread(sPath,1);

		DrawMat(m_img, IDC_PICTURE);
	}
	else
	{
		m_reLog.ShowWindow(SW_SHOW);
		strMsg.AppendFormat(L"非证书类型文件\r\n");
	}
	PrintLog(strMsg);
}

void CMFCDAFDlg::PrintLog(LPCTSTR szFormat, ...)
{
	//::EnterCriticalSection(&m_csLog);	// 会死锁，界面线程等待时，其他线程不能对m_reLog进行操作
	
	m_reLog.SetSel(0, -1);
	m_reLog.Clear();

	TCHAR szBuf[4096] = {0};
	va_list args; 
	va_start(args, szFormat); 
	_vstprintf(szBuf, szFormat, args);
	va_end(args); 

	CString strLog;
	strLog.AppendFormat(_T("%s\r\n"), szBuf);
		
	m_reLog.LineScroll(m_reLog.GetLineCount());
	int nLen = m_reLog.GetWindowTextLength();
	m_reLog.SetSel(nLen, nLen);
	
	m_reLog.ReplaceSel(strLog);

	//::LeaveCriticalSection(&m_csLog);
}


void CMFCDAFDlg::DrawMat(cv::Mat& img, UINT nID)
{
	cv::Mat imgTmp;
	CRect rect;

	GetDlgItem(nID)->GetClientRect(&rect);// 获取控件大小

	cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));

	switch (imgTmp.channels())
	{
	case 1:{cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA);}break;
	case 3:{cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);}break;
	default:{}break;
	}


	int pixelBytes = imgTmp.channels() *(imgTmp.depth()+1);
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = GetDlgItem(nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
		);
	ReleaseDC(pDC);

}
