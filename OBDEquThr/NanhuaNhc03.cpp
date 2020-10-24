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
	// 对象销毁时,关闭串口资源
	Close();
	::DeleteCriticalSection(&cInfoSection);
}

// 打开串口
DWORD CNanhuaNhc03::Open(const BYTE bPort)
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
DWORD CNanhuaNhc03::Close(void)
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
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		if (bNeedWriteChkSum && dwWriteLength>1)
		{
			// 需要写校验
			pbWriteBuffer[dwWriteLength-1] = CheckSum(pbWriteBuffer, dwWriteLength-1);
		}
		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// 分二次读数，第一次数据头（1byte），第二次读剩下的有效数据

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 1, &dwBytesRead, NULL);
		if (bSucceed && 1UL==dwBytesRead)
		{
			// 判断非法或错误指令返回
			if (0x15 != pbReadBuffer[0])
			{
				if (1 == dwReadLength)
				{
					// 只返回1个数据

					::LeaveCriticalSection(&cInfoSection);
					// 操作成功
					return DYN_WR_SUCCEED;
				}
				else
				{
					// 不只返回1个数据

					// 读余下的数据
					bSucceed = ReadFile(m_hComm, pbReadBuffer+1, dwReadLength-1, &dwBytesRead, NULL);
					if(bSucceed && dwBytesRead==(dwReadLength-1))
					{
						if (bNeedReadChkSum)
						{
							// 需要读校验

							// 验证校验和
							if (pbReadBuffer[dwReadLength-1] != CheckSum(pbReadBuffer,dwReadLength-1))
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
						// 函数错误：读指定数据失败（读余下有效数据失败）
						return DYN_WR_READ_SPECIFIED_DATA_FAIL;
					}
				}
			}
			else
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);
				// 函数错误：仪器返回错误状态码
				return DYN_WR_EQUIP_RETURE_0X15;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// 函数错误：读指定数据失败（读数据头失败）
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

// 取实时数据
DWORD CNanhuaNhc03::GetRealTimeData(float *const pfP, float *const pfF, float *const pfV)
{
	BYTE bWriteBuf[7] = {0x08, 'C', 0x07, 0x00, 0x00, 0x00, 0x52};
	BYTE bReadBuf[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 9, false, true);
	if (DYN_WR_SUCCEED == dw)
	{
		if (NULL != pfP)
		{
			// 功率(kW)
			*pfP = MAKEWORD(bReadBuf[2], bReadBuf[1]) / 100.0f;
		}
		if (NULL != pfF)
		{
			// 扭矩(N)
			*pfF = MAKEWORD(bReadBuf[4], bReadBuf[3]) + bReadBuf[5] / 10.0f;
		}
		if (NULL != pfV)
		{
			// 速度(km/h)
			*pfV = MAKEWORD(bReadBuf[7], bReadBuf[6]) / 100.0f;
		}
	}
	else
	{
		// 发生错误

		if (NULL != pfP)
		{
			// 功率(kW)
			*pfP = 0.0f;
		}
		if (NULL != pfF)
		{
			// 扭矩(N)
			*pfF = 0.0f;
		}
		if (NULL != pfV)
		{
			// 速度(km/h)
			*pfV = 0.0f;
		}
	}
	return dw;
}

// 取状态
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
		// 发生错误
		if (NULL != pbs16Status)
		{
			*pbs16Status = 0x0000;
		}
	}
	return dw;
}

// 读变量
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
		// 发生错误
		*pwVarValue = 0x0000;
	}
	return dw;
}

// 写变量
DWORD CNanhuaNhc03::WriteVar(const BYTE bVarNumber, const WORD wVarValue)
{
	BYTE bWriteBuf[7] = {0x08, 'F', 0x07, bVarNumber, HIBYTE(wVarValue), LOBYTE(wVarValue), 0x00};
	BYTE bReadBuf[1] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, 7, bReadBuf, 1, true, false);
	return dw;
}

