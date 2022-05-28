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

	// 开始监听
	bool Listen(void);
	// 接受客户端的请求
	SOCKET Accept(void);
	// 断开连接
	void DisConnect(void);
	// 关闭并删除记录
	void ClostClient(SOCKET sockfd);
	// 获取客户端信息
	CString GetPeerInfo(SOCKET sockfd);


public:
	bool RecvAll(SOCKET sockfd, char *recvbuf, int len);	
	bool SendAll(SOCKET sockfd, char *sendbuf, int len);

	bool RecvReqPacket(SOCKET sockfd, ZDPacket &sPacketReq);	// 接收请求包

	bool Response(SOCKET sockfd, ZDPacket& sPacketRes);
private:
	SOCKET m_sListen;
	int m_nPort;

	std::vector<SOCKET> m_vtClient;
	//客户信息临界保护量
	CRITICAL_SECTION m_csVS;
};