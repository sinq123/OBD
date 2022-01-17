

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
	// 返回结果Code
	std::wstring code;
	// 写入失败时的msg返回详细描述信息
	std::wstring msg;
	// 
	std::wstring data;
};




// 现在对Json的处理是简单的把所有值都当作字符串，如果联网平台做了类型检验可能有问题，要改为原生Json作为参数
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
	// 禁用拷贝构造函数和拷贝赋值函数
	CXZHttp_API(const CXZHttp_API&);
	CXZHttp_API & operator = (const CXZHttp_API&);

private:

	CString Map2Json(JsonMap mapPost);
	CString MapVec2Json(JsonMapVec vmPost);

	// Json格式转换
	CString JsonValueToCString(Json::Value root);

private:
	// 日志文件路径（主程序）
	CStringW m_strLogFilePath;
	// 本模块名
	CStringW m_strModName;

	// URL
	std::wstring m_wstrURL;
	// 用户名
	std::wstring m_wstrUserName;
	// 密码
	std::wstring m_wstrPassWord;
	// Token
	std::wstring m_wstrToken;
	// 检测站编号
	std::wstring m_wstrJCZBH;

private:
	// 加载配置文件
	void LoadConfig(void);
	// 生成日志文件
	void GenLogFile(void);
	// 初始化数据
	void InitData(void);

private:
	/*
	wstrURL-> 接口地址
	ReqBody -> 参数
	wstrRet-> 返回结果
	*/
	int Cutl(const std::wstring wstrURL, const Json::Value &ReqBody, const std::wstring strServiceCode, std::wstring& wstrRet);

public:
	// 请求Token
	bool GetToken(void);
	//OBD 检查基本信息	ODS_A7_OBD_INSPECTION_INFO
	bool OBDItemInfoA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD检查数据项（控制单元）	ODS_A8_CONTROL_LEVEL_CHECK_ITEM
	bool OBDItemControlA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD 检查数据项（故障码）	ODS_A9_ERROR_CODE_CHECK
	bool OBDItemErrorA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD检查数据项（汽油车未就绪项目）	ODS_A10_PETROL_CAR_UNREADY_PROJECT
	bool OBDItemPetrolCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD检查数据项（柴油车未就绪项目）	ODS_A11_DIESEL_CAR_UNREADY_PROJECT
	bool OBDItemDieselCarUnReadyA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD检查数据项（IUPR）	ODS_A12_IUPR_CHECK_ITEM
	bool OBDItemIUPRA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//汽油车OBD检查过程数据项（实时数据）	ODS_A13_REALTIME_DATA_PETROL_CAR_CHECK
	bool OBDItemPetrolCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//柴油车OBD检查过程数据项（实时数据）	ODS_A14_REALTIME_DATA_DIESEL_CAR_CHECK
	bool OBDItemDieselCarRealTimeA(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//OBD检查信息	ODS_B2_OBD_CHECK_INFO
	bool OBDItemInfoB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//检查控制单元信息	ODS_B3_CHECK_CONTROL_INFO
	bool OBDItemControlB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);
	//检查故障码信息	ODS_B4_CHECK_FAULT_CODE
	bool OBDItemErrorB(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg);

};