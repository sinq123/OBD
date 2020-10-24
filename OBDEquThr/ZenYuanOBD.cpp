#include "StdAfx.h"
#include "ZenYuanOBD.h"

#include <vector>
#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <atlstr.h>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

CZenYuanOBD::CZenYuanOBD(void)
{
	srand(unsigned int(time(NULL)));

	m_nRev = 0;
	m_fV = 0;
	m_fTemp = 0;
	m_LastOPTime = clock();
	m_nGetRTDataTimeInterval = 1000;
	m_bIsInit = false;

	m_mapOBDType[0x01] = L"OBD II";
	m_mapOBDType[0x02] = L"OBD";
	m_mapOBDType[0x03] = L"OBD and OBD II";
	m_mapOBDType[0x04] = L"OBD I";
	m_mapOBDType[0x05] = L"NO OBD";
	m_mapOBDType[0x06] = L"EOBD";
	m_mapOBDType[0x07] = L"EOBD and OBD II";
	m_mapOBDType[0x08] = L"EOBD and OBD";
	m_mapOBDType[0x09] = L"EOBD,OBD and OBD II";
	m_mapOBDType[0x0A] = L"JOBD";
	m_mapOBDType[0x0B] = L"JOBD and OBD II";
	m_mapOBDType[0x0C] = L"JOBD and OBD";
	m_mapOBDType[0x0D] = L"JOBD,EOBD and OBD II";
	m_mapOBDType[0x0E] = L"EURO IV B1";
	m_mapOBDType[0x0F] = L"EURO V B2";
	m_mapOBDType[0x10] = L"EURO C";
	m_mapOBDType[0x11] = L"EMD";
	m_mapOBDType[0x12] = L"EMD+";
	m_mapOBDType[0x13] = L"HD OBD-C";
	m_mapOBDType[0x14] = L"HD OBD";
	m_mapOBDType[0x15] = L"WWH-OBD";
	m_mapOBDType[0x16] = L"SAE/ISO Reerved";
	m_mapOBDType[0x17] = L"HD EOBD-I";
	m_mapOBDType[0x18] = L"HD EOBD-IN";
	m_mapOBDType[0x19] = L"HD EOBD-II";
	m_mapOBDType[0x1A] = L"HD EOBD-IIN";
	m_mapOBDType[0x1B] = L"SAE/ISO Reerved";
	m_mapOBDType[0x1C] = L"OBDBR-1";
	m_mapOBDType[0x1D] = L"OBDBR-2";
	m_mapOBDType[0x1E] = L"KOBD";
	m_mapOBDType[0x1F] = L"IOBD I";
	m_mapOBDType[0x20] = L"IOBD II";
	m_mapOBDType[0x21] = L"HD EOBD-VI";
	m_mapOBDType[0x22] = L"OBD,OBD II and HD OBD";
	m_mapOBDType[0x23] = L"OBDBR-3";
	m_mapOBDType[0x24] = L"MC EOBD-I";
	m_mapOBDType[0x25] = L"MC EOBD-II";
	m_mapOBDType[0x26] = L"MC COBD-I";
	m_mapOBDType[0x27] = L"MC TOBD-I";
	m_mapOBDType[0x28] = L"MC JOBD-I";
	m_mapOBDType[0x29] = L"CN-OBD-6";
	m_mapOBDType[0x2A] = L"OBDBR-D";
	m_mapOBDType[0x2B] = L"";
	m_mapOBDType[0x2C] = L"";
	m_mapOBDType[0x2D] = L"";
	m_mapOBDType[0x2E] = L"";
	m_mapOBDType[0x2F] = L"";
}

CZenYuanOBD::~CZenYuanOBD(void)
{
	// ��������ʱ,�رմ�����Դ
	Close();
}

