#include "StdAfx.h"
#include "NanhuaNhc03.h"

#include <cstdio>
#include <cassert>
#include <cmath>

CNanhuaNhc03::CNanhuaNhc03(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&cInfoSection);
}

CNanhuaNhc03::~CNanhuaNhc03(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();
	::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaNhc03::Open(const BYTE bPort)
{
	// �ر��Ѵ��ڵĴ��ھ��
	Close();

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
		OutputDebugStringW(L"Error:fail to open com port\n");
		// �򿪴���ʧ��
		return DYN_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���begin
	// ��������������������С
	SetupComm(m_hComm, 512, 256);
	// ����DCB�ṹ
	DCB dcb;
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(m_hComm, &dcb);
	// �趨������Ϊ9600 bps
	dcb.BaudRate = CBR_9600;
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
	CommTimeouts.ReadIntervalTimeout = 500;  // �����10,ȡʵʱ���ݳ�ʱ
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	// �򿪴��ڳɹ�
	return DYN_OPEN_SUCCEED;
}

// �رմ���
DWORD CNanhuaNhc03::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// �رմ��ڳɹ�
			return DYN_CLOSE_SUCCEED;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32 APIʧ��
			return DYN_CLOSE_FAIL_WIN32API;
		}
	}
	else
	{
		// �رմ���ʧ�ܣ�����û�гɹ���
		return DYN_CLOSE_FAIL_NOOPENPORT;
	}
}

bool CNanhuaNhc03::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

DWORD CNanhuaNhc03::WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength,  const bool bNeedWriteChkSum, const bool bNeedReadChkSum)
{
	assert(NULL != pbWriteBuffer);
	assert(dwWriteLength > 1);
	assert(NULL!=pbReadBuffer);
	assert(dwReadLength != 0);

	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			// ��ҪдУ��
			pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// �ֶ��ζ�������һ������ͷ��1byte�����ڶ��ζ�ʣ�µ���Ч����

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 1, &dwBytesRead, NULL);
		if (bSucceed && 1UL==dwBytesRead)
		{
			// �жϷǷ������ָ���
			if (0x15 != pbReadBuffer[0])
			{
				if (1 == dwReadLength)
				{
					// ֻ����1������

					::LeaveCriticalSection(&cInfoSection);
					// �����ɹ�
					return DYN_WR_SUCCEED;
				}
				else
				{
					// ��ֻ����1������

					// �����µ�����
					bSucceed = ReadFile(m_hComm, pbReadBuffer+1, dwReadLength-1, &dwBytesRead, NULL);
					if(bSucceed && dwBytesRead==(dwReadLength-1))
					{
						if (bNeedReadChkSum)
						{
							// ��Ҫ��У��

							// ��֤У���
							if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer,dwReadLength-1))
							{
								::LeaveCriticalSection(&cInfoSection);
								// ��������У���������ʧ��
								return DYN_WR_CHECK_SUM_FAIL;
							}
							else
							{
								::LeaveCriticalSection(&cInfoSection);
								// �����ɹ�
								return DYN_WR_SUCCEED;
							}
						}
						else
						{
							::LeaveCriticalSection(&cInfoSection);
							// �����ɹ�
							return DYN_WR_SUCCEED;
						}
					}
					else
					{
						::LeaveCriticalSection(&cInfoSection);
						// �������󣺶�ָ������ʧ�ܣ���������Ч����ʧ�ܣ�
						return DYN_WR_READ_SPECIFIED_DATA_FAIL;
					}
				}
			}
			else
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);
				// ���������������ش���״̬��
				return DYN_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// �������󣺶�ָ������ʧ�ܣ�������ͷʧ�ܣ�
			return DYN_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// �������󣺴���û�д�
		return DYN_WR_PORT_NOT_OPEN;
	}
}

BYTE CNanhuaNhc03::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	//bSum = ~bSum + 1;
	return bSum;                
}

