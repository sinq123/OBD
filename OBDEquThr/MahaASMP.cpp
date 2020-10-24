#include "StdAfx.h"
#include "MahaASMP.h"
#include <cstdio>
#include <cassert>
#include <cmath>

// hp->kW转换系数
// 1Hp(英制匹) = 0.7457kW
const double CMahaASMP::m_dConversionFactor_hp2kW = 0.7457;
// mph->km/h转换系数
// 1mph(英里每小时) = 1.609344km/h
const double CMahaASMP::m_dConversionFactor_mph2kmph = 1.609344;
// lbf->N转换系数
// 1lbf(磅推力:libra force) = 4.44822N 
const double CMahaASMP::m_dConversionFactor_lbf2N = 4.44822;
// lb->kg转换系数
// 1lb(磅) = 0.4536kg 
const double CMahaASMP::m_dConversionFactor_lb2kg = 0.4536;
// mi->km转换系数
// 1mi(英里) = 1.609344km
const double CMahaASMP::m_dConversionFactor_mi2km = 1.609344;

// 初始化马哈测功机Ram信息
//const int CMahaASMP::MahaDynRamInfoSize;
const CMahaASMP::MAHA_DYN_RAM_INFO CMahaASMP::m_MahaDynRamInfo[50] =
{
	// 每10个1组

	// 200~209
	{3, 1, "mph", "Target value for speed regulator [0.1 mph]"},
	{3, 1, "lbf", "Target value for traction regulator [0.1 lbf]"},
	{5, 0, "lb",  "Vehicle mass for drive resistance regulator [lb]"},
	{2, 2, "hp",  "Cv for drive simulation regulator [hp]"},
	{2, 2, "hp",  "Bv for drive simulation regulator [hp]"},
	{2, 2, "hp",  "Av for drive simulation regulator [hp]"},
	{3, 0, "%",   "Control for the brake test [%]"},
	{3, 1, "mph", "Start speed for the stop watch 1 [mph]"},
	{3, 1, "mph", "Stop speed for the stop watch 1 [mph]"},
	{3, 2, "mph", "Brake roller speed [mph]"},

	// 210~219
	{1, 3, "mph/s","Acceleration [mph/s]"},
	{3, 1, "hp",  "Power [Hp]"},
	{2, 2, "hp",  "At for tire-roll losses [hp]"},
	{2, 2, "hp",  "Bt for tire-roll losses [hp]"},
	{2, 2, "hp",  "Ct for tire-roll losses [hp]"},
	{3, 1, "mph", "Support roller speed [mph]"},
	{4, 0, "lb",  "Weight scale right [lb]"},
	{4, 0, "lb",  "Weight scale left [lb]"},
	{3, 0, "%",   "Inertia Simulation Error (ISE) [%]"},
	{1, 0, "",    "Performance-Test Flag"},

	// 220~229
	{3, 1, "lbf", "Traction - losses [lbf]"},
	{3, 1, "lbf", "Traction + losses [lbf]"},
	{3, 0, "F",   "Temperature of the force sensor [F]"},
	{2, 1, "A",   "Line current [A]"},
	{3, 0, "F",   "Temperature of the berings [F]"},
	{3, 2, "mi",  "Distance [miles]"},
	{2, 0, "",    " "},
	{3, 2, "s",   "Stop watch 1 [s]"},
	{3, 1, "hp",  "Power target value for ASM-Regulator [hp]"},
	{3, 2, "hp",  "Losses tires/roller for ASM Regulator [hp]"},

	// 230~239
	{3, 1, "mph", "Speed reference for ASM-Regulator [mph]"},
	{3, 1, "lbf", "Traction [lbf]"},
	{3, 1, "mph", "Speed for intermediate test [mph]"},
	{3, 2, "s",   "Stop watch 1 intermediate time [s]"},
	{3, 1, "mph", "Start-speed for the stop watch 2 [mph]"},
	{3, 1, "mph", "Stop-speed for the stop watch 2 [mph]"},
	{3, 2, "s",   "Stop watch 2 [s]"},
	{1, 0, "",    "Enable loss compensation [0/1]"},
	{4, 0, "lbf", "Total weight [lbf]"},
	{2, 2, "hp",  "Compensated Ap of the parasitic losses [Hp]"},

	// 240~249
	{2, 2, "hp",  "Compensated Bp of the parasitic losses [Hp]"},
	{2, 2, "hp",  "Compensated Cp of the parasitic losses [Hp]"},
	{2, 2, "hp",  "Compensated parasitic losses for ASM 5015 [Hp]"},
	{2, 2, "hp",  "Compensated parasitic losses for ASM 2525 [Hp]"},
	{3, 0, "",    "Measuring direction (0 = normal, 1 = reverse)"},
	{4, 0, "A2s", "I2t for temperature switch off [0.01 A2s]"},
	{3, 2, "",    " 46 "},
	{3, 2, "",    " 47 "},
	{3, 2, "",    " 48 "},
	{3, 2, "",    " 49 "}
};


CMahaASMP::CMahaASMP(void)
: m_hComm(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&cInfoSection);
}

CMahaASMP::~CMahaASMP(void)
{
	// 对象销毁时,关闭串口资源
	Close();
	::DeleteCriticalSection(&cInfoSection);
}


