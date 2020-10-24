#include "StdAfx.h"
#include "NH50R.h"
#include <cassert>
#include <cstdio>
#include <stdlib.h>

CNH50R::CNH50R(void)
: m_socket(INVALID_SOCKET)
, m_hComm(INVALID_HANDLE_VALUE)
, m_nSendBufLength(1024)
, m_pchSendBuf(NULL)
, m_bIsConnect_Serial(false)
, m_bIsConnect_Ethernet(false)
{
	// �ڶ��з����ڴ�
	m_pchSendBuf = new char[m_nSendBufLength]();
	if (NULL != m_pchSendBuf)
	{
		memset(m_pchSendBuf, '\0', m_nSendBufLength);
	}
}

CNH50R::~CNH50R(void)
{
	// ���ٶ���ʱ,�Ͽ�����
	DisconnectServer();

	// �ڶ��л����ڴ�
	if (NULL != m_pchSendBuf)
	{
		delete[] m_pchSendBuf;
		m_pchSendBuf = NULL;
	}
}

DWORD CNH50R::ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort)
{
	assert(NULL != pwchIP);

	// ����Ѿ�������,�Ͽ�����
	DisconnectServer();

	// ��ʼ��Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
	{
		return 0x01;
	}

	// ȷ��WinSock DLL֧��V2.2�汾
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2 )
	{
		    WSACleanup( );
			return 0x01; 
	}


	// ����socket.
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_socket) 
	{
		WSACleanup();
		return 0x01;
	}

	// IP��ַ��UNICODE�ַ�->ANSI�ַ�
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pwchIP, -1, NULL, 0, NULL, FALSE);
	char *pchIP = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, pwchIP, -1, pchIP, dwNum, NULL, FALSE);

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(pchIP);
	clientService.sin_port = htons(usPort);

	delete []pchIP;

	// ���ӷ�����
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		m_bIsConnect_Ethernet = false;
		WSACleanup();
		return 0x01;
	}
	else
	{
		m_bIsConnect_Ethernet = true;
	}

	return 0x00;
}

DWORD CNH50R::ConnectServer(const BYTE bPort, const int nBaudRate)
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
	// ���г�ʱ���ã�100����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 50;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;// ���������ݿ�����Ҫ1s�����ʱ��
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end
	m_bIsConnect_Serial = true;

	return 0x00;
}

DWORD CNH50R::DisconnectServer(void)
{
	DWORD dwReturn(0x00);

	if (INVALID_SOCKET != m_socket)
	{
		closesocket(m_socket); 
		m_socket = INVALID_SOCKET;
		dwReturn = 0x00;
		WSACleanup();
	}

	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (!b)
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			dwReturn = 0x01;
		}
	}

	m_bIsConnect_Serial = false;
	m_bIsConnect_Ethernet = false;

	return dwReturn;
}

DWORD CNH50R::SetList(void)
{
	// ���ͳ���,15�ֽ�
	const int nSendLength(15);

	// �趨�����б�Ϊ:E:\T\NH
	m_pchSendBuf[0] = 0x01;
	m_pchSendBuf[1] = 'Z';
	m_pchSendBuf[2] = '0';
	m_pchSendBuf[3] = '0';
	m_pchSendBuf[4] = 0x02;
	m_pchSendBuf[5] = 'E';
	m_pchSendBuf[6] = '.';
	m_pchSendBuf[7] = 'S';
	m_pchSendBuf[8] = 'L';
	m_pchSendBuf[9] = 0x0F;
	m_pchSendBuf[10] = 'E';
	m_pchSendBuf[11] = 'T';
	m_pchSendBuf[12] = 'N';
	m_pchSendBuf[13] = 'H';
	m_pchSendBuf[14] = 0x04;

	//DWORD dwReturn(0x00);
	//if (send(m_socket, m_pchSendBuf, nSendLength, 0) <= 0)
	//{
	//	dwReturn = 0x01;
	//}
	//else
	//{
	//	dwReturn = 0x00;
	//}

	//return dwReturn;

	return SendToServer(m_pchSendBuf, nSendLength);
}

