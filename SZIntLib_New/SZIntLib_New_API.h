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
	token 登陆成功后返回的唯一标识字符串(暂时可为空)
	unitid 机构编号
	lineid 检测线编号
	username 用户名
	pwd 密码
	vin 车架号
	plate 车牌
	checkresult字符串型，以xml字符串传入检测结果。
	checkmethod 检测方法, 1、双怠速法；2、稳态工况法，3、简易瞬态工况；4、加载减速；5、不透光烟度法；
							6、汽油车OBD数据；7、柴油车OBD数据；8、外观检测；9、汽油车OBD过程数据；10、柴油车OBD过程数据。
	*/
	// 获取字典表
	static int GetBaseTypeInfo(const char * pchURL, std::wstring& strRetStr);
	// 车辆检测登录
	static int Vehiclelogin(const char * pchURL, const std::wstring& strXmlDoc, std::wstring& strRetStr);
	// 获取待检测列表
	static int GetCheckList(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid, std::wstring& strRetStr);
	// 获取对应的车辆信息
	static int getVehicle(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid, const std::wstring& strVIN, std::wstring& strRetStr);
	// 开始上线检测
	static int BeginCheck(const char * pchURL, const std::wstring& strunitid, const std::wstring& strlineid,
		const std::wstring& strVIN, const std::wstring& strPlate, std::wstring& strRetStr);
	// 上传检测结果
	static int UploadInspectionResult(const char * pchURL, const std::wstring& strtoken, const std::wstring& strunitid,
		const std::wstring& strcheckresult, const int& ncheckmethod, std::wstring& strRetStr);
	// 登录
	static int LoginServer(const char * pchURL, const std::wstring& strunitid, const std::wstring& strlineid, 
		const std::wstring& strusername, const std::wstring& pwd, std::wstring& strRetStr);
	// 日志位置
	static std::wstring LogFilePath(void);
};