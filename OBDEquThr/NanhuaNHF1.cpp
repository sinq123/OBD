#include "StdAfx.h"
#include "NanhuaNHF1.h"

#include <cassert>
#include <cstdio>

CNanhuaNHF1::CNanhuaNHF1(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
	::InitializeCriticalSection(&cInfoSection);	
}

CNanhuaNHF1::~CNanhuaNHF1(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// �򿪴���
DWORD CNanhuaNHF1::Open(BYTE bPort)
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
DWORD CNanhuaNHF1::Close(void)
{
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

bool CNanhuaNHF1::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}


//У���У���
bool CNanhuaNHF1::CheckReadSum_Flowmeter(const BYTE *const pb, const UINT un)
{
	assert(NULL != pb);
	assert(0 != un);
	
	BYTE bCheckSum(0),bRealSum(0);
	int iTemp = 0,iTotal = 0;

	//sscanfֱ��ʮ������תBYTE���ͻ����Խ�磬���ʹ��int�����м����
	//�ų���ʼ�ַ����ӵڶ����ַ���ʼ��ÿ�����ַ���ʮ��������ʽתΪint����
	for(int i=1; i<un-4; i=i+2)
	{
		sscanf((char*)pb + i,"%02X",&iTemp);
		iTotal = iTotal + iTemp;
	}

	//�ۼӺ��ȡΪBYTE�����ȡ����һ�ó�У����
	bCheckSum = ~(BYTE)iTotal + 1;
	sscanf((char*)pb + un - 4,"%02X",&iTemp);
	bRealSum = (BYTE)iTemp;
	
	return bCheckSum == bRealSum;
	
}

//����дУ���
BYTE CNanhuaNHF1::CheckSum_Flowmeter(const BYTE *const pb, const UINT un)
{
	assert(NULL != pb);
	assert(0 != un);

	BYTE bSum(0);
	for (UINT i=0; i<un; i++)
	{
		bSum += pb[i];
	}
	bSum = ~bSum + 1;
	return bSum;
}


DWORD CNanhuaNHF1::WriteAndReadData_Flowmeter(BYTE *const bWriteBuffer, 
												 const DWORD dwWriteLength, 
												 BYTE *const bReadBuffer, 
												 const DWORD dwReadLength, 
												 const bool bNeedWriteChkSum, 
												 bool bNeedReadChkSum)
{
	assert(NULL!=bWriteBuffer);
	assert(1UL<dwWriteLength);
	assert(NULL!=bReadBuffer);
	assert(1UL<dwReadLength);
	bNeedReadChkSum = false;
	::EnterCriticalSection(&cInfoSection);
	if(IsOpen())
	{
		// �ɹ�д��������ֽ���
		DWORD dwBytesWritten(0);
		// �ɹ������������ֽ���
		DWORD dwBytesRead(0);

		// ���ò���մ���
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		WriteFile(m_hComm, bWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// �ֶ��ζ���
		// ��һ������ͷ��5byte��
		// �ڶ��ζ�ʣ�µ���Ч����(��Ч���ݸ��ݵ�һ�ζ����������ݲ�ͬ����ͬ)

		BOOL bSucceed = ReadFile(m_hComm, bReadBuffer, 5, &dwBytesRead, NULL);
		if (bSucceed && 5==dwBytesRead)
		{
			if (0x33||0x36 == bReadBuffer[4])
			{
				// �����µ�����
				bSucceed = ReadFile(m_hComm, bReadBuffer+5, dwReadLength-5, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-5))
				{
					if (bNeedReadChkSum)
					{
						if (!CheckReadSum_Flowmeter(bReadBuffer,dwReadLength))
						{
							::LeaveCriticalSection(&cInfoSection);
							// ��������У���������ʧ��
							return ANA_WR_CHECK_SUM_FAIL;
						}
						else
						{
							::LeaveCriticalSection(&cInfoSection);
							return ANA_WR_SUCCEED;	
						}

					}
					else
					{
						::LeaveCriticalSection(&cInfoSection);
						return ANA_WR_SUCCEED;	
						
					}
				}
				else
				{
					::LeaveCriticalSection(&cInfoSection);
					// �������󣺶�ָ������ʧ�ܣ���������Ч����ʧ�ܣ�
					return ANA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				// �������������
				// ���������������
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				::LeaveCriticalSection(&cInfoSection);
				// ���������������ش�����
				return ANA_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// �������󣺶�ָ������ʧ�ܣ�������ͷʧ�ܣ�
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


//1.��ȡϡ�����������¶ȣ�����ѹ����������������׼����
DWORD CNanhuaNHF1::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp, BYTE *const pbResult)
{
	assert(NULL != pbResult);
	
	const DWORD dwBytesToRead(31);
	const DWORD dwBytesToWrite(17);
	BYTE bWriteBuf[dwBytesToWrite], bReadBuf[dwBytesToRead], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;	//��ʼ�ַ�:	
	bCheck[0] = 0x01;		//��ַ
	bCheck[1] = 0x03; 		//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x58;		//���ݵ�ַ
	bCheck[4] = 0x00;  
	bCheck[5] = 0x05;	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);		//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;	//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytesToWrite, bReadBuf, dwBytesToRead, true, true);
	// ��¼���

	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[6];	//41H
	}
	
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x41 == bReadBuf[6])
		{
			int iTemp = 0;

			// UDilO2��δ����ϡ����
			float fUDilO2 = 0;
			sscanf((const char*)bReadBuf+7,"%04X",&iTemp);
			fUDilO2 = iTemp / 100.0f;
			if (NULL != pfUDilO2)
			{
				*pfUDilO2 = fUDilO2;
			}

			// ����ѹ��
			float fPres = 0;
			sscanf((const char*)bReadBuf+11,"%04X",&iTemp);
			fPres = iTemp / 100.0f;
			if (NULL != pfPres)
			{
				*pfPres = fPres;
			}

			// ϡ�����¶�
			float fGasTemp = 0;
			sscanf((const char*)bReadBuf+15,"%04X",&iTemp);
			fGasTemp = iTemp / 10.0f;
			if (NULL != pfGasTemp)
			{
				*pfGasTemp = fGasTemp;
			}

			// ʵ������
			float fFluxAct = 0;
			sscanf((const char*)bReadBuf+19,"%04X",&iTemp);
			// ��׼����Ӣ��/����->��/��
			fFluxAct = iTemp * 0.471917f;
			// ����2λС��
			fFluxAct = CNHCommonAPI::round(fFluxAct * 100.0f) / 100.0f;
			if (NULL != pfFluxAct)
			{
				*pfFluxAct = fFluxAct;
			}

			// 0���׼����
			float fFluxStd0 = 0;
			sscanf((const char*)bReadBuf+23,"%04X",&iTemp);
			// ��׼����Ӣ��/����->��/��
			fFluxStd0 = iTemp * 0.471917f;
			// ����2λС��
			fFluxStd0 = CNHCommonAPI::round(fFluxStd0 * 100.0f) / 100.0f;
			if (NULL != pfFluxStd0)
			{
				*pfFluxStd0 = fFluxStd0;
			}		

			// 20���׼����
			if (NULL != pfFluxStd20)
			{
				*pfFluxStd20 = 0;
			}
		}
	}
	else
	{
		// Pres:ѹ����ת��ΪkPa
		if (NULL != pfPres)
		{
			*pfPres = 0.0f;
		}
		// UDilO2��δ����ϡ����
		if (NULL != pfUDilO2)
		{
			*pfUDilO2 = 0.0f;
		}
		// ϡ�����¶�
		if (NULL != pfGasTemp)
		{
			*pfGasTemp = 0.0f;
		}
		// 20���׼����
		// ��׼����Ӣ��/����->��/��
		if (NULL != pfFluxStd20)
		{
			*pfFluxStd20 = 0;
		}
		// 0���׼����
		if (NULL != pfFluxStd0)
		{
			*pfFluxStd0 = 0;
		}
		// ʵ������
		if (NULL != pfFluxAct)
		{
			*pfFluxAct = 0;
		}
	}
	
	return dw;
}

//2.��ȡ״̬
DWORD CNanhuaNHF1::Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau)
{
	assert(NULL != pbResult);
	
	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x03; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x5D;			//���ݵ�ַ
	bCheck[4] = 0x00;  
	bCheck[5] = 0x01;	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[6];	//32H
		*pbMachineStau = bReadBuf[10];	//30H���� 31HԤ��
		*pbFanStau = bReadBuf[9];	//30H����ر� 31H�������
		*pbFlowStau = bReadBuf[8];	//30H�����쳣 31H��������
	}
	return dw;
}


