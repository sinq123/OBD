#include "StdAfx.h"
#include "NHSZKS.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>


CNHSZKS::CNHSZKS(void)
	: m_hComm(INVALID_HANDLE_VALUE)
	, m_bIsConnect_Serial(false)
{
}


CNHSZKS::~CNHSZKS(void)
{
	DisconnectServer();
}

DWORD CNHSZKS::ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort)
{
	return 0x15;
}

DWORD CNHSZKS::ConnectServer(const BYTE bPort, const int nBaudRate /*= CBR_19200*/)
{
	assert(0 != bPort);

	// 关闭已存在的串口句柄
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
		// 打开串口失败
		//OutputDebugStringW(L"Error:fail to open com port\n");
		m_bIsConnect_Serial = false;
		return 0x01;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数begin
	// 格式：波特率=9600 BIT/S，1起始位，1停止位，8位数据："9600,N,8,1"
	// 设置输入和输出缓存区大小
	SetupComm(m_hComm, 3072, 1024);
	// 设置DCB结构
	DCB dcb;
	// 获取缺省的DCB结构的值
	GetCommState(m_hComm, &dcb);
	// 设定波特率，从外部读取
	dcb.BaudRate = nBaudRate;
	// 设定无奇偶校验
	dcb.Parity = NOPARITY;
	// 设定数据位为8
	dcb.ByteSize = 8;
	// 设定一个停止位
	dcb.StopBits = ONESTOPBIT;
	// 设置串行设备控制参数
	SetCommState(m_hComm, &dcb);
	// 进行超时设置
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 50;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 100; // 读过程数据可能需要100ms或更长时间
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end
	m_bIsConnect_Serial = true;

	return 0x00;
}

DWORD CNHSZKS::DisconnectServer(void)
{
	DWORD dwReturn(0x00);

	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (b)
		{
			// 关闭串口成功
			dwReturn = 0x00;
		}
		else
		{
			// 关闭串口失败：执行win32函数失败
			dwReturn = 0x01;
		}
	}

	m_bIsConnect_Serial = false;

	return dwReturn;
}

bool CNHSZKS::IsConnect(void) const
{
	return m_bIsConnect_Serial;
}

DWORD CNHSZKS::SetList(void)
{
	return 0x15;
}

DWORD CNHSZKS::DelList(void)
{
	return 0x15;
}

DWORD CNHSZKS::Display(const wchar_t *const pwchDisplayTxt,
		const bool bTwinking/*=false*/,
		const char chLineSpace/*='0'*/,
		const char chInDisplayMode/*=0x30*/,
		const char chOutDisplayMode/*=0x30*/,
		const char chFramePauseTime/*='3'*/,
		const char chDisplaySpeed/*='0'*/,
		const char chFontAndSize/*='5'*/,
		const char chSetupType/*='b'*/,
		const char chhAlign/*='0'*/,
		const char chvAlign/*='1'*/
		)
{
	// 如果有换行就分成两行
	std::wstring strDisTxt(pwchDisplayTxt);
	std::vector<std::wstring> vStr;
	SplitLines(strDisTxt, vStr);

	// 至少2行，至多4行
	if (vStr.size() == 0)
	{
		// 补第一、二行
		vStr.push_back(L"");
		vStr.push_back(L"");
	}
	else if (vStr.size() == 1)
	{
		// 补第二行
		vStr.push_back(L"");
	}

	// 初始化发送字符串
	const int nSendBufLength(128);
	char chSendBuf[nSendBufLength] = {'\0'};
	int nBufOffset = LineToBuf(1, 1, vStr[0], chSendBuf); // 第一行写入buf
	nBufOffset = LineToBuf(1, 2, vStr[1], chSendBuf, nBufOffset); // 第二行写入buf
	if (vStr.size() >= 3)
	{
		nBufOffset = LineToBuf(1, 3, vStr[2], chSendBuf, nBufOffset); // 第三行写入buf
	}
	if (vStr.size() >= 4)
	{
		nBufOffset = LineToBuf(1, 4, vStr[3], chSendBuf, nBufOffset); // 第四行写入buf
	}
	return SendToServer(chSendBuf, nBufOffset);
}