// 打开串口
DWORD CMahaASMP::Open(const BYTE bPort)
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
	// 进行超时设置（200毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 100;  // 经测试，不能设成南华测功机的20ms，100ms
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 200;  // 不能设成南华测功机的100ms，马哈测功机有些命令返回结果要100多ms，例如：升举升器
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end

	// 打开串口成功
	return DYN_OPEN_SUCCEED;
}

// 关闭串口
DWORD CMahaASMP::Close(void)
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

bool CMahaASMP::IsOpen(void) const
{
	bool b = (m_hComm!=INVALID_HANDLE_VALUE);
	return b;
}

DWORD CMahaASMP::WriteAndReadData(const BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const bool bNeedWriteChkSum, const bool bNeedReadChkSum)
{
	assert(NULL != pbWriteBuffer);
	assert(dwWriteLength > 1);
	assert(NULL != pbReadBuffer);
	assert(dwReadLength != 0);

	::EnterCriticalSection(&cInfoSection);
	if (IsOpen())
	{
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);
		
		// 分二次读数，第一次数据头（1byte），第二次读剩下的有效数据

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 1, &dwBytesRead, NULL);
		if (bSucceed && 1UL==dwBytesRead)
		{
			// 判断数据头是否STX
			if (0x02 == pbReadBuffer[0])
			{
				if (1UL == dwReadLength)
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
					if (bSucceed && dwBytesRead==(dwReadLength-1))
					{
						if (bNeedReadChkSum)
						{
							// 需要读校验

							// 验证校验和
							const BYTE bSum = CheckSum(pbReadBuffer, dwReadLength-3);
							char chSum[3];
							ZeroMemory(chSum, sizeof(chSum));
							_snprintf_s(chSum, _countof(chSum), _TRUNCATE, "%02X", bSum);
							// 高4bit的ASCII码
							const BYTE bHigh4BitASCIICode = static_cast<BYTE>(chSum[0]);
							// 低4bit的ASCII码
							const BYTE bLow4BitASCIICode = static_cast<BYTE>(chSum[1]);
							if (bHigh4BitASCIICode!=pbReadBuffer[dwReadLength-3] || bLow4BitASCIICode!=pbReadBuffer[dwReadLength-2])
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
			else if (0x06 == pbReadBuffer[0])
			{
				// TODO:需要确认0x06后面是否还有数据传过来

				::LeaveCriticalSection(&cInfoSection);
				// 操作成功
				return DYN_WR_SUCCEED;
			}
			else if (0x15 == pbReadBuffer[0])
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);
				// 函数错误：仪器返回的数据头0x15，仪器正在忙等原因造成，不能返回数据
				return DYN_WR_EQUIP_RETURE_0X15;
			}
			else
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);
				// 函数错误：未知错误（返回未知的数据头）
				// 一般不可能执行到这里
				return DYN_WR_UNKNOWN_ERROR;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// 函数错误：读指定数据失败（读数据头失败）
			return DYN_WR_READ_SPECIFIED_DATA_FAIL;
		}  // end of if (bSucceed && 1UL==dwBytesRead)
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// 函数错误：串口没有打开
		return DYN_WR_PORT_NOT_OPEN;
	}  // end of if (IsOpen())
}