//3.���ط��
DWORD CNanhuaNHF1::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult)
{
	assert(NULL != pbResult);
	
	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:	
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x5E;			//���ݵ�ַ
	bCheck[4] = 0x00; 
	if (bRun)
	{
		bCheck[5] = 0x01;		// �����
	}
	else
	{
		bCheck[5] = 0x00;		// �ط��
	}		
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//4.ϡ������������
DWORD CNanhuaNHF1::SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult)
{
	
	assert(NULL != pbResult);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x5F;			//���ݵ�ַ
	bCheck[4] = 0x00;  
	bCheck[5] = 0x01;	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}




//5.����ѹ������У׼
DWORD CNanhuaNHF1::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	assert(NULL != pbResult);
	assert(fCalPres>50.0f && fCalPres<400.0f);
	
	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	// ��ǧ��->����,��������
	WORD wCalPres = WORD(fCalPres * 100.0f + 0.5f);

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x60;			//���ݵ�ַ
	bCheck[4] = HIBYTE((WORD)wCalPres);  
	bCheck[5] = LOBYTE((WORD)wCalPres);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//6.�¶ȵ���У׼
DWORD CNanhuaNHF1::SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp)
{
	
	assert(NULL != pbResult);
	assert(fCalTemp>-50.0f && fCalTemp<100.0f);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	// ��������
	WORD wCalTemp = WORD(fCalTemp * 10.0f + 0.5f);

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:	
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;	
	bCheck[3] = 0x61;			//���ݵ�ַ
	bCheck[4] = HIBYTE((WORD)wCalTemp);  
	bCheck[5] = LOBYTE((WORD)wCalTemp);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}



