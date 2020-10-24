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
	// 对象销毁时,关闭串口资源
	Close();

	//::DeleteCriticalSection(&cInfoSection);
}

// 打开串口
DWORD CNanhuaNHF1::Open(BYTE bPort)
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
		// 打开串口失败
		OutputDebugStringW(L"Error:fail to open com port\n");
		return ANA_OPEN_FAIL;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数begin
	// 设置输入和输出缓存区大小
	SetupComm(m_hComm, 512, 256);
	// 设置DCB结构
	DCB dcb;
	// 获取缺省的DCB结构的值
	GetCommState(m_hComm, &dcb);
	// 设定波特率为9600 bps
	dcb.BaudRate = CBR_9600;
	// 设定无奇偶校验
	dcb.Parity = NOPARITY;
	// 设定数据位为8
	dcb.ByteSize = 8;
	// 设定一个停止位
	dcb.StopBits = ONESTOPBIT;
	// 设置串行设备控制参数
	SetCommState(m_hComm, &dcb);
	// 进行超时设置（2500毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 2500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	return ANA_OPEN_SUCCEED;
}

// 关闭串口
DWORD CNanhuaNHF1::Close(void)
{
	if (IsOpen())
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// 关闭串口成功
			return ANA_CLOSE_SUCCEED;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			return ANA_CLOSE_FAIL_WIN32_API;
		}
	}
	else
	{
		// 关闭串口失败：串口没有成功打开
		return ANA_CLOSE_FAIL_NO_OPEN_PORT;
	}
}

bool CNanhuaNHF1::IsOpen(void) const
{
	return m_hComm != INVALID_HANDLE_VALUE;
}


//校验读校验和
bool CNanhuaNHF1::CheckReadSum_Flowmeter(const BYTE *const pb, const UINT un)
{
	assert(NULL != pb);
	assert(0 != un);
	
	BYTE bCheckSum(0),bRealSum(0);
	int iTemp = 0,iTotal = 0;

	//sscanf直接十六进制转BYTE类型会出现越界，因此使用int类型中间变量
	//排除起始字符，从第二个字符开始，每两个字符以十六进制形式转为int类型
	for(int i=1; i<un-4; i=i+2)
	{
		sscanf((char*)pb + i,"%02X",&iTemp);
		iTotal = iTotal + iTemp;
	}

	//累加后截取为BYTE，最后取反加一得出校验码
	bCheckSum = ~(BYTE)iTotal + 1;
	sscanf((char*)pb + un - 4,"%02X",&iTemp);
	bRealSum = (BYTE)iTemp;
	
	return bCheckSum == bRealSum;
	
}

