/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NSSocket.h
* 文件标识：
* 摘要：东软联网Socket底层封装
*
* 版本：1.0
* 作者：Raylee
* 日期：2019-05-10
*
* 历史信息：
*

* 版本：1.0.0
* 日期：2019-05-10
* 作者：Raylee
* 描述：正式发布使用
*/

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef NS_SOCKET_H
#define NS_SOCKET_H

class CNSSocket
{
public:
	CNSSocket(void);
	~CNSSocket(void);

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

protected:
	// 客户端连接线程
	SOCKET m_socket;

	std::wstring m_strIP;
	int m_nPort;

	CStringW LogPath(void);
};
#endif