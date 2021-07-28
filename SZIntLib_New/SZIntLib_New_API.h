#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CSZIntLib_New_API
{
public:
	CSZIntLib_New_API(void);
	virtual ~CSZIntLib_New_API(void);

private:
	CSZIntLib_New_API(const CSZIntLib_New_API &);
	CSZIntLib_New_API & operator =(const CSZIntLib_New_API &);

public:
	/*
	token ��½�ɹ��󷵻ص�Ψһ��ʶ�ַ���(��ʱ��Ϊ��)
	unitid �������
	lineid ����߱��
	username �û���
	pwd ����
	vin ���ܺ�
	plate ����
	checkresult�ַ����ͣ���xml�ַ�������������
	checkmethod ��ⷽ��, 1��˫���ٷ���2����̬��������3������˲̬������4�����ؼ��٣�5����͸���̶ȷ���
							6�����ͳ�OBD���ݣ�7�����ͳ�OBD���ݣ�8����ۼ�⣻9�����ͳ�OBD�������ݣ�10�����ͳ�OBD�������ݡ�
	*/
	// ��ȡ�ֵ��
	static int GetBaseTypeInfo(const char * pchURL, std::wstring& strRetStr);
	// ��������¼
	static int Vehiclelogin(const char * pchURL, const std::wstring& strXmlDoc, std::wstring& strRetStr);
	// ��ȡ������б�
	static int GetCheckList(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid, std::wstring& strRetStr);
	// ��ȡ��Ӧ�ĳ�����Ϣ
	static int getVehicle(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid, const std::wstring& strVIN, std::wstring& strRetStr);
	// ��ʼ���߼��
	static int BeginCheck(const char * pchURL, const std::wstring& strunitid, const std::wstring& strlineid,
		const std::wstring& strVIN, const std::wstring& strPlate, std::wstring& strRetStr);
	// �ϴ������
	static int UploadInspectionResult(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid,
		const std::wstring& strcheckresult, const int& ncheckmethod, std::wstring& strRetStr);
	// ��¼
	static int LoginServer(const char * pchURL, const std::wstring& strunitid, const std::wstring& strlineid, 
		const std::wstring& strusername, const std::wstring& pwd, std::wstring& strRetStr);
	// ��־λ��
	static std::wstring LogFilePath(void);
};