DWORD CZenYuanOBD::Open(BYTE bPort, const int nBaudRate)
{
	assert(0 != bPort);

	// �ر��Ѵ��ڵĴ��ھ��
	Close();

	wchar_t szPort[MAX_PATH] = {0};
	swprintf_s(szPort, 20, L"\\\\.\\COM%hhu", bPort);
	m_hComm = CreateFileW(szPort,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		// �򿪴���ʧ��
		OutputDebugStringW(L"Error:fail to open com port\n");
		return COM_OPEN_FAIL;
	}

	///////////////////////////////////////////// ��ʼ�����ڲ���begin
	// ��������������������С
	SetupComm(m_hComm, 512, 256);
	// ����DCB�ṹ
	DCB dcb;
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(m_hComm, &dcb);
	// �趨�����ʣ����ⲿ��ȡ
	if (nBaudRate != NULL)
	{
		dcb.BaudRate = nBaudRate;
	}
	else
	{
		dcb.BaudRate = CBR_115200;
	}
	// �趨����żУ��
	dcb.Parity = NOPARITY;
	// �趨����λ
	dcb.ByteSize = 8;
	// �趨ֹͣλ
	dcb.StopBits = ONESTOPBIT;
	// ���ô����豸���Ʋ���
	SetCommState(m_hComm, &dcb);
	// ���г�ʱ����
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 500;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	/////////////////////////////////////////////// ��ʼ�����ڲ���end
	
	return COM_OPEN_SUCCEED;
}

DWORD CZenYuanOBD::Close(void)
{
	return COBDAdapter::Close();
}

bool CZenYuanOBD::SendCMD(ZYPacket sZYPSend, ZYPacket &sZYPRecv)
{
	// �Զ���
	CAutoLock autoLock(GetLogLocker());

	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	
	std::string strSend = sZYPSend.ToString();
#ifdef DEBUG
	CString strMsg, strTemp;
	for (int i=0; i<strSend.size(); i++)
	{
		strTemp.AppendFormat(L"%02X ", (unsigned char)strSend.at(i));
	}
	strMsg.Format(L"ZYOBD����[%d]", strSend.size());
	CNHLogAPI::WriteLog(LOG_MSG, strMsg, strTemp);
#endif
	std::string strRecv;
	if (WriteCommBuffer((BYTE *)strSend.data(), strSend.size()))
	{
		char szHeader[MAX_PATH] = {0};
		char szRecv[MAX_PATH] = {0};
		
		//clock_t tStartClock;
		//tStartClock  = clock();
		//bool bRecv = ReadCommBuffer((BYTE *)&szRecv[0], 1);
		//TRACE("BYTE 1 use %d ms\n", clock()-tStartClock);

		//tStartClock  = clock();
		//bRecv &= ReadCommBuffer((BYTE *)&szRecv[1], 1);
		//TRACE("BYTE 2 use %d ms\n", clock()-tStartClock);

		//tStartClock  = clock();
		//bRecv &= ReadCommBuffer((BYTE *)&szRecv[2], 1);
		//TRACE("BYTE 3 use %d ms\n", clock()-tStartClock);

		//tStartClock  = clock();
		//bRecv &= ReadCommBuffer((BYTE *)&szRecv[3], 1);
		//TRACE("BYTE 4 use %d ms\n", clock()-tStartClock);

		if (ReadCommBuffer((BYTE *)szRecv, 4))
		//if (bRecv)
		{
			strRecv.append((char *)szRecv, 4);
			DWORD dwLen = szRecv[2]*0xFF + szRecv[3];
			DWORD dwLeft = dwLen - 2 + 1;
			if (ReadCommBuffer((BYTE *)szRecv+4, dwLeft))
			{
				strRecv.append((char *)szRecv+4, dwLeft);				
#ifdef DEBUG
				strMsg = L"";
				strTemp = L"";
				for (int i=0; i<strRecv.size(); i++)
				{
					strTemp.AppendFormat(L"%02X ", (unsigned char)strRecv.at(i));
				}
				strMsg.Format(L"ZYOBD����[%d]", strRecv.size());
				CNHLogAPI::WriteLog(LOG_MSG, strMsg, strTemp);
#endif				
				if (sZYPRecv.Prase(strRecv))
				{
					TRACE("OBD SendCMD recv [%d] data ", sZYPRecv.strData.size());
					for (WORD i=0; i<sZYPRecv.strData.size(); i++)
					{
						TRACE("%02X ", (BYTE)sZYPRecv.strData.at(i));
					}
					TRACE("\n");

					if (sZYPRecv.bCmdG == 0x7F
						&& sZYPRecv.bCmdF == 0x21
						)
					{
						// ���͵�ָ������д�
						TRACE("OBD SendCMD data checksum error\n");
						return false;
					}

					return true;
				}
			}
		}
	}
	return false;
}

