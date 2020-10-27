#include "StdAfx.h"
#include "NanhuaNha501A.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <stdlib.h>
#include <cassert>
#include <cstdio>

CNanhuaNha501A::CNanhuaNha501A(void)
//: m_hComm(INVALID_HANDLE_VALUE)
//, m_opUsedNow(ToAnalyzer)
{
	//::InitializeCriticalSection(&cInfoSection);

	m_bStatus = 0;
	m_unTimerID = NULL;
}

CNanhuaNha501A::~CNanhuaNha501A(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaNha501A::Open(BYTE bPort)
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
		// �򿪴���ʧ��
		OutputDebugStringW(L"Error:fail to open com port\n");
		return ANA_OPEN_FAIL;
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
	// ���г�ʱ���ã�2500����֮��Ҫִ�����д������
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 2500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// ��ʼ�����ڲ���end

	return ANA_OPEN_SUCCEED;
}

// �رմ���
DWORD CNanhuaNha501A::Close(void)
{
	if (NULL != m_unTimerID)
	{
		timeKillEvent(m_unTimerID);
		m_unTimerID = NULL;
	}

	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// �رմ��ڳɹ�
			return ANA_CLOSE_SUCCEED;
		}
		else
		{
			// �رմ���ʧ�ܣ�ִ��win32����ʧ��
			return ANA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// �رմ���ʧ�ܣ�����û�гɹ���
		return ANA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CNanhuaNha501A::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}

DWORD CNanhuaNha501A::WriteAndReadData(BYTE *const pbWriteBuffer, 
								const DWORD dwWriteLength, 
								BYTE *const pbReadBuffer, 
								const DWORD dwReadLength, 
								const bool bNeedWriteChkSum, 
								const bool bNeedReadChkSum, 
								const bool bPurgeComm)
{
	//assert(NULL!=pbWriteBuffer);
	//assert(0UL!=dwWriteLength);
	assert(NULL!=pbReadBuffer);
	assert(0UL!=dwReadLength);

	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		// �ж��Ƿ����ô��ڲ���
		if (bPurgeComm)
		{
			PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
		}

		// �ж��Ƿ���Ҫ����д���ݲ���
		if (NULL != pbWriteBuffer)
		{
			// ��Ҫ����д�����ݲ���

			// �ж�д�������Ƿ���Ҫ����У��
			if (dwWriteLength>1 && bNeedWriteChkSum)
			{
				pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
			}
			WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		}
		
		// һ�ζ���Ԥ��Ҫ���յ��ֽ���

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwReadLength, &dwBytesRead, NULL);
		if (bSucceed && dwReadLength==dwBytesRead)
		{
			if (dwReadLength>1 && bNeedReadChkSum)
			{
				// ��֤У���
				if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer, dwReadLength-1))
				{
					::LeaveCriticalSection(&cInfoSection);

					// ��������У���������ʧ��
					return ANA_WR_CHECK_SUM_FAIL;
				}
				else
				{
					::LeaveCriticalSection(&cInfoSection);

					// �����ɹ�
					return ANA_WR_SUCCEED;
				}
			}
			else
			{
				::LeaveCriticalSection(&cInfoSection);

				// �����ɹ�
				return ANA_WR_SUCCEED;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);

			// �������󣺶�ָ������ʧ��
			return ANA_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);

		// �������󣺴���û�д�
		return ANA_WR_PORT_NOT_OPEN;
	}
}

BYTE CNanhuaNha501A::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}
	
	return bSum;
}

DWORD CNanhuaNha501A::WriteAndReadData_Flowmeter(BYTE *const pbWriteBuffer, 
												 const DWORD dwWriteLength, 
												 BYTE *const pbReadBuffer, 
												 const DWORD dwReadLength, 
												 const bool bNeedWriteChkSum, 
												 const bool bNeedReadChkSum)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

WORD CNanhuaNha501A::CheckSum_Flowmeter(const BYTE *const pb, const UINT un)
{
	assert(NULL != pb);
	assert(0 != un);

	WORD wSum(0);
	for (UINT i=0; i<un; i++)
	{
		wSum += pb[i];
	}
	return wSum;
}

