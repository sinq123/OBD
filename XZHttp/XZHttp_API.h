

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "..\JsonnLib\JsonnLib.h"

#include "..\NHWin32Lib\NHWin32Lib.h"

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\JsonnLib\JsonnLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\JsonnLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\JsonnLib.lib")
#endif

struct SHBMsg
{
	// ���ؽ��Code
	std::wstring code;
	// д��ʧ��ʱ��msg������ϸ������Ϣ
	std::wstring msg;
	// 
	std::wstring data;
};




// ���ڶ�Json�Ĵ����Ǽ򵥵İ�����ֵ�������ַ������������ƽ̨�������ͼ�����������⣬Ҫ��Ϊԭ��Json��Ϊ����
typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;



class AFX_EXT_CLASS CXZHttp_API
{
	friend class std::auto_ptr<CXZHttp_API>;


public:
	static CXZHttp_API& GetInstance(void);

public:
	CXZHttp_API(void);
	virtual ~CXZHttp_API(void);

private:
	// ���ÿ������캯���Ϳ�����ֵ����
	CXZHttp_API(const CXZHttp_API&);
	CXZHttp_API & operator = (const CXZHttp_API&);

private:

	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);

	// Json��ʽת��
	CString JsonValueToCString(Json::Value root);

private:
	// ��־�ļ�·����������
	CStringW m_strLogFilePath;
	// ��ģ����
	CStringW m_strModName;

	// URL
	std::wstring m_wstrURL;
	// �û���
	std::wstring m_wstrUserName;
	// ����
	std::wstring m_wstrPassWord;
	// Token
	std::wstring m_wstrToken;
	// ���վ���
	std::wstring m_wstrJCZBH;

private:
	// ���������ļ�
	void LoadConfig(void);
	// ������־�ļ�
	void GenLogFile(void);
	// ��ʼ������
	void InitData(void);

private:
	/*
	wstrURL-> �ӿڵ�ַ
	ReqBody -> ����
	wstrRet-> ���ؽ��
	*/
	int Cutl(const std::wstring wstrURL, const Json::Value &ReqBody, const std::wstring strServiceCode, std::wstring& wstrRet);

public:
	// ����Token
	bool GetToken(void);
	//OBD ��������Ϣ	ODS_A7_OBD_INSPECTION_INFO
	bool OBDItemInfoA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD�����������Ƶ�Ԫ��	ODS_A8_CONTROL_LEVEL_CHECK_ITEM
	bool OBDItemControlA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD �������������룩	ODS_A9_ERROR_CODE_CHECK
	bool OBDItemErrorA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD�����������ͳ�δ������Ŀ��	ODS_A10_PETROL_CAR_UNREADY_PROJECT
	bool OBDItemPetrolCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD�����������ͳ�δ������Ŀ��	ODS_A11_DIESEL_CAR_UNREADY_PROJECT
	bool OBDItemDieselCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD��������IUPR��	ODS_A12_IUPR_CHECK_ITEM
	bool OBDItemIUPRA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//���ͳ�OBD�����������ʵʱ���ݣ�	ODS_A13_REALTIME_DATA_PETROL_CAR_CHECK
	bool OBDItemPetrolCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//���ͳ�OBD�����������ʵʱ���ݣ�	ODS_A14_REALTIME_DATA_DIESEL_CAR_CHECK
	bool OBDItemDieselCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD�����Ϣ	ODS_B2_OBD_CHECK_INFO
	bool OBDItemInfoB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//�����Ƶ�Ԫ��Ϣ	ODS_B3_CHECK_CONTROL_INFO
	bool OBDItemControlB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//����������Ϣ	ODS_B4_CHECK_FAULT_CODE
	bool OBDItemErrorB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);

};