bool CZenYuanOBD::Init(DWORD &dwProtocol, const int nFuelType)
{
	m_nFuelType = nFuelType;

	ZYPacket sZYPSend(0x33, 0x00);
	sZYPSend.strData.append(1, (BYTE)dwProtocol);
	sZYPSend.strData.append(1, (BYTE)nFuelType);
	ZYPacket sZYPRecv;

	COMMTIMEOUTS CommTimeouts;
	GetCommTimeouts(m_hComm, &CommTimeouts);
	CommTimeouts.ReadTotalTimeoutConstant = 30*1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::Init", L"��ʼ��");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);

	if (bRet && sZYPRecv.strData.size()==1)
	{
		TRACE("OBD Init return %02X\n", (BYTE)sZYPRecv.strData.at(0));
		dwProtocol = (BYTE)sZYPRecv.strData.at(0);
		if (dwProtocol != 0x00)
		{
			switch (dwProtocol)
			{
			case 1:	m_nGetRTDataTimeInterval = 1500;	break;	//KWP2000 FastInit
			case 2:	m_nGetRTDataTimeInterval = 1500;	break;	//KWP2000 5baudInit
			case 3:	m_nGetRTDataTimeInterval = 1500;	break;	//ISO9141-2 5baudInit
			case 4:	m_nGetRTDataTimeInterval = 1000;	break;	//��׼CAN ISO15765
			case 5:	m_nGetRTDataTimeInterval = 1000;	break;	//��չCAN ISO15765
			case 6:	m_nGetRTDataTimeInterval = 2000;	break;	//VPW
			case 7:	m_nGetRTDataTimeInterval = 2000;	break;	//PWM
			case 8:	m_nGetRTDataTimeInterval = 1000;	break;	//J1939
			case 9:	m_nGetRTDataTimeInterval = 1500;	break;	//KWP1281
			default:m_nGetRTDataTimeInterval = 1500;	break;
			}

			m_bIsInit = true;
			return true;
		}
		else
		{
			m_bIsInit = false;
			return false;
		}
	}
	return false;
}

bool CZenYuanOBD::GetInitState(OBDInitState &enState, DWORD &dwProtocol)
{
	if (m_bIsInit)
	{
		enState = OBD_InitSucceed;
	}
	else
	{
		enState = OBD_InitFailed;
	}
	return true;
}

bool CZenYuanOBD::IsAdapterOnline(void)
{
	ZYPacket sZYPSend(0xA1, 0x01);
	ZYPacket sZYPRecv;
	
#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::IsAdapterOnline", L"��ѯ״̬");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	if (bRet && sZYPRecv.strData.size()==1)
	{
		TRACE("OBD IsAdapterOnline return %02X\n", (BYTE)sZYPRecv.strData.at(0));
		if ((BYTE)sZYPRecv.strData.at(0) == 0x02)
		{
			return true;
		}
	}
	return false;
}

bool CZenYuanOBD::GetDTC(std::vector<SDTCData> &vtDTC)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x01);
	ZYPacket sZYPRecv;

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetDTC", L"ȡ������");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	if (bRet && !sZYPRecv.strData.empty())
	{
		int nCount = sZYPRecv.strData.at(0);
		TRACE("OBD GetDTC return %d\n", nCount);
		
		if (sZYPRecv.strData.size() != nCount*2+1)
		{
			return false;
		}

		vtDTC.clear();
		for (int i=0; i<nCount; i++)
		{
			SDTCData sDTCData;
			WORD wDTC = 0;
			memcpy(&wDTC, sZYPRecv.strData.data()+i*2+1, sizeof(WORD));
			wDTC = ntohs(wDTC);
			TRACE("[%d]=%x\n", i, wDTC);

			// ����
			CString strDTC;
			if (wDTC != 0x00)
			{
				BYTE bType = (wDTC&0xC000)>>14;
				WORD wValue = wDTC&0x3FFF;
				switch (bType)
				{
				case 0:	strDTC.AppendFormat(L"P%04X,", wValue);	break;
				case 1:	strDTC.AppendFormat(L"C%04X,", wValue);	break;
				case 2:	strDTC.AppendFormat(L"B%04X,", wValue);	break;
				case 3:	strDTC.AppendFormat(L"U%04X,", wValue);	break;
				default: strDTC.AppendFormat(L"%04X,", wDTC);break;
				}
			}
			sDTCData.strCode = (LPCTSTR)strDTC;
			vtDTC.push_back(sDTCData);
		}

		return true;
	}
	return false;
}

