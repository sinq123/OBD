// SmoThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SmoThread.h"


// CSmoThread

IMPLEMENT_DYNCREATE(CSmoThread, CWinThread)

CSmoThread::CSmoThread()
: m_bQuitThread(false)
, m_bIsQuit(FALSE)
, m_pSmo(NULL)
{
	// ���󴴽�ʱ,��ʼ���豸Ϊ:YD-1
	InitializeInstrument(IM_NANHUA_YD1);

	memset(m_fRepatData, 0, sizeof(m_fRepatData));
	memset(m_fRealData, 0, sizeof(m_fRealData));
}

CSmoThread::~CSmoThread()
{
	// ��������ʱ,ж���豸
	UninitializeInstrument();
}

BOOL CSmoThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CSmoThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSmoThread, CWinThread)
	ON_THREAD_MESSAGE(WM_SMO_REPEAT_BEGIN, OnRepeatCheck)
	ON_THREAD_MESSAGE(WM_SMO_REAL_BEGIN, OnRealCheck)
	ON_THREAD_MESSAGE(WM_SMO_REAL_END, OnRealCheck)
	ON_THREAD_MESSAGE(WM_SMO_RESET, OnReset)
	ON_THREAD_MESSAGE(WM_SMO_GET_DATA1234, OnGetData1234)
END_MESSAGE_MAP()


// CSmoThread ��Ϣ�������

BOOL CSmoThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (m_bQuitThread)
	{
		if(WM_QUIT == pMsg->message)
		{
			return CWinThread::PreTranslateMessage(pMsg);
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return CWinThread::PreTranslateMessage(pMsg);
	}
	//return CWinThread::PreTranslateMessage(pMsg);
}

void CSmoThread::QuitThread()
{
	m_bQuitThread = true;
	//PostQuitMessage(0);
	PostThreadMessage(WM_QUIT, 0, 0);
}

DWORD CSmoThread::Open(const BYTE bPort, const enum InstrumentModel im)
{
	InitializeInstrument(im);
	return m_pSmo->Open(bPort);
}

DWORD CSmoThread::Close(void)
{
	return m_pSmo->Close();
}

DWORD CSmoThread::AutoAdj(BYTE *const pbResult)
{
	return m_pSmo->AutoAdj(pbResult);
}

DWORD CSmoThread::AutoReadData(BYTE *const pbResult)
{
	return m_pSmo->AutoReadData(pbResult);
}


DWORD CSmoThread::AllowTest(BYTE *const pbResult)
{

	return m_pSmo->AllowTest(pbResult);
}

DWORD CSmoThread::Reset(BYTE *const pbResult)
{
	return m_pSmo->Reset(pbResult);
}


DWORD CSmoThread::StartAutoTest(BYTE *const pbResult)
{
	return m_pSmo->StartAutoTest(pbResult);
}


DWORD CSmoThread::GetData1(BYTE *const pbResult,float *const pfData1)
{
	return m_pSmo->GetData1(pbResult,pfData1);
}


DWORD CSmoThread::GetData1234(BYTE  *const pbResult, float *const pfData1, float *const pfData2,  float *const pfData3,  float *const pfData4)
{
	return m_pSmo->GetData1234(pbResult,pfData1,pfData2,pfData3,pfData4);
}


DWORD CSmoThread::GetTestState(BYTE *const pbResult,  BYTE *const pbState)
{
	return m_pSmo->GetTestState(pbResult,pbState);
}


DWORD CSmoThread::StartRealTest(BYTE *const pbResult)
{
	return m_pSmo->StartRealTest(pbResult);
}


DWORD CSmoThread::GetSign(BYTE *const pbResult)
{
	return m_pSmo->GetSign(pbResult);
}

DWORD CSmoThread::QuitAutoTest(BYTE *const pbResult)
{
	return m_pSmo->QuitAutoTest(pbResult);
}

