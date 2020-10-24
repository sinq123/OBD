
#pragma once
#include "string"


class CHYIntLib_API
{
public:
	CHYIntLib_API(void);
	virtual ~CHYIntLib_API(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CHYIntLib_API(const CHYIntLib_API &);
	CHYIntLib_API & operator =(const CHYIntLib_API &);

private:
	/*static */ char* m_pchURL;
	/*static */ CStringW m_strkey;
	/*static */ CStringW m_StationNum;
	/*static */ CStringW m_LineNum;
	// ��ȡ������Ϣ
	void GetConfig(void);
	// ������־�ļ�
	CStringW m_strLogFilePath;
	void GetLogFilePath(void);


public:
	// 2.10�ϴ����ͳ����̲⹦�����м���¼13W01
	/*static */ int ConstLoad_13W01(std::wstring& strRetStr);
	// 2.11�ϴ����ͳ����̲⹦��������ʧ���Լ�¼13W02
	/*static */ int PLHP_13W02(std::wstring& strRetStr);
	// 2.12�ϴ����ͳ����̲⹦������¼13W03// Heavy
	/*static */ int ConstLoadHeavy_13W03(std::wstring& strRetStr);
	// 2.13�ϴ����ͳ����̲⹦�����ӹ�����ʧ���Լ�¼13W04
	/*static */ int PLHPHeavy_13W04(std::wstring& strRetStr);
	// �ϴ������飨�ͱ�������¼13W05
	/*static */ int GasCheck_13W05(std::wstring& strRetStr);
	// 2.15�ϴ������飨��������¼13W06
	/*static */ int GasCheck_13W06(std::wstring& strRetStr);
	// 2.16�ϴ������飨�߱�������¼13W07
	/*static */ int GasCheck_13W07(std::wstring& strRetStr);
	// 2.17�ϴ�������������¼13W08
	/*static */ int GasCheck_13W08(std::wstring& strRetStr);
	// 2.18�ϴ�й©����¼13W09
	/*static */ int LeakageCheck_13W09(std::wstring& strRetStr);
	// 2.19�ϴ��̶ȼƼ���¼13W10
	/*static */ int YDCalChk_13W010(std::wstring& strRetStr);
	// 2.20�ϴ��豸�����̼�¼13W11
	/*static */ int EqulChk_13W011(std::wstring& strRetStr);
	// 2.21�ϴ��豸�Լ��¼13W12
	/*static */ int EqulSelfIns_13W012(std::wstring& strRetStr);
	// 2.22�ϴ����춨��¼13W14
	/*static */ int EqulCal_13W014(std::wstring& strRetStr);
};