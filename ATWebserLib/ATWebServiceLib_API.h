#pragma once

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include <memory>
#include <string>

class AFX_EXT_CLASS CATWebServic_API
{
public:
	CATWebServic_API(void);
	virtual ~CATWebServic_API(void);

private:
	CATWebServic_API(const CATWebServic_API &);
	CATWebServic_API & operator =(const CATWebServic_API &);

public:
	// 7ע����֤�ӿ�
	// JczId:���վ����	ZcTime���Ǽ�ʱ��	ZcTime���Ǽ�ʱ��
	static int VehicleRegJudge(const char * pchURL, const std::wstring& strJczId , const std::wstring& strZcTime , 
		const std::wstring& strCarItems, std::wstring& strRetStr);
	// 8���߼��Ȩ���жϽӿ�
	// jczNo�����վ���	CarPZ�����ƺ�	jcxNo�����������	bdrq:�궨����
	static int OnlineJudge(const char * pchURL, const std::wstring& strjczNo , const std::wstring& strjcxNo , 
		const std::wstring& strbdrq, const std::wstring& strCarPZ, std::wstring& strRetStr);
	// 9������ݽӿ�
	// CheckDataItems������������	CheckEndDataItems�� ���������
	static int AddCheckData(const char * pchURL, const std::wstring& strCheckDataItems , const std::wstring& strCheckEndDataItems , 
		 std::wstring& strRetStr);
	// 10OBD������ݽӿ�
	// ObdDataItems�����OBD����
	static int AddObdData(const char * pchURL, const std::wstring& strObdDataItems, std::wstring& strRetStr);
	// 11OBDIUPR������ݽӿ�
	// IuprDataItems�����IUPR����	ObdType�� ȼ�����࣬����:CY ���ͣ�QY
	static int AddIuprData(const char * pchURL, const std::wstring& strIuprDataItems , const std::wstring& strObdType , 
		 std::wstring& strRetStr);
	// 12��������ۼ�����ݽӿ�
	// WgjcDataItems����ۼ������
	static int AddWGJCData(const char * pchURL, const std::wstring& strWgjcDataItems, std::wstring& strRetStr);
	// 13����豸��Ϣ�ϴ��ӿ�
	// addDeviceCheckData������豸����
	static int AddDeviceCheckData(const char * pchURL, const std::wstring& straddDeviceCheckData, std::wstring& strRetStr);
	// 14�豸�Լ������ϴ��ӿ�
	// DeviceType�豸���� �⹦�����ͣ�DY ���������ǣ�AN �̶ȼƣ�SM  Items
	static int AddVerify(const char * pchURL, const std::wstring& strDeviceType , const std::wstring& strItems , 
		 std::wstring& strRetStr);
	// 15�����ӡ�жϽӿ�
	// lsbh����ˮ��	jczNo�����վ���
	static int PrintJudge(const char * pchURL, const std::wstring& strlsbh , const std::wstring& strjczNo , 
		 std::wstring& strRetStr);
	// 16�����ӡ���ϴ��ӿ�
	//lsbh����ˮ��	EncodeReport����ⱨ�����ת�������
	static int PrintUp(const char * pchURL, const std::wstring& strlsbh , const std::wstring& strencodeReport , 
		 std::wstring& strRetStr);
	// 17����豸�Զ������ӿ�
	// jczNo�����վ����	devBm���豸Ψһ��
	static int LokeJudge(const char * pchURL, const std::wstring& strjczNo  , const std::wstring& strdevBm , 
		 std::wstring& strRetStr);
	// 18ȼ�������ŷſ���ϵͳ���ݽӿ�
	// rykzItems��ȼ���������ݲ�����
	static int RyzfControl (const char * pchURL, const std::wstring& strrykzItems, std::wstring& strRetStr);
	
	// ��־λ��
	static std::wstring LogFilePath(void);
	// ������־�ļ�
	static DWORD SetLogFilePath(const std::wstring& strLogFilePath);
private:
	// ��־�ļ�
	static std::wstring m_strLogFilePath;
};