DWORD CNHSZKS::DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt)
{
	// 初始化发送字符串
	const int nSendBufLength(64);
	char chSendBuf[nSendBufLength] = {'\0'};
	int nBufOffset = LineToBuf(nPanel, nLine, pwchLineTxt, chSendBuf);
	return SendToServer(chSendBuf, nBufOffset);
}

DWORD CNHSZKS::SendToServer(const char *const pchBuf, const int nLen)
{
	assert(NULL != pchBuf);
	assert(0 != nLen);

	DWORD dwNumberOfBytesToWrite(nLen), dwBytesWritten(0x00), dwReturn(0x00);

	if (!m_bIsConnect_Serial)
	{
		return 0x02;
	}

	// 清除缓冲数据
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	// 写Comm
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
	return dwReturn;
}

int CNHSZKS::LineToBuf(const int nPanelIdx, const int nLineIdx, const std::wstring& strLineTxt, char chSendBuf[], const int nIdx /*= 0*/)
{
	// 命令开始符：00H
	chSendBuf[nIdx] = 0x00;
	// 屏号>30H
	// 30H：所有屏
	// 31H：1号屏
	// 32H：2号屏
	// 33H：3号屏
	chSendBuf[nIdx+1] = (char)(0x30 + nPanelIdx);
	// 31H：第一行，在第一行显示16点阵字符
	// 32H：第二行，在第二行显示16点阵字符
	// 33H：第三行，在第一行显示16点阵字符
	// 34H：第四行，在第一行显示16点阵字符
	chSendBuf[nIdx+2] = (char)(0x30 + nLineIdx);
	// 30H：左移方式
	// 31H：靠左显示
	// 32H：居中显示
	// 33H：靠右显示
	// 34H：向上翻页，向上拉幕
	// 35H：向下翻页，向下拉幕
	// 36H：从中间展开
	// 37H：向中间合并
	chSendBuf[nIdx+3] = 0x32; // 居中

	if (strLineTxt.empty())
	{
		// 空文本
		chSendBuf[nIdx+4] = 0x0D;
		return nIdx+5; // 指向下一位
	}
	else
	{
		// 显示文本：UNICODE字符->ANSI字符
		const DWORD dwNum = ::WideCharToMultiByte(CP_OEMCP, NULL, strLineTxt.c_str(), -1, NULL, 0, NULL, FALSE); // dwNum包含结束符'\0'的一位长度
		char *pchDisTxt = new char[dwNum];
		::ZeroMemory(pchDisTxt, dwNum*sizeof(char));
		::WideCharToMultiByte(CP_OEMCP, NULL, strLineTxt.c_str(), -1, pchDisTxt, dwNum, NULL, FALSE);

		// 不复制最后的结束符
		::memcpy_s(&chSendBuf[nIdx+4], dwNum-1, pchDisTxt, dwNum-1);
		// 0DH：结束
		chSendBuf[nIdx+4+dwNum-1] = 0x0D;

		delete[] pchDisTxt;
		pchDisTxt = NULL;
		return nIdx+4+dwNum; // 指向下一位
	}
}

void CNHSZKS::SplitLines(const std::wstring& str, std::vector<std::wstring>& vResult, const bool bKeepEnds/* = false*/)
{
	vResult.clear();
	std::wstring::size_type nLen = str.size(), nIdx(0), nBegOfLine(0), nEndOfLine(0);

	for ( ; nIdx < nLen; nBegOfLine = nIdx) 
	{
		while (nIdx < nLen && str[nIdx] != L'\n' && str[nIdx] != L'\r') nIdx++;

		nEndOfLine = nIdx;
		if (nIdx < nLen)
		{
			if (str[nIdx] == L'\r' && (nIdx+1<nLen) && str[nIdx+1] == L'\n') 
			{
				// 换行符：\r\n
				nIdx += 2;
			}
			else
			{
				// 换行符：\r或\n
				nIdx++;
			}
			if (bKeepEnds)
			{
				// 此时nEndOfLine指向\r\n后一个字符
				nEndOfLine = nIdx;
			}
		}
		vResult.push_back(str.substr(nBegOfLine, nEndOfLine - nBegOfLine));
	}

	if (nBegOfLine < nLen)
	{
		vResult.push_back(str.substr(nBegOfLine, nLen - nBegOfLine));
	}
}