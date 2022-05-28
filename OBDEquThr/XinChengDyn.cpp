#include "StdAfx.h"
#include "XinChengDyn.h"


CXinChengDyn::CXinChengDyn(void)
	: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&cInfoSection);
}


CXinChengDyn::~CXinChengDyn(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();
	::DeleteCriticalSection(&cInfoSection);
}


// �򿪴���
DWORD CXinChengDyn::Open(const BYTE bPort)
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
	// �趨������Ϊ38400 bps
	dcb.BaudRate = CBR_38400;
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
DWORD CXinChengDyn::Close(void)
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

bool CXinChengDyn::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

BYTE CXinChengDyn::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum(0);
	for (UINT ui=0; ui<un; ui++)
	{
		bSum += pb[ui];
	}

	return bSum;
}

//ʮ������ת��ʮ������   IEEE754������
uint32_t CXinChengDyn::data2hex(uint16_t data)
{
	uint32_t temp;
	uint8_t i = 0;

	temp = data;//ת����Ҫ�������ֵ  
	while (temp)
	{
		temp >>= 1;
		i++;//���㵱ǰֵ��β��ռ�е�λ��  
	}
	i--;//����������i����һ�Σ��������  
	temp = data;  //�ٴθ�ֵ��׼���ϲ�
	temp = temp << (23 - i);//����23λβ��  
	temp = (i + 127) << 23 | temp;//����ָ��������β���ϲ�����   
	temp = temp & ~(1 << 23);//ȷ������  �����������������û�ܸ�����  

	return temp;//������Ѿ����Ը������Ĵ洢��ʽ��ʾ�Ĵ������Ĳ�����
}

unsigned int CXinChengDyn::flo_hex(float fvalue) 
{
	float temp = fvalue;
	unsigned int *hvalue;
	hvalue = (unsigned int *)&temp;
	//printf("%X\n", *hvalue);
	return *hvalue;
}

LONGLONG CXinChengDyn::ReadCharToLL(unsigned char* str, int len)
{
	if (len <= 0)
	{
		return 0;
	}

	int i = 0;
	int value = 0;
	for (i = 0; i<len; i++)
	{
		value = value * 16 * 16 + (str[i] / 16) * 16 + str[i] % 16;
	}

	return value;
}

float CXinChengDyn::Hex2Float(unsigned long long number)
{
	unsigned int sign = (number & 0x80000000) ? -1 : 1;//��Ŀ�����
	unsigned int exponent = ((number >> 23) & 0xff) - 127;//�����Ʋ������ٰ�λ����㣬���������30��23λ��Ӧ��e
	float mantissa = 1 + ((float)(number & 0x7fffff) / 0x7fffff);//��22~0ת��Ϊ10���ƣ��õ���Ӧ��x
	return sign * mantissa * pow((double)2, (double)exponent);
}


DWORD CXinChengDyn::WriteAndReadData(BYTE *const pbWriteBuffer, 
	const DWORD dwWriteLength, 
	BYTE *const pbReadBuffer, 
	const DWORD dwReadLength,  
	const bool bNeedWriteChkSum, 
	const bool bNeedReadChkSum)
{
	//assert(NULL != pbWriteBuffer);
	//assert(dwWriteLength > 1);
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

		// �ж��Ƿ���Ҫ����д���ݲ���
		if (NULL != pbWriteBuffer)
		{
			if (bNeedWriteChkSum && dwWriteLength>1)
			{
				// ��ҪдУ��
				pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
			}
			WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		}

		// һ�ζ���Ԥ��Ҫ���յ��ֽ���
		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwReadLength, &dwBytesRead, NULL);
		// �ɹ������Ͷ������ֽ�����ͬ
		if (bSucceed && dwReadLength==dwBytesRead)
		{
			if (dwReadLength>1 && bNeedReadChkSum)
			{
				// ��֤У���
				if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer, dwReadLength-1))
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

			// �������󣺶�ָ������ʧ��
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