// ȡʵʱ����
DWORD CNanhuaNhc03::GetRealTimeData(float *const pfP, float *const pfF, float *const pfV)
{
	BYTE bWriteBuf[7] = {0x08, 'C', 0x07, 0x00, 0x00, 0x00, 0x52};
	BYTE bReadBuf[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 9, false, true);
	if (DYN_WR_SUCCEED == dw)
	{
		if (NULL != pfP)
		{
			// ����(kW)
			*pfP = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 100.0f;
		}
		if (NULL != pfF)
		{
			// Ť��(N)
			*pfF = MAKEWORD(bReadBuf[4], bReadBuf[3]) + bReadBuf[5] / 10.0f;
		}
		if (NULL != pfV)
		{
			// �ٶ�(km/h)
			*pfV = MAKEWORD(bReadBuf[7], bReadBuf[6]) / 100.0f;
		}
	}
	else
	{
		// ��������

		if (NULL != pfP)
		{
			// ����(kW)
			*pfP = 0.0f;
		}
		if (NULL != pfF)
		{
			// Ť��(N)
			*pfF = 0.0f;
		}
		if (NULL != pfV)
		{
			// �ٶ�(km/h)
			*pfV = 0.0f;
		}
	}
	return dw;
}

// ȡ״̬
DWORD CNanhuaNhc03::GetStatus(std::bitset<16> *const pbs16Status)
{
	BYTE bWriteBuf[7] = {0x08, 'D', 0x07, 0x00, 0x00, 0x00, 0x53};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 4, false, true);
	if (DYN_WR_SUCCEED == dw)
	{
		if (NULL != pbs16Status)
		{
			*pbs16Status = MAKEWORD(bReadBuf[2], bReadBuf[1]);
		}
	}
	else
	{
		// ��������
		if (NULL != pbs16Status)
		{
			*pbs16Status = 0x0000;
		}
	}
	return dw;
}

// ������
DWORD CNanhuaNhc03::ReadVar(const BYTE bVarNumber, WORD *const pwVarValue)
{
	assert(NULL != pwVarValue);

	BYTE bWriteBuf[7] = {0x08, 'E', 0x07, bVarNumber, 0x00, 0x00, 0x00};
	BYTE bReadBuf[4] = {0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 4, true, true);
	if (DYN_WR_SUCCEED == dw)
	{
		*pwVarValue = MAKEWORD(bReadBuf[2], bReadBuf[1]);
	}
	else
	{
		// ��������
		*pwVarValue = 0x0000;
	}
	return dw;
}

// д����
DWORD CNanhuaNhc03::WriteVar(const BYTE bVarNumber, const WORD wVarValue)
{
	BYTE bWriteBuf[7] = {0x08, 'F', 0x07, bVarNumber, HIBYTE(wVarValue), LOBYTE(wVarValue), 0x00};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 1, true, false);
	return dw;
}

// ���Ϳ�����
DWORD CNanhuaNhc03::SendCtrlWord(const BYTE bCtrlWord)
{
	//// ֻ������������
	//assert('0'==bCtrlWord ||
	//	'1'==bCtrlWord ||
	//	'2'==bCtrlWord ||
	//	'3'==bCtrlWord ||
	//	'4'==bCtrlWord ||
	//	'5'==bCtrlWord ||
	//	'6'==bCtrlWord ||
	//	'7'==bCtrlWord ||
	//	'A'==bCtrlWord ||
	//	'B'==bCtrlWord ||
	//	'H'==bCtrlWord ||
	//	'I'==bCtrlWord ||
	//	'J'==bCtrlWord ||
	//	'K'==bCtrlWord ||
	//	'L'==bCtrlWord ||
	//	'M'==bCtrlWord ||
	//	'N'==bCtrlWord ||
	//	'Q'==bCtrlWord ||
	//	'R'==bCtrlWord ||
	//	'S'==bCtrlWord ||
	//	'T'==bCtrlWord ||
	//	'U'==bCtrlWord ||
	//	'V'==bCtrlWord ||
	//	'X'==bCtrlWord ||
	//	'Y'==bCtrlWord);

	BYTE bWriteBuf[7] = {0x08, bCtrlWord, 0x07, 0x00, 0x00, 0x00, 0x00};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 1, true, false);
	return dw;
}

// ���������
DWORD CNanhuaNhc03::LiftBeamUp(void)
{
	return SendCtrlWord('5');
}

