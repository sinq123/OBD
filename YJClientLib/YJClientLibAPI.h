/*
* Copyright (c) 2009,��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�YJClientLib.h
* �ļ���ʶ��
* ժҪ���ƾ������ӿڷ�װ
*
* �汾��1.0.0
* ���ߣ�Sjming
* ���ڣ�2019-06-29
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.0
* ���ڣ�2019-06-29
* ���ߣ�Sjming
* ����������ing
*/
#pragma once

#include <queue>
#include <memory>
#include <string>
#include <map>
#include <windows.h>
#include "YJSocket.h"
#include "json\json.h"

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
typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;

class AFX_EXT_CLASS CYJClientLib
{
public:
	static CYJClientLib& GetInstance(void);

private:
	CYJClientLib(void);
	~CYJClientLib(void);
	CYJClientLib(const CYJClientLib &other);
	CYJClientLib& operator=(const CYJClientLib &other);
	friend class std::auto_ptr<CYJClientLib>;

public:
	inline bool IsUsed(void) {	return m_bIsUsed;	}
	// ����IP��ַ
	inline void SetIP(LPCTSTR szIP) { m_strIP = szIP; }
	// ���ö˿�
	inline void SetPort(int nPort)  { m_nPort = nPort; }
	// ���ü���߱��
	inline LPCTSTR GetLineNumber(void) {	return m_strLineNumber;	}
	inline void SetLineNumber(LPCTSTR szLineNumber) {	m_strLineNumber = szLineNumber;	}
	// ���ü���߱��
	inline LPCTSTR GetStationNumber(void) {	return m_strStationNumber;	}
	inline void SetStationNumber(LPCTSTR szStationNumber) {	m_strStationNumber = szStationNumber;	}
	// ���ù�����
	inline void SetPocessCount(int nPocessCount) {	m_nPocessCount = nPocessCount;	}
	// ������ˮ��
	inline LPCTSTR GetBusinessID(void) {	return m_strBusinessID;	}
	inline void SetBusinessID(LPCTSTR szBusinessID) {	m_strBusinessID = szBusinessID;	}
	// ���ò⹦���������ʲ���,��ʣ��
	inline void SetPLHPParams(float fAp, float fBp, float fCp) { m_fAp=fAp;m_fBp=fBp;m_fCp=fCp;	}
	// �����������
	inline float CalculatePLHP(const float fV)
	{
		const double dVelocity = fV;
		const double dPLHP = m_fAp*dVelocity/80.5 + m_fBp*dVelocity*dVelocity/80.5/80.5 + m_fCp*dVelocity*dVelocity*dVelocity/80.5/80.5/80.5;
		// ��ʽ��Ϊ2λС��,����������
		const float fPLHP = (int)(dPLHP * 100.0 + 0.5) / 100.0f;
		return fPLHP;
	}
	// �ر�����
	void CloseConnect(void);
	
public:
	//// ͬ��OBDʱ��
	//bool OBDGetTime(std::wstring &strResult, std::wstring &strMsg);
	//// OBD����¼
	//bool OBDVerify(std::wstring &strResult, std::wstring &strMsg);
	//// OBD�����б�
	//bool GetOBDVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg);
	//// OBD���쳵����Ϣ
	//bool GetOBDVehicleInfo(LPCTSTR szVIN, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);

	// �ŷż����֤\����ע��
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	// ͬ��ʱ��
	bool GetTime(std::wstring &strResult, std::wstring &strMsg);
	// ��ȡ�����û��б�
	bool GetUsers(std::wstring &strResult, std::wstring &strMsg);
	// ��չ����б�
	void ClearProcessData(void);
	//// ����¼
	//bool EmiVerify(std::wstring &strResult, std::wstring &strMsg);
	//// �û��˳�
	//bool UserExit(LPCTSTR szOperator, std::wstring &strResult, std::wstring &strMsg);
	//// ��ȡ��������
	//bool GetEvnSet(std::wstring &strResult, std::wstring &strMsg);
	// �����б�
	bool GetVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strFilterData, std::wstring &strResult, std::wstring &strMsg);
	//// ���쳵����Ϣ
	//bool GetVehicleInfo(LPCTSTR szOutlookID, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);
	// ��ȡ��ֵ
	bool GetLimit(LIMITOFASM &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFVMAS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFDIS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFFSUNHT &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFLUGDOWN &sLimit, std::wstring &strResult, std::wstring &strMsg);
	