DWORD CXinChengDyn::GetRealTimeData(float *const pfP/*=NULL*/, float *const pfF/*=NULL*/, float *const pfV/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'I';
	bWriteBuf[2] = 'I';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pfP = 0.0f;
	*pfF = 0.0f;
	*pfV = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		// �ٶȣ�25.6km/h��Ť����850.7N��pwmռ�ձȣ�20.0%
		// AII+0xcd+0xcc+0xcc+0x41+0xcd+0xac+0x54+0x44 +0x0+0x0+0xa0+0x41��0x0��C
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'I')
			&& (bReadBuf[2] == 'I'))
		{
			unsigned char szbuff[4] = { 0 };

			// 
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pfP = (float)Hex2Float(ll);

			// 
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pfF = (float)Hex2Float(ll);

			// ����ѹ
			szbuff[3] = bReadBuf[11];
			szbuff[2] = bReadBuf[12];
			szbuff[1] = bReadBuf[13];
			szbuff[0] = bReadBuf[14];
			ll = ReadCharToLL(szbuff, 4);

			*pfV = (float)Hex2Float(ll);
		}
	}
	return dw;
}


// ���ú��ٶ�Ŀ��ֵ
// (in)fVTarget�����ٶ�Ŀ��ֵ����λ-km/h
DWORD CXinChengDyn::SetConstantVelocityTarget(const float fVTarget)
{
	unsigned int v = 0;
	v = flo_hex(fVTarget);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'S';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ���ú���Ŀ��ֵ
// (in)fFTarget������Ŀ��ֵ����λ-N
DWORD CXinChengDyn::SetConstantForceTarget(const float fFTarget)
{
	unsigned int v = 0;
	v = flo_hex(fFTarget);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'F';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ���ú㹦��Ŀ��ֵ
// (in)fPTarget���㹦��Ŀ��ֵ����λ-kW
DWORD CXinChengDyn::SetConstantPowerTarget(const float fPTarget)
{
	unsigned int v = 0;
	v = flo_hex(fPTarget);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'P';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ����IGBT��pwm��ռ�ձ�(%)
// ����pwmռ�ձ�Ϊ50%��
// XSI +0x00+0x20+0x7a+0x44+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantIGBT(const float fpwm)
{
	unsigned int v = 0;
	v = flo_hex(fpwm);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'I';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ����ѡ�õ���ͨ����ͨ��1��ͨ��2��ͨ��3��
// Ĭ��ֵΪͨ��1��������Ϊͨ��1/2/3����������ֱ���õ�һ�������ֽ�����ʾѡ�õ�ͨ����
// XSI +0+0+0+1/2/3+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceChannel(const int nChannel)
{
	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'C';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	// ��֪�Ƿ���Ҫ������ʱ������
	bWriteBuf[6] = nChannel;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ������
//��Ŀǰ����ʹ�õ�ͨ���������㣺
// XSZ+0+0+0+0+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceClearing(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'Z';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// �������ֵ
DWORD CXinChengDyn::XC_ClearKeyValue(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'K';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ���ù�Ͳֱ��(cm)��
// ���Ͳֱ��Ϊ14.736cm��
//XCS +0xa8+0xc6+0x6b+0x41+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantRollerDiameter(const int nRollerDim)
{
	unsigned int v = 0;
	v = flo_hex((float)nRollerDim);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'G';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ���ñ�����������(ÿȦ)
// �������������Ϊ1000��
//XCS +0x00+0x00+0x7a+0x44+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantEncoderPulNum(const int nEnPulNum)
{
	unsigned int v = 0;
	v = flo_hex((float)nEnPulNum);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'P';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ����ͨ��1���궨ϵ��b[0]��force=b[0]*(ad_value-c[0])
// ����ϵ��b[0]=1158.4��
// XCA +0xcd+0xcc+0x90+0x44+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceCailFac_B1(const float fb)
{
	unsigned int v = 0;
	v = flo_hex((float)fb);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'A';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����ͨ��1���궨ϵ��c[0]��	
// ����ϵ��c[0]=0.01��
//XCB +0x0a+0xd7+0x23+0x3c+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceCailFac_C1(const float fc)
{
	unsigned int v = 0;
	v = flo_hex((float)fc);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'B';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����ͨ��2���궨ϵ��b[1]��	
//����ϵ��b[1]=1158.4��
//XCC +0xcd+0xcc+0x90+0x44+ ��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceCailFac_B2(const float fb)
{
	unsigned int v = 0;
	v = flo_hex((float)fb);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'C';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����ͨ��2���궨ϵ��c[1]��	
//����ϵ��c[1]=0.01��
//XCD +0x0a+0xd7+0x23+0x3c + ��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceCailFac_C2(const float fc)
{
	unsigned int v = 0;
	v = flo_hex((float)fc);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'D';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����ͨ��3���궨ϵ��b[2]��	
//����ϵ��b[2]=1158.4��
//XCE +0xcd+0xcc+0x90+0x44 + ��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceCailFac_B3(const float fb)
{
	unsigned int v = 0;
	v = flo_hex((float)fb);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'E';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����ͨ��3���궨ϵ��c[2]��	
//����ϵ��c[2]=0.01��
//XCF +0x0a+0xd7+0x23+0x3c  +��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceCailFac_C3(const float fc)
{
	unsigned int v = 0;
	v = flo_hex((float)fc);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'F';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//���ú��ٿ���PID���Ʊ���ϵ��kp��	
//����ϵ��c[2]=0.01��
//XCH +0x0a+0xd7+0x23+0x3c  +��У���룫C
DWORD CXinChengDyn::XC_SetConstantSpeedControl_P(const float fkp)
{
	unsigned int v = 0;
	v = flo_hex((float)fkp);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'H';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//���ú��ٿ���PID���ƻ���ϵ��ki��	
//����ϵ��c[2]=0.01��
//XCI +0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CXinChengDyn::XC_SetConstantSpeedControl_I(const float fki)
{
	unsigned int v = 0;
	v = flo_hex((float)fki);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'I';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//���ú��ٿ���PID����΢��ϵ��kd��	
//����ϵ��c[2]=0.01��
//XCJ+0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CXinChengDyn::XC_SetConstantSpeedControl_D(const float fkd)
{
	unsigned int v = 0;
	v = flo_hex((float)fkd);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'J';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//���ú�������PID���Ʊ���ϵ��kp_force��	
//����ϵ��c[2]=0.01��
//XCK +0x0a+0xd7+0x23+0x3c  +��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceControl_P(const float fkp)
{
	unsigned int v = 0;
	v = flo_hex((float)fkp);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'K';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//���ú�������PID���Ʊ���ϵ�� ki_force��	
//����ϵ��c[2]=0.01��
//XCL +0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceControl_I(const float fki)
{
	unsigned int v = 0;
	v = flo_hex((float)fki);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'L';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//���ú�������PID���Ʊ���ϵ�� kd_force��	
//����ϵ��c[2]=0.01��
//XCM +0x0a+0xd7+0x23+0x3c +��У���룫C
DWORD CXinChengDyn::XC_SetConstantForceControl_D(const float fkd)
{
	unsigned int v = 0;
	v = flo_hex((float)fkd);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'C';
	bWriteBuf[2] = 'M';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// �������ٶȿ���
//�������ٶȿ���	XBS + 0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::ConstantVelocityControlModeOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'S';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//������Ť�����	XBF + 0+0+0+0+ ��У���� �� C
// ������������ģʽ
DWORD CXinChengDyn::ConstantForceControlModeOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'F';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//�����㹦�ʿ���	XBP + 0+0+ 0+0+ ��У���� �� C
// �����㹦�ʿ���ģʽ
DWORD CXinChengDyn::ConstantPowerControlModeOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'P';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//��������趨pwmֵ	XBI + 0+0+ 0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_StartSetPWN(const float fPWN)
{
	// �Ƿ���Ҫ��������
	unsigned int v = 0;
	v = flo_hex((float)fPWN);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'I';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//�õ����ֽ�
	bWriteBuf[4] = (BYTE)(l >> 8);//�õ����ֽ�
	bWriteBuf[5] = (BYTE)(h & 255);//�õ����ֽ�
	bWriteBuf[6] = (BYTE)(h >> 8);//�õ����ֽ�
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//ֹͣ����,pwm���0	XBS + 0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_StopPWN(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'Q';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// �������
//�𶯵��	XEB + 0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::MotorOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'B';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// ֹͣ���
//ֹͣ���	XES + 0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::MotorOff(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'S';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//��ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH1: XEO + 0+0+0+1+ ��У���� �� C
DWORD CXinChengDyn::XC_OpenRelay(const int nLH)
{
	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'Y';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	// ��֪�Ƿ���Ҫ������ʱ������
	bWriteBuf[6] = nLH;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//�ر�ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH3: XEF + 0+0+0+3+ ��У���� �� C
DWORD CXinChengDyn::XC_CloseRelay(const int nLH)
{
	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'N';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	// ��֪�Ƿ���Ҫ������ʱ������
	bWriteBuf[6] = nLH;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����������	XJU+ 0+0+0+0+ ��У���� �� C
// ���������
DWORD CXinChengDyn::LiftBeamUp(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'J';
	bWriteBuf[2] = 'U';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//�������½�	XJD +0+0+0+0+ ��У���� �� C
// ���¾�����
DWORD CXinChengDyn::LiftBeamDown(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'J';
	bWriteBuf[2] = 'D';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����궨״̬	XDF +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_EnterCaliSta(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'D';
	bWriteBuf[2] = 'F';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	return dw;
}

//�˳��궨״̬	 XDS +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_ExitCaliSta(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'D';
	bWriteBuf[2] = 'S';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}


//��ȡ��������	 XRT +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_ReadEnvPar(float *const pfET/*=NULL*/, float *const pfAP/*=NULL*/, float *const pfRH/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'T';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pfET = 0.0f;
	*pfAP = 0.0f;
	*pfRH = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		// ART+0x00+0x00+0x06+0x42+0x00+0x00+0x83+0x42 +0x9a+0x99+0xbf+0x42��0x0�� C
		// �ж��Ƿ�ART
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 'T'))
		{
			unsigned char szbuff[4] = { 0 };

			// �¶�
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pfET = (float)Hex2Float(ll);

			// ʪ��
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pfRH = (float)Hex2Float(ll);

			// ����ѹ
			szbuff[3] = bReadBuf[11];
			szbuff[2] = bReadBuf[12];
			szbuff[1] = bReadBuf[13];
			szbuff[0] = bReadBuf[14];
			ll = ReadCharToLL(szbuff, 4);

			*pfAP = (float)Hex2Float(ll);
		}
	}
	return dw;
}

//��ȡ���궨ϵ��b,c	XRF +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_ReadForCalFac(float *const pfb/*=NULL*/, float *const pfc/*=NULL*/, float *const pfb1/*=NULL*/, 
	float *const pfc1/*=NULL*/, float *const pfb2/*=NULL*/, float *const pfc2/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'F';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pfb = 0.0f;
	*pfc = 0.0f;
	*pfb1 = 0.0f;
	*pfc1 = 0.0f;
	*pfb2 = 0.0f;
	*pfc2 = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		// b[0]=1158.4��c[0]=0.01; b[1]=1128.6��c[1]=0.02;b[2]=1000.4��c[2]=0.03;
		// ARA+0xcd+0xcc+0x90+0x44+0xa+0xd7+0x23+0x3c +0x33+0x13+0x8D+0x44+ 0x0��C 
		// ARB+0xa+0xd7+0xa3+0x3c+0x9a+0x19+0x7a+0x44 +0x8f+0xc2+0xf5+0x3c +0x0�� C

		// �ж��Ƿ�ART
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			)
		{
			if (bReadBuf[2] == 'A')
			{
				unsigned char szbuff[4] = { 0 };

				// b[0] // c[1]
				szbuff[3] = bReadBuf[3];
				szbuff[2] = bReadBuf[4];
				szbuff[1] = bReadBuf[5];
				szbuff[0] = bReadBuf[6];
				unsigned long ll = ReadCharToLL(szbuff, 4);

				*pfb = (float)Hex2Float(ll);

				// c[0]// b[2]
				szbuff[3] = bReadBuf[7];
				szbuff[2] = bReadBuf[8];
				szbuff[1] = bReadBuf[9];
				szbuff[0] = bReadBuf[10];
				ll = ReadCharToLL(szbuff, 4);

				*pfc = (float)Hex2Float(ll);

				// b[1] // c[2]
				szbuff[3] = bReadBuf[11];
				szbuff[2] = bReadBuf[12];
				szbuff[1] = bReadBuf[13];
				szbuff[0] = bReadBuf[14];
				ll = ReadCharToLL(szbuff, 4);

				*pfb1 = (float)Hex2Float(ll);
			}
			else if (bReadBuf[2] == 'B')
			{
				unsigned char szbuff[4] = { 0 };

				// b[0] // c[1]
				szbuff[3] = bReadBuf[3];
				szbuff[2] = bReadBuf[4];
				szbuff[1] = bReadBuf[5];
				szbuff[0] = bReadBuf[6];
				unsigned long ll = ReadCharToLL(szbuff, 4);

				*pfc1 = (float)Hex2Float(ll);

				// c[0]// b[2]
				szbuff[3] = bReadBuf[7];
				szbuff[2] = bReadBuf[8];
				szbuff[1] = bReadBuf[9];
				szbuff[0] = bReadBuf[10];
				ll = ReadCharToLL(szbuff, 4);

				*pfb2 = (float)Hex2Float(ll);

				// b[1] // c[2]
				szbuff[3] = bReadBuf[11];
				szbuff[2] = bReadBuf[12];
				szbuff[1] = bReadBuf[13];
				szbuff[0] = bReadBuf[14];
				ll = ReadCharToLL(szbuff, 4);

				*pfc2 = (float)Hex2Float(ll);
			}
		}
	}
	return dw;
}

//��ȡ��ͨ��	XRC +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_ReadForCha(int *const pnCha/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'C';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pnCha = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		//kp +ki+kd 
		///5ms����	kp=0.01��ki=0.01�� kd=0.01��
		//ARs+ 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0x0��C 
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 'C'))
		{
			unsigned char szbuff[4] = { 0 };

			// 
			*pnCha = (int)bReadBuf[6];
		}
	}
	return dw;
}

//��ȡ����PIDϵ��	XRs +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_ReadConSpeed(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 's';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pfP = 0.0f;
	*pfI = 0.0f;
	*pfD = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		//kp +ki+kd 
		///5ms����	kp=0.01��ki=0.01�� kd=0.01��
		//ARs+ 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0x0��C 
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 's'))
		{
			unsigned char szbuff[4] = { 0 };

			// �¶�
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pfP = (float)Hex2Float(ll);

			// ʪ��
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pfI = (float)Hex2Float(ll);

			// ����ѹ
			szbuff[3] = bReadBuf[11];
			szbuff[2] = bReadBuf[12];
			szbuff[1] = bReadBuf[13];
			szbuff[0] = bReadBuf[14];
			ll = ReadCharToLL(szbuff, 4);

			*pfD = (float)Hex2Float(ll);
		}
	}
	return dw;
}

//��ȡ����PIDϵ��	XRf +0+0+0+0+ ��У���� �� C	
DWORD CXinChengDyn::XC_ReadConForce(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'f';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pfP = 0.0f;
	*pfI = 0.0f;
	*pfD = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		//kp +ki+kd 
		///5ms����	kp=0.01��ki=0.01�� kd=0.01��
		//ARs+ 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0x0��C 
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 'f'))
		{
			unsigned char szbuff[4] = { 0 };

			// �¶�
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pfP = (float)Hex2Float(ll);

			// ʪ��
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pfI = (float)Hex2Float(ll);

			// ����ѹ
			szbuff[3] = bReadBuf[11];
			szbuff[2] = bReadBuf[12];
			szbuff[1] = bReadBuf[13];
			szbuff[0] = bReadBuf[14];
			ll = ReadCharToLL(szbuff, 4);

			*pfD = (float)Hex2Float(ll);
		}
	}
	return dw;
}

//��ȡ��Ͳֱ���ͱ���������	 XRS +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_ReadRollerAndEncoder(int *const pnDia/*=NULL*/, int *const pnPul/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'S';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pnDia = 0;
	*pnPul = 0;
	if (DYN_WR_SUCCEED == dw)
	{
		//ֱ��Ϊ21.6cm������Ϊ3600��
		//	ARS+0x41+0xac+0xcc+0xcd+0x45+0x61+0+0+0+ 0+0+0��0x0��C

		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 'S'))
		{
			unsigned char szbuff[4] = { 0 };

			// ֱ��
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pnDia = (int)Hex2Float(ll);

			// ����
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pnPul = (int)Hex2Float(ll);
		}
	}
	return dw;
}

//��3��ͨ���ı궨ϵ���̻���ROM��	 XMA +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_SolCalFac(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'F';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����Ͳֱ���ͱ���������̻���ROM��	XMS +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_SolRollerAndEncoder(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'S';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//����ͨ����ѡ��̻���ROM��	XMC +0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_SolForCha(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'C';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//�����ٿ��Ƶ�PIDϵ���̻���ROM��	XMD + 0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_SolConSheepPID(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 's';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//���������Ƶ�PIDϵ���̻���ROM��	XME + 0+0+0+0+ ��У���� �� C
DWORD CXinChengDyn::XC_SolConForcePID(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'f';
	// float����(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// �����
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}