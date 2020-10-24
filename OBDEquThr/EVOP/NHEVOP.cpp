#include "StdAfx.h"
#include "NHEVOP.h"

#include <vector>
#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

#include "..\..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

CNHEVOP::CNHEVOP(void)
{
	srand(unsigned int(time(NULL)));

}

CNHEVOP::~CNHEVOP(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();
}

DWORD CNHEVOP::Open(BYTE bPort, const int nBaudRate)
{
	assert(0 != bPort);

	// �ر��Ѵ��ڵĴ��ھ��
	Close();

	wchar_t szPort[MAX_PATH] = {0};
	swprintf_s(szPort, 20, L"\\\\.\\COM%hhu", bPort);
	m_hComm = CreateFileW(szPort,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		// �򿪴���ʧ��
		OutputDebugStringW(L"Error:fail to open com port\n");
		return COM_OPEN_FAIL;
	}

	///////////////////////////////////////////// ��ʼ�����ڲ���begin
	// ��������������������С
	SetupComm(m_hComm, 512, 256);
	// ����DCB�ṹ
	DCB dcb;
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(m_hComm, &dcb);
	// �趨�����ʣ����ⲿ��ȡ
	if (nBaudRate != NULL)
	{
		dcb.BaudRate = nBaudRate;
	}
	else
	{
		dcb.BaudRate = CBR_9600;
	}
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λ
	dcb.ByteSize = 8;
	// �趨ֹͣλ
	dcb.StopBits = ONESTOPBIT;
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	// ���г�ʱ����
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 500;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////// ��ʼ�����ڲ���end
	
	return COM_OPEN_SUCCEED;
}

DWORD CNHEVOP::Close(void)
{
	return CEVOP::Close();
}

DWORD CNHEVOP::SendCMD(NHEVOPPacket sNHPSend, NHEVOPPacket &sNHPRecv)
{
	// �Զ���
	CAutoLock autoLock(GetLogLocker());

	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	
	std::string strSend = sNHPSend.ToString();
	std::string strRecv;
	if (WriteCommBuffer((BYTE *)strSend.data(), strSend.size()))
	{
		char szRecv[MAX_PATH] = {0};
		
		if (ReadCommBuffer((BYTE *)szRecv, 4))
		{
			strRecv.append((char *)szRecv, 4);
			
			char szPacketLen[3] = {0};
			szPacketLen[0] = szRecv[2];
			szPacketLen[1] = szRecv[3];
			int nPacketLen = atoi(szPacketLen);

			DWORD dwLeft = nPacketLen + 3;
			if (ReadCommBuffer((BYTE *)szRecv+4, dwLeft))
			{
				strRecv.append((char *)szRecv+4, dwLeft);				
				
				if (sNHPRecv.Prase(strRecv))
				{
					TRACE("NHEVOP SendCMD recv [%d] data ", sNHPRecv.strData.size());
					for (WORD i=0; i<sNHPRecv.strData.size(); i++)
					{
						TRACE("%02X ", (BYTE)sNHPRecv.strData.at(i));
					}
					TRACE("\n");

					return COM_WR_SUCCEED;
				}
				else
				{
					return COM_WR_CHECK_SUM_FAIL;
				}
			}
			else
			{
				return COM_WR_READ_SPECIFIED_DATA_FAIL;
			}
		}
		else
		{
			return COM_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		return COM_WR_WRITE_SPECIFIED_DATA_FAIL;
	}
}

DWORD CNHEVOP::StartTest(const int nItem, const int nType)
{
	NHEVOPPacket sNHPSend('T');
	sNHPSend.strData += (char)(nItem+'0');
	sNHPSend.strData += (char)(nType+'0');
	NHEVOPPacket sNHPRecv;

	DWORD dwRet = SendCMD(sNHPSend, sNHPRecv);
	return dwRet;
}

DWORD CNHEVOP::GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T)
{
	NHEVOPPacket sNHPSend('G');
	NHEVOPPacket sNHPRecv;
	
	DWORD dwRet = SendCMD(sNHPSend, sNHPRecv);
	if (sNHPRecv.strData.size() == 8)
	{
	}
	return dwRet;
}

DWORD CNHEVOP::GetState(int &nState)
{
	NHEVOPPacket sNHPSend('S');
	NHEVOPPacket sNHPRecv;
	
	DWORD dwRet = SendCMD(sNHPSend, sNHPRecv);
	if (sNHPRecv.strData.size() == 1)
	{
		nState = sNHPRecv.strData.at(0) - '0';
	}
	return dwRet;
}

DWORD CNHEVOP::GetTestInfo(int &nTestState, int &nTimeCount)
{
	NHEVOPPacket sNHPSend('M');
	NHEVOPPacket sNHPRecv;
	
	DWORD dwRet = SendCMD(sNHPSend, sNHPRecv);
	if (sNHPRecv.strData.size() >= 1)
	{
		nTestState = sNHPRecv.strData.at(0) - '0';
		if ((nTestState==2 || nTestState==6) && sNHPRecv.strData.size()>=3)
		{
			char szTimeCount[3] = {0};
			szTimeCount[0] = sNHPRecv.strData.at(1);
			szTimeCount[1] = sNHPRecv.strData.at(2);
			nTimeCount = atoi(szTimeCount);
		}
		else if ((nTestState==8) && sNHPRecv.strData.size()>=4)
		{
			char szTimeCount[4] = {0};
			szTimeCount[0] = sNHPRecv.strData.at(1);
			szTimeCount[1] = sNHPRecv.strData.at(2);
			szTimeCount[2] = sNHPRecv.strData.at(3);
			nTimeCount = atoi(szTimeCount);
		}
		else
		{
			nTimeCount = 0;
		}
	}
	return dwRet;
}

DWORD CNHEVOP::EndTest(int &nItem, int &nJudge)
{
	NHEVOPPacket sNHPSend('U');
	NHEVOPPacket sNHPRecv;
	
	DWORD dwRet = SendCMD(sNHPSend, sNHPRecv);
	if (sNHPRecv.strData.size() == 2)
	{
		nItem = sNHPRecv.strData.at(0) - '0';
		nJudge = sNHPRecv.strData.at(1) - '0';
	}
	return dwRet;
}