#pragma once

#include <queue>
#include <memory>
#include <list>
using std::list;
#include"atlstr.h"
#include "IocpModeSvr.h"
#include "json\json.h"
#include "SocketNetLibAPI.h"

#include <Windows.h>

//��ʱ�ı��ĺ��塰�����Ϊ�����롱
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT
//#include "../NHDef/NHDef.h"
//#include "../NHDef/NHStructDef.h"

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

//#include "..\NHNetWork\NHNet.h"
//#ifdef _DEBUG
//#pragma comment(lib, "..\\Debug\\NHNetWork_D.lib")
//#else
//#pragma comment(lib, "..\\Release\\NHNetWork.lib")
//#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

class AFX_EXT_CLASS WebServiceLibAPI
{
public:
	static WebServiceLibAPI& GetInstance(void);

private:
	WebServiceLibAPI(void);
	~WebServiceLibAPI(void);
	WebServiceLibAPI(const WebServiceLibAPI &other);
	WebServiceLibAPI& operator=(const WebServiceLibAPI &other);
	friend class std::auto_ptr<WebServiceLibAPI>;
	
	UINT m_unTimerID;
	static void CALLBACK TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
public:
	//��ʼ��Ⲣ¼��
	bool StartDetAndVideo();
	//������⣬ֹͣ¼��
	bool EndDetAndVideo(int nTestType);
	//�ϴ���������
	bool UploadProcessData(int nTestType);

	//�ϴ�OBD����
	bool UploadOBDTestData();
	//�ϴ�ȼ����������
	bool UploadFuelEvaporationData(bool bAirInlet, bool bAirOutlet, bool bPass);
	// �Լ쿪ʼ��¼�� BeginSelfTestRecord
	bool BeginSelfTestRecord(CString strTestType);
	////�ϴ��Լ�����Ҫ���Լ�¼��ĵ�����ʱ������������ɾ������Ҫ��¼���������ʹ�ã�
	//bool UploadSelfTestData(CString strTestType, bool bResult = false, CString strResultData = NULL);
	/* �Լ������ֹͣ¼�� EndSelfTestRecordAndUpload
	 * strTestType: �����Ŀ
	 * bResult: �����
	 * strResultData: �ϴ����xml��Ϣ ,�б�����ʱ�ļ��ģ��˲�����NULL
	 * strProcessData���ϴ�����json��Ϣ
	 */
	bool EndSelfTestRecordAndUpload(CString strTestType, bool bResult = false, CString strResultData= NULL, CString strProcessData= NULL);
	
	bool GetIsUsedNetTest(){return m_bUsed;};
	//����ͨ��
	bool ESToEP(int nNumber, int nOrder);

	//���ó�������
	CString SetVehicleType(CString strVehicleType);

	bool IsNeedTestOBD(const VehicleInfo &sVehicleInfo);

private:
	 //WebService��ַ
	CString m_strURL;
	//���IP
	CString m_strIP;
	//��ض˿�
	CString m_strPort;
	
	 //�û���
	CString m_strName;
	//����
	CString m_strPassWord;
	//�Ƿ�ʹ������(��׼��Ĭ�Ϸ�������ӦΪ��������)
	bool m_bUsed;
public:
	//���ͺ�
	//CString m_strVehicleType;
	int m_nServerPort;
	//�Լ��Ƿ�����
	//bool m_bSelfCheckUsedVideo;
public:
	static void __stdcall RecvData(unsigned long sIP,SOCKET sClient,char * pData,unsigned long DataLength);
	// ����������
	CIocpModeSvr m_IocpModeSvr;
	RealTimeDataOfLUGDOWN *m_RealTimeDataOfLUGDOWN;
	RealTimeDataOfDIS *m_RealTimeDataOfDIS;
    RealTimeDataOfVMAS *m_RealTimeDataOfVMAS;
	RealTimeDataOfFSUNHT *m_RealTimeDataOfFSUNHT;

	CSocketNetLibAPI m_sSocketNet;
	// ���״̬
	int m_nState; 
	// ��������
	int m_nOrder;
	// ����ͨ�ŷ���Ƶ��
	int m_nSendTime;
	// �Ƿ�ʹ�ù���ͨ��
	bool m_bIsUsedSendState;
	// ���嵥�����Ƿ�ֻ�ϴ��ϸ�����
	bool m_bIsChkPassData;
	
//	// �����������߳�
//	void StartNHServer(void);
//	// ֹͣ�������߳�
//	void StopNHServer(void);
//
//private:
//	// �������
//	CNHNetServer m_NHNetServer;
//	// ������Ϣ�Ŀͻ���IP
//	CString m_strClientIP;
//	// ���ݴ���ص�����
//	static void __stdcall DataSolutionCallBack(const wchar_t* pwchIP, const BYTE* pBytes, const size_t size);
};