//7.������/��/�����̱궨
DWORD CNanhuaNHF1::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm,const float fCalFlux)
{
	
	assert(NULL != pbResult);
	assert(fCalFlux>10e-6 && fCalFlux<400.0f);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	// L/sת��Ϊ����Ӣ��/����,��������
	WORD wCalFlux = WORD(fCalFlux / 1000.0f / 0.028315f * 60.0f + 0.5f);

	if (HighFlowCal == cm) 
	{
		// ������У׼
		bWriteBuf[0] = 0x3A;		//��ʼ�ַ�	
		bCheck[0] = 0x01;			//��ַ
		bCheck[1] = 0x06; 			//������
		bCheck[2] = 0x12;
		bCheck[3] = 0x62;			//���ݵ�ַ
		bCheck[4] = HIBYTE(wCalFlux);  
		bCheck[5] = LOBYTE(wCalFlux);	
		bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
		sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
		bWriteBuf[15] = 0x0D;
		bWriteBuf[16] = 0x0A;		//�����ַ�CR LF
	}
	else if (LowFlowCal == cm)
	{
		// ������У׼
		bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
		bCheck[0] = 0x01;			//��ַ
		bCheck[1] = 0x06; 			//������
		bCheck[2] = 0x12;
		bCheck[3] = 0x63;			//���ݵ�ַ
		bCheck[4] = HIBYTE(wCalFlux);  
		bCheck[5] = LOBYTE(wCalFlux);	
		bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
		sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
		bWriteBuf[15] = 0x0D;
		bWriteBuf[16] = 0x0A;		//�����ַ�CR LF
	}
	else if (SinglePointFlowCal == cm)
	{
		// ��������У׼
		bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
		bCheck[0] = 0x01;			//��ַ
		bCheck[1] = 0x06; 			//������
		bCheck[2] = 0x12;
		bCheck[3] = 0x64;			//���ݵ�ַ
		bCheck[4] = HIBYTE(wCalFlux);  
		bCheck[5] = LOBYTE(wCalFlux);	
		bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
		sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
		bWriteBuf[15] = 0x0D;
		bWriteBuf[16] = 0x0A;		//�����ַ�CR LF
	}

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//8.ϡ���������̱궨
DWORD CNanhuaNHF1::SetCalibrationDilO2_Flowmeter_High(BYTE *const pbResult, const float fCalDilO2)
{
	
	assert(NULL != pbResult);
	assert(fCalDilO2>10e-6f && fCalDilO2<100.0f);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	WORD wCalDilO2 = WORD(fCalDilO2 * 100.0f + 0.5f);

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x65;			//���ݵ�ַ
	bCheck[4] = HIBYTE(wCalDilO2);  
	bCheck[5] = LOBYTE(wCalDilO2);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//9.ϡ���������̱궨
DWORD CNanhuaNHF1::SetCalibrationDilO2_Flowmeter_Middle(BYTE *const pbResult, const float fCalDilO2)
{
	
	assert(NULL != pbResult);
	assert(fCalDilO2>10e-6f && fCalDilO2<100.0f);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	WORD wCalDilO2 = WORD(fCalDilO2 * 100.0f + 0.5f);

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x66;			//���ݵ�ַ
	bCheck[4] = HIBYTE(wCalDilO2);  
	bCheck[5] = LOBYTE(wCalDilO2);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//10.ϡ���������̱궨
DWORD CNanhuaNHF1::SetCalibrationDilO2_Flowmeter_Low(BYTE *const pbResult, const float fCalDilO2)
{
	
	assert(NULL != pbResult);
	assert(fCalDilO2>10e-6f && fCalDilO2<100.0f);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	WORD wCalDilO2 = WORD(fCalDilO2 * 100.0f + 0.5f);

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x67;			//���ݵ�ַ
	bCheck[4] = HIBYTE(wCalDilO2);  
	bCheck[5] = LOBYTE(wCalDilO2);	 
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);		//У����
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF
	
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}

//11.�ָ���������
DWORD CNanhuaNHF1::RestoreFactorySettings_Flowmeter(BYTE *const pbResult)
{
	
	assert(NULL != pbResult);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(bCheck));

	bWriteBuf[0] = 0x3A;		//��ʼ�ַ�:
	bCheck[0] = 0x01;			//��ַ
	bCheck[1] = 0x06; 			//������
	bCheck[2] = 0x12;
	bCheck[3] = 0x6C;			//���ݵ�ַ
	bCheck[4] = 0x00; 
	bCheck[5] = 0x01;			//����
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//�����ַ�CR LF
	
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// ��¼���
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