void CSmoThread::OnRepeatCheck(WPARAM wParam, LPARAM lParam)
{
	HWND hCallBackWnd;
	hCallBackWnd=(HWND)wParam;
	BYTE  statBye;
	//HardReset(&statBye); // ����Ӳ��λ
	Reset(&statBye);//��λ
	Sleep(500);
	for(int i=1;i<6;i++)
	{
		AutoReadData(&statBye);      //��һ�η���9c�Զ����
		Sleep(2000);
		AutoReadData(&statBye);     //�ڶ��η���9c�Զ����
		Sleep(2000);
		/////ȡ��������
		GetData1234(&statBye,&m_fRepatData[i],&m_fRepatData[i],&m_fRepatData[i],&m_fRepatData[i]);
		Sleep(500);
		//////////�˳��Զ����
		QuitAutoTest(&statBye);
		Sleep(500);
		///////��������λ����
		Reset(&statBye);
		Sleep(500);
		///���͵�n���Զ�������
		// wparamΪ������
		SendMessage(hCallBackWnd,WM_SMO_REPEAT_END,WPARAM(i),NULL);
		if(m_bIsQuit)
		{
			break;
		}
	}
}

void CSmoThread::OnRealCheck(WPARAM wParam,LPARAM lParam)
{
	HWND hCallBackWnd;
	int  iCycle;            //��ʶ�˵ڼ��μ������
	iCycle = (int)lParam;
	hCallBackWnd = (HWND)wParam;
	BYTE  statBye;
	//HardReset(&statBye); // ����Ӳ��λ
	Reset(&statBye);//��λ
	Sleep(500);
	for(int i =1; i<4;i++)
	{
		AutoReadData(&statBye);//��һ�η���9c�Զ����
		Sleep(2000);
		AutoReadData(&statBye);//�ڶ��η���9c�Զ����
		Sleep(2000);
		GetData1234(&statBye,&m_fRealData[iCycle][i],&m_fRealData[iCycle][i],&m_fRealData[iCycle][i],&m_fRealData[iCycle][i]);
		//////////�˳��Զ����
		QuitAutoTest(&statBye);
		Sleep(500);
		///////��������λ����
		Reset(&statBye);
		Sleep(500);
		//wParam ��ʾ�˼�������lParam ��ʾ�˵ڼ��μ��
		SendMessage(hCallBackWnd,WM_SMO_REAL_END,WPARAM(i),lParam);
		if(m_bIsQuit)
		{
			break;
		}
	}
}

void CSmoThread::Reset_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_SMO_RESET, 0, (LPARAM)hCallBackWnd);
}

void CSmoThread::OnReset(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult;
	DWORD dw = m_pSmo->Reset(&bResult);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_SMO_RESET, (WPARAM)dw, (LPARAM)bResult);
	}
}

void CSmoThread::GetData1234_Asyn(HWND hCallBackWnd)
{
	PostThreadMessage(WM_SMO_GET_DATA1234, 0, (LPARAM)hCallBackWnd);
}

void CSmoThread::OnGetData1234(WPARAM wParam, LPARAM lParam)
{
	BYTE bResult;
	DWORD dw = m_pSmo->GetData1234(&bResult,&m_fData1,&m_fData2,&m_fData3,&m_fData4);
	if (NULL != (HWND)lParam)
	{
		::PostMessage((HWND)lParam, WM_SMO_GET_DATA1234, (WPARAM)dw, (LPARAM)bResult);
	}
}

DWORD CSmoThread::HardReset(BYTE *const pbResult)
{
	return m_pSmo->HardReset(pbResult);
}

void CSmoThread::InitializeInstrument(const enum InstrumentModel im)
{
	UninitializeInstrument();

	if (IM_NANHUA_YD1 == im)
	{
		m_pSmo = new CNanhuaYd1();
	}
	else if (IM_FOFEN_FBY201 == im)
	{
		m_pSmo = new CFoFenFBY201();
	}
	else
	{
		// ��ƥ��
		m_pSmo = new CNanhuaYd1();
	}
}

void CSmoThread::UninitializeInstrument(void)
{
	if (NULL != m_pSmo)
	{
		delete m_pSmo;
		m_pSmo = NULL;
	}
}