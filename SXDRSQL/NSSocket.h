/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NSSocket.h
* �ļ���ʶ��
* ժҪ����������Socket�ײ��װ
*
* �汾��1.0
* ���ߣ�Raylee
* ���ڣ�2019-05-10
*
* ��ʷ��Ϣ��
*

* �汾��1.0.0
* ���ڣ�2019-05-10
* ���ߣ�Raylee
* ��������ʽ����ʹ��
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
	// ��������
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// ��������
	bool SendAll(SOCKET s, char *sendbuf, int len);

public:
	// ���ͽ������ݰ�
	bool SendAndRecvPacket(LPCTSTR szSend, std::wstring &strRecv);

protected:
	// �ͻ��������߳�
	SOCKET m_socket;

	std::wstring m_strIP;
	int m_nPort;

	CStringW LogPath(void);
};
#endif