bool CZenYuanOBD::GetLampState(bool &bIsOn)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x03);
	ZYPacket sZYPRecv;

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetLampState", L"ȡ���ϵ�״̬");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	if (bRet && !sZYPRecv.strData.empty())
	{
		std::vector<DWORD> vtJudge;
		for (DWORD i=0; i<sZYPRecv.strData.size(); i++)
		{
			vtJudge.push_back(sZYPRecv.strData.at(i));
		}
		for (DWORD i=0; i<vtJudge.size(); i++)
		{
			TRACE("[%d]=%x\n", i, vtJudge[i]);
		}
				
		// ��1���ֽ�1���ϵ�״̬��=0 OFF��=1 ON
		bIsOn = (vtJudge[0]==0x01);

		return true;
	}
	return false;
}

bool CZenYuanOBD::GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x03);
	ZYPacket sZYPRecv;

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetSystemCheck", L"ȡϵͳ���״̬");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	if (bRet && !sZYPRecv.strData.empty())
	{
		std::vector<DWORD> vtJudge;
		for (DWORD i=0; i<sZYPRecv.strData.size(); i++)
		{
			vtJudge.push_back(sZYPRecv.strData.at(i));
		}
		for (DWORD i=0; i<vtJudge.size(); i++)
		{
			TRACE("[%d]=%x\n", i, vtJudge[i]);
		}
		
		while (vtJudge.size() < 15)
		{
			vtJudge.push_back(0x00);
		}

		// �������˴������ַ��ͽ���һ��
		sSystemCheckItem.nFST = vtJudge[2];
		sSystemCheckItem.nEGR = vtJudge[10];
		sSystemCheckItem.nDOC = vtJudge[11];
		sSystemCheckItem.nSCR = vtJudge[12];
		sSystemCheckItem.nDPF = vtJudge[13];
		//sSystemCheckItem.nASC = ;
		sSystemCheckItem.nPOC = vtJudge[14];

		sSystemCheckItem.nMFT = vtJudge[1];
		sSystemCheckItem.nFST = vtJudge[2];
		sSystemCheckItem.nCC = vtJudge[4];
		sSystemCheckItem.nCCH = vtJudge[5];
		sSystemCheckItem.nOS = vtJudge[8];
		sSystemCheckItem.nOSH = vtJudge[9];
		sSystemCheckItem.nEECS = vtJudge[6];
		sSystemCheckItem.nEGR = vtJudge[10];
		sSystemCheckItem.nSAIS = vtJudge[7];
		sSystemCheckItem.nICM = vtJudge[3];

		return true;
	}
	return false;
}

bool CZenYuanOBD::ClearDTC(void)
{
	if (!m_bIsInit)
	{
		return false;
	}

	return false;
}

bool CZenYuanOBD::GetRTData(int &nRev, float &fV, float &fTemp)
{
	if (!m_bIsInit)
	{
		nRev = 0;
		m_nRev = 0;
		fV = 0.0f;
		m_fV = 0.0f;
		fTemp = 0.0f;
		m_fTemp = 0.0f;
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x06);
	ZYPacket sZYPRecv;

	if (clock()-m_LastOPTime > m_nGetRTDataTimeInterval)	// ǿ���ڵײ����Ʋ�������ķ��ͼ��
	{
		m_LastOPTime = clock();

#ifdef DEBUG
		CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetRTData", L"ȡ������1");
#endif
		bool bRet = SendCMD(sZYPSend, sZYPRecv);
		if (bRet && sZYPRecv.strData.size()==4)
		{
			TRACE("OBD EngineRev:%02X %02X\n", (BYTE)sZYPRecv.strData.at(0), (BYTE)sZYPRecv.strData.at(1));
			TRACE("OBD Velocity: %02X\n", (BYTE)sZYPRecv.strData.at(2));
			TRACE("OBD CoolantTemp: %02X\n", (BYTE)sZYPRecv.strData.at(3));

			WORD wTemp;
			memcpy(&wTemp, sZYPRecv.strData.data(), sizeof(wTemp));
			wTemp = ntohs(wTemp);	// �ߵ��ֽڷ�ת
			m_nRev = wTemp/4;		// ת/���ӣ�min��

			m_fV = (BYTE)sZYPRecv.strData.at(2);	//����Χ 0-255 KM/h��
			m_fTemp = (BYTE)sZYPRecv.strData.at(3)-40.0f;	//����Χ-40�� - 215�����϶ȣ�
			
			nRev = m_nRev;
			fV = m_fV;
			fTemp = m_fTemp;
			return true;
		}
		else
		{
			nRev = 0;
			m_nRev = 0;
			fV = 0.0f;
			m_fV = 0.0f;
			fTemp = 0.0f;
			m_fTemp = 0.0f;
		}
		return false;
	}
	else
	{
		nRev = m_nRev;
		fV = m_fV;
		fTemp = m_fTemp;
		return true;
	}
}