// ���¾�����
DWORD CNanhuaNhc03::LiftBeamDown(void)
{
	return SendCtrlWord('6');
}

// �������
DWORD CNanhuaNhc03::MotorOn(void)
{
	return SendCtrlWord('J');
}

// ֹͣ���
DWORD CNanhuaNhc03::MotorOff(void)
{
	return SendCtrlWord('K');
}

// �˳�����ģʽ
DWORD CNanhuaNhc03::ControlModeOff(void)
{
	return SendCtrlWord('0');
}

// ����ASM����ģʽ
DWORD CNanhuaNhc03::AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses)
{
	// ����100��,��������
	assert(fAsmSpeed >= 0.0f);
	WORD wAsmSpeedOn = (WORD)(fAsmSpeed*100.0f + 0.5f);
	// ����100��,��������
	assert(fTargetPower >= 0.0f);
	WORD wTargetPowerOn = (WORD)(fTargetPower*100.0f + 0.5f);
	WriteVar(229, 0);
	WriteVar(230, wAsmSpeedOn);
	WriteVar(228, wTargetPowerOn);

	return SendCtrlWord('4');
}

// ���ú㹦��Ŀ��ֵ
// (in)fPTarget���㹦��Ŀ��ֵ����λ-kW
DWORD CNanhuaNhc03::SetConstantPowerTarget(const float fPTarget)
{
	WORD wPTarget = WORD(fPTarget * 100.0f + 0.5f);
	return WriteVar(228, wPTarget);
}

// �����㹦�ʿ���ģʽ
DWORD CNanhuaNhc03::ConstantPowerControlModeOn(void)
{
	return SendCtrlWord('7');
}

// ���ú���Ŀ��ֵ
// (in)fFTarget������Ŀ��ֵ����λ-N
DWORD CNanhuaNhc03::SetConstantForceTarget(const float fFTarget)
{
	// ����λ
	WORD wFTarget = (WORD)fFTarget;
	DWORD dw = WriteVar(201, wFTarget);
	if (DYN_WR_SUCCEED != dw)
	{
		return dw;
	}
	// С��λ
	wFTarget = WORD(fFTarget * 10.0f + 0.5f) % 10;
	return WriteVar(202, wFTarget);
}

// ������������ģʽ
DWORD CNanhuaNhc03::ConstantForceControlModeOn(void)
{
	return SendCtrlWord('2');
}

// �������غ�ģʽ
DWORD CNanhuaNhc03::VarLoadControlModeOn(void)
{
	return SendCtrlWord('Q');
}

// ���ú��ٶ�Ŀ��ֵ
// (in)fVTarget�����ٶ�Ŀ��ֵ����λ-km/h
DWORD CNanhuaNhc03::SetConstantVelocityTarget(const float fVTarget)
{
	assert(fVTarget >= 0.0f);

	WORD wVTarget = WORD(fVTarget * 100.0f + 0.5f);
	return WriteVar(200, wVTarget);
}

// �������ٶȿ���
DWORD CNanhuaNhc03::ConstantVelocityControlModeOn(void)
{
	return SendCtrlWord('1');
}

// �������1��ʼ�ٶ�
// (in)fUpperVelocity����ʼ�ٶȣ���λ-km/h
DWORD CNanhuaNhc03::SetUpperVelocityOfTimer1(const float fUpperVelocity)
{
	assert(fUpperVelocity >= 0.0f);

	WORD wV = WORD(fUpperVelocity * 100.0f + 0.5f);
	return WriteVar(207, wV);
}

// �������1�м��ٶ�
// (in)fMiddleVelocity���м��ٶȣ���λ-km/h
DWORD CNanhuaNhc03::SetMiddleVelocityOfTimer1(const float fMiddleVelocity)
{
	assert(fMiddleVelocity >= 0.0f);

	WORD wV = WORD(fMiddleVelocity * 100.0f + 0.5f);
	return WriteVar(232, wV);
}

// �������1ֹͣ�ٶ�
// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
DWORD CNanhuaNhc03::SetLowerVelocityOfTimer1(const float fLowerVelocity)
{
	assert(fLowerVelocity >= 0.0f);

	WORD wV = WORD(fLowerVelocity * 100.0f + 0.5f);
	return WriteVar(208, wV);
}

