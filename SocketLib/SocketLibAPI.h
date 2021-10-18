

#pragma once

#include <queue>
#include <memory>
#include <string>
#include <map>
#include <windows.h>
#include "Socket.h"
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

class AFX_EXT_CLASS CSocketLib_API
{
public:
	static CSocketLib_API& GetInstance(void);

private:
	CSocketLib_API(void);
	~CSocketLib_API(void);
	CSocketLib_API(const CSocketLib_API &other);
	CSocketLib_API& operator=(const CSocketLib_API &other);
	friend class std::auto_ptr<CSocketLib_API>;

public:
	// �ŷż����֤\����ע��
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);

	// �ϱ��豸�����̼�¼��
	bool UploadDeviceChkProcess(const std::wstring& strCheckType, const std::wstring strCheckTimeStart, const std::wstring strCheckTimeEnd, 
		std::wstring &strResult, std::wstring &strMsg);

	// �ϱ����ͳ����̲⹦�����м���¼��
	bool UploadGasDynCoastdown(std::wstring &strResult, std::wstring &strMsg);
	// �ϱ����ͳ����̲⹦��������ʧ���Լ�¼��
	bool UploadGasDynParasiticLose(std::wstring &strResult, std::wstring &strMsg);
	// �ϱ����ͳ����̲⹦�����м���¼��
	bool UploadDiesDynCoastdown(std::wstring &strResult, std::wstring &strMsg);
	// �ϱ����ͳ����̲⹦�����ӹ�����ʧ���Լ�¼��
	bool UploadDiesDynParasiticLose(std::wstring &strResult, std::wstring &strMsg);

	// �ϱ�������(�ͱ���)��¼��
	bool UploadAnaLowStdGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�������(����)��¼��
	bool UploadAnaZeroGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�������(�߱���)��¼��
	bool UploadAnaHighStdGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�������������¼��
	bool UploadAnaFullCalChk(std::wstring &strResult, std::wstring &strMsg);
	// �ϱ�й©����¼��
	bool UploadAnaLeakTest(std::wstring &strResult, std::wstring &strMsg);

	// �ϱ��̶ȼƼ���¼��
	bool UploadOpaCalChk(std::wstring &strResult, std::wstring &strMsg);


	// �ر�����
	void CloseConnect(void);
private:
	// ����Ψһ����
	CString GenerateInsNum(void);
	// ���ͽ������ݰ�
	bool SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv);
	
	// У����
	// ����ǰnλ��У���
	// ��ӣ�Ȼ��ȡ����ֽ�λ
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	CString CheckSum(const char *ch, const UINT un);
	
private:

	// ���ͽ���������
	CSocket m_Socket;

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
};