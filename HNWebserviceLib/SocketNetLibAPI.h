/*
* Copyright (c) 佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：SocketNetLibAPI.h
* 文件标识：
* 摘要：南京联网视频拍照操作封装
*
* 版本：1.0.0
* 作者：Raylee
* 日期：2017-07-20
*
* 历史信息：
*
* 版本：1.0.0
* 日期：2017-07-20
* 作者：Raylee
* 描述：开发ing
*/

#pragma once

#include <memory>
#include <windows.h>
#include <winsock2.h> 
#include <string>

//enum NHPosition
//{
//	NHP_Enter = 05,
//	NHP_Break = 06,
//	NHP_Light = 07,
//	NHP_Cha = 08,
//};

class AFX_EXT_CLASS CSocketNetLibAPI
{
public:
	static inline CSocketNetLibAPI& GetInstance(void)
	{
		static std::auto_ptr<CSocketNetLibAPI> auto_ptr_instance;
		if (auto_ptr_instance.get() == NULL) 
		{
			auto_ptr_instance.reset(new CSocketNetLibAPI());
		}
		return *(auto_ptr_instance.get());
	}

public:
	CSocketNetLibAPI(void);
	~CSocketNetLibAPI(void);
	CSocketNetLibAPI(const CSocketNetLibAPI &other);
	CSocketNetLibAPI& operator=(const CSocketNetLibAPI &other);
	friend class std::auto_ptr<CSocketNetLibAPI>;

public:
	// 抓拍
	bool Snap(LPCTSTR szRunningNumber, int nPosition, 
		LPCTSTR szPlateTypeCode, LPCTSTR szPlateNumber, LPCTSTR szVIN, bool bNewCar);

	bool SendToServer(const char *const pchSend, const int nLen);

public:
	inline void SetUsed(bool bIsUse)					{	m_bIsUse = bIsUse;					}
	inline bool GetUsed(void)							{	return m_bIsUse;					}
	
	inline void SetServerIP(LPCTSTR szServerIP)			{	m_strServerIP = szServerIP;			}
	inline void SetServerPort(int nPort)				{	m_nPort = nPort;					}
	inline void SetLineNumber(LPCTSTR szLineNumber)		{	m_strLineNumber = szLineNumber;		}
	inline LPCTSTR GetLineNumber(void)					{	return m_strLineNumber;				}
	inline void SetStationNumber(LPCTSTR szStationNumber){	m_strStationNumber = szStationNumber;		}
	inline LPCTSTR GetStationNumber(void)				{	return m_strStationNumber;				}
	
public:
	// 连接服务器
	bool Connect(LPCTSTR lpServerIP, int port);
	// 断开连接
	bool DisConnect(void);
	// 接收数据
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// 发送数据
	bool SendAll(SOCKET s, char *sendbuf, int len);

private:
	// 是否启用联网
	bool m_bIsUse;

	// 服务器IP
	CString m_strServerIP;
	// 服务器端口
	int m_nPort;
	// 检测线编号
	CString m_strLineNumber;
	// 检测站编号
	CString m_strStationNumber;

	// 调用网络超时，ms
	int m_nTimeOut;
	// 连接SOCKET
	SOCKET m_sockfd;
};