// 发送控制字
DWORD CNanhuaNhc03::SendCtrlWord(const BYTE bCtrlWord)
{
	//// 只能是以下命令
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

// 升起举升器
DWORD CNanhuaNhc03::LiftBeamUp(void)
{
	return SendCtrlWord('5');
}

// 降下举升器
DWORD CNanhuaNhc03::LiftBeamDown(void)
{
	return SendCtrlWord('6');
}

// 启动电机
DWORD CNanhuaNhc03::MotorOn(void)
{
	return SendCtrlWord('J');
}

// 停止电机
DWORD CNanhuaNhc03::MotorOff(void)
{
	return SendCtrlWord('K');
}

// 退出控制模式
DWORD CNanhuaNhc03::ControlModeOff(void)
{
	return SendCtrlWord('0');
}

// 启动ASM加载模式
DWORD CNanhuaNhc03::AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses)
{
	// 扩大100倍,四舍五入
	assert(fAsmSpeed >= 0.0f);
	WORD wAsmSpeedOn = (WORD)(fAsmSpeed*100.0f + 0.5f);
	// 扩大100倍,四舍五入
	assert(fTargetPower >= 0.0f);
	WORD wTargetPowerOn = (WORD)(fTargetPower*100.0f + 0.5f);
	WriteVar(229, 0);
	WriteVar(230, wAsmSpeedOn);
	WriteVar(228, wTargetPowerOn);

	return SendCtrlWord('4');
}

// 设置恒功率目标值
// (in)fPTarget：恒功率目标值，单位-kW
DWORD CNanhuaNhc03::SetConstantPowerTarget(const float fPTarget)
{
	WORD wPTarget = WORD(fPTarget * 100.0f + 0.5f);
	return WriteVar(228, wPTarget);
}

// 启动恒功率控制模式
DWORD CNanhuaNhc03::ConstantPowerControlModeOn(void)
{
	return SendCtrlWord('7');
}

// 设置恒力目标值
// (in)fFTarget：恒力目标值，单位-N
DWORD CNanhuaNhc03::SetConstantForceTarget(const float fFTarget)
{
	// 整数位
	WORD wFTarget = (WORD)fFTarget;
	DWORD dw = WriteVar(201, wFTarget);
	if (DYN_WR_SUCCEED != dw)
	{
		return dw;
	}
	// 小数位
	wFTarget = WORD(fFTarget * 10.0f + 0.5f) % 10;
	return WriteVar(202, wFTarget);
}

// 启动恒力控制模式
DWORD CNanhuaNhc03::ConstantForceControlModeOn(void)
{
	return SendCtrlWord('2');
}

// 启动变载荷模式
DWORD CNanhuaNhc03::VarLoadControlModeOn(void)
{
	return SendCtrlWord('Q');
}

// 设置恒速度目标值
// (in)fVTarget：恒速度目标值，单位-km/h
DWORD CNanhuaNhc03::SetConstantVelocityTarget(const float fVTarget)
{
	assert(fVTarget >= 0.0f);

	WORD wVTarget = WORD(fVTarget * 100.0f + 0.5f);
	return WriteVar(200, wVTarget);
}

// 启动恒速度控制
DWORD CNanhuaNhc03::ConstantVelocityControlModeOn(void)
{
	return SendCtrlWord('1');
}

// 设置秒表1开始速度
// (in)fUpperVelocity：开始速度，单位-km/h
DWORD CNanhuaNhc03::SetUpperVelocityOfTimer1(const float fUpperVelocity)
{
	assert(fUpperVelocity >= 0.0f);

	WORD wV = WORD(fUpperVelocity * 100.0f + 0.5f);
	return WriteVar(207, wV);
}

// 设置秒表1中间速度
// (in)fMiddleVelocity：中间速度，单位-km/h
DWORD CNanhuaNhc03::SetMiddleVelocityOfTimer1(const float fMiddleVelocity)
{
	assert(fMiddleVelocity >= 0.0f);

	WORD wV = WORD(fMiddleVelocity * 100.0f + 0.5f);
	return WriteVar(232, wV);
}

// 设置秒表1停止速度
// (in)fLowerVelocity：停止速度，单位-km/h
DWORD CNanhuaNhc03::SetLowerVelocityOfTimer1(const float fLowerVelocity)
{
	assert(fLowerVelocity >= 0.0f);

	WORD wV = WORD(fLowerVelocity * 100.0f + 0.5f);
	return WriteVar(208, wV);
}

// 设置测功机电机最大速度
// (in)fMaxVelocity: 最大速度，单位-km/h
DWORD CNanhuaNhc03::SetMotorMaxVelocity(const float fMaxVelocity)
{
	assert(fMaxVelocity>0.0f && fMaxVelocity<130.0f);

	const WORD wV = WORD(fMaxVelocity * 0.8f);
	return WriteVar(44, wV);

}