// ���ò⹦���������ٶ�
// (in)fMaxVelocity: ����ٶȣ���λ-km/h
DWORD CNanhuaNhc03::SetMotorMaxVelocity(const float fMaxVelocity)
{
	assert(fMaxVelocity>0.0f && fMaxVelocity<130.0f);

	const WORD wV = WORD(fMaxVelocity * 0.8f);
	return WriteVar(44, wV);

}

// ��ȡ�⹦���������ٶ�
// (out)fMaxVelocity: ����ٶȣ���λ-km/h
DWORD CNanhuaNhc03::GetMotorMaxVelocity(float *const fMaxVelocity)
{
	WORD Frequency(0);
	DWORD dw = ReadVar(44, &Frequency);
	*fMaxVelocity = (Frequency * 100.0f) / 80.0f ;
	return dw;
}
// �������1
DWORD CNanhuaNhc03::SetTimer1(void)
{
	return SendCtrlWord('H');
}

// �ر����1
DWORD CNanhuaNhc03::KillTimer1(void)
{
	return SendCtrlWord('I');
}

// ��ȡ���1ʱ��
// (out)punTime_MS;ʱ��ָ�룬��λ-ms
DWORD CNanhuaNhc03::GetTimeOfTimer1(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);
	//���1��ʱֵ����Ram[215]*65535+Ram[227]��*0.005�� 20170726 �¸���Э�� change by qin
	WORD wTimeL(0),wTimeH(0);
	DWORD dwL = ReadVar(227, &wTimeL);
	DWORD dwH = ReadVar(215, &wTimeH);
	if (DYN_WR_SUCCEED == dwL)
	{
		*punTime_MS = (wTimeH*65535 + wTimeL) * 5;
	}
	else
	{
		*punTime_MS = 0;
	}
	return dwH;
}

// ��ȡ���1�м�ʱ��
// (out)punTime_MS;ʱ��ָ�룬��λ-ms
DWORD CNanhuaNhc03::GetMiddleTimeOfTimer1(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);
  //���1�м��ʱֵ����Ram[215]*65535+Ram[233]��*0.005�� 20170726 �¸���Э�� change by qin
	WORD wTimeL(0),wTimeH(0);
	DWORD dwL = ReadVar(233, &wTimeL);
	DWORD dwH = ReadVar(215, &wTimeH);
	if (DYN_WR_SUCCEED == dwL)
	{
		*punTime_MS = (wTimeH*65535 + wTimeL) * 5;
	}
	else
	{
		*punTime_MS = 0;
	}
	return dwH;
}

// �������2��ʼ�ٶ�
// (in)fUpperVelocity����ʼ�ٶȣ���λ-km/h
DWORD CNanhuaNhc03::SetUpperVelocityOfTimer2(const float fUpperVelocity)
{
	assert(fUpperVelocity >= 0.0f);

	WORD wV = WORD(fUpperVelocity * 100.0f + 0.5f);
	return WriteVar(234, wV);
}

// �������2ֹͣ�ٶ�
// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
DWORD CNanhuaNhc03::SetLowerVelocityOfTimer2(const float fLowerVelocity)
{
	assert(fLowerVelocity >= 0.0f);

	WORD wV = WORD(fLowerVelocity * 100.0f + 0.5f);
	return WriteVar(235, wV);
}


// �������2
DWORD CNanhuaNhc03::SetTimer2(void)
{
	return SendCtrlWord('R');
}

// �ر����2
DWORD CNanhuaNhc03::KillTimer2(void)
{
	return SendCtrlWord('S');
}

// ��ȡ���2ʱ��
// (out)punTime_MS;ʱ��ָ�룬��λ-ms
DWORD CNanhuaNhc03::GetTimeOfTimer2(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);
	//���2��ʱֵ����Ram[237]*65535+Ram[236]��*0.005�� 20170726 �¸���Э�� change by qin
	WORD wTimeL(0),wTimeH(0);
	DWORD dwL = ReadVar(236, &wTimeL);
	DWORD dwH = ReadVar(237, &wTimeH);
	if (DYN_WR_SUCCEED == dwL)
	{
		*punTime_MS = (wTimeH*65535 + wTimeL) * 5;
	}
	else
	{
		*punTime_MS = 0;
	}
	return dwH;
}