//计算写校验和
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
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		// 重置并清空串口
		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		WriteFile(m_hComm, bWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// 分二次读数
		// 第一次数据头（5byte）
		// 第二次读剩下的有效数据(有效数据根据第一次读回来的数据不同而不同)

		BOOL bSucceed = ReadFile(m_hComm, bReadBuffer, 5, &dwBytesRead, NULL);
		if (bSucceed && 5==dwBytesRead)
		{
			if (0x33||0x36 == bReadBuffer[4])
			{
				// 读余下的数据
				bSucceed = ReadFile(m_hComm, bReadBuffer+5, dwReadLength-5, &dwBytesRead, NULL);
				if(bSucceed && dwBytesRead==(dwReadLength-5))
				{
					if (bNeedReadChkSum)
					{
						if (!CheckReadSum_Flowmeter(bReadBuffer,dwReadLength))
						{
							::LeaveCriticalSection(&cInfoSection);
							// 函数错误：校验读出数据失败
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
					// 函数错误：读指定数据失败（读余下有效数据失败）
					return ANA_WR_READ_SPECIFIED_DATA_FAIL;
				}
			}
			else
			{
				// 流量计命令错误
				// 清除后续无用数据
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				::LeaveCriticalSection(&cInfoSection);
				// 函数错误：仪器返回错误码
				return ANA_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// 函数错误：读指定数据失败（读数据头失败）
			return ANA_WR_READ_SPECIFIED_DATA_FAIL;
		}
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// 函数错误：串口没有打开
		return ANA_WR_PORT_NOT_OPEN;
	}
}


//1.获取稀释氧，气体温度，气体压力，工况流量，标准流量
DWORD CNanhuaNHF1::GetRealTimeData_Flowmeter(float *const pfFluxStd0, float *const pfFluxStd20, float *const pfFluxAct, float *const pfPres, float *const pfUDilO2, float *const pfGasTemp, BYTE *const pbResult)
{
	assert(NULL != pbResult);
	
	const DWORD dwBytesToRead(31);
	const DWORD dwBytesToWrite(17);
	BYTE bWriteBuf[dwBytesToWrite], bReadBuf[dwBytesToRead], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;	//起始字符:	
	bCheck[0] = 0x01;		//地址
	bCheck[1] = 0x03; 		//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x58;		//数据地址
	bCheck[4] = 0x00;  
	bCheck[5] = 0x05;	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);		//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;	//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytesToWrite, bReadBuf, dwBytesToRead, true, true);
	// 记录结果

	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[6];	//41H
	}
	
	if (ANA_WR_SUCCEED == dw)
	{
		if (0x41 == bReadBuf[6])
		{
			int iTemp = 0;

			// UDilO2：未修正稀释氧
			float fUDilO2 = 0;
			sscanf((const char*)bReadBuf+7,"%04X",&iTemp);
			fUDilO2 = iTemp / 100.0f;
			if (NULL != pfUDilO2)
			{
				*pfUDilO2 = fUDilO2;
			}

			// 气体压力
			float fPres = 0;
			sscanf((const char*)bReadBuf+11,"%04X",&iTemp);
			fPres = iTemp / 100.0f;
			if (NULL != pfPres)
			{
				*pfPres = fPres;
			}

			// 稀释气温度
			float fGasTemp = 0;
			sscanf((const char*)bReadBuf+15,"%04X",&iTemp);
			fGasTemp = iTemp / 10.0f;
			if (NULL != pfGasTemp)
			{
				*pfGasTemp = fGasTemp;
			}

			// 实际流量
			float fFluxAct = 0;
			sscanf((const char*)bReadBuf+19,"%04X",&iTemp);
			// 标准立方英尺/分钟->升/秒
			fFluxAct = iTemp * 0.471917f;
			// 保留2位小数
			fFluxAct = CNHCommonAPI::round(fFluxAct * 100.0f) / 100.0f;
			if (NULL != pfFluxAct)
			{
				*pfFluxAct = fFluxAct;
			}

			// 0℃标准流量
			float fFluxStd0 = 0;
			sscanf((const char*)bReadBuf+23,"%04X",&iTemp);
			// 标准立方英尺/分钟->升/秒
			fFluxStd0 = iTemp * 0.471917f;
			// 保留2位小数
			fFluxStd0 = CNHCommonAPI::round(fFluxStd0 * 100.0f) / 100.0f;
			if (NULL != pfFluxStd0)
			{
				*pfFluxStd0 = fFluxStd0;
			}		

			// 20℃标准流量
			if (NULL != pfFluxStd20)
			{
				*pfFluxStd20 = 0;
			}
		}
	}
	else
	{
		// Pres:压力，转化为kPa
		if (NULL != pfPres)
		{
			*pfPres = 0.0f;
		}
		// UDilO2：未修正稀释氧
		if (NULL != pfUDilO2)
		{
			*pfUDilO2 = 0.0f;
		}
		// 稀释气温度
		if (NULL != pfGasTemp)
		{
			*pfGasTemp = 0.0f;
		}
		// 20℃标准流量
		// 标准立方英尺/分钟->升/秒
		if (NULL != pfFluxStd20)
		{
			*pfFluxStd20 = 0;
		}
		// 0℃标准流量
		if (NULL != pfFluxStd0)
		{
			*pfFluxStd0 = 0;
		}
		// 实际流量
		if (NULL != pfFluxAct)
		{
			*pfFluxAct = 0;
		}
	}
	
	return dw;
}

