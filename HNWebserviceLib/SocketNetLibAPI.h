/*
* Copyright (c) ��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�SocketNetLibAPI.h
* �ļ���ʶ��
* ժҪ���Ͼ�������Ƶ���ղ�����װ
*
* �汾��1.0.0
* ���ߣ�Raylee
* ���ڣ�2017-07-20
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2017-07-20
* ���ߣ�Raylee
* ����������ing
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
	// ץ��
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
	// ���ӷ�����
	bool Connect(LPCTSTR lpServerIP, int port);
	// �Ͽ�����
	bool DisConnect(void);
	// ��������
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// ��������
	bool SendAll(SOCKET s, char *sendbuf, int len);

private:
	// �Ƿ���������
	bool m_bIsUse;

	// ������IP
	CString m_strServerIP;
	// �������˿�
	int m_nPort;
	// ����߱��
	CString m_strLineNumber;
	// ���վ���
	CString m_strStationNumber;

	// �������糬ʱ��ms
	int m_nTimeOut;
	// ����SOCKET
	SOCKET m_sockfd;
};