bool CZenYuanOBD::GetRTData(std::map<std::wstring, std::wstring> &mapData)
{
	if (!m_bIsInit)
	{
		//nRev = 0;
		m_nRev = 0;
		//fV = 0.0f;
		m_fV = 0.0f;
		//fTemp = 0.0f;
		m_fTemp = 0.0f;
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x0B);
	ZYPacket sZYPRecv;

	if (clock()-m_LastOPTime > m_nGetRTDataTimeInterval)	// ǿ���ڵײ����Ʋ�������ķ��ͼ��
	{
		m_LastOPTime = clock();

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetRTData", L"ȡ������2");
#endif
		bool bRet = SendCMD(sZYPSend, sZYPRecv);
		if (bRet)
		{
			WORD wTemp;
			BYTE bTemp;
			CString strTemp;
			if (m_nFuelType==1 && sZYPRecv.strData.size()>=10)
			{
				memcpy(&bTemp, sZYPRecv.strData.data(), sizeof(bTemp));		//�����ſ���[%]
				strTemp.Format(L"%d", bTemp/255*100);
				mapData[L"SolarTermDoorPosition"] = (LPCTSTR)strTemp;
				
				memcpy(&bTemp, sZYPRecv.strData.data()+1, sizeof(bTemp));	//���㸺��ֵ[%]
				strTemp.Format(L"%d", bTemp/255*100);
				mapData[L"CalculationLoad"] = (LPCTSTR)strTemp;

				//memcpy(&wTemp, sZYPRecv.strData.data()+2, sizeof(wTemp));	//��������ϵ��(��)
				//strTemp.Format(L"%f", 1.0f*ntohs(wTemp)*2/65535);
				//mapData[L"AirCoefficient"] = (LPCTSTR)strTemp;

				memcpy(&wTemp, sZYPRecv.strData.data()+2, sizeof(wTemp));	//ǰ�������ź�(mA)
				strTemp.Format(L"%f", 1.0f*ntohs(wTemp)*128/32768-128);
				mapData[L"AirCoefficient"] = (LPCTSTR)strTemp;

				memcpy(&bTemp, sZYPRecv.strData.data()+4, sizeof(bTemp));	//����[km/h]
				strTemp.Format(L"%d", bTemp);
				mapData[L"Velocity"] = (LPCTSTR)strTemp;

				memcpy(&wTemp, sZYPRecv.strData.data()+5, sizeof(wTemp));	//������ת��[rpm]
				strTemp.Format(L"%d", ntohs(wTemp)/4);
				mapData[L"EngineRev"] = (LPCTSTR)strTemp;// ת/���ӣ�min��	

				//memcpy(&bTemp, sZYPRecv.strData.data()+7, sizeof(bTemp));	//����ѹ��(kpa)
				//strTemp.Format(L"%d", bTemp);
				//mapData[L"MAF"] = (LPCTSTR)strTemp;

				memcpy(&wTemp, sZYPRecv.strData.data()+8, sizeof(wTemp));	//������(g/s)
				strTemp.Format(L"%d", wTemp);
				mapData[L"MAF"] = (LPCTSTR)strTemp;
			
				m_mapRTData = mapData;
			}
			else if (m_nFuelType==2 && sZYPRecv.strData.size()>=25)
			{
				memcpy(&bTemp, sZYPRecv.strData.data(), sizeof(bTemp));		//���ſ���[%]
				strTemp.Format(L"%d", bTemp/255*100);
				mapData[L"OilDoorPosition"] = (LPCTSTR)strTemp;
				
				memcpy(&bTemp, sZYPRecv.strData.data()+1, sizeof(bTemp));	//����[km/h]
				strTemp.Format(L"%d", bTemp);
				mapData[L"Velocity"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+4, sizeof(wTemp));	//������ת��[rpm]
				strTemp.Format(L"%d", wTemp);
				mapData[L"EngineRev"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+2, sizeof(wTemp));	//ͨ��Ť�ؼ��� �������������(kw)
				strTemp.Format(L"%d", wTemp*_wtoi(mapData[L"EngineRev"].c_str())/9550);
				mapData[L"EngineOutputPower"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+6, sizeof(wTemp));	//������(g/s)
				strTemp.Format(L"%d", wTemp/100);
				mapData[L"MAF"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+8, sizeof(wTemp));	//��ѹѹ��[kpa]
				strTemp.Format(L"%d", wTemp/32);
				mapData[L"ChargeAirPressure"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+10, sizeof(wTemp));	//����������Ũ��[ppm]
				strTemp.Format(L"%d", wTemp);
				mapData[L"NOXConcentration"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+12, sizeof(wTemp));	//����������[mL/s]
				strTemp.Format(L"%d", wTemp/200);
				mapData[L"UreaInjectionVolume"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+14, sizeof(wTemp));	//�����¶�[��]
				strTemp.Format(L"%d", wTemp-40);
				mapData[L"EGT"] = (LPCTSTR)strTemp;
				
				//����������ѹ��[Kpa]
				memcpy(&wTemp, sZYPRecv.strData.data()+16, sizeof(wTemp));
				float fP1 = wTemp/100.0f;
				memcpy(&wTemp, sZYPRecv.strData.data()+18, sizeof(wTemp));
				float fP2 = wTemp/100.0f;
				strTemp.Format(L"%f", fP2-fP1);
				mapData[L"DPFDifferentialPressure"] = (LPCTSTR)strTemp;
				
				memcpy(&bTemp, sZYPRecv.strData.data()+20, sizeof(bTemp));	//EGR ����[%]
				strTemp.Format(L"%d", bTemp);
				mapData[L"EGRPosition"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+21, sizeof(wTemp));	//ȼ������ѹ��[bar]
				strTemp.Format(L"%d", wTemp*10*1000);
				mapData[L"FuelDeliveryPressure"] = (LPCTSTR)strTemp;
				
				memcpy(&wTemp, sZYPRecv.strData.data()+23, sizeof(wTemp));	//������[L/100km]
				float fV = (float)_wtof(mapData[L"Velocity"].c_str());
				if (fV <= 1e-6)
				{
					strTemp.Format(L"%f", wTemp/20.0f);
				}
				else
				{
					strTemp.Format(L"%f", wTemp*5/fV);
				}
				mapData[L"FuelConsumption"] = (LPCTSTR)strTemp;

				m_mapRTData = mapData;
			}
			return true;
		}
		else
		{
			//nRev = 0;
			m_nRev = 0;
			//fV = 0.0f;
			m_fV = 0.0f;
			//fTemp = 0.0f;
			m_fTemp = 0.0f;
		}
		return false;
	}
	else
	{
		//nRev = m_nRev;
		//fV = m_fV;
		//fTemp = m_fTemp;
		mapData = m_mapRTData;
		return true;
	}
}