// 获取测功机电机最大速度
// (out)fMaxVelocity: 最大速度，单位-km/h
DWORD CNanhuaNhc03::GetMotorMaxVelocity(float *const fMaxVelocity)
{
	WORD Frequency(0);
	DWORD dw = ReadVar(44, &Frequency);
	*fMaxVelocity = (Frequency * 100.0f) / 80.0f ;
	return dw;
}
// 启动秒表1
DWORD CNanhuaNhc03::SetTimer1(void)
{
	return SendCtrlWord('H');
}

// 关闭秒表1
DWORD CNanhuaNhc03::KillTimer1(void)
{
	return SendCtrlWord('I');
}

// 获取秒表1时间
// (out)punTime_MS;时间指针，单位-ms
DWORD CNanhuaNhc03::GetTimeOfTimer1(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);
	//秒表1计时值＝（Ram[215]*65535+Ram[227]）*0.005秒 20170726 新改了协议 change by qin
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

// 获取秒表1中间时间
// (out)punTime_MS;时间指针，单位-ms
DWORD CNanhuaNhc03::GetMiddleTimeOfTimer1(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);
  //秒表1中间计时值＝（Ram[215]*65535+Ram[233]）*0.005秒 20170726 新改了协议 change by qin
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

// 设置秒表2开始速度
// (in)fUpperVelocity：开始速度，单位-km/h
DWORD CNanhuaNhc03::SetUpperVelocityOfTimer2(const float fUpperVelocity)
{
	assert(fUpperVelocity >= 0.0f);

	WORD wV = WORD(fUpperVelocity * 100.0f + 0.5f);
	return WriteVar(234, wV);
}

// 设置秒表2停止速度
// (in)fLowerVelocity：停止速度，单位-km/h
DWORD CNanhuaNhc03::SetLowerVelocityOfTimer2(const float fLowerVelocity)
{
	assert(fLowerVelocity >= 0.0f);

	WORD wV = WORD(fLowerVelocity * 100.0f + 0.5f);
	return WriteVar(235, wV);
}


// 启动秒表2
DWORD CNanhuaNhc03::SetTimer2(void)
{
	return SendCtrlWord('R');
}

// 关闭秒表2
DWORD CNanhuaNhc03::KillTimer2(void)
{
	return SendCtrlWord('S');
}

// 获取秒表2时间
// (out)punTime_MS;时间指针，单位-ms
DWORD CNanhuaNhc03::GetTimeOfTimer2(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);
	//秒表2计时值＝（Ram[237]*65535+Ram[236]）*0.005秒 20170726 新改了协议 change by qin
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

// 获取基本惯量
// (out)pusDIW：基本惯量指针，单位-kg
DWORD CNanhuaNhc03::GetDIW(USHORT *const pusDIW)
{
	assert(NULL != pusDIW);

	return ReadVar(6, pusDIW);
}

// 设置基本惯量
DWORD CNanhuaNhc03::SetDIW(const USHORT usDIW)
{
	return WriteVar(6, usDIW);
}

// 写驱动阻力模拟控制模式的车辆基准质量[kg]
// (in)车辆基准质量,单位-kg
DWORD CNanhuaNhc03::WriteVehicleRM(const USHORT usVehicleRM)
{
	return WriteVar(246, usVehicleRM);
}


// 写驱动阻力模拟控制模式的Cv
// 内部将会转化为80km/h对应的值
// (in)Cv值，50km/h对应的值，单位:kW
DWORD CNanhuaNhc03::WriteCv(const float fCv)
{
	// 放大100倍,通讯协议要求
	// 转化为80km/h对应的Cv值(*4.096)
	// 进行四舍五入
	USHORT usCv = USHORT(fCv*100.0f*4.096f + 0.5f);
	return WriteVar(203, usCv);
}

// 写驱动阻力模拟控制模式的Bv
// (in)Bv值，单位:kW
DWORD CNanhuaNhc03::WriteBv(const float fBv)
{
	// 放大100倍,通讯协议要求
	// 进行四舍五入
	USHORT usBv = USHORT(fBv*100.0f + 0.5f);
	return WriteVar(204, usBv);
}

