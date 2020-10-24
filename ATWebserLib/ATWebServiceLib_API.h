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
	// 7注册验证接口
	// JczId:检测站编码	ZcTime：登记时间	ZcTime：登记时间
	static int VehicleRegJudge(const char * pchURL, const std::wstring& strJczId , const std::wstring& strZcTime , 
		const std::wstring& strCarItems, std::wstring& strRetStr);
	// 8上线检测权限判断接口
	// jczNo：检测站编号	CarPZ：车牌号	jcxNo：检测线名称	bdrq:标定日期
	static int OnlineJudge(const char * pchURL, const std::wstring& strjczNo , const std::wstring& strjcxNo , 
		const std::wstring& strbdrq, const std::wstring& strCarPZ, std::wstring& strRetStr);
	// 9检测数据接口
	// CheckDataItems：检测基本数据	CheckEndDataItems： 检测结果数据
	static int AddCheckData(const char * pchURL, const std::wstring& strCheckDataItems , const std::wstring& strCheckEndDataItems , 
		 std::wstring& strRetStr);
	// 10OBD检测数据接口
	// ObdDataItems：检测OBD数据
	static int AddObdData(const char * pchURL, const std::wstring& strObdDataItems, std::wstring& strRetStr);
	// 11OBDIUPR检测数据接口
	// IuprDataItems：检测IUPR数据	ObdType： 燃油种类，柴油:CY 汽油：QY
	static int AddIuprData(const char * pchURL, const std::wstring& strIuprDataItems , const std::wstring& strObdType , 
		 std::wstring& strRetStr);
	// 12机动车外观检查数据接口
	// WgjcDataItems：外观检查数据
	static int AddWGJCData(const char * pchURL, const std::wstring& strWgjcDataItems, std::wstring& strRetStr);
	// 13检测设备信息上传接口
	// addDeviceCheckData：检测设备数据
	static int AddDeviceCheckData(const char * pchURL, const std::wstring& straddDeviceCheckData, std::wstring& strRetStr);
	// 14设备自检数据上传接口
	// DeviceType设备类型 测功机类型：DY 五气分析仪：AN 烟度计：SM  Items
	static int AddVerify(const char * pchURL, const std::wstring& strDeviceType , const std::wstring& strItems , 
		 std::wstring& strRetStr);
	// 15报告打印判断接口
	// lsbh：流水号	jczNo：检测站编号
	static int PrintJudge(const char * pchURL, const std::wstring& strlsbh , const std::wstring& strjczNo , 
		 std::wstring& strRetStr);
	// 16报告打印并上传接口
	//lsbh：流水号	EncodeReport：检测报告进行转换后编码
	static int PrintUp(const char * pchURL, const std::wstring& strlsbh , const std::wstring& strencodeReport , 
		 std::wstring& strRetStr);
	// 17检测设备自动锁定接口
	// jczNo：检测站编码	devBm：设备唯一码
	static int LokeJudge(const char * pchURL, const std::wstring& strjczNo  , const std::wstring& strdevBm , 
		 std::wstring& strRetStr);
	// 18燃油蒸发排放控制系统数据接口
	// rykzItems：燃油蒸发数据参数项
	static int RyzfControl (const char * pchURL, const std::wstring& strrykzItems, std::wstring& strRetStr);
	
	// 日志位置
	static std::wstring LogFilePath(void);
	// 设置日志文件
	static DWORD SetLogFilePath(const std::wstring& strLogFilePath);
private:
	// 日志文件
	static std::wstring m_strLogFilePath;
};