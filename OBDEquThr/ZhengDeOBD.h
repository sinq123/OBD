/*
* Copyright (c) ��ɽ�л�������������޹�˾
* All rights reserved.
*
* �ļ����ƣ�ZhengDeOBD.h
* �ļ���ʶ��
* ����: ���������Ѱ�ODB���ݽ���
*
* �汾��1.0
* ���ڣ�2018-06-01
* ���ߣ�Raylee
*
*
* ��ʷ��Ϣ��
*
* �汾��1.0.0
* ���ڣ�2018-06-01
* ���ߣ�Raylee
* ���������ڿ���
*/

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef ZhengDeOBD_H
#define ZhengDeOBD_H

#include <vector>
#include "OBDAdapter.h"

struct ZDPacket
{
	WORD wHeader;				// ��ͷ2�ֽ� NH	
	char szPacketLen[7];		// ������6�ֽ��ַ�����ʾ
	std::wstring strXML;		// UTF8��ʽ��xml�ַ���
	BYTE bTail;					// ��β1�ֽ� $
	
	ZDPacket()
	{
		ZDPacket(L"");
	}

	ZDPacket(LPCTSTR szXML)
	{
		strXML = szXML;

		wHeader = MAKEWORD('N', 'H');
		memset(szPacketLen, 0x00, sizeof(szPacketLen));
		bTail = '$';
	}

	std::string ToString()
	{
		std::string strData = TCharToUTF8(strXML.c_str());

		std::string strRet;
		strRet.append((char *)&wHeader, sizeof(wHeader));

		sprintf_s(szPacketLen, "%06d", min(strData.size()+9, 999999));
		strRet.append((char *)&szPacketLen, sizeof(szPacketLen)-1);	

		strRet.append(strData);
		strRet.append((char *)&bTail, sizeof(bTail));

		return strRet;
	}

	bool Prase(std::string strRecv)
	{
		if (strRecv.size() < 9)
		{
			return false;
		}

		int nOffset = 0;
		memcpy(&wHeader, strRecv.data()+nOffset, sizeof(wHeader));
		nOffset += sizeof(wHeader);
		// У���β
		if (wHeader != MAKEWORD('N', 'H'))
		{
			return false;
		}
		
		memcpy(&szPacketLen, strRecv.data()+nOffset, sizeof(szPacketLen)-1);
		szPacketLen[6] = '\0';
		nOffset += sizeof(szPacketLen)-1;
		unsigned int nPacketLen = atoi(szPacketLen);	
		// У�����ݳ���
		if (nPacketLen != strRecv.size())
		{
			return false;
		}
		
		std::string strData;
		strData.assign(strRecv.data()+nOffset, nPacketLen-9);
		nOffset += nPacketLen-9;

		memcpy(&bTail, strRecv.data()+nOffset, sizeof(bTail));
		nOffset += sizeof(bTail);
		
		// У���β
		if (bTail != '$')
		{
			return false;
		}

		strXML = UTF8ToTChar(strData.c_str());

		return true;
	}
};

class CZhengDeOBD : public COBDAdapter
{
public:
	CZhengDeOBD(void);
	~CZhengDeOBD(void);

public:
	////////////////////////////////////////////////////////////////////////////////////���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// ������
	// bPort�����ں�(1~255)
	// nBaudRate�������� (110��300��600��1200��2400��4800��9600��14400��19200��38400��56000��57600��115200��128000��256000)
	// ���أ�
	// COM_OPEN_SUCCEED
	// COM_OPEN_FAIL
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_115200){ return COM_OPEN_FAIL;}
	DWORD Open(std::wstring strIP, const int nPort);
	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���أ�
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
	
	bool IsOpen(void) const;
	////////////////////////////////////////////////////////////////////////////////////���ڲ���end

public:
	// �������Ƿ�����
	bool IsAdapterOnline(void);
	// ��ʼ��������
	bool Init(DWORD &dwProtocol, const int nFuelType);
	// ��ʼ����״̬
	bool GetInitState(OBDInitState &enState, DWORD &dwProtocol);
	// ȡ������
	bool GetDTC(std::vector<SDTCData> &vtDTC);
	// ȡ����ָʾ��״̬
	bool GetLampState(bool &bIsOn);
	// ȡϵͳ�����
	bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem);
	// ���������
	bool ClearDTC(void);
	// ȡʵʱ����
	bool GetRTData(int &nRev, float &fV, float &fTemp);
	bool GetRTData(std::map<std::wstring, std::wstring> &mapData);
	// ��ȡ������̣���λkm
	bool GetDTCMileage(DWORD &dwMileage);
	// ��ȡ��������
	bool GetFreezeData(std::wstring &strData);
	// ȡVIN
	bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL);
	// ȡOBD��ʽ����Ҫ��
	bool GetOBDType(std::wstring &strType);
	// ȡ��ʻ��̣���λkm
	bool GetMileage(DWORD &dwMileage);
	// ȡECU��Ϣ
	bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// ȡ����汾
	bool GetSoftwareVer(std::wstring &strVer);
	// ȡIUPR����
	bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// ����ʼ��
	bool UnInit(void);
	// ȡOBD����Э��
	std::wstring GetOBDProtocolName(DWORD dwProtocol);

private:
	bool SendAndRecvPacket(ZDPacket sZYPSend, ZDPacket &sZYPRecv);
	
	// ��������
	bool RecvAll(SOCKET s, char *recvbuf, int len);
	// ��������
	bool SendAll(SOCKET s, char *sendbuf, int len);
	
	bool Xml2Map(LPCTSTR szXML, 
		std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, 
		std::map<std::wstring, std::wstring> &mapData);

	// �ͻ��������߳�
	SOCKET m_socket;
	//// ������IP��ַ
	//char m_chServerIP[50];
	//// �������˿ں�
	//unsigned short m_usServerPort;

	// ��������
	int m_nRev;		// ������ת��
	float m_fV;		// ����
	float m_fTemp;	// ��ȴҺ�¶�
	int m_nFuelType;	// ȼ�����ͣ�1���ͣ�2����
	std::map<std::wstring, std::wstring> m_mapRTData;

	// �ϴβ���ʱ��
	clock_t m_LastOPTime;
	// ȡʵʱ����
	int m_nGetRTDataTimeInterval;

	// ��־�ļ�·����������
	CStringW m_strLogFilePath;
	// ��ģ����
	CStringW m_strModName;
	// ������־�ļ�
	void GenLogFile(void);

};
#endif