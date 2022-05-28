#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include "Packet.h"
struct ZDPacket;
struct EPPacket;

class CTCPClient
{
public:
	CTCPClient(void);
	virtual ~CTCPClient(void);

public:
	void inline SetClient(const CString strIP, const int nPort)
	{
		m_strIP = strIP;
		m_nPort = nPort;
	}

	void inline GetClient(CString& strIP, int& nPort)
	{
		strIP = m_strIP;
		nPort = nPort;
	}

	// ��ʼ����
	bool Listen(void);
	// �Ͽ�����
	void DisConnect(void);

public:
	bool RecvAll(SOCKET sockfd, char *recvbuf, int len);	
	bool SendAll(SOCKET sockfd, char *sendbuf, int len);
	bool IsOpen(void) const;

	bool SendAndRecvPacket(ZDPacket sZDPSend, ZDPacket &sZDPRecv); // 

private:
	SOCKET m_sClient;
	int m_nPort;
	CString m_strIP;

};