// 写驱动阻力模拟控制模式的Av
// (in)Av值，单位:kW
DWORD CNanhuaNhc03::WriteAv(const float fAv)
{
	// 放大100倍,通讯协议要求
	// 进行四舍五入
	USHORT usAv = USHORT(fAv*100.0f + 0.5f);
	return WriteVar(205, usAv);
}

// 写驱动阻力模拟控制模式的At
// (in)At值，单位:kW
DWORD CNanhuaNhc03::WriteAt(const float fAt)
{
	// 放大100倍,通讯协议要求
	// 进行四舍五入
	USHORT usAt = USHORT(fAt*100.0f + 0.5f);
	return WriteVar(212, usAt);
}

// 写驱动阻力模拟控制模式的Bt
// (in)Bt值，单位:kW
DWORD CNanhuaNhc03::WriteBt(const float fBt)
{
	// 放大100倍,通讯协议要求
	// 进行四舍五入
	USHORT usBt = USHORT(fBt*100.0f + 0.5f);
	return WriteVar(213, usBt);
}

// 写驱动阻力模拟控制模式的Ct
// (in)Ct值，单位:kW
DWORD CNanhuaNhc03::WriteCt(const float fCt)
{
	// 放大100倍,通讯协议要求
	// 进行四舍五入
	USHORT usCt = USHORT(fCt*100.0f + 0.5f);
	return WriteVar(214, usCt);
}

// 启动驱动阻力模拟控制模式
DWORD CNanhuaNhc03::DriveResistanceSimulationControlModeOn(void)
{
	return SendCtrlWord('3');
}

// 重设行驶距离
DWORD CNanhuaNhc03::ResetDistance(void)
{
	return SendCtrlWord('9');
}

// 读取行驶距离
// (out)pusDistance:距离指针,单位:米(m)
DWORD CNanhuaNhc03::ReadDistance(USHORT *const pusDistance)
{
	assert(NULL != pusDistance);

	return ReadVar(225, pusDistance);
}


// 读取显示Ap(未去除剩磁力)
// (out)fAp:功率,单位:kW
DWORD CNanhuaNhc03::ReadApWithRemanence(float *const pfAp)
{
	assert(NULL != pfAp);

	WORD wAp(0);
	DWORD dw = ReadVar(82, &wAp);
	//*pfAp = wAp / 100.0f;
	ConvertWordToFloat(wAp, pfAp);
	return dw;
}

// 读取显示Bp(未去除剩磁力)
// (out)fBp:功率,单位:kW
DWORD CNanhuaNhc03::ReadBpWithRemanence(float *const pfBp)
{
	assert(NULL != pfBp);

	WORD wBp(0);
	DWORD dw = ReadVar(83, &wBp);
	//*pfBp = wBp / 100.0f;
	ConvertWordToFloat(wBp, pfBp);
	return dw;
}

// 读取显示Cp(未去除剩磁力)
// (out)fCp:功率,单位:kW
DWORD CNanhuaNhc03::ReadCpWithRemanence(float *const pfCp)
{
	assert(NULL != pfCp);

	WORD wCp(0);
	DWORD dw = ReadVar(84, &wCp);
	//*pfCp = wCp / 100.0f;
	ConvertWordToFloat(wCp, pfCp);
	return dw;
}

// 写入显示Ap(未去除剩磁力)
// (in)fAp:功率，单位:kW
DWORD CNanhuaNhc03::WriteApWithRemanence(const float fAp)
{
	//// 放大100倍，并四舍五入
	//WORD wAp = WORD(fAp*100.0f + 0.5f);

	WORD wAp(0);
	ConvertFloatToWord(fAp, &wAp);

	return WriteVar(82, wAp);
}

// 写入显示Bp(未去除剩磁力)
// (in)fBp:功率，单位:kW
DWORD CNanhuaNhc03::WriteBpWithRemanence(const float fBp)
{
	//// 放大100倍，并四舍五入
	//WORD wBp = WORD(fBp*100.0f + 0.5f);

	WORD wBp(0);
	ConvertFloatToWord(fBp, &wBp);

	return WriteVar(83, wBp);
}

// 写入显示Cp(未去除剩磁力)
// (in)fCp:功率，单位:kW
DWORD CNanhuaNhc03::WriteCpWithRemanence(const float fCp)
{
	//// 放大100倍，并四舍五入
	//WORD wCp = WORD(fCp*100.0f + 0.5f);

	WORD wCp(0);
	ConvertFloatToWord(fCp, &wCp);

	return WriteVar(84, wCp);
}