DWORD CMahaASMP::WriteAndReadData(const BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadBufferLength, DWORD *const pdwReadLength, const bool bNeedWriteChkSum, const bool bNeedReadChkSum)
{
	assert(NULL != pbWriteBuffer);
	assert(dwWriteLength > 1);
	assert(NULL != pbReadBuffer);
	assert(0 != dwReadBufferLength);

	::EnterCriticalSection(&cInfoSection);
	if (IsOpen())
	{
		// 成功写入的数据字节数
		DWORD dwBytesWritten(0);
		// 成功读出的数据字节数
		DWORD dwBytesRead(0);

		PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		WriteFile(m_hComm, pbWriteBuffer, dwWriteLength, &dwBytesWritten, NULL);

		// 分二次读数，第一次数据头（1byte），第二次读剩下的有效数据

		BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, 1, &dwBytesRead, NULL);
		if (bSucceed && 1UL==dwBytesRead)
		{
			// 判断数据头是否STX
			if (0x02 == pbReadBuffer[0])
			{
				//////////////////////////////////////////////////////////////////////////////////// 循环读剩下的有效数据(begin)
				// 正常停止读数情况：1)遇到结束符'$' 2)读缓存已满
				// 异常停止读数情况：读指定数据失败
				bool bReadSpecifiedDataFail(false);
				DWORD dwReadLength(1);
				do 
				{
					bSucceed = ReadFile(m_hComm, pbReadBuffer+dwReadLength, 1, &dwBytesRead, NULL);
					if (bSucceed && 1UL==dwBytesRead)
					{
						dwReadLength++;
					}
					else
					{
						// 函数错误：读指定数据失败（读剩下的有效数据失败）
						bReadSpecifiedDataFail = true;
						break;
					}
				} while ('$'!=pbReadBuffer[dwReadLength-1] && dwReadBufferLength!=dwReadLength);
				//////////////////////////////////////////////////////////////////////////////////// 循环读剩下的有效数据(end)

				// 记录成功读出的总字节数
				if (NULL != pdwReadLength)
				{
					*pdwReadLength = dwReadLength;
				}

				if (bReadSpecifiedDataFail)
				{
					::LeaveCriticalSection(&cInfoSection);
					// 函数错误：读指定数据失败（读剩下的有效数据失败）
					return DYN_WR_READ_SPECIFIED_DATA_FAIL;
				}
				else
				{
					if (bNeedReadChkSum)
					{
						// 需要读校验

						// 验证校验和
						const BYTE bSum = CheckSum(pbReadBuffer, dwReadLength-3);
						char chSum[3];
						ZeroMemory(chSum, sizeof(chSum));
						_snprintf_s(chSum, _countof(chSum), _TRUNCATE, "%02X", bSum);
						// 高4bit的ASCII码
						const BYTE bHigh4BitASCIICode = static_cast<BYTE>(chSum[0]);
						// 低4bit的ASCII码
						const BYTE bLow4BitASCIICode = static_cast<BYTE>(chSum[1]);
						if (bHigh4BitASCIICode!=pbReadBuffer[dwReadLength-3] || bLow4BitASCIICode!=pbReadBuffer[dwReadLength-2])
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
			}
			else if (0x06 == pbReadBuffer[0])
			{
				// TODO:需要确认0x06后面是否还有数据传过来

				::LeaveCriticalSection(&cInfoSection);
				// 操作成功
				return DYN_WR_SUCCEED;
			}
			else if (0x15 == pbReadBuffer[0])
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);
				// 函数错误：仪器返回的数据头0x15，仪器正在忙等原因造成，不能返回数据
				return DYN_WR_EQUIP_RETURE_0X15;
			}
			else
			{
				PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

				::LeaveCriticalSection(&cInfoSection);
				// 函数错误：未知错误（返回未知的数据头）
				// 一般不可能执行到这里
				return DYN_WR_UNKNOWN_ERROR;
			}
		}
		else
		{
			::LeaveCriticalSection(&cInfoSection);
			// 函数错误：读指定数据失败（读数据头失败）
			return DYN_WR_READ_SPECIFIED_DATA_FAIL;
		}  // end of if (bSucceed && 1UL==dwBytesRead)
	}
	else
	{
		::LeaveCriticalSection(&cInfoSection);
		// 函数错误：串口没有打开
		return DYN_WR_PORT_NOT_OPEN;
	}  // end of if (IsOpen())
}

BYTE CMahaASMP::CheckSum(const BYTE *const pb, const UINT un)
{
	assert(NULL!=pb && 0UL!=un);

	BYTE bSum = pb[0];
	for (UINT ui=1; ui<un; ui++)
	{
		bSum ^= pb[ui];
	}
	return bSum;
}