// ��ȡ��������
// (out)pusDIW����������ָ�룬��λ-kg
DWORD CNanhuaNhc03::GetDIW(USHORT *const pusDIW)
{
	assert(NULL != pusDIW);

	return ReadVar(6, pusDIW);
}

// ���û�������
DWORD CNanhuaNhc03::SetDIW(const USHORT usDIW)
{
	return WriteVar(6, usDIW);
}

// д��������ģ�����ģʽ�ĳ�����׼����[kg]
// (in)������׼����,��λ-kg
DWORD CNanhuaNhc03::WriteVehicleRM(const USHORT usVehicleRM)
{
	return WriteVar(246, usVehicleRM);
}


// д��������ģ�����ģʽ��Cv
// �ڲ�����ת��Ϊ80km/h��Ӧ��ֵ
// (in)Cvֵ��50km/h��Ӧ��ֵ����λ:kW
DWORD CNanhuaNhc03::WriteCv(const float fCv)
{
	// �Ŵ�100��,ͨѶЭ��Ҫ��
	// ת��Ϊ80km/h��Ӧ��Cvֵ(*4.096)
	// ������������
	USHORT usCv = USHORT(fCv*100.0f*4.096f + 0.5f);
	return WriteVar(203, usCv);
}

// д��������ģ�����ģʽ��Bv
// (in)Bvֵ����λ:kW
DWORD CNanhuaNhc03::WriteBv(const float fBv)
{
	// �Ŵ�100��,ͨѶЭ��Ҫ��
	// ������������
	USHORT usBv = USHORT(fBv*100.0f + 0.5f);
	return WriteVar(204, usBv);
}

// д��������ģ�����ģʽ��Av
// (in)Avֵ����λ:kW
DWORD CNanhuaNhc03::WriteAv(const float fAv)
{
	// �Ŵ�100��,ͨѶЭ��Ҫ��
	// ������������
	USHORT usAv = USHORT(fAv*100.0f + 0.5f);
	return WriteVar(205, usAv);
}

// д��������ģ�����ģʽ��At
// (in)Atֵ����λ:kW
DWORD CNanhuaNhc03::WriteAt(const float fAt)
{
	// �Ŵ�100��,ͨѶЭ��Ҫ��
	// ������������
	USHORT usAt = USHORT(fAt*100.0f + 0.5f);
	return WriteVar(212, usAt);
}

// д��������ģ�����ģʽ��Bt
// (in)Btֵ����λ:kW
DWORD CNanhuaNhc03::WriteBt(const float fBt)
{
	// �Ŵ�100��,ͨѶЭ��Ҫ��
	// ������������
	USHORT usBt = USHORT(fBt*100.0f + 0.5f);
	return WriteVar(213, usBt);
}

// д��������ģ�����ģʽ��Ct
// (in)Ctֵ����λ:kW
DWORD CNanhuaNhc03::WriteCt(const float fCt)
{
	// �Ŵ�100��,ͨѶЭ��Ҫ��
	// ������������
	USHORT usCt = USHORT(fCt*100.0f + 0.5f);
	return WriteVar(214, usCt);
}

// ������������ģ�����ģʽ
DWORD CNanhuaNhc03::DriveResistanceSimulationControlModeOn(void)
{
	return SendCtrlWord('3');
}

// ������ʻ����
DWORD CNanhuaNhc03::ResetDistance(void)
{
	return SendCtrlWord('9');
}

// ��ȡ��ʻ����
// (out)pusDistance:����ָ��,��λ:��(m)
DWORD CNanhuaNhc03::ReadDistance(USHORT *const pusDistance)
{
	assert(NULL != pusDistance);

	return ReadVar(225, pusDistance);
}