DWORD CNanhuaNha501A::CmdFormat_OnlyReturn06(const BYTE bCmd, BYTE *const pbResult)
{
	BYTE bWriteBuf = bCmd;
	BYTE bReadBuf(0);
	DWORD dw = WriteAndReadData(&bWriteBuf, 1, &bReadBuf, 1, false, false, true);
	if (NULL != pbResult)
	{
		// ��ȡ����ִ�н��

		if (ANA_WR_SUCCEED == dw)
		{
			// �������ؽ��,һ�㷵��0x06
			*pbResult = bReadBuf;
		}
	}
	return dw;
}

DWORD CNanhuaNha501A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::CmdFormat_Return00_01_05(const BYTE bCmd, BYTE *const pbResult, short *const psHC, float *const pfCO, float *const pfCO2, float *const pfO2, short *const psNO, float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::CmdFormat_Return06_00_01(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::CmdFormat_Return00_01(const BYTE bCmd, BYTE *const pbResult, const USHORT usC3H8, const float fCO, const float fCO2, const USHORT usNO)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::CmdFormat_Return00_01_02(const BYTE bCmd, BYTE *const pbResult, const float fValue)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}


// 1ȡ������
DWORD CNanhuaNha501A::GetMajorData(short *const psHC, 
							float *const pfCO, 
							float *const pfCO2, 
							float *const pfO2, 
							short *const psNO,
							short *const psNO2,
							short *const psNOx,
							USHORT *const pusRpm, 
							float *const pfOilTemp, 
							USHORT *const pusFlux, 
							float *const pfLambda,
							bool *const pbO2Sensor,
							bool *const pbNOSensor)
{
	BYTE bWriteBuf[] = {0x03};
	BYTE bReadBuf[19];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		// (1) HC
		if (NULL != psHC)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*psHC = (short)(MAKEWORD(bReadBuf[2], bReadBuf[1]));
		}
		// (2) CO
		if (NULL != pfCO)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfCO = (short)(MAKEWORD(bReadBuf[4], bReadBuf[3])) / 100.0f;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfCO2 = (short)(MAKEWORD(bReadBuf[6], bReadBuf[5])) / 100.0f;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfO2 = (short)(MAKEWORD(bReadBuf[8], bReadBuf[7])) / 100.0f;
		}
		// (5) NO
		if (NULL != psNO)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*psNO = (short)(MAKEWORD(bReadBuf[10], bReadBuf[9]));
		}
		// (6) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = MAKEWORD(bReadBuf[12], bReadBuf[11]);
		}
		// (7) OilTemp
		if (NULL != pfOilTemp)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfOilTemp = (float)MAKEWORD(bReadBuf[14], bReadBuf[13]);
		}
		// Lambda
		if (NULL != pfLambda)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfLambda = (short)(MAKEWORD(bReadBuf[16], bReadBuf[15])) / 100.0f;
		}

		// ����ȡֵ��Χ
