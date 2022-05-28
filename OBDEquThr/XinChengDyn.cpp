#include "StdAfx.h"
#include "XinChengDyn.h"


CXinChengDyn::CXinChengDyn(void)
	: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&cInfoSection);
}


CXinChengDyn::~CXinChengDyn(void)
{
	// 对象销毁时,关闭串口资源
	Close();
	::DeleteCriticalSection(&cInfoSection);
}


// 打开串口
DWORD CXinChengDyn::Open(const BYTE bPort)
{
	// 关闭已存在的串口句柄
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
		// 打开串口失败
		return DYN_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数begin
	// 设置输入和输出缓存区大小
	SetupComm(m_hComm, 512, 256);
	// 设置DCB结构
	DCB dcb;
	// 获取缺省的DCB结构的值
	GetCommState(m_hComm, &dcb);
	// 设定波特率为38400 bps
	dcb.BaudRate = CBR_38400;
	// 设定无奇偶校验
	dcb.Parity = NOPARITY;
	// 设定数据位为8
	dcb.ByteSize = 8;
	// 设定一个停止位
	dcb.StopBits = ONESTOPBIT;
	// 设置串行设备控制参数
	SetCommState(m_hComm, &dcb);
	// 进行超时设置（100毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 500;  // 如果设10,取实时数据超时
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	// 打开串口成功
	return DYN_OPEN_SUCCEED;
}

// 关闭串口
DWORD CXinChengDyn::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// 关闭串口成功
			return DYN_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32 API失败
			return DYN_CLOSE_FAIL_WIN32API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
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

//十进制数转成十六进制   IEEE754单精度
uint32_t CXinChengDyn::data2hex(uint16_t data)
{
	uint32_t temp;
	uint8_t i = 0;

	temp = data;//转存需要计算的数值  
	while (temp)
	{
		temp >>= 1;
		i++;//计算当前值的尾数占有的位数  
	}
	i--;//计算下来，i会多加一次，这儿减掉  
	temp = data;  //再次赋值，准备合并
	temp = temp << (23 - i);//补足23位尾数  
	temp = (i + 127) << 23 | temp;//计算指数，并与尾数合并起来   
	temp = temp & ~(1 << 23);//确定正负  我这儿都是正数，就没管负数了  

	return temp;//这里就已经是以浮点数的存储方式表示的传进来的参数了
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
	unsigned int sign = (number & 0x80000000) ? -1 : 1;//三目运算符
	unsigned int exponent = ((number >> 23) & 0xff) - 127;//先右移操作，再按位与计算，出来结果是30到23位对应的e
	float mantissa = 1 + ((float)(number & 0x7fffff) / 0x7fffff);//将22~0转化为10进制，得到对应的x
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
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		// 判断是否需要进行写数据操作
		if (NULL != pbWriteBuffer)
		{
			if (bNeedWriteChkSum && dwWriteLength>1)
			{
				// 需要写校验
				pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
			}
			WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		}

		// 一次读完预计要接收的字节数
		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwReadLength, &dwBytesRead, NULL);
		// 成功读出和读出的字节数相同
		if (bSucceed && dwReadLength==dwBytesRead)
		{
			if (dwReadLength>1 && bNeedReadChkSum)
			{
				// 验证校验和
				if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer, dwReadLength-1))
				{
					::LeaveCriticalSection(&cInfoSection);

					// 函数错误：校验读出数据失败
					return DYN_WR_CHECK_SUM_FAIL;
				}
				else
				{
					::LeaveCriticalSection(&cInfoSection);

					// 函数成功
					return DYN_WR_SUCCEED;
				}
			}
			else
			{
				::LeaveCriticalSection(&cInfoSection);

				// 函数成功
				return DYN_WR_SUCCEED;
			}

		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);

			// 函数错误：读指定数据失败
			return DYN_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// 函数错误：串口没有打开
		return DYN_WR_PORT_NOT_OPEN;
	}
}

