
#pragma once
#include "string"


class CHYIntLib_API
{
public:
	CHYIntLib_API(void);
	virtual ~CHYIntLib_API(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CHYIntLib_API(const CHYIntLib_API &);
	CHYIntLib_API & operator =(const CHYIntLib_API &);

private:
	/*static */ char* m_pchURL;
	/*static */ CStringW m_strkey;
	/*static */ CStringW m_StationNum;
	/*static */ CStringW m_LineNum;
	// 获取配置信息
	void GetConfig(void);
	// 生成日志文件
	CStringW m_strLogFilePath;
	void GetLogFilePath(void);


public:
	// 2.10上传汽油车底盘测功机滑行检查记录13W01
	/*static */ int ConstLoad_13W01(std::wstring& strRetStr);
	// 2.11上传汽油车底盘测功机附加损失测试记录13W02
	/*static */ int PLHP_13W02(std::wstring& strRetStr);
	// 2.12上传柴油车底盘测功机检查记录13W03// Heavy
	/*static */ int ConstLoadHeavy_13W03(std::wstring& strRetStr);
	// 2.13上传柴油车底盘测功机附加功率损失测试记录13W04
	/*static */ int PLHPHeavy_13W04(std::wstring& strRetStr);
	// 上传单点检查（低标气）记录13W05
	/*static */ int GasCheck_13W05(std::wstring& strRetStr);
	// 2.15上传单点检查（零气）记录13W06
	/*static */ int GasCheck_13W06(std::wstring& strRetStr);
	// 2.16上传单点检查（高标气）记录13W07
	/*static */ int GasCheck_13W07(std::wstring& strRetStr);
	// 2.17上传分析仪五点检查记录13W08
	/*static */ int GasCheck_13W08(std::wstring& strRetStr);
	// 2.18上传泄漏检查记录13W09
	/*static */ int LeakageCheck_13W09(std::wstring& strRetStr);
	// 2.19上传烟度计检查记录13W10
	/*static */ int YDCalChk_13W010(std::wstring& strRetStr);
	// 2.20上传设备检查过程记录13W11
	/*static */ int EqulChk_13W011(std::wstring& strRetStr);
	// 2.21上传设备自检记录13W12
	/*static */ int EqulSelfIns_13W012(std::wstring& strRetStr);
	// 2.22上传备检定记录13W14
	/*static */ int EqulCal_13W014(std::wstring& strRetStr);
};