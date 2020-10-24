/*
* Copyright (c) 2009,��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�NSClientLib.h
* �ļ���ʶ��
* ժҪ�����������ӿڷ�װ
*
* �汾��1.0.0
* ���ߣ�Raylee
* ���ڣ�2019-05-10
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.0
* ���ڣ�2019-05-10
* ���ߣ�Raylee
* ����������ing
*/
#pragma once

#include <memory>
#include <string>
#include <map>
#include <windows.h>

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

typedef std::map<std::wstring,std::wstring> MapData;

class AFX_EXT_CLASS CNSClientLib
{
public:
	static CNSClientLib& GetInstance(void);

private:
	CNSClientLib(void);
	~CNSClientLib(void);
	CNSClientLib(const CNSClientLib &other);
	CNSClientLib& operator=(const CNSClientLib &other);
	friend class std::auto_ptr<CNSClientLib>;

public:
	inline bool IsUsed(void) {	return m_bIsUsed;	}
	// ����IP��ַ
	inline void SetIP(LPCTSTR szIP) { m_strIP = szIP; }
	// ���ö˿�
	inline void SetPort(int nPort)  { m_nPort = nPort; }
	// ���ü���߱��
	inline LPCTSTR GetDeviceSN(void) {	return m_strDeviceSN.c_str();	}
	inline void SetDeviceSN(LPCTSTR szDeviceSN) {	m_strDeviceSN = szDeviceSN;	}
	// ������ˮ��
	inline LPCTSTR GetOutlookID(void) {	return m_strOutlookID.c_str();	}
	inline void SetOutlookID(LPCTSTR szOutlookID) {	m_strOutlookID = szOutlookID;	}
	// ��ȡ�����û��б�
	inline std::vector<USERINFO> GetUserList(void) {	return m_vtNetUserInfo;	}
	
public:
	// ͬ��OBDʱ��
	bool OBDGetTime(std::wstring &strResult, std::wstring &strMsg);
	// OBD����¼
	bool OBDVerify(std::wstring &strResult, std::wstring &strMsg);
	// OBD�����б�
	bool GetOBDVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg);
	// OBD���쳵����Ϣ
	bool GetOBDVehicleInfo(LPCTSTR szVIN, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);

	// �ŷż����֤
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	// ͬ��ʱ��
	bool GetTime(std::wstring &strResult, std::wstring &strMsg);
	// ��ȡ�����û��б�
	bool GetUsers(std::wstring &strResult, std::wstring &strMsg);
	// ����¼
	bool EmiVerify(std::wstring &strResult, std::wstring &strMsg);
	// �û��˳�
	bool UserExit(LPCTSTR szOperator, std::wstring &strResult, std::wstring &strMsg);
	// ��ȡ��������
	bool GetEvnSet(std::wstring &strResult, std::wstring &strMsg);
	// �����б�
	bool GetVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg);
	// ���쳵����Ϣ
	bool GetVehicleInfo(LPCTSTR szOutlookID, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);
	// ��ȡ��ֵ
	bool GetLimit(LIMITOFASM &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFVMAS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFDIS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFFSUNHT &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFLUGDOWN &sLimit, std::wstring &strResult, std::wstring &strMsg);

	// ��ʼ���
	bool StartTest(NHTestType emTestType, std::wstring &strResult, std::wstring &strMsg);

	// �ϴ�����˲̬���
	bool UploadVMAS(SResultOfVMAS &sResultData, std::vector<SRealTimeDataOfVMAS> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// �ϴ���̬���
	bool UploadASM(SResultOfASM &sResultData, std::vector<SRealTimeDataOfASM> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// �ϴ����ؼ��ٽ��
	bool UploadLUGDOWN(SResultOfLUGDOWN &sResultData, std::vector<SRealTimeDataOfLUGDOWN> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// �ϴ�˫���ٽ��
	bool UploadDIS(SResultOfDIS &sResultData, std::vector<SRealTimeDataOfDIS> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// �ϴ����ɼ��ٽ��
	bool UploadFSUNHT(SResultOfFSUNHT &sResultData, std::vector<SRealTimeDataOfFSUNHT> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);

	// �ϴ�OBD���
	bool UploadOBD(SResultOfOBD &sResultOfOBD, std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	
	// �궨��¼
	bool DemVerify(std::wstring &strResult, std::wstring &strMsg);
	// �⹦�����ػ���
	bool DynCoastdown(std::wstring &strResult, std::wstring &strMsg);
	// �⹦���������ʻ���
	bool DynParasiticLose(std::wstring &strResult, std::wstring &strMsg);
	// �⹦����У׼
	bool DynTorqueCal(std::wstring &strResult, std::wstring &strMsg);
	// �⹦���ٶȼ��
	bool DynSpeedCal(std::wstring &strResult, std::wstring &strMsg);

	// �����Ǽ�©
	bool AnaLeakTest(std::wstring &strResult, std::wstring &strMsg);
	// �����������̼��
	bool AnaO2RangeCheck(const float fO2, std::wstring &strResult, std::wstring &strMsg);
	// ����������У׼��飨���㣩
	bool AnaCalCheck(std::wstring &strResult, std::wstring &strMsg);
	// �����ǵͱ������
	bool AnaLowStandardGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// ����������У׼����㣩
	bool AnaFullCal(std::wstring &strResult, std::wstring &strMsg);

	// ��͸��У׼���
	bool OpaCalChk(std::wstring &strResult, std::wstring &strMsg);

	// ���������Լ�
	bool SelfcheckResult(float fET, float fRH, float fAP, std::wstring &strResult, std::wstring &strMsg);
	// �豸Ԥ��
	bool PreheatResult(std::wstring &strResult, std::wstring &strMsg);

private:
	// ���ͽ������ݰ�
	bool SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv);
	// ��ȡ��ֵ
	bool GetLimit(LPCTSTR szOutlookID, MapData &mapLimit, std::wstring &strResult, std::wstring &strMsg);

private:
	// VMAS����
	std::vector<float> m_vtVSTD;
	std::vector<float> m_vtVSTDMax;
	std::vector<float> m_vtVSTDMin;

	// �����Ƿ�����
	bool m_bIsUsed;
	// IP��ַ
	std::wstring m_strIP;
	// �˿�
	int m_nPort;

	// Ӧ�÷����ַ
	std::wstring m_strURL;
	// ����߱�ţ���Ӧ�����豸���
	std::wstring m_strDeviceSN;
	// ��֤��Ϣ��EISϵͳ�汾��
	std::wstring m_strVersion;
	// ��֤��Ϣ��EISϵͳ����޸�ʱ��
	std::wstring m_strModifyDateTime;
	// ��֤��Ϣ��֤�����к�
	std::wstring m_strCertificate;
	// ��֤��Ϣ��Ӳ�����к�
	std::wstring m_strHardDiskSN;

	// ƽ̨���ص��û��б�
	std::vector<USERINFO> m_vtNetUserInfo;
	// ��ǰ��¼��Ϣ
	std::wstring m_strOperatorName;
	std::wstring m_strOperatorPassword;
	std::wstring m_strDriverName;
	std::wstring m_strDriverPassword;

	// ��ǰ�����ˮ�ţ���Ӧ����OutlookID
	std::wstring m_strOutlookID;
	// ��¼�Ƿ��Ѿ���ʼ���
	MapData m_mapTesting;
};