// 读取真实Ap(去除剩磁力后)
// (out)fAp:功率,单位:kW
DWORD CNanhuaNhc03::ReadApWithoutRemanence(float *const pfAp)
{
	assert(NULL != pfAp);

	WORD wAp(0);
	DWORD dw = ReadVar(39, &wAp);
	//*pfAp = wAp / 100.0f;
	ConvertWordToFloat(wAp, pfAp);
	return dw;
}

// 读取真实Bp(去除剩磁力后)
// (out)fBp:功率,单位:kW
DWORD CNanhuaNhc03::ReadBpWithoutRemanence(float *const pfBp)
{
	assert(NULL != pfBp);

	WORD wBp(0);
	DWORD dw = ReadVar(40, &wBp);
	//*pfBp = wBp / 100.0f;
	ConvertWordToFloat(wBp, pfBp);
	return dw;
}

// 读取真实Cp(去除剩磁力后)
// (out)fCp:功率,单位:kW
DWORD CNanhuaNhc03::ReadCpWithoutRemanence(float *const pfCp)
{
	assert(NULL != pfCp);

	WORD wCp(0);
	DWORD dw = ReadVar(41, &wCp);
	//*pfCp = wCp / 100.0f;
	ConvertWordToFloat(wCp, pfCp);
	return dw;
}

// 写入真实Ap(去除剩磁力后)
// (in)fAp:功率，单位:kW
DWORD CNanhuaNhc03::WriteApWithoutRemanence(const float fAp)
{
	//// 放大100倍，并四舍五入
	//WORD wAp = WORD(fAp*100.0f + 0.5f);

	WORD wAp(0);
	ConvertFloatToWord(fAp, &wAp);

	return WriteVar(39, wAp);
}

// 写入真实Bp(去除剩磁力后)
// (in)fBp:功率，单位:kW
DWORD CNanhuaNhc03::WriteBpWithoutRemanence(const float fBp)
{
	//// 放大100倍，并四舍五入
	//WORD wBp = WORD(fBp*100.0f + 0.5f);

	WORD wBp(0);
	ConvertFloatToWord(fBp, &wBp);

	return WriteVar(40, wBp);
}

// 写入真实Cp(去除剩磁力后)
// (in)fCp:功率，单位:kW
DWORD CNanhuaNhc03::WriteCpWithoutRemanence(const float fCp)
{
	//// 放大100倍，并四舍五入
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
		// float是非负数
		*pwDestination = WORD(fSource*100.0f + 0.5f);
	}
	else
	{
		// float是负数
		// 取绝对值
		const float f = fabs(fSource);
		*pwDestination = WORD(f*100.0f + 0.5f);
		// 将符号位(最高位)置为1
		*pwDestination = 0x8000 | *pwDestination;
	}
}

void CNanhuaNhc03::ConvertWordToFloat(const WORD wSource, float *const pfDestination)
{
	assert(NULL != pfDestination);

	if (0 ==(0x8000&wSource))
	{
		// 符号位(最高位)为0，即非负数
		*pfDestination = wSource / 100.0f;
	}
	else
	{
		// 符号位(最高位)为1，即负数
		// 获取除了非符号位的其它15位
		const WORD w = 0x7FFF & wSource;
		// 置为负数
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
			// 力(N)
			for (int i=0; i<nLength; i++)
			{
				pfF[i] = MAKEWORD(bReadBuf[i*2+3], bReadBuf[i*2+2]);
			}
		}
		if (NULL != pfV)
		{
			int j(0);
			// 速度(10m/h)
			for (int i=nLength; i<nLength*2; i++)
			{
				pfV[j] = MAKEWORD(bReadBuf[i*2+3], bReadBuf[i*2+2])/100.0f;
				j++;
			}
		}
	}
	else
	{
		// 发生错误
		if (NULL != pfF)
		{
			// 扭矩(N)
			for(int i= 0; i<10; i++)
			{
				pfF[i] = 0.0f;
			}
		}
		if (NULL != pfV)
		{
			// 速度(km/h)
			for(int i= 0; i<10; i++)
			{
				pfV[i] = 0.0f;
			}
		}
	}
	delete[] bReadBuf;
	return dw;
}