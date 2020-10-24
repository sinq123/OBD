#include "StdAfx.h"
#include "NHSZKS.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>


CNHSZKS::CNHSZKS(void)
	: m_hComm(INVALID_HANDLE_VALUE)
	, m_bIsConnect_Serial(false)
{
}


CNHSZKS::~CNHSZKS(void)
{
	DisconnectServer();
}

DWORD CNHSZKS::ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort)
{
	return 0x15;
}

DWORD CNHSZKS::ConnectServer(const BYTE bPort, const int nBaudRate /*= CBR_19200*/)
{
	assert(0 != bPort);

	// �ر��Ѵ��ڵĴ��ھ��
	DisconnectServer();

	wchar_t *pwchPort = new wchar_t[20]();
	swprintf_s(pwchPort, 20, L"\\\\.\\COM%hhu", bPort);
	m_hComm = CreateFileW(pwchPort,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	delete[] pwchPort;
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		// �򿪴���ʧ��
		//OutputDebugStringW(L"Error:fail to open com port\n");
		m_bIsConnect_Serial = false;
		return 0x01;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���begin
	// ��ʽ��������=9600 BIT/S��1��ʼλ��1ֹͣλ��8λ���ݣ�"9600,N,8,1"
	// ��������������������С
	SetupComm(m_hComm, 3072, 1024);
	// ����DCB�ṹ
	DCB dcb;
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(m_hComm, &dcb);
	// �趨�����ʣ����ⲿ��ȡ
	dcb.BaudRate = nBaudRate;
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λΪ8
	dcb.ByteSize = 8;
	// �趨һ��ֹͣλ
	dcb.StopBits = ONESTOPBIT;
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	// ���г�ʱ����
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 50;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 100; // ���������ݿ�����Ҫ100ms�����ʱ��
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end
	m_bIsConnect_Serial = true;

	return 0x00;
}

DWORD CNHSZKS::DisconnectServer(void)
{
	DWORD dwReturn(0x00);

	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// �رմ��ڳɹ�
			dwReturn = 0x00;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			dwReturn = 0x01;
		}
	}

	m_bIsConnect_Serial = false;

	return dwReturn;
}

bool CNHSZKS::IsConnect(void) const
{
	return m_bIsConnect_Serial;
}

DWORD CNHSZKS::SetList(void)
{
	return 0x15;
}

DWORD CNHSZKS::DelList(void)
{
	return 0x15;
}

DWORD CNHSZKS::Display(const wchar_t *const pwchDisplayTxt,
		const bool bTwinking/*=false*/,
		const char chLineSpace/*='0'*/,
		const char chInDisplayMode/*=0x30*/,
		const char chOutDisplayMode/*=0x30*/,
		const char chFramePauseTime/*='3'*/,
		const char chDisplaySpeed/*='0'*/,
		const char chFontAndSize/*='5'*/,
		const char chSetupType/*='b'*/,
		const char chhAlign/*='0'*/,
		const char chvAlign/*='1'*/
		)
{
	// ����л��оͷֳ�����
	std::wstring strDisTxt(pwchDisplayTxt);
	std::vector<std::wstring> vStr;
	SplitLines(strDisTxt, vStr);

	// ����2�У�����4��
	if (vStr.size() == 0)
	{
		// ����һ������
		vStr.push_back(L"");
		vStr.push_back(L"");
	}
	else if (vStr.size() == 1)
	{
		// ���ڶ���
		vStr.push_back(L"");
	}

	// ��ʼ�������ַ���
	const int nSendBufLength(128);
	char chSendBuf[nSendBufLength] = {'\0'};
	int nBufOffset = LineToBuf(1, 1, vStr[0], chSendBuf); // ��һ��д��buf
	nBufOffset = LineToBuf(1, 2, vStr[1], chSendBuf, nBufOffset); // �ڶ���д��buf
	if (vStr.size() >= 3)
	{
		nBufOffset = LineToBuf(1, 3, vStr[2], chSendBuf, nBufOffset); // ������д��buf
	}
	if (vStr.size() >= 4)
	{
		nBufOffset = LineToBuf(1, 4, vStr[3], chSendBuf, nBufOffset); // ������д��buf
	}
	return SendToServer(chSendBuf, nBufOffset);
}

DWORD CNHSZKS::DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt)
{
	// ��ʼ�������ַ���
	const int nSendBufLength(64);
	char chSendBuf[nSendBufLength] = {'\0'};
	int nBufOffset = LineToBuf(nPanel, nLine, pwchLineTxt, chSendBuf);
	return SendToServer(chSendBuf, nBufOffset);
}