DWORD CXinChengDyn::GetRealTimeData(float *const pfP/*=NULL*/, float *const pfF/*=NULL*/, float *const pfV/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'I';
	bWriteBuf[2] = 'I';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pfP = 0.0f;
	*pfF = 0.0f;
	*pfV = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		// 速度：25.6km/h、扭力：850.7N、pwm占空比：20.0%
		// AII+0xcd+0xcc+0xcc+0x41+0xcd+0xac+0x54+0x44 +0x0+0x0+0xa0+0x41＋0x0＋C
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

			// 大气压
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


// 设置恒速度目标值
// (in)fVTarget：恒速度目标值，单位-km/h
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 设置恒力目标值
// (in)fFTarget：恒力目标值，单位-N
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 设置恒功率目标值
// (in)fPTarget：恒功率目标值，单位-kW
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 设置IGBT的pwm波占空比(%)
// 设置pwm占空比为50%：
// XSI +0x00+0x20+0x7a+0x44+ 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 设置选用的力通道（通道1，通道2，通道3）
// 默认值为通道1：如设置为通道1/2/3，该条命令直接用第一个数据字节来表示选用的通道。
// XSI +0+0+0+1/2/3+ 和校验码＋C
DWORD CXinChengDyn::XC_SetConstantForceChannel(const int nChannel)
{
	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'C';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	// 不知是否需要处理，暂时不处理
	bWriteBuf[6] = nChannel;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 力清零
//对目前设置使用的通道进行清零：
// XSZ+0+0+0+0+ 和校验码＋C
DWORD CXinChengDyn::XC_SetConstantForceClearing(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'Z';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 清除按键值
DWORD CXinChengDyn::XC_ClearKeyValue(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'S';
	bWriteBuf[2] = 'K';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 设置滚筒直径(cm)：
// 如滚筒直径为14.736cm：
//XCS +0xa8+0xc6+0x6b+0x41+ 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 设置编码器脉冲数(每圈)
// 如编码器脉冲数为1000：
//XCS +0x00+0x00+0x7a+0x44+ 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 设置通道1力标定系数b[0]：force=b[0]*(ad_value-c[0])
// 设置系数b[0]=1158.4：
// XCA +0xcd+0xcc+0x90+0x44+ 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置通道1力标定系数c[0]：	
// 设置系数c[0]=0.01：
//XCB +0x0a+0xd7+0x23+0x3c+ 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置通道2力标定系数b[1]：	
//设置系数b[1]=1158.4：
//XCC +0xcd+0xcc+0x90+0x44+ 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置通道2力标定系数c[1]：	
//设置系数c[1]=0.01：
//XCD +0x0a+0xd7+0x23+0x3c + 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置通道3力标定系数b[2]：	
//设置系数b[2]=1158.4：
//XCE +0xcd+0xcc+0x90+0x44 + 和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置通道3力标定系数c[2]：	
//设置系数c[2]=0.01：
//XCF +0x0a+0xd7+0x23+0x3c  +和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置恒速控制PID控制比例系数kp：	
//设置系数c[2]=0.01：
//XCH +0x0a+0xd7+0x23+0x3c  +和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置恒速控制PID控制积分系数ki：	
//设置系数c[2]=0.01：
//XCI +0x0a+0xd7+0x23+0x3c +和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置恒速控制PID控制微分系数kd：	
//设置系数c[2]=0.01：
//XCJ+0x0a+0xd7+0x23+0x3c +和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置恒力控制PID控制比例系数kp_force：	
//设置系数c[2]=0.01：
//XCK +0x0a+0xd7+0x23+0x3c  +和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置恒力控制PID控制比例系数 ki_force：	
//设置系数c[2]=0.01：
//XCL +0x0a+0xd7+0x23+0x3c +和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//设置恒力控制PID控制比例系数 kd_force：	
//设置系数c[2]=0.01：
//XCM +0x0a+0xd7+0x23+0x3c +和校验码＋C
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
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 启动恒速度控制
//启动恒速度控制	XBS + 0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::ConstantVelocityControlModeOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'S';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//启动恒扭距控制	XBF + 0+0+0+0+ 和校验码 ＋ C
// 启动恒力控制模式
DWORD CXinChengDyn::ConstantForceControlModeOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'F';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//启动恒功率控制	XBP + 0+0+ 0+0+ 和校验码 ＋ C
// 启动恒功率控制模式
DWORD CXinChengDyn::ConstantPowerControlModeOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'P';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//启动输出设定pwm值	XBI + 0+0+ 0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_StartSetPWN(const float fPWN)
{
	// 是否需要参数？？
	unsigned int v = 0;
	v = flo_hex((float)fPWN);
	DWORD h = HIWORD(v);
	DWORD l = LOWORD(v);

	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'I';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = (BYTE)(l & 255);//得到低字节
	bWriteBuf[4] = (BYTE)(l >> 8);//得到高字节
	bWriteBuf[5] = (BYTE)(h & 255);//得到低字节
	bWriteBuf[6] = (BYTE)(h >> 8);//得到高字节
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//停止控制,pwm输出0	XBS + 0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_StopPWN(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'B';
	bWriteBuf[2] = 'Q';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 启动电机
//起动电机	XEB + 0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::MotorOn(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'B';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

// 停止电机
//停止电机	XES + 0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::MotorOff(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'S';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//打开指定继电器（LH1/LH2/LH3）	（LH1/LH2/LH3）分别对应数据1/2/3：如打开继电器LH1: XEO + 0+0+0+1+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_OpenRelay(const int nLH)
{
	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'Y';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	// 不知是否需要处理，暂时不处理
	bWriteBuf[6] = nLH;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//关闭指定继电器（LH1/LH2/LH3）	（LH1/LH2/LH3）分别对应数据1/2/3：如打开继电器LH3: XEF + 0+0+0+3+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_CloseRelay(const int nLH)
{
	BYTE bWriteBuf[9];

	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'E';
	bWriteBuf[2] = 'N';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	// 不知是否需要处理，暂时不处理
	bWriteBuf[6] = nLH;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//举升器上升	XJU+ 0+0+0+0+ 和校验码 ＋ C
// 升起举升器
DWORD CXinChengDyn::LiftBeamUp(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'J';
	bWriteBuf[2] = 'U';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//举升器下降	XJD +0+0+0+0+ 和校验码 ＋ C
// 降下举升器
DWORD CXinChengDyn::LiftBeamDown(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'J';
	bWriteBuf[2] = 'D';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//进入标定状态	XDF +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_EnterCaliSta(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'D';
	bWriteBuf[2] = 'F';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	return dw;
}

//退出标定状态	 XDS +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_ExitCaliSta(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'D';
	bWriteBuf[2] = 'S';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}


//读取环境参数	 XRT +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_ReadEnvPar(float *const pfET/*=NULL*/, float *const pfAP/*=NULL*/, float *const pfRH/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'T';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pfET = 0.0f;
	*pfAP = 0.0f;
	*pfRH = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		// ART+0x00+0x00+0x06+0x42+0x00+0x00+0x83+0x42 +0x9a+0x99+0xbf+0x42＋0x0＋ C
		// 判断是否ART
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 'T'))
		{
			unsigned char szbuff[4] = { 0 };

			// 温度
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pfET = (float)Hex2Float(ll);

			// 湿度
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pfRH = (float)Hex2Float(ll);

			// 大气压
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

//读取力标定系数b,c	XRF +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_ReadForCalFac(float *const pfb/*=NULL*/, float *const pfc/*=NULL*/, float *const pfb1/*=NULL*/, 
	float *const pfc1/*=NULL*/, float *const pfb2/*=NULL*/, float *const pfc2/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'F';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
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
		// b[0]=1158.4、c[0]=0.01; b[1]=1128.6、c[1]=0.02;b[2]=1000.4、c[2]=0.03;
		// ARA+0xcd+0xcc+0x90+0x44+0xa+0xd7+0x23+0x3c +0x33+0x13+0x8D+0x44+ 0x0＋C 
		// ARB+0xa+0xd7+0xa3+0x3c+0x9a+0x19+0x7a+0x44 +0x8f+0xc2+0xf5+0x3c +0x0＋ C

		// 判断是否ART
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

//读取力通道	XRC +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_ReadForCha(int *const pnCha/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'C';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pnCha = 0.0f;
	if (DYN_WR_SUCCEED == dw)
	{
		//kp +ki+kd 
		///5ms以内	kp=0.01、ki=0.01、 kd=0.01、
		//ARs+ 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0x0＋C 
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

//读取恒速PID系数	XRs +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_ReadConSpeed(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 's';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
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
		///5ms以内	kp=0.01、ki=0.01、 kd=0.01、
		//ARs+ 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0x0＋C 
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 's'))
		{
			unsigned char szbuff[4] = { 0 };

			// 温度
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pfP = (float)Hex2Float(ll);

			// 湿度
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pfI = (float)Hex2Float(ll);

			// 大气压
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

//读取恒力PID系数	XRf +0+0+0+0+ 和校验码 ＋ C	
DWORD CXinChengDyn::XC_ReadConForce(float *const pfP/*=NULL*/, float *const pfI/*=NULL*/, float *const pfD/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'f';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
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
		///5ms以内	kp=0.01、ki=0.01、 kd=0.01、
		//ARs+ 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0xa+ 0xd7+ 0x23+ 0x3c + 0x0＋C 
		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 'f'))
		{
			unsigned char szbuff[4] = { 0 };

			// 温度
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pfP = (float)Hex2Float(ll);

			// 湿度
			szbuff[3] = bReadBuf[7];
			szbuff[2] = bReadBuf[8];
			szbuff[1] = bReadBuf[9];
			szbuff[0] = bReadBuf[10];
			ll = ReadCharToLL(szbuff, 4);

			*pfI = (float)Hex2Float(ll);

			// 大气压
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

//读取滚筒直径和编码器脉冲	 XRS +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_ReadRollerAndEncoder(int *const pnDia/*=NULL*/, int *const pnPul/*=NULL*/)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'R';
	bWriteBuf[2] = 'S';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	DWORD dw = WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);

	*pnDia = 0;
	*pnPul = 0;
	if (DYN_WR_SUCCEED == dw)
	{
		//直径为21.6cm，脉冲为3600：
		//	ARS+0x41+0xac+0xcc+0xcd+0x45+0x61+0+0+0+ 0+0+0＋0x0＋C

		if (bReadBuf[0] == 'A'
			&& (bReadBuf[1] == 'R')
			&& (bReadBuf[2] == 'S'))
		{
			unsigned char szbuff[4] = { 0 };

			// 直径
			szbuff[3] = bReadBuf[3];
			szbuff[2] = bReadBuf[4];
			szbuff[1] = bReadBuf[5];
			szbuff[0] = bReadBuf[6];
			unsigned long ll = ReadCharToLL(szbuff, 4);

			*pnDia = (int)Hex2Float(ll);

			// 脉冲
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

//将3个通道的标定系数固化到ROM中	 XMA +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_SolCalFac(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'F';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//将滚筒直径和编码器脉冲固化到ROM中	XMS +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_SolRollerAndEncoder(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'S';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//将力通道的选择固化到ROM中	XMC +0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_SolForCha(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'C';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//将恒速控制的PID系数固化到ROM中	XMD + 0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_SolConSheepPID(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 's';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}

//将恒力控制的PID系数固化到ROM中	XME + 0+0+0+0+ 和校验码 ＋ C
DWORD CXinChengDyn::XC_SolConForcePID(void)
{
	BYTE bWriteBuf[9];
	bWriteBuf[0] = 'X';
	bWriteBuf[1] = 'M';
	bWriteBuf[2] = 'f';
	// float数据(BYTE1+BYTE2+BYTE3+BYTE4)
	bWriteBuf[3] = 0x00;
	bWriteBuf[4] = 0x00;
	bWriteBuf[5] = 0x00;
	bWriteBuf[6] = 0x00;
	// 检验和
	bWriteBuf[7] = 0x00;
	bWriteBuf[8] = 'C';

	BYTE bReadBuf[17] = {0};

	return WriteAndReadData(bWriteBuf, 9, bReadBuf, 17, true, false);
}