// �������ź���Ϣ
enum TestSignalInfo
{
	START = 0,		// ��ʼ�ź�	0
	INTUBE,			// ����ź�	1
	COLSTART,		// ������ʼ�ź�	2
	COLEND,			// ���������ź�	3
	END,			// �����ź�	4
};
	// �ϱ�����ź�
	bool UploadTestSignal(TestSignalInfo emInfo, std::wstring &strResult, std::wstring &strMsg);
	// �ϴ�OBD�����
	bool UploadOBDResult(const LineInfo &sLineInfo,const TestLog &sTestLog, const SResultOfOBD &sResultData);
	// �ϴ�OBD��������
	bool UploadOBDProcessData(SOBDRTData &sRTData);
	// �ϴ�ASM��������
	bool UploadASMProcessData(const RealTimeDataOfASM &sRTData);
	// �ϴ�ASM�������
	bool UploadASMResult(const TestLog &sTestLog, SResultOfASM &sResultData);
	// �ϴ�VMAS��������
	bool UploadVMASProcessData(const RealTimeDataOfVMAS &sRTData);
	// �ϴ�VMAS�������
	bool UploadVMASResult(const TestLog &sTestLog, const SResultOfVMAS &sResultData);
	// �ϴ�DIS��������
	bool UploadDISProcessData(const RealTimeDataOfDIS &sRTData);
	// �ϴ�DIS�������
	bool UploadDISResult(const TestLog &sTestLog, const SResultOfDIS &sResultData);
	// �ϴ����ɼ��ٹ�������
	bool UploadFSUNHTProcessData(const RealTimeDataOfFSUNHT &sRTData);
	// �ϴ����ɼ��ټ������
	bool UploadFSUNHTResult(const TestLog &sTestLog, const SResultOfFSUNHT &sResultData);
	// �ϴ����ؼ��ٹ�������
	bool UploadLUGDOWNProcessData(const RealTimeDataOfLUGDOWN &sRTData);
	// �ϴ����ؼ��ټ������
	bool UploadLUGDOWNResult(const TestLog &sTestLog, const SResultOfLUGDOWN &sResultData);

	// �ϴ�������豸״̬��Ϣ
	bool UploadDeviceStatus(std::wstring strDStatus, std::wstring &strResult, std::wstring &strMsg);
	// �ϴ�������豸����汾������Ϣ
	bool UploadDeviceSoftwareVerson(std::wstring &strSoftWareVersion, std::wstring &strResult, std::wstring &strMsg);


	// �ϴ����ɼ����豸�Լ���Ϣ
	bool UploadFSUSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// �ϴ����ؼ����豸�Լ���Ϣ
	bool UploadLUGDOWNSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// �ϴ���̬�豸�Լ���Ϣ
	bool UploadASMSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// �ϴ�˫�����豸�Լ���Ϣ
	bool UploadDISSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// �ϴ�����˲̬�豸�Լ���Ϣ
	bool UploadVMASSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);


	// �ϱ������߱궨��Ϣ
	bool UploadGasLineCalInfo(DEMARCATIONRESULTOFANAGASCAL &sResultOfAnaGasCal, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ������߱궨��Ϣ
	bool UploadDiesLineCalInfo(DEMARCATIONRESULTOFANAGASCAL &sResultOfAnaGasCal, std::wstring &strResult, std::wstring &strMsg);

	// �ϱ����ͳ����̲⹦�����м���¼��
	bool UploadGasDynCoastdown(SGasDynCoastdownData &sCoastdownData, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ����ͳ����̲⹦��������ʧ���Լ�¼��
	bool UploadGasDynParasiticLose(DEMARCATIONRESULTOFDYNPLHP &sResultOfDynPLHP, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ����ͳ����̲⹦�����м���¼��
	bool UploadDiesDynCoastdown(SDiesDynCoastdownData &sCoastdownData, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ����ͳ����̲⹦�����ӹ�����ʧ���Լ�¼��
	bool UploadDiesDynParasiticLose(DEMARCATIONRESULTOFDYNPLHP &sResultOfDynPLHP, std::wstring &strResult, std::wstring &strMsg);

	// �ϱ�������(�ͱ���)��¼��
	bool UploadAnaLowStdGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�������(����)��¼��
	bool UploadAnaZeroGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�������(�߱���)��¼��
	bool UploadAnaHighStdGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�������������¼��
	bool UploadAnaFullCalChk(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, int nCheckType, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�й©����¼��
	bool UploadAnaLeakTest(std::wstring &strStratTime, std::wstring &strPass, std::wstring &strResult, std::wstring &strMsg);

	// �ϱ��̶ȼƼ���¼��
	bool UploadOpaCalChk(DEMARCATIONRESULTOFOPACALCHK &sResultOfOpaCalChk, std::wstring &strResult, std::wstring &strMsg);

	// �ϱ��豸�����̼�¼��
	bool UploadDeviceChkProcess(std::wstring &strJsonData, std::wstring &strResult, std::wstring &strMsg);
	// �ϱ��豸ά�ޱ�����¼��
	bool UploadDeviceMaintenance(std::wstring &strJsonData, std::wstring &strResult, std::wstring &strMsg);

private:
	// ����Ψһ����
	CString GenerateInsNum(void);
	// ���ͽ������ݰ�
	bool SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv);
	// ��ȡ��ֵ
	//bool GetLimit(LPCTSTR szOutlookID, MapData &mapLimit, std::wstring &strResult, std::wstring &strMsg);

	// У����
	// ����ǰnλ��У���
	// ��ӣ�Ȼ��ȡ����ֽ�λ
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	CString CheckSum(const char *ch, const UINT un);

	// Json�ַ���ת����ʽ
	bool Json2Map(LPCTSTR szJson, JsonMap &mapRet);
	CString Map2Json(JsonMap mapPost);
	CString Json2Cstring(Json::Value &value);

private:

	// ���ͽ���������
	CYJSocket m_yjSocket;

	// �����Ƿ�����
	bool m_bIsUsed;
	// IP��ַ
	CString m_strIP;
	// �˿�
	int m_nPort;

	// ���վ����
	CString m_strStationNumber;
	// ����ߺ�
	CString m_strLineNumber;
	// ������ˮ��
	CString m_strBusinessID;

public:
	// ƽ̨���ص��û��б�
	std::vector<USERINFO> m_vtNetUserInfo;

	// ������ɫ
	std::map<std::wstring, std::wstring> m_mapPlateColor;
	// ȼ������
	std::map<std::wstring, std::wstring> m_mapFuelType;
	// ������ʽ
	std::map<std::wstring, std::wstring> m_mapAirIntakeMode;
	// ȼ�Ϲ���ϵͳ��ʽ
	std::map<std::wstring, std::wstring> m_mapOilSupplyMode;
	// �������
	std::map<std::wstring, std::wstring> m_mapTestType;
	std::map<std::wstring, std::wstring> m_mapTestTypeToCode;
	
	// �ϴ������������м���
	int m_nPocessCount;

private:
	// �⹦���������ʲ���
	float m_fAp;
	float m_fBp;
	float m_fCp;

private:
	// �첽�ϴ����������߳�
	static DWORD WINAPI UploadThreadFunc(LPVOID pParam);
	HANDLE m_RunMutex;

public:
	// ��������ѹ������
	void PushProcessData(const RealTimeDataOfVMAS &sRealTimeDataOfVMAS);
	void PushProcessData(const RealTimeDataOfASM &sRealTimeDataOfASM);
	void PushProcessData(const RealTimeDataOfLUGDOWN &sRealTimeDataOfLUGDOWN);
	void PushProcessData(const RealTimeDataOfDIS &sRealTimeDataOfDIS);
	void PushProcessData(const RealTimeDataOfFSUNHT &sRealTimeDataOfFSUNHT);

	std::queue<RealTimeDataOfVMAS> m_RealTimeDataOfVMASQueue;
	std::queue<RealTimeDataOfLUGDOWN> m_RealTimeDataOfLUGDOWNQueue;
	std::queue<RealTimeDataOfDIS> m_RealTimeDataOfDISQueue;
	std::queue<RealTimeDataOfASM> m_RealTimeDataOfASMQueue;
	std::queue<RealTimeDataOfFSUNHT> m_RealTimeDataOfFSUNHTQueue;

	CRITICAL_SECTION m_CriticalSection;
};