bool CNH50R::IsConnect(void) const
{
	return m_bIsConnect_Serial||m_bIsConnect_Ethernet;
}

DWORD CNH50R::DelList(void)
{
	// ���ͳ���,8�ֽ�
	const int nSendLength(8);

	m_pchSendBuf[0] = 0x01;
	m_pchSendBuf[1] = 'Z';
	m_pchSendBuf[2] = '0';
	m_pchSendBuf[3] = '0';
	m_pchSendBuf[4] = 0x02;
	m_pchSendBuf[5] = 'E';
	m_pchSendBuf[6] = '.';
	m_pchSendBuf[7] = 0x04;

	//DWORD dwReturn(0x00);
	//if (send(m_socket, m_pchSendBuf, nSendLength, 0) <= 0)
	//{
	//	dwReturn = 0x01;
	//}
	//else
	//{
	//	dwReturn = 0x00;
	//}

	//return dwReturn;
	return SendToServer(m_pchSendBuf, nSendLength);
}

DWORD CNH50R::Display(const wchar_t *const pwchDisplayTxt, 
					  const bool bTwinking,
					  const char chLineSpace,
					  const char chInDisplayMode,
					  const char chOutDisplayMode,
					  const char chFramePauseTime,
					  const char chDisplaySpeed,
					  const char chFontAndSize,
					  const char chSetupType,
					  const char chhAlign,
					  const char chvAlign)
{
	///////////////////////////////////////////�ӿ���ʾ�ٶ�(��������������)
	//assert(NULL != pwchDisplayTxt);
	//assert('0'==chFramePauseTime ||
	//	'1'==chFramePauseTime ||
	//	'2'==chFramePauseTime ||
	//	'3'==chFramePauseTime ||
	//	'4'==chFramePauseTime ||
	//	'5'==chFramePauseTime ||
	//	'6'==chFramePauseTime ||
	//	'7'==chFramePauseTime ||
	//	'8'==chFramePauseTime ||
	//	'9'==chFramePauseTime);
	///////////////////////////////////////////�ӿ���ʾ�ٶ�(��������������)

	int nIndex(0);

	// д�ļ���E:\T\NH
	m_pchSendBuf[nIndex++] = 0x01;
	m_pchSendBuf[nIndex++] = 'Z';
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = 0x02;
	m_pchSendBuf[nIndex++] = 'A';
	m_pchSendBuf[nIndex++] = 0x0F;
	m_pchSendBuf[nIndex++] = 'E';
	m_pchSendBuf[nIndex++] = 'T';
	m_pchSendBuf[nIndex++] = 'N';
	m_pchSendBuf[nIndex++] = 'H';

	// Э����Ʒ�
	m_pchSendBuf[nIndex++] = 0x06;

	/////////////////////////////////////////////////////////////////////////////////////////////��ʾ���Ʒ�begin
	if (bTwinking)
	{
		// ��˸���Ʒ�
		m_pchSendBuf[nIndex++] = 0x07;
		m_pchSendBuf[nIndex++] = '1';
	}

	// �м�����
	m_pchSendBuf[nIndex++] = 0x08;
	//m_pchSendBuf[nIndex++] = '0';  // 0�м��
	m_pchSendBuf[nIndex++] = chLineSpace;  // �м��

	// �������Ʒ�	
	m_pchSendBuf[nIndex++] = 0x0A;
	m_pchSendBuf[nIndex++] = 'I';
	m_pchSendBuf[nIndex++] = chInDisplayMode;  // �뻨��
	m_pchSendBuf[nIndex++] = 0x0A;
	m_pchSendBuf[nIndex++] = 'O';
	m_pchSendBuf[nIndex++] = chOutDisplayMode;  // ������

	// �����ַ�

	// ֡ͣ��ʱ��
	m_pchSendBuf[nIndex++] = 0x0E;
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = chFramePauseTime;  // ͣ������

	// �ٶ�
	m_pchSendBuf[nIndex++] = 0x0F;
	//m_pchSendBuf[nIndex++] = '0';  // ���
	m_pchSendBuf[nIndex++] = chDisplaySpeed;

	// ���弰��С
	m_pchSendBuf[nIndex++] = 0x1A;
	//m_pchSendBuf[nIndex++] = '5';  // 16*16 ��׼��������
	m_pchSendBuf[nIndex++] = chFontAndSize;
	
	// ����ģʽ
	m_pchSendBuf[nIndex++] = 0x1B;
	m_pchSendBuf[nIndex++] = '0';
	//m_pchSendBuf[nIndex++] = 'b';  // a:���Զ��Ű�,b:Ĭ�ϴ���ģʽ,�Զ�����,�Զ��Ű�
	m_pchSendBuf[nIndex++] = chSetupType;

	//// ������ɫ
	//m_pchSendBuf[nIndex++] = 0x1C;
	//m_pchSendBuf[nIndex++] = 0x31;

	//// ���屳����ɫ
	//m_pchSendBuf[nIndex++] = 0x1D;
	//m_pchSendBuf[nIndex++] = 0x30; // ��ɫ

	// ˮƽ����
	m_pchSendBuf[nIndex++] = 0x1E;
	m_pchSendBuf[nIndex++] = chhAlign;  // 0:����,1:����,2:����

	// ��ֱ����
	m_pchSendBuf[nIndex++] = 0x1F;
	m_pchSendBuf[nIndex++] = chvAlign;  // 0,����,1:����,2:����
	/////////////////////////////////////////////////////////////////////////////////////////////��ʾ���Ʒ�end

	if (NULL != pwchDisplayTxt)
	{
		// ��ʾ�ı���UNICODE�ַ�->ANSI�ַ�
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pwchDisplayTxt, -1, NULL, 0, NULL, FALSE);
		char *pchDisplayTxt = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, pwchDisplayTxt, -1, pchDisplayTxt, dwNum, NULL, FALSE);

		for (size_t st=0; st<strlen(pchDisplayTxt); st++)
		{
			if ('\n' == pchDisplayTxt[st])
			{
				// ����
				m_pchSendBuf[nIndex++] = 0x0d;
			}
			else if ('\f' == pchDisplayTxt[st])
			{
				// ��֡
				m_pchSendBuf[nIndex++] = 0x0c;
			}
			else
			{
				m_pchSendBuf[nIndex++] = pchDisplayTxt[st];
			}

			if (nIndex >= (m_nSendBufLength-1))
			{
				// ���ﻺ��������m_nSendBufLength=1024Bytes
				// �����ַ�������ʾ
				break;
			}

		}  // for (size_t st=0; st<strlen(pchDisplayTxt); st++)

		delete []pchDisplayTxt;

	}  // if (NULL != pwchDisplayTxt)

	// ���������
	m_pchSendBuf[nIndex++] = 0x04;

	//DWORD dwReturn(0x00);
	//if (send(m_socket, m_pchSendBuf, nIndex, 0) <= 0)
	//{
	//	dwReturn = 0x01;
	//}
	//else
	//{
	//	dwReturn = 0x00;
	//}

	//return dwReturn;

	return SendToServer(m_pchSendBuf, nIndex);
}

DWORD CNH50R::DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt)
{
	return 0x15;
}

DWORD CNH50R::SendToServer(const char *const pchBuf, const int nLen)
{
	assert(NULL != pchBuf);
	assert(nLen > 0);

	DWORD dwReturn(0x00);
	if (m_bIsConnect_Ethernet)
	{	
		if (send(m_socket, pchBuf, nLen, 0) <= 0)
		{
			dwReturn = 0x01;
		}
		else
		{
			dwReturn = 0x00;
		}
	}
	else if (m_bIsConnect_Serial)
	{
		DWORD dwNumberOfBytesToWrite(nLen), dwBytesWritten(0x00);

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
	}
	else
	{
		dwReturn = 0x02;
	}

	return dwReturn;
}