//2.获取状态
DWORD CNanhuaNHF1::Getstatus_Flowmeter(BYTE *const pbResult,BYTE *const pbMachineStau,BYTE *const pbFanStau,BYTE *const pbFlowStau)
{
	assert(NULL != pbResult);
	
	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;		//起始字符:
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x03; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x5D;			//数据地址
	bCheck[4] = 0x00;  
	bCheck[5] = 0x01;	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[6];	//32H
		*pbMachineStau = bReadBuf[10];	//30H待机 31H预热
		*pbFanStau = bReadBuf[9];	//30H风机关闭 31H风机启动
		*pbFlowStau = bReadBuf[8];	//30H流量异常 31H流量正常
	}
	return dw;
}


//3.开关风机
DWORD CNanhuaNHF1::RunFan_Flowmeter(const bool bRun, BYTE *const pbResult)
{
	assert(NULL != pbResult);
	
	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;		//起始字符:	
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x5E;			//数据地址
	bCheck[4] = 0x00; 
	if (bRun)
	{
		bCheck[5] = 0x01;		// 开风机
	}
	else
	{
		bCheck[5] = 0x00;		// 关风机
	}		
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//4.稀释氧空气调零
DWORD CNanhuaNHF1::SetCalibrationDilO2_Flowmeter_Zero(BYTE *const pbResult)
{
	
	assert(NULL != pbResult);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	bWriteBuf[0] = 0x3A;		//起始字符:
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x5F;			//数据地址
	bCheck[4] = 0x00;  
	bCheck[5] = 0x01;	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}




//5.气体压力单点校准
DWORD CNanhuaNHF1::SetCalibrationPres_Flowmeter(BYTE *const pbResult, const float fCalPres)
{
	assert(NULL != pbResult);
	assert(fCalPres>50.0f && fCalPres<400.0f);
	
	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	// 由千帕->百帕,四舍五入
	WORD wCalPres = WORD(fCalPres * 100.0f + 0.5f);

	bWriteBuf[0] = 0x3A;		//起始字符:
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x60;			//数据地址
	bCheck[4] = HIBYTE((WORD)wCalPres);  
	bCheck[5] = LOBYTE((WORD)wCalPres);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//6.温度单点校准
DWORD CNanhuaNHF1::SetCalibrationTemp_Flowmeter(BYTE *const pbResult, const float fCalTemp)
{
	
	assert(NULL != pbResult);
	assert(fCalTemp>-50.0f && fCalTemp<100.0f);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	// 四舍五入
	WORD wCalTemp = WORD(fCalTemp * 10.0f + 0.5f);

	bWriteBuf[0] = 0x3A;		//起始字符:	
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;	
	bCheck[3] = 0x61;			//数据地址
	bCheck[4] = HIBYTE((WORD)wCalTemp);  
	bCheck[5] = LOBYTE((WORD)wCalTemp);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}



//7.流量高/低/单量程标定
DWORD CNanhuaNHF1::SetCalibrationFlux_Flowmeter(BYTE *const pbResult, const enum FluxCalibrationMode cm,const float fCalFlux)
{
	
	assert(NULL != pbResult);
	assert(fCalFlux>10e-6 && fCalFlux<400.0f);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(7));

	// L/s转化为立方英尺/分钟,四舍五入
	WORD wCalFlux = WORD(fCalFlux / 1000.0f / 0.028315f * 60.0f + 0.5f);

	if (HighFlowCal == cm) 
	{
		// 高流量校准
		bWriteBuf[0] = 0x3A;		//起始字符	
		bCheck[0] = 0x01;			//地址
		bCheck[1] = 0x06; 			//命令码
		bCheck[2] = 0x12;
		bCheck[3] = 0x62;			//数据地址
		bCheck[4] = HIBYTE(wCalFlux);  
		bCheck[5] = LOBYTE(wCalFlux);	
		bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
		sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
		bWriteBuf[15] = 0x0D;
		bWriteBuf[16] = 0x0A;		//结束字符CR LF
	}
	else if (LowFlowCal == cm)
	{
		// 低流量校准
		bWriteBuf[0] = 0x3A;		//起始字符:
		bCheck[0] = 0x01;			//地址
		bCheck[1] = 0x06; 			//命令码
		bCheck[2] = 0x12;
		bCheck[3] = 0x63;			//数据地址
		bCheck[4] = HIBYTE(wCalFlux);  
		bCheck[5] = LOBYTE(wCalFlux);	
		bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
		sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
		bWriteBuf[15] = 0x0D;
		bWriteBuf[16] = 0x0A;		//结束字符CR LF
	}
	else if (SinglePointFlowCal == cm)
	{
		// 单点流量校准
		bWriteBuf[0] = 0x3A;		//起始字符:
		bCheck[0] = 0x01;			//地址
		bCheck[1] = 0x06; 			//命令码
		bCheck[2] = 0x12;
		bCheck[3] = 0x64;			//数据地址
		bCheck[4] = HIBYTE(wCalFlux);  
		bCheck[5] = LOBYTE(wCalFlux);	
		bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
		sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
		bWriteBuf[15] = 0x0D;
		bWriteBuf[16] = 0x0A;		//结束字符CR LF
	}

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//8.稀释氧高量程标定
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

	bWriteBuf[0] = 0x3A;		//起始字符:
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x65;			//数据地址
	bCheck[4] = HIBYTE(wCalDilO2);  
	bCheck[5] = LOBYTE(wCalDilO2);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);		
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//9.稀释氧中量程标定
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

	bWriteBuf[0] = 0x3A;		//起始字符:
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x66;			//数据地址
	bCheck[4] = HIBYTE(wCalDilO2);  
	bCheck[5] = LOBYTE(wCalDilO2);	
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF

	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


//10.稀释氧低量程标定
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

	bWriteBuf[0] = 0x3A;		//起始字符:
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x67;			//数据地址
	bCheck[4] = HIBYTE(wCalDilO2);  
	bCheck[5] = LOBYTE(wCalDilO2);	 
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);		//校验码
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF
	
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}