// ��ȡ��ʾAp(δȥ��ʣ����)
// (out)fAp:����,��λ:kW
DWORD CNanhuaNhc03::ReadApWithRemanence(float *const pfAp)
{
	assert(NULL != pfAp);

	WORD wAp(0);
	DWORD dw = ReadVar(82, &wAp);
	//*pfAp = wAp / 100.0f;
	ConvertWordToFloat(wAp, pfAp);
	return dw;
}

// ��ȡ��ʾBp(δȥ��ʣ����)
// (out)fBp:����,��λ:kW
DWORD CNanhuaNhc03::ReadBpWithRemanence(float *const pfBp)
{
	assert(NULL != pfBp);

	WORD wBp(0);
	DWORD dw = ReadVar(83, &wBp);
	//*pfBp = wBp / 100.0f;
	ConvertWordToFloat(wBp, pfBp);
	return dw;
}

// ��ȡ��ʾCp(δȥ��ʣ����)
// (out)fCp:����,��λ:kW
DWORD CNanhuaNhc03::ReadCpWithRemanence(float *const pfCp)
{
	assert(NULL != pfCp);

	WORD wCp(0);
	DWORD dw = ReadVar(84, &wCp);
	//*pfCp = wCp / 100.0f;
	ConvertWordToFloat(wCp, pfCp);
	return dw;
}

// д����ʾAp(δȥ��ʣ����)
// (in)fAp:���ʣ���λ:kW
DWORD CNanhuaNhc03::WriteApWithRemanence(const float fAp)
{
	//// �Ŵ�100��������������
	//WORD wAp = WORD(fAp*100.0f + 0.5f);

	WORD wAp(0);
	ConvertFloatToWord(fAp, &wAp);

	return WriteVar(82, wAp);
}

// д����ʾBp(δȥ��ʣ����)
// (in)fBp:���ʣ���λ:kW
DWORD CNanhuaNhc03::WriteBpWithRemanence(const float fBp)
{
	//// �Ŵ�100��������������
	//WORD wBp = WORD(fBp*100.0f + 0.5f);

	WORD wBp(0);
	ConvertFloatToWord(fBp, &wBp);

	return WriteVar(83, wBp);
}

// д����ʾCp(δȥ��ʣ����)
// (in)fCp:���ʣ���λ:kW
DWORD CNanhuaNhc03::WriteCpWithRemanence(const float fCp)
{
	//// �Ŵ�100��������������
	//WORD wCp = WORD(fCp*100.0f + 0.5f);

	WORD wCp(0);
	ConvertFloatToWord(fCp, &wCp);

	return WriteVar(84, wCp);
}

// ��ȡ��ʵAp(ȥ��ʣ������)
// (out)fAp:����,��λ:kW
DWORD CNanhuaNhc03::ReadApWithoutRemanence(float *const pfAp)
{
	assert(NULL != pfAp);

	WORD wAp(0);
	DWORD dw = ReadVar(39, &wAp);
	//*pfAp = wAp / 100.0f;
	ConvertWordToFloat(wAp, pfAp);
	return dw;
}

// ��ȡ��ʵBp(ȥ��ʣ������)
// (out)fBp:����,��λ:kW
DWORD CNanhuaNhc03::ReadBpWithoutRemanence(float *const pfBp)
{
	assert(NULL != pfBp);

	WORD wBp(0);
	DWORD dw = ReadVar(40, &wBp);
	//*pfBp = wBp / 100.0f;
	ConvertWordToFloat(wBp, pfBp);
	return dw;
}

// ��ȡ��ʵCp(ȥ��ʣ������)
// (out)fCp:����,��λ:kW
DWORD CNanhuaNhc03::ReadCpWithoutRemanence(float *const pfCp)
{
	assert(NULL != pfCp);

	WORD wCp(0);
	DWORD dw = ReadVar(41, &wCp);
	//*pfCp = wCp / 100.0f;
	ConvertWordToFloat(wCp, pfCp);
	return dw;
}

// д����ʵAp(ȥ��ʣ������)
// (in)fAp:���ʣ���λ:kW
DWORD CNanhuaNhc03::WriteApWithoutRemanence(const float fAp)
{
	//// �Ŵ�100��������������
	//WORD wAp = WORD(fAp*100.0f + 0.5f);

	WORD wAp(0);
	ConvertFloatToWord(fAp, &wAp);

	return WriteVar(39, wAp);
}

