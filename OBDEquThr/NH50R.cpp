#include "StdAfx.h"
#include "NH50R.h"
#include <cassert>
#include <cstdio>
#include <stdlib.h>

CNH50R::CNH50R(void)
: m_socket(INVALID_SOCKET)
, m_hComm(INVALID_HANDLE_VALUE)
, m_nSendBufLength(1024)
, m_pchSendBuf(NULL)
, m_bIsConnect_Serial(false)
, m_bIsConnect_Ethernet(false)
{
	// 在堆中分配内存
	m_pchSendBuf = new char[m_nSendBufLength]();
	if (NULL != m_pchSendBuf)
	{
		memset(m_pchSendBuf, '\0', m_nSendBufLength);
	}
}

CNH50R::~CNH50R(void)
{
	// 销毁对象时,断开连接
	DisconnectServer();

	// 在堆中回收内存
	if (NULL != m_pchSendBuf)
	{
		delete[] m_pchSendBuf;
		m_pchSendBuf = NULL;
	}
}

DWORD CNH50R::ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort)
{
	assert(NULL != pwchIP);

	// 如果已经有连接,断开连接
	DisconnectServer();

	// 初始化Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
	{
		return 0x01;
	}

	// 确定WinSock DLL支持V2.2版本
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2 )
	{
		    WSACleanup( );
			return 0x01; 
	}


	// 创建socket.
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_socket) 
	{
		WSACleanup();
		return 0x01;
	}

	// IP地址：UNICODE字符->ANSI字符
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pwchIP, -1, NULL, 0, NULL, FALSE);
	char *pchIP = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, pwchIP, -1, pchIP, dwNum, NULL, FALSE);

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(pchIP);
	clientService.sin_port = htons(usPort);

	delete []pchIP;

	// 连接服务器
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		m_bIsConnect_Ethernet = false;
		WSACleanup();
		return 0x01;
	}
	else
	{
		m_bIsConnect_Ethernet = true;
	}

	return 0x00;
}

DWORD CNH50R::ConnectServer(const BYTE bPort, const int nBaudRate)
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
	// 进行超时设置（100毫秒之内要执行完读写操作）
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 50;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;// 读过程数据可能需要1s或更长时间
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////////////////////////////////////////////////////// 初始化串口参数end
	m_bIsConnect_Serial = true;

	return 0x00;
}

DWORD CNH50R::DisconnectServer(void)
{
	DWORD dwReturn(0x00);

	if (INVALID_SOCKET != m_socket)
	{
		closesocket(m_socket); 
		m_socket = INVALID_SOCKET;
		dwReturn = 0x00;
		WSACleanup();
	}

	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		BOOL b = CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		if (!b)
		{
			// 关闭串口失败：执行win32函数失败
			dwReturn = 0x01;
		}
	}

	m_bIsConnect_Serial = false;
	m_bIsConnect_Ethernet = false;

	return dwReturn;
}

DWORD CNH50R::SetList(void)
{
	// 发送长度,15字节
	const int nSendLength(15);

	// 设定播放列表为:E:\T\NH
	m_pchSendBuf[0] = 0x01;
	m_pchSendBuf[1] = 'Z';
	m_pchSendBuf[2] = '0';
	m_pchSendBuf[3] = '0';
	m_pchSendBuf[4] = 0x02;
	m_pchSendBuf[5] = 'E';
	m_pchSendBuf[6] = '.';
	m_pchSendBuf[7] = 'S';
	m_pchSendBuf[8] = 'L';
	m_pchSendBuf[9] = 0x0F;
	m_pchSendBuf[10] = 'E';
	m_pchSendBuf[11] = 'T';
	m_pchSendBuf[12] = 'N';
	m_pchSendBuf[13] = 'H';
	m_pchSendBuf[14] = 0x04;

	//DWORD dwReturn(0x00);
	//if (send(m_socket, m_pchSendBuf, nSendLength, 0) <= 0)
	//{
	//	dwReturn = 0x01;
	//}
	//else
	//{
	//	dwReturn = 0x00;
	//}

	//return dwReturn;

	return SendToServer(m_pchSendBuf, nSendLength);
}