#ifdef ANALYZER_DATA_RANGE
		// HC
		if (*psHC < ANALYZER_HC_LOWER_LIMIT)
		{
			*psHC = ANALYZER_HC_LOWER_LIMIT;
		}
		else if (*psHC > ANALYZER_HC_UPPER_LIMIT)
		{
			*psHC = ANALYZER_HC_UPPER_LIMIT;
		}
		// CO
		if (*pfCO < ANALYZER_CO_LOWER_LIMIT)
		{
			*pfCO = ANALYZER_CO_LOWER_LIMIT;
		}
		else if (*pfCO > ANALYZER_CO_UPPER_LIMIT)
		{
			*pfCO = ANALYZER_CO_UPPER_LIMIT;
		}
		// CO2
		if (*pfCO2 < ANALYZER_CO2_LOWER_LIMIT)
		{
			*pfCO2 = ANALYZER_CO2_LOWER_LIMIT;
		}
		else if (*pfCO2 > ANALYZER_CO2_UPPER_LIMIT)
		{
			*pfCO2 = ANALYZER_CO2_UPPER_LIMIT;
		}
		 // O2
		if (*pfO2 < ANALYZER_O2_LOWER_LIMIT)
		{
			*pfO2 = ANALYZER_O2_LOWER_LIMIT;
		}
		else if (*pfO2 > ANALYZER_O2_UPPER_LIMIT)
		{
			*pfO2 = ANALYZER_O2_UPPER_LIMIT;
		}
		 // NO
		if (*psNO < ANALYZER_NO_LOWER_LIMIT)
		{
			*psNO = ANALYZER_NO_LOWER_LIMIT;
		}
		else if (*psNO > ANALYZER_NO_UPPER_LIMIT)
		{
			*psNO = ANALYZER_NO_UPPER_LIMIT;
		}
		 // RPM
		if (*pusRpm > ANALYZER_RPM_UPPER_LIMIT)
		{
			*pusRpm = ANALYZER_RPM_UPPER_LIMIT;
		}
		 // OilTemp
		if (*pfOilTemp < ANALYZER_OILTEMP_LOWER_LIMIT)
		{
			*pfOilTemp = ANALYZER_OILTEMP_LOWER_LIMIT;
		}
		else if (*pfOilTemp > ANALYZER_OILTEMP_UPPER_LIMIT)
		{
			*pfOilTemp = ANALYZER_OILTEMP_UPPER_LIMIT;
		}
		 // Flux
		if (*pusFlux > ANALYZER_FLUX_LOWER_LIMIT)
		{
			*pusFlux = ANALYZER_FLUX_UPPER_LIMIT;
		}
		 // Lambda
		if (*pfLambda > ANALYZER_LAMBDA_LOWER_LIMIT)
		{
			*pfLambda = ANALYZER_LAMBDA_LOWER_LIMIT;
		}
		else if (*pfLambda > ANALYZER_LAMBDA_UPPER_LIMIT)
		{
			*pfLambda = ANALYZER_LAMBDA_UPPER_LIMIT;
		}
#endif

	}
	else
	{
		//// ��������
		// (1) HC
		if (NULL != psHC)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*psHC = 0;
		}
		// (2) CO
		if (NULL != pfCO)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfCO = 0;
		}
		// (3) CO2
		if (NULL != pfCO2)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfCO2 = 0;
		}
		// (4) O2
		if (NULL != pfO2)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfO2 = 0;
		}
		// (5) NO
		if (NULL != psNO)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*psNO = 0;
		}
		// (6) RPM
		if (NULL != pusRpm)
		{
			*pusRpm = 0;
		}
		// (7) OilTemp
		if (NULL != pfOilTemp)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfOilTemp = 0;
		}
		// Lambda
		if (NULL != pfLambda)
		{
			// ����Ϊ�����߸���,����shortǿ��ת��
			*pfLambda = 0;
		}
	}
	return dw;
}

DWORD CNanhuaNha501A::GetMajorData(short *const psHC, 
							float *const pfCO, 
							float *const pfCO2, 
							float *const pfO2, 
							short *const psNO,
							short *const psNO2,
							short *const psNOx,
							USHORT *const pusRpm, 
							float *const pfOilTemp, 
							USHORT *const pusFlux, 
							float *const pfLambda,
							bool *const pbO2Sensor,
							bool *const pbNOSensor,
							bool *const pbWaterFull)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 2ȡ��������
// ET:Environment Temperature��д
// AP:Atmospheric Pressure��д
// RH:Relative Humidity��д
DWORD CNanhuaNha501A::GetMinorData(float *const pfET, float *const pfAP, float *const pfRH)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 3�����
DWORD CNanhuaNha501A::DeflateEnvAir(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 4��ܵ���(����,������̽ͷ����)
DWORD CNanhuaNha501A::DeflatePipeAir(BYTE *const pbResult)
{
	BYTE bWriteBuf[] = {0x01};
	BYTE bReadBuf[] = {0x06};

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 5��ϴ�ܵ�(����)
DWORD CNanhuaNha501A::InverseBlow(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 6ֹͣ��ϴ(ֹͣ������ͣ��)
DWORD CNanhuaNha501A::StopPump(BYTE *const pbResult)
{
	BYTE bWriteBuf[] = {0x02};
	BYTE bReadBuf[] = {0x06};

	DWORD dw = WriteAndReadData(bWriteBuf, sizeof(bWriteBuf), bReadBuf, sizeof(bReadBuf), false, false, true);
	if (ANA_WR_SUCCEED == dw)
	{
		if (bReadBuf[0] == 0x06)
		{
			dw = ANA_WR_SUCCEED;
		}
		else
		{
			dw = ANA_WR_CHECK_SUM_FAIL;
		}
	}
	else
	{
		//// ��������
		dw = ANA_WR_EQUIP_RETURE_0X15;
	}
	return dw;
}

// 7��©
DWORD CNanhuaNha501A::CheckLeak(BYTE *const pbResult)
{
	return CmdFormat_Return00_01_05(7, pbResult);
}

// 8����
DWORD CNanhuaNha501A::Zero(BYTE *const pbResult)
{
	m_bStatus = 0;
	if (NULL != m_unTimerID)
	{
		timeKillEvent(m_unTimerID);
		m_unTimerID = NULL;
	}
	m_unTimerID = timeSetEvent(30*1000, 1000, (LPTIMECALLBACK)TimePro, (DWORD_PTR)this, TIME_PERIODIC);
	if (NULL == m_unTimerID)
	{
		// ������ý�嶨ʱ��ʧ��
		//TRACE("������ý�嶨ʱ��ʧ��");
	}
	return CmdFormat_OnlyReturn06(0x0C, pbResult);
}

// 9д��У׼��Ũ��
DWORD CNanhuaNha501A::SetCalibrationGasConcentration(BYTE *const pbResult, const int nPoint, 
	const USHORT usCalC3H8, const float fCalCO, const float fCalCO2, const USHORT usCalNO, const USHORT usCalNO2)
{
	return CmdFormat_Return00_01(9, pbResult, usCalC3H8, fCalCO, fCalCO2, usCalNO);
}

// 10У׼
DWORD CNanhuaNha501A::GasCalibration(BYTE *const pbResult, const int nChanel, 
							  short *const psHC, 
							  float *const pfCO, 
							  float *const pfCO2, 
							  float *const pfO2, 
							  short *const psNO, 
							  short *const psNO2, 
							  float *const pfPef)
{
	return CmdFormat_Return00_01_05(10, pbResult, psHC, pfCO, pfCO2, pfO2, psNO, pfPef);
}

// 11д������Ũ��
DWORD CNanhuaNha501A::SetCheckGasConcentration(BYTE *const pbResult, const USHORT usChkC3H8, const float fChkCO, const float fChkCO2, const USHORT usChkNO)
{
	return CmdFormat_Return00_01(36, pbResult, usChkC3H8, fChkCO, fChkCO2, usChkNO);
}

// 12�Զ����
DWORD CNanhuaNha501A::GasCheck(BYTE *const pbResult, 
						short *const psHC, 
						float *const pfCO, 
						float *const pfCO2, 
						float *const pfO2, 
						short *const psNO, 
						float *const pfPef)
{
	return CmdFormat_Return00_01_05(12, pbResult, psHC, pfCO, pfCO2, pfO2, psNO, pfPef);
}

// 13ȡPEF
DWORD CNanhuaNha501A::GetPef(float *const pfPef)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 14���NO�ϻ���־
// 503����
DWORD CNanhuaNha501A::ClearNoSign(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 15���õ�������Ϊ����
DWORD CNanhuaNha501A::SetZeroAirTypeToEnvAir(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 16���õ�������Ϊ����
DWORD CNanhuaNha501A::SetZeroAirTypeToZeroAir(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 17���ó��Ϊ4
DWORD CNanhuaNha501A::SetStrokeTypeToFourStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x04, pbResult);
}

// 18���ó��Ϊ2
DWORD CNanhuaNha501A::SetStrokeTypeToTwoStroke(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x05, pbResult);
}

// 19���õ��ʽΪ����
DWORD CNanhuaNha501A::SetFireTypeToSingleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x0a, pbResult);
}

// 20���õ��ʽΪ����
DWORD CNanhuaNha501A::SetFireTypeToDoubleFire(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x0b, pbResult);
}