// д����ʵBp(ȥ��ʣ������)
// (in)fBp:���ʣ���λ:kW
DWORD CNanhuaNhc03::WriteBpWithoutRemanence(const float fBp)
{
	//// �Ŵ�100��������������
	//WORD wBp = WORD(fBp*100.0f + 0.5f);

	WORD wBp(0);
	ConvertFloatToWord(fBp, &wBp);

	return WriteVar(40, wBp);
}

// д����ʵCp(ȥ��ʣ������)
// (in)fCp:���ʣ���λ:kW
DWORD CNanhuaNhc03::WriteCpWithoutRemanence(const float fCp)
{
	//// �Ŵ�100��������������
	//WORD wCp = WORD(fCp*100.0f + 0.5f);

	WORD wCp(0);
	ConvertFloatToWord(fCp, &wCp);

	return WriteVar(41, wCp);
}

void CNanhuaNhc03::ConvertFloatToWord(const float fSource, WORD *const pwDestination)
{
	assert(NULL != pwDestination);

	if (fSource >= 0.0f)
	{
		// float�ǷǸ���
		*pwDestination = WORD(fSource*100.0f + 0.5f);
	}
	else
	{
		// float�Ǹ���
		// ȡ����ֵ
		const float f = fabs(fSource);
		*pwDestination = WORD(f*100.0f + 0.5f);
		// ������λ(���λ)��Ϊ1
		*pwDestination = 0x8000 | *pwDestination;
	}
}

void CNanhuaNhc03::ConvertWordToFloat(const WORD wSource, float *const pfDestination)
{
	assert(NULL != pfDestination);

	if (0 ==(0x8000&wSource))
	{
		// ����λ(���λ)Ϊ0�����Ǹ���
		*pfDestination = wSource / 100.0f;
	}
	else
	{
		// ����λ(���λ)Ϊ1��������
		// ��ȡ���˷Ƿ���λ������15λ
		const WORD w = 0x7FFF & wSource;
		// ��Ϊ����
		*pfDestination = w / -100.0f;
	}
}

DWORD CNanhuaNhc03::SetConstantAccStep(const float fStep)
{
	WORD wStep(0);

	ConvertFloatToWord(fStep, &wStep);

	return WriteVar(217, wStep);
}

DWORD CNanhuaNhc03::ConstantAccControlModeOn(void)
{
	return SendCtrlWord('W');
}

DWORD CNanhuaNhc03::ResponseTimeControlModeStart(void)
{
	return SendCtrlWord('a');
}

DWORD CNanhuaNhc03::ResponseTimeControlModeEnd(void)
{
	return SendCtrlWord('b');
}

DWORD CNanhuaNhc03::GetResponseTimeData(float *const pfV, float *const pfF, const int nLength)
{
	BYTE bWriteBuf[7] = {0x08, 'c', 0x07, 0x00, 0x00, 0x00, 0x72};
	BYTE *bReadBuf = new BYTE[nLength*4+3]();
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, nLength*4+3, false, false);
	if (DYN_WR_SUCCEED == dw)
	{
		if (NULL != pfF)
		{
			// ��(N)
			for (int i=0; i<nLength; i++)
			{
				pfF[i] = MAKEWORD(bReadBuf[i*2+3], bReadBuf[i*2+2]);
			}
		}
		if (NULL != pfV)
		{
			int j(0);
			// �ٶ�(10m/h)
			for (int i=nLength; i<nLength*2; i++)
			{
				pfV[j] = MAKEWORD(bReadBuf[i*2+3], bReadBuf[i*2+2])/100.0f;
				j++;
			}
		}
	}
	else
	{
		// ��������
		if (NULL != pfF)
		{
			// Ť��(N)
			for(int i= 0; i<10; i++)
			{
				pfF[i] = 0.0f;
			}
		}
		if (NULL != pfV)
		{
			// �ٶ�(km/h)
			for(int i= 0; i<10; i++)
			{
				pfV[i] = 0.0f;
			}
		}
	}
	delete[] bReadBuf;
	return dw;
}