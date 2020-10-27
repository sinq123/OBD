
// MFCDAFDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCDAF.h"
#include "MFCDAFDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDAFDlg �Ի���




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


// CMFCDAFDlg ��Ϣ�������

BOOL CMFCDAFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_reLog.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PICTURE)->ShowWindow(SW_HIDE);
	// �����϶��ļ�
	DragAcceptFiles(true);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCDAFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCDAFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCDAFDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	UINT count;
	TCHAR filePath[MAX_PATH] = { 0 };
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//�ӳɹ����ϷŲ����м����ļ������ơ���ȡ������ק�ļ�����Ŀ

	CString strPath;
	if (count == 1)//���ֻ��קһ���ļ���
	{
		DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));//�����ק���ļ���
		strPath.Format(L"%s,",filePath);
		UpdateData(FALSE);
		DragFinish(hDropInfo);//�Ϸųɹ����ͷ��ڴ�
	}
	else//�����ק����ļ���
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strPath(L"");
	m_img = cv::Mat();
	m_edFilePath.GetWindowTextW(strPath);

	CString strMsg;
	if (strPath.GetLength() >= 3 && strPath.Find(L".cer") != std::string::npos)
	{
		m_reLog.ShowWindow(SW_SHOW);
		bool bRet(false);
		strMsg.AppendFormat(L"�ļ�·��:%s\r\n\r\n", strPath);
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
				strMsg.AppendFormat(L"����:%s\r\n", L"��֤��ʧ��");
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
			strMsg.AppendFormat(L"����:%s\r\n", L"ȱ��֤��(�ļ��쳣)");
		}


		if (bRet)
		{
			byte * pbDeData = new byte[unDataLen+1]();

			// ����
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
			strMsg.AppendFormat(L"֤���:%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 1, L'\n');
			strMsg.AppendFormat(L"����֤��PC:%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 2, L'\n');
			strMsg.AppendFormat(L"����֤������:%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 3, L'\n');
			strMsg.AppendFormat(L"����֤�鵽������(δ��):%s\r\n\r\n", str);

			str.Empty();
			AfxExtractSubString(str, strCerData, 4, L'\n');
			strMsg.AppendFormat(L"��¼����:%s\r\n\r\n", str);

			str.Empty();
			CString strTemp;
			AfxExtractSubString(str, strCerData, 5, L'\n');
			if (str.Find(L"1") != -1)
			{
				strTemp.AppendFormat(L"OBD��⣬");
			}
			if (str.Find(L"2") != -1)
			{
				strTemp.AppendFormat(L"�Լ��ϴ���");
			}

			if (strTemp.IsEmpty())
			{
				strMsg.AppendFormat(L"ʹ������:%s\r\n\r\n", str);
			}
			else
			{
				// ɾ�����һ���ַ�
				strTemp.Delete(strTemp.GetLength()-1, 1);
				strMsg.AppendFormat(L"ʹ������:%s\r\n\r\n", strTemp);
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
		strMsg.AppendFormat(L"�ļ�·��:%s\r\n\r\n", strPath);
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
				strMsg.AppendFormat(L"����:%s\r\n\r\n", L"���ļ�ʧ��");
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
			strMsg.AppendFormat(L"����:%s\r\n", L"ȱ��֤��(�ļ��쳣)");
		}

		if (bRet)
		{
			strTemp.Format(_T("%s"), pbEnData);
			if (!ToUnicode(strTemp))
			{
				CString strCerData = KANSIToUTF16((char*)pbEnData);
				strTemp.Format(L"%s", strCerData);
			}
			strMsg.AppendFormat(L"����:\r\n%s", strTemp);
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
		strMsg.AppendFormat(L"��֤�������ļ�\r\n");
	}
	PrintLog(strMsg);
}

void CMFCDAFDlg::PrintLog(LPCTSTR szFormat, ...)
{
	//::EnterCriticalSection(&m_csLog);	// �������������̵߳ȴ�ʱ�������̲߳��ܶ�m_reLog���в���
	
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

	GetDlgItem(nID)->GetClientRect(&rect);// ��ȡ�ؼ���С

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