bool CZenYuanOBD::GetDTCMileage(DWORD &dwMileage)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x02);
	ZYPacket sZYPRecv;

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetDTCMileage", L"ȡ�������");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	if (bRet && sZYPRecv.strData.size()==2)
	{
		TRACE("OBD GetDTCMileage return %02X %02X\n", (BYTE)sZYPRecv.strData.at(0), (BYTE)sZYPRecv.strData.at(1));
		
		WORD wTemp;
		memcpy(&wTemp, sZYPRecv.strData.data(), sizeof(wTemp));
		dwMileage = ntohs(wTemp);	// �ߵ��ֽڷ�ת		

		return true;
	}
	return false;
}

bool CZenYuanOBD::GetFreezeData(std::wstring &strData)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x04);
	ZYPacket sZYPRecv;
	
	COMMTIMEOUTS CommTimeouts;
	GetCommTimeouts(m_hComm, &CommTimeouts);
	CommTimeouts.ReadTotalTimeoutConstant = 5*1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetFreezeData", L"ȡ��������֡");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
	if (bRet && sZYPRecv.strData.size()>1)
	{
		CString strTemp;
		for (UINT i=0; i<sZYPRecv.strData.size(); i++)
		{
			strTemp.AppendFormat(L"%02X", (unsigned char)sZYPRecv.strData.at(i));
		}
		strData = (LPCTSTR)strTemp;
		return true;
	}
	return false;
}

bool CZenYuanOBD::GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode/*=NULL*/)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x05);
	ZYPacket sZYPRecv;
	
	COMMTIMEOUTS CommTimeouts;
	GetCommTimeouts(m_hComm, &CommTimeouts);
	CommTimeouts.ReadTotalTimeoutConstant = 5*1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);
#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetVIN", L"ȡVIN");
#endif
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComm, &CommTimeouts);

	if (bRet && sZYPRecv.strData.size()==17)
	{
		std::string strTemp;
		strTemp.assign(sZYPRecv.strData.data(), 17);
		strVIN = ANSIToTChar(strTemp.c_str());
		return true;
	}
	return false;
}