DWORD CMahaASMP::GetRealTimeData(float *const pfP, float *const pfF, float *const pfV)
{
	assert(NULL!=pfP);
	assert(NULL!=pfF);
	assert(NULL!=pfV);

	const DWORD dwWriteLength = 4;
	const DWORD dwReadLength = 51;
	BYTE bWriteBuf[dwWriteLength] = {0xB4, 0x11, 'C', 0x05};
	BYTE bReadBuf[dwReadLength];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, dwWriteLength, bReadBuf, dwReadLength, false, true);
	if (DYN_WR_SUCCEED == dw)
	{
		float f(0.0f);
		const int nLengthInBytes(8);
		char *pch = new char[nLengthInBytes]();
		if (NULL != pch)
		{
			// 功率(kW)
			if (NULL != pfP)
			{
				ZeroMemory(pch, nLengthInBytes);
				memcpy_s(pch, nLengthInBytes, bReadBuf+5, 6);
				f = static_cast<float>(m_dConversionFactor_hp2kW * atof(pch));
				// 四舍五入,并保留2位小数
				*pfP = int(f * 100.0f + 0.5f) / 100.0f;
			}

			// 扭矩(N)
			if (NULL != pfF)
			{
				ZeroMemory(pch, nLengthInBytes);
				memcpy_s(pch, nLengthInBytes, bReadBuf+19, 7);
				f = static_cast<float>(m_dConversionFactor_lbf2N * atof(pch));
				// 四舍五入,并保留2位小数
				*pfF = int(f * 100.0f + 0.5f) / 100.0f;
			}

			// 速度(km/h)
			if (NULL != pfV)
			{
				ZeroMemory(pch, nLengthInBytes);
				memcpy_s(pch, nLengthInBytes, bReadBuf+35, 7);
				f = static_cast<float>(m_dConversionFactor_mph2kmph * atof(pch));
				// 四舍五入,并保留2位小数
				*pfV = int(f * 100.0f + 0.5f) / 100.0f;
			}

			delete[] pch;
			pch = NULL;
		}
		else
		{
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
	}
	else
	{
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

DWORD CMahaASMP::GetStatus(std::bitset<16> *const pbs16Status)
{
	const DWORD dwWriteLength = 4;
	const DWORD dwReadLength = 21;
	BYTE bWriteBuf[dwWriteLength] = {0xB4, 0x11, 'D', 0x05};
	BYTE bReadBuf[dwReadLength];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));
	DWORD dw = WriteAndReadData(bWriteBuf, dwWriteLength, bReadBuf, dwReadLength, false, true);
	if (DYN_WR_SUCCEED == dw)
	{
		if (NULL != pbs16Status)
		{
			for (int i=0; i<16; i++)
			{
				if ('1' == bReadBuf[i+1])
				{
					(*pbs16Status).set(i);
				}
				else
				{
					(*pbs16Status).reset(i);
				}
			}
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

DWORD CMahaASMP::ReadVar(const BYTE bVarNumber, WORD *const pwVarValue)
{
	assert(NULL != pwVarValue);

	char chVarNumber[4];
	ZeroMemory(chVarNumber, sizeof(chVarNumber));
	_snprintf_s(chVarNumber, _countof(chVarNumber), _TRUNCATE, "%03hhu", bVarNumber);
	const DWORD dwWriteLength = 7;
	const DWORD dwReadBufferLength = 50;
	BYTE bWriteBuf[dwWriteLength] = {0xB4, 0x11, 'E', chVarNumber[0], chVarNumber[1], chVarNumber[2], 0x05};
	BYTE bReadBuf[dwReadBufferLength];
	ZeroMemory(bReadBuf, sizeof(bReadBuf));

	DWORD dwReadLength(0);
	DWORD dw = WriteAndReadData(bWriteBuf, dwWriteLength, bReadBuf, dwReadBufferLength, &dwReadLength, false, true);
	if (DYN_WR_SUCCEED == dw)
	{
		// 以下读数逻辑从V3.4版移植过来

		const char *pchBegin = static_cast<const char*>(memchr(bReadBuf, '=', sizeof(bReadBuf))) + 1;
		const char *pchEnd = static_cast<const char*>(memchr(bReadBuf, 0x0D, sizeof(bReadBuf)));
		const char *pchOrigin = reinterpret_cast<const char *>(&(bReadBuf[0]));
		if ((pchEnd-pchOrigin) > 5)
		{
			const int nLengthInBytes(100);
			// 数值和单位
			const char *pchValueAndUnit = new char[nLengthInBytes]();
			// 数值
			char *pchValue = new char[nLengthInBytes]();
			// 单位
			char *pchUnit = new char[nLengthInBytes]();
			ZeroMemory(const_cast<char*>(pchValueAndUnit), nLengthInBytes);
			ZeroMemory(pchValue, nLengthInBytes);
			ZeroMemory(pchUnit, nLengthInBytes);

			memcpy_s(const_cast<char*>(pchValueAndUnit), nLengthInBytes, pchBegin, pchEnd-pchBegin);
			if (bVarNumber >= 200)
			{
				// 大于等于200的变量需要分离数值和单位
				// 并且根据m_MahaDynRamInfo小数位数化成整数

				// 数值和单位的间隔位置，数值和单位用空格间隔
				const char *pchSeparation = strrchr(pchValueAndUnit, ' ');
				strncpy_s(pchValue, nLengthInBytes, pchValueAndUnit, pchSeparation-pchValueAndUnit);
				pchSeparation++;
				strcpy_s(pchUnit, nLengthInBytes, pchSeparation);

				const float fVarValue = static_cast<float>(atof(pchValue));
				WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);
				*pwVarValue = wVarValue;
				//TRACE("Number:%s, ValueAndUnit:%s, Value:%s, Unit:%s\n", chVarNumber, pchValueAndUnit, pchValue, pchUnit);
				//TRACE("translate\n");
				//TRACE("Value:%.2f\n", fVarValue);
				//TRACE("Value:%hu\n\n", wVarValue);
				//TRACE("\n\n");
			}
			else
			{
				strcpy_s(pchValue, nLengthInBytes, pchValueAndUnit);

				const float fVarValue = static_cast<float>(atof(pchValue));
				WORD wVarValue(0);
				if (fVarValue >= 0.0f)
				{
					wVarValue = static_cast<WORD>(fVarValue + 0.5f);
				}
				else
				{
					wVarValue = static_cast<WORD>(fVarValue - 0.5f);
				}
				*pwVarValue = wVarValue;
				//TRACE("Number:%s, ValueAndUnit:%s, Value:%s, Unit:%s\n", chVarNumber, pchValueAndUnit, pchValue, pchUnit);
				//TRACE("translate\n");
				//TRACE("Value:%.2f\n", fVarValue);
				//TRACE("Value:%hu\n\n", wVarValue);
				//TRACE("\n\n");
			} // if (bVarNumber >= 200)

			if (NULL != pchValueAndUnit)
			{
				delete[] pchValueAndUnit;
				pchValueAndUnit = NULL;
			}
			if (NULL != pchValue)
			{
				delete[] pchValue;
				pchValue = NULL;
			}
			if (NULL != pchUnit)
			{
				delete[] pchUnit;
				pchUnit = NULL;
			}
		}
	}
	else
	{
		*pwVarValue = 0x0000;
	}  // if (DYN_WR_SUCCEED == dw)

	return dw;
}

DWORD CMahaASMP::WriteVar(const BYTE bVarNumber, const WORD wVarValue)
{
	// 以下写变量逻辑从V3.4版移植过来

	char chVarNumber[4];
	ZeroMemory(chVarNumber, sizeof(chVarNumber));
	_snprintf_s(chVarNumber, _countof(chVarNumber), _TRUNCATE, "%03hhu", bVarNumber);

	const int nLengthInBytes(100);
	char *pchValueAndUnit = new char[nLengthInBytes]();
	ZeroMemory(pchValueAndUnit, nLengthInBytes);
	if (bVarNumber >= 200)
	{
		// 获取变量整数位数、小数位数、单位
		const int nIntegral = m_MahaDynRamInfo[bVarNumber-200].nIntegral;
		const int nDecimal = m_MahaDynRamInfo[bVarNumber-200].nDecimal;
		const char chUnit[10] = {'\0'};
		ZeroMemory(const_cast<char*>(chUnit), sizeof(chUnit));
		strncpy_s(const_cast<char*>(chUnit), _countof(chUnit), m_MahaDynRamInfo[bVarNumber-200].chUnit, _TRUNCATE);

		char *pchFormat = new char[nLengthInBytes]();
		ZeroMemory(pchFormat, nLengthInBytes);

		// 单位是%，要做特殊处理
		if (0 == strcmp(chUnit, "%"))
		{
			_snprintf_s(pchFormat, nLengthInBytes, _TRUNCATE, "%%%d.%df %%%s", nIntegral, nDecimal, chUnit);
		}
		else
		{
			_snprintf_s(pchFormat, nLengthInBytes, _TRUNCATE, "%%%d.%df %s", nIntegral, nDecimal, chUnit);
		}
		// TODO:用ConvertVarValueFromWordToFloat代替以下
		float fValue = static_cast<float>(wVarValue) / pow(10.0f, nDecimal);
		if (0 == strcmp(chUnit, "hp"))
		{
			// hp->kW
			fValue = static_cast<float>(fValue * m_dConversionFactor_hp2kW);
			// 四舍五入,并保留2位小数
			fValue = int(fValue * 100.0f + 0.5f) / 100.0f;
		}
		_snprintf_s(pchValueAndUnit, nLengthInBytes, _TRUNCATE, pchFormat, fValue);

		if (NULL != pchFormat)
		{
			delete[] pchFormat;
			pchFormat = NULL;
		}
	}
	else
	{
		_snprintf_s(pchValueAndUnit, nLengthInBytes, _TRUNCATE, "%hu", wVarValue);
	}

	const int nLengthOfValueAndUnit = static_cast<int>(strlen(pchValueAndUnit));

	DWORD dwWriteLength(0);
	const DWORD dwReadLength(1);
	BYTE bWriteBuf[nLengthInBytes];
	ZeroMemory(bWriteBuf, sizeof(bWriteBuf));
	BYTE bReadBuf[1] = {0};

	dwWriteLength = 0;
	bWriteBuf[dwWriteLength++] = 0xB4;
	bWriteBuf[dwWriteLength++] = 0x11;
	bWriteBuf[dwWriteLength++] = 'F';
	bWriteBuf[dwWriteLength++] = chVarNumber[0];
	bWriteBuf[dwWriteLength++] = chVarNumber[1];
	bWriteBuf[dwWriteLength++] = chVarNumber[2];
	bWriteBuf[dwWriteLength++] = '=';
	for (int i=0; i<nLengthOfValueAndUnit; i++)
	{			
		bWriteBuf[dwWriteLength++] = pchValueAndUnit[i];
	}
	bWriteBuf[dwWriteLength++] = 0x0D;
	bWriteBuf[dwWriteLength++] = 0x17;
	const BYTE bSum = CheckSum(bWriteBuf+1, dwWriteLength-1);
	char chSum[3];
	ZeroMemory(chSum, sizeof(chSum));
	_snprintf_s(chSum, _countof(chSum), _TRUNCATE, "%02X", bSum);
	bWriteBuf[dwWriteLength++] = chSum[0];
	bWriteBuf[dwWriteLength++] = chSum[1];
	bWriteBuf[dwWriteLength++] = '$';

	if (NULL != pchValueAndUnit)
	{
		delete[] pchValueAndUnit;
		pchValueAndUnit = NULL;
	}

	DWORD dw = WriteAndReadData(bWriteBuf, dwWriteLength, bReadBuf, dwReadLength, false, false);
	return dw;
}

DWORD CMahaASMP::SendCtrlWord(const BYTE bCtrlWord)
{
	const DWORD dwWriteLength = 3;
	const DWORD dwReadLength = 1;
	BYTE bWriteBuf[dwWriteLength] = {0xB4, 0x11, bCtrlWord};
	BYTE bReadBuf[dwReadLength] = {0};
	DWORD dw = WriteAndReadData(bWriteBuf, dwWriteLength, bReadBuf, dwReadLength, false, false);
	return dw;
}

DWORD CMahaASMP::LiftBeamUp(void)
{
	return SendCtrlWord('5');
}

DWORD CMahaASMP::LiftBeamDown(void)
{
	return SendCtrlWord('6');
}

DWORD CMahaASMP::MotorOn(void)
{
	return SendCtrlWord('J');
}

DWORD CMahaASMP::MotorOff(void)
{
	return SendCtrlWord('K');
}

DWORD CMahaASMP::ControlModeOff(void)
{
	return SendCtrlWord('0');
}

DWORD CMahaASMP::AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses)
{
	assert(fTargetPower >= 0.0f);
	assert(fAsmSpeed >= 0.0f);
	assert(fTireLosses >= 0.0f);

	// kW->hp
	float fVar228Value = static_cast<float>(fTargetPower / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVar228Value = static_cast<int>(fVar228Value * 100.0f + 0.5f) / 100.0f;
	const WORD wVar228Value = ConvertVarValueFromFloatToWord(228, fVar228Value);

	// km/h->mph
	float fVar230Value = static_cast<float>(fAsmSpeed / m_dConversionFactor_mph2kmph);
	// 四舍五入,并保留2位小数
	fVar230Value = static_cast<int>(fVar230Value * 100.0f + 0.5f) / 100.0f;
	const WORD wVar230Value = ConvertVarValueFromFloatToWord(230, fVar230Value);

	// kW->hp
	float fVar229Value = static_cast<float>(fTireLosses / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVar229Value = static_cast<int>(fVar229Value * 100.0f + 0.5f) / 100.0f;
	const WORD wVar229Value = ConvertVarValueFromFloatToWord(229, fVar229Value);

	WriteVar(228, wVar228Value);
	WriteVar(230, wVar230Value);
	WriteVar(229, wVar229Value);
	return SendCtrlWord('4');
}

DWORD CMahaASMP::SetConstantPowerTarget(const float fPTarget)
{
	assert(fPTarget >= 0.0f);

	const BYTE bVarNumber(228);

	// kW->hp
	float fVarValue = static_cast<float>(fPTarget / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::ConstantPowerControlModeOn(void)
{
	return SendCtrlWord('4');
}

DWORD CMahaASMP::SetConstantForceTarget(const float fFTarget)
{
	assert(fFTarget >= 0.0f);

	const BYTE bVarNumber(201);

	// N->lbf
	float fVarValue = static_cast<float>(fFTarget / m_dConversionFactor_lbf2N);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::ConstantForceControlModeOn(void)
{
	return SendCtrlWord('2');
}

DWORD CMahaASMP::VarLoadControlModeOn(void)
{
	return SendCtrlWord('Q');
}

DWORD CMahaASMP::SetConstantVelocityTarget(const float fVTarget)
{
	assert(fVTarget >= 0.0f);

	const BYTE bVarNumber(200);

	// km/h->mph
	float fVarValue = static_cast<float>(fVTarget / m_dConversionFactor_mph2kmph);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::ConstantVelocityControlModeOn(void)
{
	return SendCtrlWord('1');
}

DWORD CMahaASMP::SetUpperVelocityOfTimer1(const float fUpperVelocity)
{
	assert(fUpperVelocity >= 0.0f);

	const BYTE bVarNumber(207);

	// km/h->mph
	float fVarValue = static_cast<float>(fUpperVelocity / m_dConversionFactor_mph2kmph);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::SetMiddleVelocityOfTimer1(const float fMiddleVelocity)
{
	assert(fMiddleVelocity >= 0.0f);

	const BYTE bVarNumber(232);

	// km/h->mph
	float fVarValue = static_cast<float>(fMiddleVelocity / m_dConversionFactor_mph2kmph);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::SetLowerVelocityOfTimer1(const float fLowerVelocity)
{
	assert(fLowerVelocity >= 0.0f);

	const BYTE bVarNumber(208);

	// km/h->mph
	float fVarValue = static_cast<float>(fLowerVelocity / m_dConversionFactor_mph2kmph);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::SetTimer1(void)
{
	return SendCtrlWord('H');
}

DWORD CMahaASMP::KillTimer1(void)
{
	return SendCtrlWord('I');
}

DWORD CMahaASMP::GetTimeOfTimer1(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);

	const BYTE bVarNumber(227);

	WORD wVarValue(0);
	DWORD dw = ReadVar(bVarNumber, &wVarValue);
	if (DYN_WR_SUCCEED == dw)
	{
		float fVarValue = ConvertVarValueFromWordToFloat(bVarNumber, wVarValue);
		// s->ms
		*punTime_MS = static_cast<UINT>(fVarValue * 1000.0f + 0.5f);
	}
	else
	{
		*punTime_MS = 0;
	}
	return dw;
}

DWORD CMahaASMP::GetMiddleTimeOfTimer1(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);

	const BYTE bVarNumber(233);

	WORD wVarValue(0);
	DWORD dw = ReadVar(bVarNumber, &wVarValue);
	if (DYN_WR_SUCCEED == dw)
	{
		float fVarValue = ConvertVarValueFromWordToFloat(bVarNumber, wVarValue);
		// s->ms
		*punTime_MS = static_cast<UINT>(fVarValue * 1000.0f + 0.5f);
	}
	else
	{
		*punTime_MS = 0;
	}
	return dw;
}

DWORD CMahaASMP::SetUpperVelocityOfTimer2(const float fUpperVelocity)
{
	assert(fUpperVelocity >= 0.0f);

	const BYTE bVarNumber(234);

	// km/h->mph
	float fVarValue = static_cast<float>(fUpperVelocity / m_dConversionFactor_mph2kmph);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::SetLowerVelocityOfTimer2(const float fLowerVelocity)
{
	assert(fLowerVelocity >= 0.0f);

	const BYTE bVarNumber(235);

	// km/h->mph
	float fVarValue = static_cast<float>(fLowerVelocity / m_dConversionFactor_mph2kmph);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::SetTimer2(void)
{
	return SendCtrlWord('R');
}

DWORD CMahaASMP::KillTimer2(void)
{
	return SendCtrlWord('S');
}

DWORD CMahaASMP::GetTimeOfTimer2(UINT *const punTime_MS)
{
	assert(NULL != punTime_MS);

	const BYTE bVarNumber(236);

	WORD wVarValue(0);
	DWORD dw = ReadVar(bVarNumber, &wVarValue);
	if (DYN_WR_SUCCEED == dw)
	{
		float fVarValue = ConvertVarValueFromWordToFloat(bVarNumber, wVarValue);
		// s->ms
		*punTime_MS = static_cast<UINT>(fVarValue * 1000.0f + 0.5f);
	}
	else
	{
		*punTime_MS = 0;
	}
	return dw;
}

DWORD CMahaASMP::GetDIW(USHORT *const pusDIW)
{
	assert(NULL != pusDIW);

	return ReadVar(6, pusDIW);
}

DWORD CMahaASMP::SetDIW(const USHORT usDIW)
{
	// 打开写保护
	WriteVar(0, 9864);
	DWORD dw = WriteVar(6, usDIW);
	// 关闭写保护
	WriteVar(0, 0);

	return dw;
}

DWORD CMahaASMP::WriteVehicleRM(const USHORT usVehicleRM)
{
	const BYTE bVarNumber(202);

	// kg->lb
	float fVarValue = static_cast<float>(usVehicleRM / m_dConversionFactor_lb2kg);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::WriteCv(const float fCv)
{
	const BYTE bVarNumber(203);

	// 转化为80km/h对应的Cv值(*4.096)，并且kW->hp
	float fVarValue = static_cast<float>(fCv * 4.096 / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::WriteBv(const float fBv)
{
	const BYTE bVarNumber(204);

	// kW->hp
	float fVarValue = static_cast<float>(fBv / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::WriteAv(const float fAv)
{
	const BYTE bVarNumber(205);

	// kW->hp
	float fVarValue = static_cast<float>(fAv / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::WriteAt(const float fAt)
{
	const BYTE bVarNumber(212);

	// kW->hp
	float fVarValue = static_cast<float>(fAt / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::WriteBt(const float fBt)
{
	const BYTE bVarNumber(213);

	// kW->hp
	float fVarValue = static_cast<float>(fBt / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::WriteCt(const float fCt)
{
	const BYTE bVarNumber(214);

	// kW->hp
	float fVarValue = static_cast<float>(fCt / m_dConversionFactor_hp2kW);
	// 四舍五入,并保留2位小数
	fVarValue = static_cast<int>(fVarValue * 100.0f + 0.5f) / 100.0f;
	const WORD wVarValue = ConvertVarValueFromFloatToWord(bVarNumber, fVarValue);

	return WriteVar(bVarNumber, wVarValue);
}

DWORD CMahaASMP::DriveResistanceSimulationControlModeOn(void)
{
	return SendCtrlWord('3');
}

DWORD CMahaASMP::ResetDistance(void)
{
	return SendCtrlWord('9');
}

DWORD CMahaASMP::ReadDistance(USHORT *const pusDistance)
{
	assert(NULL != pusDistance);

	const BYTE bVarNumber(225);

	WORD wVarValue(0);
	DWORD dw = ReadVar(bVarNumber, &wVarValue);
	if (DYN_WR_SUCCEED == dw)
	{
		float fVarValue = ConvertVarValueFromWordToFloat(bVarNumber, wVarValue);
		// mi->km
		fVarValue = static_cast<float>(fVarValue * m_dConversionFactor_mi2km);
		// km->m
		*pusDistance = static_cast<USHORT>(fVarValue * 1000.0f + 0.5f);
	}
	else
	{
		*pusDistance = 0;
	}
	return dw;
}

DWORD CMahaASMP::ReadApWithRemanence(float *const pfAp)
{
	// 马哈测功机无此功能，用ReadApWithoutRemanence兼容
	return ReadApWithoutRemanence(pfAp);
}

DWORD CMahaASMP::ReadBpWithRemanence(float *const pfBp)
{
	// 马哈测功机无此功能，用ReadBpWithoutRemanence兼容
	return ReadBpWithoutRemanence(pfBp);
}

DWORD CMahaASMP::ReadCpWithRemanence(float *const pfCp)
{
	// 马哈测功机无此功能，用ReadCpWithoutRemanence兼容
	return ReadCpWithoutRemanence(pfCp);
}

DWORD CMahaASMP::WriteApWithRemanence(const float fAp)
{
	return DYN_WR_EQUIP_RETURE_0X15;
}

DWORD CMahaASMP::WriteBpWithRemanence(const float fBp)
{
	return DYN_WR_EQUIP_RETURE_0X15;
}

DWORD CMahaASMP::WriteCpWithRemanence(const float fCp)
{
	return DYN_WR_EQUIP_RETURE_0X15;
}

DWORD CMahaASMP::ReadApWithoutRemanence(float *const pfAp)
{
	assert(NULL != pfAp);

	WORD wAp(0);
	DWORD dw = ReadVar(51, &wAp);
	// 缩小100倍
	*pfAp = wAp / 100.0f;

	return dw;
}

DWORD CMahaASMP::ReadBpWithoutRemanence(float *const pfBp)
{
	assert(NULL != pfBp);

	WORD wBp(0);
	DWORD dw = ReadVar(52, &wBp);
	// 缩小100倍
	*pfBp = wBp / 100.0f;

	return dw;
}

DWORD CMahaASMP::ReadCpWithoutRemanence(float *const pfCp)
{
	assert(NULL != pfCp);

	WORD wCp(0);
	DWORD dw = ReadVar(53, &wCp);
	// 缩小100倍
	*pfCp = wCp / 100.0f;

	return dw;
}

DWORD CMahaASMP::WriteApWithoutRemanence(const float fAp)
{
	// 放大100倍，并四舍五入
	const WORD wAp = WORD(fAp*100.0f + 0.5f);

	// 打开写保护
	WriteVar(0, 9864);
	const DWORD dw = WriteVar(51, wAp);

	return dw;
}

DWORD CMahaASMP::WriteBpWithoutRemanence(const float fBp)
{
	// 放大100倍，并四舍五入
	const WORD wBp = WORD(fBp*100.0f + 0.5f);

	return WriteVar(52, wBp);
}

DWORD CMahaASMP::WriteCpWithoutRemanence(const float fCp)
{
	// 放大100倍，并四舍五入
	const WORD wCp = WORD(fCp*100.0f + 0.5f);

	const DWORD dw = WriteVar(53, wCp);
	// 关闭写保护
	WriteVar(0, 0);

	return dw;
}

WORD CMahaASMP::ConvertVarValueFromFloatToWord(const BYTE bVarNumber, const float fVarValue)
{
	assert(bVarNumber >= 200);

	// 获取变量整数位数、小数位数、单位
	const int nIntegral = m_MahaDynRamInfo[bVarNumber-200].nIntegral;
	const int nDecimal = m_MahaDynRamInfo[bVarNumber-200].nDecimal;
	const char chUnit[10] = {'\0'};
	ZeroMemory(const_cast<char*>(chUnit), sizeof(chUnit));
	strncpy_s(const_cast<char*>(chUnit), _countof(chUnit), m_MahaDynRamInfo[bVarNumber-200].chUnit, _TRUNCATE);

	WORD wVarValue(0);
	if (fVarValue >= 0.0f)
	{
		wVarValue = static_cast<WORD>(fVarValue * pow(10.0f, nDecimal) + 0.5f);
	}
	else
	{
		wVarValue = static_cast<WORD>(fVarValue * pow(10.0f, nDecimal) - 0.5f);
	}

	return wVarValue;
}

float CMahaASMP::ConvertVarValueFromWordToFloat(const BYTE bVarNumber, const WORD wVarValue)
{
	assert(bVarNumber >= 200);

	// 获取变量整数位数、小数位数、单位
	const int nIntegral = m_MahaDynRamInfo[bVarNumber-200].nIntegral;
	const int nDecimal = m_MahaDynRamInfo[bVarNumber-200].nDecimal;
	const char chUnit[10] = {'\0'};
	ZeroMemory(const_cast<char*>(chUnit), sizeof(chUnit));
	strncpy_s(const_cast<char*>(chUnit), _countof(chUnit), m_MahaDynRamInfo[bVarNumber-200].chUnit, _TRUNCATE);

	// TODO:待研究问题：是否需要处理正负问题
	float fVarValue = static_cast<float>(wVarValue) / pow(10.0f, nDecimal);

	return fVarValue;
}

DWORD CMahaASMP::SetMotorMaxVelocity(const float fMaxVelocity)
{
	assert(fMaxVelocity>0.0f && fMaxVelocity<130.0f);

	// 马哈测功机最大频率是7500对应是130km/h ??待确认
	// 打开写保护
	WriteVar(0, 9864);
	const WORD wV = WORD(fMaxVelocity);
	DWORD dw = WriteVar(66, wV);
	// 关闭写保护
	WriteVar(0, 0);
	return dw;
}

// 获取测功机电机最大速度
// (out)fMaxVelocity: 最大速度，单位-km/h
DWORD CMahaASMP::GetMotorMaxVelocity(float *const fMaxVelocity)
{
	WORD Frequency(0);
	DWORD dw = ReadVar(66, &Frequency);
	*fMaxVelocity = Frequency;
	return dw;
}

DWORD CMahaASMP::SetConstantAccStep(const float fStep)
{
	return 0x00;
}

DWORD CMahaASMP::ConstantAccControlModeOn(void)
{
	return 0x00;
}

DWORD CMahaASMP::ResponseTimeControlModeStart(void)
{
	return DYN_NOT_SUPPORT;
}

DWORD CMahaASMP::ResponseTimeControlModeEnd(void)
{
	return DYN_NOT_SUPPORT;
}

DWORD CMahaASMP::GetResponseTimeData(float *const pfV, float *const pfF, const int nLength)
{
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
	return DYN_NOT_SUPPORT;
}