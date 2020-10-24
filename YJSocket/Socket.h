#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef NS_SOCKET_H
#define NS_SOCKET_H

class CSocket
{
public:
	CSocket(void);
	~CSocket(void);

public:
	bool Open(LPCTSTR szIP=NULL, const int nPort=NULL);
	bool Close(void);
	bool IsOpen(void) const;

private:	
	// 接收数据
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// 发送数据
	bool SendAll(SOCKET s, char *sendbuf, int len);

public:
	// 发送接收数据包
	bool SendAndRecvPacket(LPCTSTR szSend, std::wstring &strRecv);
	bool RecvPacket(std::wstring &strRecv);

	inline CStringW GetLogFilePath(void) {return m_strLogFilePath;};

protected:
	// 客户端连接线程
	SOCKET m_socket;

	std::wstring m_strIP;
	int m_nPort;
	// 日志文件路径（主程序）
	CStringW m_strLogFilePath;
};
#endif