bool CZenYuanOBD::GetOBDType(std::wstring &strType)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x08);
	ZYPacket sZYPRecv;

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetOBDType", L"ȡ��ʽ����Ҫ��");
#endif	
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	
	if (bRet && sZYPRecv.strData.size()==1)
	{
		strType = m_mapOBDType[sZYPRecv.strData.at(0)];
		return true;
	}
	return false;
}

bool CZenYuanOBD::GetMileage(DWORD &dwMileage)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x0A);
	ZYPacket sZYPRecv;

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetMileage", L"ȡ��ʻ���");
#endif		
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	if (bRet && sZYPRecv.strData.size()==4)
	{
		TRACE("OBD GetMileage return %02X %02X %02X %02X\n", (BYTE)sZYPRecv.strData.at(0), (BYTE)sZYPRecv.strData.at(1), (BYTE)sZYPRecv.strData.at(2), (BYTE)sZYPRecv.strData.at(3));
		
		DWORD dwTemp;
		memcpy(&dwTemp, sZYPRecv.strData.data(), sizeof(dwTemp));
		dwMileage = ntohs(dwTemp) * 0.1f;	// �ߵ��ֽڷ�ת		

		return true;
	}
	return false;
}

bool CZenYuanOBD::GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	if (!m_bIsInit)
	{
		return false;
	}
	
	{
		ZYPacket sZYPSend(0x33, 0x07);
		sZYPSend.strData.append(1, 0x01);
		ZYPacket sZYPRecv;
	
#ifdef DEBUG
		CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetECUInfo", L"ȡECU��Ϣ�����Ƶ�Ԫ���ƣ�");
#endif	
		bool bRet = SendCMD(sZYPSend, sZYPRecv);
	
		if (bRet && sZYPRecv.strData.size()>1)
		{
			//mapData[] = ANSIToTChar(sZYPRecv.strData.c_str());
		}
	}

	{
#ifdef DEBUG
		CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetECUInfo", L"ȡECU��Ϣ��CAL ID��");
#endif	
		ZYPacket sZYPSend(0x33, 0x07);
		sZYPSend.strData.append(1, 0x02);
		ZYPacket sZYPRecv;
	
		bool bRet = SendCMD(sZYPSend, sZYPRecv);
	
		if (bRet && sZYPRecv.strData.size()>1)
		{
			mapData[L"EngineCALID"] = ANSIToTChar(sZYPRecv.strData.c_str());
		}
	}

	{
		ZYPacket sZYPSend(0x33, 0x07);
		sZYPSend.strData.append(1, 0x03);
		ZYPacket sZYPRecv;
	
#ifdef DEBUG
		CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetECUInfo", L"ȡECU��Ϣ��CVN��");
#endif	
		bool bRet = SendCMD(sZYPSend, sZYPRecv);
	
		if (bRet && sZYPRecv.strData.size()>1)
		{
			CString strTemp;
			int nCount = sZYPRecv.strData.at(0);
			for (UINT i=0; i<nCount; i++)
			{
				strTemp.AppendFormat(L"%02X%02X%02X%02X-", (unsigned char)sZYPRecv.strData.at(i*4+1)
														, (unsigned char)sZYPRecv.strData.at(i*4+2)
														, (unsigned char)sZYPRecv.strData.at(i*4+3)
														, (unsigned char)sZYPRecv.strData.at(i*4+4));
			}
			strTemp.TrimRight(L"-");
			mapData[L"EngineCVN"] = (LPCTSTR)strTemp;
		}
	}
	return false;
}

