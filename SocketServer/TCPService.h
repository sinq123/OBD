#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include "Packet.h"

struct ZDPacket;
struct EPPacket;

class CTCPService
{
public:
	CTCPService(void);
	virtual ~CTCPService(void);
	
public:
	void inline SetServer(int nPort)
	{
		m_nPort = nPort;
	}

	// ��ʼ����
	bool Listen(void);
	// ���ܿͻ��˵�����
	SOCKET Accept(void);
	// �Ͽ�����
	void DisConnect(void);
	// �رղ�ɾ����¼
	void ClostClient(SOCKET sockfd);
	// ��ȡ�ͻ�����Ϣ
	CString GetPeerInfo(SOCKET sockfd);


public:
	bool RecvAll(SOCKET sockfd, char *recvbuf, int len);	
	bool SendAll(SOCKET sockfd, char *sendbuf, int len);

	bool RecvReqPacket(SOCKET sockfd, ZDPacket &sPacketReq);	// ���������

	bool Response(SOCKET sockfd, ZDPacket& sPacketRes);
private:
	SOCKET m_sListen;
	int m_nPort;

	std::vector<SOCKET> m_vtClient;
	//�ͻ���Ϣ�ٽ籣����
	CRITICAL_SECTION m_csVS;
};