//11.恢复出厂设置
DWORD CNanhuaNHF1::RestoreFactorySettings_Flowmeter(BYTE *const pbResult)
{
	
	assert(NULL != pbResult);

	const DWORD dwBytes(17);
	BYTE bWriteBuf[dwBytes], bReadBuf[dwBytes], bCheck[7];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	ZeroMemory(bCheck, sizeof(bCheck));

	bWriteBuf[0] = 0x3A;		//起始字符:
	bCheck[0] = 0x01;			//地址
	bCheck[1] = 0x06; 			//命令码
	bCheck[2] = 0x12;
	bCheck[3] = 0x6C;			//数据地址
	bCheck[4] = 0x00; 
	bCheck[5] = 0x01;			//数据
	bCheck[6] = CheckSum_Flowmeter(bCheck, 6);			
	sprintf((char*)bWriteBuf+1,"%02X%02X%02X%02X%02X%02X%02X",bCheck[0],bCheck[1],bCheck[2],bCheck[3],bCheck[4],bCheck[5],bCheck[6]);	
	bWriteBuf[15] = 0x0D;
	bWriteBuf[16] = 0x0A;		//结束字符CR LF
	
	DWORD dw = WriteAndReadData_Flowmeter(bWriteBuf, dwBytes, bReadBuf, dwBytes, true, true);
	// 记录结果
	if (ANA_WR_SUCCEED == dw)
	{
		*pbResult = bReadBuf[12];	//0x31
	}
	return dw;
}