bool CNH50R::IsConnect(void) const
{
	return m_bIsConnect_Serial||m_bIsConnect_Ethernet;
}

DWORD CNH50R::DelList(void)
{
	// 发送长度,8字节
	const int nSendLength(8);

	m_pchSendBuf[0] = 0x01;
	m_pchSendBuf[1] = 'Z';
	m_pchSendBuf[2] = '0';
	m_pchSendBuf[3] = '0';
	m_pchSendBuf[4] = 0x02;
	m_pchSendBuf[5] = 'E';
	m_pchSendBuf[6] = '.';
	m_pchSendBuf[7] = 0x04;

	//DWORD dwReturn(0x00);
	//if (send(m_socket, m_pchSendBuf, nSendLength, 0) <= 0)
	//{
	//	dwReturn = 0x01;
	//}
	//else
	//{
	//	dwReturn = 0x00;
	//}

	//return dwReturn;
	return SendToServer(m_pchSendBuf, nSendLength);
}

DWORD CNH50R::Display(const wchar_t *const pwchDisplayTxt, 
					  const bool bTwinking,
					  const char chLineSpace,
					  const char chInDisplayMode,
					  const char chOutDisplayMode,
					  const char chFramePauseTime,
					  const char chDisplaySpeed,
					  const char chFontAndSize,
					  const char chSetupType,
					  const char chhAlign,
					  const char chvAlign)
{
	///////////////////////////////////////////加快显示速度(输入参数不作检查)
	//assert(NULL != pwchDisplayTxt);
	//assert('0'==chFramePauseTime ||
	//	'1'==chFramePauseTime ||
	//	'2'==chFramePauseTime ||
	//	'3'==chFramePauseTime ||
	//	'4'==chFramePauseTime ||
	//	'5'==chFramePauseTime ||
	//	'6'==chFramePauseTime ||
	//	'7'==chFramePauseTime ||
	//	'8'==chFramePauseTime ||
	//	'9'==chFramePauseTime);
	///////////////////////////////////////////加快显示速度(输入参数不作检查)

	int nIndex(0);

	// 写文件到E:\T\NH
	m_pchSendBuf[nIndex++] = 0x01;
	m_pchSendBuf[nIndex++] = 'Z';
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = 0x02;
	m_pchSendBuf[nIndex++] = 'A';
	m_pchSendBuf[nIndex++] = 0x0F;
	m_pchSendBuf[nIndex++] = 'E';
	m_pchSendBuf[nIndex++] = 'T';
	m_pchSendBuf[nIndex++] = 'N';
	m_pchSendBuf[nIndex++] = 'H';

	// 协议控制符
	m_pchSendBuf[nIndex++] = 0x06;

	/////////////////////////////////////////////////////////////////////////////////////////////显示控制符begin
	if (bTwinking)
	{
		// 闪烁控制符
		m_pchSendBuf[nIndex++] = 0x07;
		m_pchSendBuf[nIndex++] = '1';
	}

	// 行间距控制
	m_pchSendBuf[nIndex++] = 0x08;
	//m_pchSendBuf[nIndex++] = '0';  // 0行间距
	m_pchSendBuf[nIndex++] = chLineSpace;  // 行间距

	// 花样控制符	
	m_pchSendBuf[nIndex++] = 0x0A;
	m_pchSendBuf[nIndex++] = 'I';
	m_pchSendBuf[nIndex++] = chInDisplayMode;  // 入花样
	m_pchSendBuf[nIndex++] = 0x0A;
	m_pchSendBuf[nIndex++] = 'O';
	m_pchSendBuf[nIndex++] = chOutDisplayMode;  // 出花样

	// 特殊字符

	// 帧停留时间
	m_pchSendBuf[nIndex++] = 0x0E;
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = '0';
	m_pchSendBuf[nIndex++] = chFramePauseTime;  // 停留秒数

	// 速度
	m_pchSendBuf[nIndex++] = 0x0F;
	//m_pchSendBuf[nIndex++] = '0';  // 最快
	m_pchSendBuf[nIndex++] = chDisplaySpeed;

	// 字体及大小
	m_pchSendBuf[nIndex++] = 0x1A;
	//m_pchSendBuf[nIndex++] = '5';  // 16*16 标准中文字体
	m_pchSendBuf[nIndex++] = chFontAndSize;
	
	// 处理模式
	m_pchSendBuf[nIndex++] = 0x1B;
	m_pchSendBuf[nIndex++] = '0';
	//m_pchSendBuf[nIndex++] = 'b';  // a:不自动排版,b:默认处理模式,自动换行,自动排版
	m_pchSendBuf[nIndex++] = chSetupType;

	//// 字体颜色
	//m_pchSendBuf[nIndex++] = 0x1C;
	//m_pchSendBuf[nIndex++] = 0x31;

	//// 字体背景颜色
	//m_pchSendBuf[nIndex++] = 0x1D;
	//m_pchSendBuf[nIndex++] = 0x30; // 黑色

	// 水平对齐
	m_pchSendBuf[nIndex++] = 0x1E;
	m_pchSendBuf[nIndex++] = chhAlign;  // 0:居中,1:居左,2:居右

	// 垂直对齐
	m_pchSendBuf[nIndex++] = 0x1F;
	m_pchSendBuf[nIndex++] = chvAlign;  // 0,居中,1:居上,2:居下
	/////////////////////////////////////////////////////////////////////////////////////////////显示控制符end

	if (NULL != pwchDisplayTxt)
	{
		// 显示文本：UNICODE字符->ANSI字符
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pwchDisplayTxt, -1, NULL, 0, NULL, FALSE);
		char *pchDisplayTxt = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, pwchDisplayTxt, -1, pchDisplayTxt, dwNum, NULL, FALSE);

		for (size_t st=0; st<strlen(pchDisplayTxt); st++)
		{
			if ('\n' == pchDisplayTxt[st])
			{
				// 换行
				m_pchSendBuf[nIndex++] = 0x0d;
			}
			else if ('\f' == pchDisplayTxt[st])
			{
				// 换帧
				m_pchSendBuf[nIndex++] = 0x0c;
			}
			else
			{
				m_pchSendBuf[nIndex++] = pchDisplayTxt[st];
			}

			if (nIndex >= (m_nSendBufLength-1))
			{
				// 到达缓冲区容量m_nSendBufLength=1024Bytes
				// 后面字符不作显示
				break;
			}

		}  // for (size_t st=0; st<strlen(pchDisplayTxt); st++)

		delete []pchDisplayTxt;

	}  // if (NULL != pwchDisplayTxt)

	// 命令结束符
	m_pchSendBuf[nIndex++] = 0x04;

	//DWORD dwReturn(0x00);
	//if (send(m_socket, m_pchSendBuf, nIndex, 0) <= 0)
	//{
	//	dwReturn = 0x01;
	//}
	//else
	//{
	//	dwReturn = 0x00;
	//}

	//return dwReturn;

	return SendToServer(m_pchSendBuf, nIndex);
}

DWORD CNH50R::DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt)
{
	return 0x15;
}

DWORD CNH50R::SendToServer(const char *const pchBuf, const int nLen)
{
	assert(NULL != pchBuf);
	assert(nLen > 0);

	DWORD dwReturn(0x00);
	if (m_bIsConnect_Ethernet)
	{	
		if (send(m_socket, pchBuf, nLen, 0) <= 0)
		{
			dwReturn = 0x01;
		}
		else
		{
			dwReturn = 0x00;
		}
	}
	else if (m_bIsConnect_Serial)
	{
		DWORD dwNumberOfBytesToWrite(nLen), dwBytesWritten(0x00);

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
	}
	else
	{
		dwReturn = 0x02;
	}

	return dwReturn;
}