bool CZenYuanOBD::GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode/*=NULL*/)
{
	if (!m_bIsInit)
	{
		return false;
	}

	ZYPacket sZYPSend(0x33, 0x09);
	ZYPacket sZYPRecv;
	
#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetIUPR", L"ȡIUPR");
#endif	
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	
	if (bRet)
	{
		CString strTemp;
		WORD wTemp;
		if (m_nFuelType==1 && sZYPRecv.strData.size()>=(2+4*9))
		{
			memcpy(&wTemp, sZYPRecv.strData.data()+2, sizeof(wTemp));	//�߻���������ɴ�������1
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"CMCCB1"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+4, sizeof(wTemp));	//�߻��������������ϴ�������1
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"CMCECB1"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+6, sizeof(wTemp));	//�߻���������ɴ�������2
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"CMCCB2"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+8, sizeof(wTemp));	//�߻��������������ϴ�������2
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"CMCECB2"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+10, sizeof(wTemp));	//��������������ɴ�������1
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"O2SMCCB1"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+12, sizeof(wTemp));	//�������������������ϴ�������1
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"O2SMCECB1"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+14, sizeof(wTemp));	//��������������ɴ�������2
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"O2SMCCB2"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+16, sizeof(wTemp));	//�������������������ϴ�������2
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"O2SMCECB2"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+18, sizeof(wTemp));	//EVAP ������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"EVAPC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+20, sizeof(wTemp));	//EVAP �����������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"EVAPEC"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+22, sizeof(wTemp));	//EGR ��/��VVT ������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"EGRC"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+24, sizeof(wTemp));	//EGR ��/��VVT �����������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"EGREC"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+26, sizeof(wTemp));	//GPF��1 ������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"GPFC1"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+28, sizeof(wTemp));	//GPF��1 �����������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"GPFEC1"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+30, sizeof(wTemp));	//GPF��2 ������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"GPFC2"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+32, sizeof(wTemp));	//GPF��2 �����������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"GPFEC2"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+34, sizeof(wTemp));	//���ο�������ϵͳ������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"AMCCC"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+36, sizeof(wTemp));	//���ο�������ϵͳ�����������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"AMCEC"] = (LPCTSTR)strTemp;
		}
		else if (m_nFuelType==2 && sZYPRecv.strData.size()>=(2+4*7))
		{
			memcpy(&wTemp, sZYPRecv.strData.data()+2, sizeof(wTemp));	//NMHC �߻��������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"NMHCC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+4, sizeof(wTemp));	//NMHC �߻�������������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"NMHCEC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+6, sizeof(wTemp));	//NOX �߻��������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"NOXCC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+8, sizeof(wTemp));	//NOX �߻�������������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"NOXCEC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+10, sizeof(wTemp));	//NOX �����������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"NOXAC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+12, sizeof(wTemp));	//NOX ����������������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"NOXAEC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+14, sizeof(wTemp));	//PM �����������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"PMC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+16, sizeof(wTemp));	//PM ����������������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"PMEC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+18, sizeof(wTemp));	//���������������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"WSC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+20, sizeof(wTemp));	//��������������������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"WSEC"] = (LPCTSTR)strTemp;			
			
			memcpy(&wTemp, sZYPRecv.strData.data()+22, sizeof(wTemp));	//EGR ��/��VVT ������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"EGRC"] = (LPCTSTR)strTemp;
			
			memcpy(&wTemp, sZYPRecv.strData.data()+24, sizeof(wTemp));	//EGR ��/��VVT �����������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"EGREC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+26, sizeof(wTemp));	//��ѹѹ�������ɴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"PPC"] = (LPCTSTR)strTemp;

			memcpy(&wTemp, sZYPRecv.strData.data()+28, sizeof(wTemp));	//��ѹѹ������������ϴ���
			strTemp.Format(L"%d", ntohs(wTemp));
			mapData[L"PPEC"] = (LPCTSTR)strTemp;
		}

		return true;
	}
	return false;
}

bool CZenYuanOBD::GetSoftwareVer(std::wstring &strVer)
{
	ZYPacket sZYPSend(0xA1, 0x08);
	ZYPacket sZYPRecv;

#ifdef DEBUG
	CNHLogAPI::WriteLog(LOG_MSG, L"CZenYuanOBD::GetSoftwareVer", L"ȡ����汾");
#endif		
	bool bRet = SendCMD(sZYPSend, sZYPRecv);
	if (bRet && sZYPRecv.strData.size()>10)
	{
		strVer = ANSIToTChar(sZYPRecv.strData.c_str());
		return true;
	}
	return false;
}

bool CZenYuanOBD::UnInit(void)
{
	m_bIsInit = false;
	return true;
}

std::wstring CZenYuanOBD::GetOBDProtocolName(DWORD dwProtocol)
{
	switch (dwProtocol)
	{
	case 1:	return L"KWP2000 FastInit";
	case 2:	return L"KWP2000 5baudInit";
	case 3:	return L"ISO9141-2 5baudInit";
	case 4:	return L"��׼CAN ISO15765";
	case 5:	return L"��չCAN ISO15765";
	case 6:	return L"VPW";
	case 7:	return L"PWM";
	case 8:	return L"J1939";
	default:return L"δ֪Э��";
	}
}