DWORD CNHSZKS::SendToServer(const char *const pchBuf, const int nLen)
{
	assert(NULL != pchBuf);
	assert(0 != nLen);

	DWORD dwNumberOfBytesToWrite(nLen), dwBytesWritten(0x00), dwReturn(0x00);

	if (!m_bIsConnect_Serial)
	{
		return 0x02;
	}

	// �����������
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	// дComm
	if (WriteFile(m_hComm, pchBuf, dwNumberOfBytesToWrite, &dwBytesWritten, NULL))
	{
		if (dwNumberOfBytesToWrite == dwBytesWritten)
		{
			dwReturn = 0x00;
		}
		else
		{
			dwReturn = 0x01;
		}
	}
	else
	{
		dwReturn = 0x01;
	}
	return dwReturn;
}

int CNHSZKS::LineToBuf(const int nPanelIdx, const int nLineIdx, const std::wstring& strLineTxt, char chSendBuf[], const int nIdx /*= 0*/)
{
	// ���ʼ����00H
	chSendBuf[nIdx] = 0x00;
	// ����>30H
	// 30H��������
	// 31H��1����
	// 32H��2����
	// 33H��3����
	chSendBuf[nIdx+1] = (char)(0x30 + nPanelIdx);
	// 31H����һ�У��ڵ�һ����ʾ16�����ַ�
	// 32H���ڶ��У��ڵڶ�����ʾ16�����ַ�
	// 33H�������У��ڵ�һ����ʾ16�����ַ�
	// 34H�������У��ڵ�һ����ʾ16�����ַ�
	chSendBuf[nIdx+2] = (char)(0x30 + nLineIdx);
	// 30H�����Ʒ�ʽ
	// 31H��������ʾ
	// 32H��������ʾ
	// 33H��������ʾ
	// 34H�����Ϸ�ҳ��������Ļ
	// 35H�����·�ҳ��������Ļ
	// 36H�����м�չ��
	// 37H�����м�ϲ�
	chSendBuf[nIdx+3] = 0x32; // ����

	if (strLineTxt.empty())
	{
		// ���ı�
		chSendBuf[nIdx+4] = 0x0D;
		return nIdx+5; // ָ����һλ
	}
	else
	{
		// ��ʾ�ı���UNICODE�ַ�->ANSI�ַ�
		const DWORD dwNum = ::WideCharToMultiByte(CP_OEMCP, NULL, strLineTxt.c_str(), -1, NULL, 0, NULL, FALSE); // dwNum����������'\0'��һλ����
		char *pchDisTxt = new char[dwNum];
		::ZeroMemory(pchDisTxt, dwNum*sizeof(char));
		::WideCharToMultiByte(CP_OEMCP, NULL, strLineTxt.c_str(), -1, pchDisTxt, dwNum, NULL, FALSE);

		// ���������Ľ�����
		::memcpy_s(&chSendBuf[nIdx+4], dwNum-1, pchDisTxt, dwNum-1);
		// 0DH������
		chSendBuf[nIdx+4+dwNum-1] = 0x0D;

		delete[] pchDisTxt;
		pchDisTxt = NULL;
		return nIdx+4+dwNum; // ָ����һλ
	}
}

void CNHSZKS::SplitLines(const std::wstring& str, std::vector<std::wstring>& vResult, const bool bKeepEnds/* = false*/)
{
	vResult.clear();
	std::wstring::size_type nLen = str.size(), nIdx(0), nBegOfLine(0), nEndOfLine(0);

	for ( ; nIdx < nLen; nBegOfLine = nIdx) 
	{
		while (nIdx < nLen && str[nIdx] != L'\n' && str[nIdx] != L'\r') nIdx++;

		nEndOfLine = nIdx;
		if (nIdx < nLen)
		{
			if (str[nIdx] == L'\r' && (nIdx+1<nLen) && str[nIdx+1] == L'\n') 
			{
				// ���з���\r\n
				nIdx += 2;
			}
			else
			{
				// ���з���\r��\n
				nIdx++;
			}
			if (bKeepEnds)
			{
				// ��ʱnEndOfLineָ��\r\n��һ���ַ�
				nEndOfLine = nIdx;
			}
		}
		vResult.push_back(str.substr(nBegOfLine, nEndOfLine - nBegOfLine));
	}

	if (nBegOfLine < nLen)
	{
		vResult.push_back(str.substr(nBegOfLine, nLen - nBegOfLine));
	}
}