// 21�����Զ�����
DWORD CNanhuaNha501A::EnableAutoZero(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 22��ֹ�Զ�����
DWORD CNanhuaNha501A::DisableAutoZero(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 23�������̲���

// 24��ֹ���̲���

// 25LCD��

// 26LCD��

// 27�����¶�У׼
DWORD CNanhuaNha501A::ETCalibration(BYTE *const pbResult, const float fCalET)
{
	//assert(NULL != pbResult);
	assert(fCalET>-20.0f && fCalET<150.0f);

	return CmdFormat_Return00_01_02(27, pbResult, fCalET);
}

// 28����У׼

// 29����У׼
DWORD CNanhuaNha501A::OTCalibration(BYTE *const pbResult, const float fCalOT)
{
	//assert(NULL != pbResult);
	assert(fCalOT>-20.0f && fCalOT<150.0f);

	return CmdFormat_Return00_01_02(29, pbResult, fCalOT);
}

// 30����ʪ��У׼
DWORD CNanhuaNha501A::RHCalibration(BYTE *const pbResult, const float fCalRH)
{
	//assert(NULL != pbResult);
	assert(fCalRH>0.0f && fCalRH<101.0f);

	return CmdFormat_Return00_01_02(30, pbResult, fCalRH);
}

// 31����ѹ��У׼
DWORD CNanhuaNha501A::APCalibration(BYTE *const pbResult, const float fCalAP)
{
	//assert(NULL != pbResult);
	assert(fCalAP>0.0f && fCalAP<150.0f);

	return CmdFormat_Return00_01_02(31, pbResult, fCalAP);
}

// 32�������
DWORD CNanhuaNha501A::OpenCheckGas(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 33�������
DWORD CNanhuaNha501A::CloseCheckGas(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 34ȡ����״̬
DWORD CNanhuaNha501A::GetStatus(BYTE *const pbStatus)
{
	assert(NULL != pbStatus);
	*pbStatus = m_bStatus;
	return ANA_WR_SUCCEED;
}

// 35ȡԤ��ʣ��ʱ��
DWORD CNanhuaNha501A::GetWarmUpTimeLeft(USHORT *const pusSecondsLeft)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 36����ȼ������Ϊ����
DWORD CNanhuaNha501A::SetFuelTypeToGas(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x06, pbResult);
}


// 37����ȼ������ΪҺ��ʯ����
DWORD CNanhuaNha501A::SetFuelTypeToLPG(BYTE *const pbResult)
{
	return CmdFormat_OnlyReturn06(0x07, pbResult);
}

// 38����ȼ������Ϊ��Ȼ��
DWORD CNanhuaNha501A::SetFuelTypeToNG(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// 39����ȼ������Ϊ�Ҵ�����
DWORD CNanhuaNha501A::SetFuelTypeToGasohol(BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetOperationMode(const enum OperationMode op, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}


DWORD CNanhuaNha501A::GetProcessStatus_Flowmeter(BYTE *const pbResult, WORD *const pwProcessStatus, WORD *const pwProcess)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::StartCollectData_Flowmeter(const WORD wCollectionTime/*=195*/, WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::StopCollectData_Flowmeter(WORD *const pwMiniStatus/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetProcessDataStatus_WT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		float *const pfHCwt, 
		float *const pfCOwt, 
		float *const pfCO2wt, 
		float *const pfNOwt,
		WORD *const pwMiniStatus/*=NULL*/,
		BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetProcessDataStatus_CT_Flowmeter(const WORD wCntStart,
		const WORD wCntLength,
		short *const psHCct, 
		float *const pfCOct, 
		float *const pfCO2ct, 
		short *const psNOct, 
		float *const pfDilO2, 
		float *const pfBenchO2, 
		short *const psGasTemp, 
		float *const pfGasFluxStd0, 
		float *const pfGasFluxStd2, 
		float *const pfExhaustFluxStd0, 
		float *const pfExhaustFluxStd20, 
		float *const pfGasPres, 
		float *const pfGasFluxAct, 
		USHORT *const pusRpm,
		WORD *const pwMiniStatus,
		const BYTE bDataMode,
		BYTE *const pbResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetTotalWeight_Flowmeter(USHORT *const pusHCwt, 
		USHORT *const pusCOwt, 
		USHORT *const pusCO2wt, 
		USHORT *const pusNOwt,
		float *const pfExhaustVolume,
		float *const pfGasVolume,
		WORD *const pwMiniStatus/*=NULL*/,
		BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetCollectedDataNumber_Flowmeter(WORD *const pwCorrectedDataNumber, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetRealTimeData_Flowmeter_ALL(WORD * const pwStatus0,WORD * const pwStatus1,WORD * const pwStatus2,WORD * const pwStatus3,WORD * const pwStatus4,WORD * const pwStatus5,WORD * const pwStatus6,
	                                       WORD * const pwStatus7,WORD * const pwStatus8,WORD * const pwStatus9,WORD * const pwStatus10,WORD * const pwStatus11,WORD * const pwStatus12,WORD * const pwStatus13,
										   WORD * const pwStatus14,WORD * const pwStatus15,WORD * const pwStatus16,WORD * const pwStatus17,WORD * const pwStatus18,WORD * const pwStatus19,WORD * const pwStatus20,
										   WORD * const pwStatus21,WORD * const pwStatus22,WORD * const pwStatus23,WORD * const pwStatus24,WORD * const pwStatus25,
										   float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, 
										float *const pfUDilO2, float *const pfGasTemp/*=NULL*/, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm, const float fCalFlux)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::FluxCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, const enum FluxCalibrationMode cm)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::PresCalibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetCalibrationDilO2_Flowmeter(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::DilO2Calibration_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::DilO2Calibration_Flowmeter_High(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::DilO2Calibration_Flowmeter_Middle(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::DilO2Calibration_Flowmeter_Low(BYTE *const pbResult, BYTE *const pbCalResult, bool bDataRequestMode)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetHCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::HCCOCO2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetO2DelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetO2DelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::O2DelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetNOxDelayTime_Flowmeter(BYTE *const pbResult, float *const pfDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SetNOxDelayTime_Flowmeter(BYTE *const pbResult, const float fDelayTime)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::NOxDelayTime_Flowmeter(BYTE *const pbResult, BYTE *const pbCalResult)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::SatrtGetEnvironmentO2_Flowmeter(void)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::GetEnvironmentO2_Flowmeter(BYTE *const pbResult, float *const pfEnvO2)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::ReadCalibrationData_Flowmeter(BYTE *const pbResult, const BYTE bCalMode, BYTE *const pbReadBuf, const DWORD dwBytesToRead)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

DWORD CNanhuaNha501A::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

// ��ȡ�ֶ���������
DWORD CNanhuaNha501A::GetTestResultData(short *const psHCHigh/*=NULL*/, 
		float *const pfCOHigh/*=NULL*/, 
		float *const pfCO2High/*=NULL*/, 
		float *const pfO2High/*=NULL*/, 
		short *const psNOHigh/*=NULL*/,
		USHORT *const pusRpmHigh/*=NULL*/, 
		float *const pfOilTempHigh/*=NULL*/, 
		float *const pfLambdaHigh/*=NULL*/,
		short *const psHCLow/*=NULL*/, 
		float *const pfCOLow/*=NULL*/, 
		float *const pfCO2Low/*=NULL*/, 
		float *const pfO2Low/*=NULL*/, 
		short *const psNOLow/*=NULL*/,
		USHORT *const pusRpmLow/*=NULL*/, 
		float *const pfOilTempLow/*=NULL*/, 
		float *const pfLambdaLow/*=NULL*/)
{
	return ANA_WR_EQUIP_RETURE_0X15;
}

void  CNanhuaNha501A::TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNanhuaNha501A *pf = (CNanhuaNha501A *)dwUser;
	pf->m_bStatus = 0x10;
	timeKillEvent(uID);
}