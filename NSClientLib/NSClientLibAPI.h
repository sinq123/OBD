/*
* Copyright (c) 2009,佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：NSClientLib.h
* 文件标识：
* 摘要：东软联网接口封装
*
* 版本：1.0.0
* 作者：Raylee
* 日期：2019-05-10
*
* 历史信息：
*
*
* 版本：1.0.0
* 日期：2019-05-10
* 作者：Raylee
* 描述：开发ing
*/
#pragma once

#include <memory>
#include <string>
#include <map>
#include <windows.h>

//临时改变宏的含义“输出”为“输入”
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

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

typedef std::map<std::wstring,std::wstring> MapData;

class AFX_EXT_CLASS CNSClientLib
{
public:
	static CNSClientLib& GetInstance(void);

private:
	CNSClientLib(void);
	~CNSClientLib(void);
	CNSClientLib(const CNSClientLib &other);
	CNSClientLib& operator=(const CNSClientLib &other);
	friend class std::auto_ptr<CNSClientLib>;

public:
	inline bool IsUsed(void) {	return m_bIsUsed;	}
	// 设置IP地址
	inline void SetIP(LPCTSTR szIP) { m_strIP = szIP; }
	// 设置端口
	inline void SetPort(int nPort)  { m_nPort = nPort; }
	// 设置检测线编号
	inline LPCTSTR GetDeviceSN(void) {	return m_strDeviceSN.c_str();	}
	inline void SetDeviceSN(LPCTSTR szDeviceSN) {	m_strDeviceSN = szDeviceSN;	}
	// 设置流水号
	inline LPCTSTR GetOutlookID(void) {	return m_strOutlookID.c_str();	}
	inline void SetOutlookID(LPCTSTR szOutlookID) {	m_strOutlookID = szOutlookID;	}
	// 获取联网用户列表
	inline std::vector<USERINFO> GetUserList(void) {	return m_vtNetUserInfo;	}
	
public:
	// 同步OBD时间
	bool OBDGetTime(std::wstring &strResult, std::wstring &strMsg);
	// OBD检测登录
	bool OBDVerify(std::wstring &strResult, std::wstring &strMsg);
	// OBD待检列表
	bool GetOBDVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg);
	// OBD待检车辆信息
	bool GetOBDVehicleInfo(LPCTSTR szVIN, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);

	// 排放检测认证
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	// 同步时间
	bool GetTime(std::wstring &strResult, std::wstring &strMsg);
	// 获取联网用户列表
	bool GetUsers(std::wstring &strResult, std::wstring &strMsg);
	// 检测登录
	bool EmiVerify(std::wstring &strResult, std::wstring &strMsg);
	// 用户退出
	bool UserExit(LPCTSTR szOperator, std::wstring &strResult, std::wstring &strMsg);
	// 获取环境限制
	bool GetEvnSet(std::wstring &strResult, std::wstring &strMsg);
	// 待检列表
	bool GetVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg);
	// 待检车辆信息
	bool GetVehicleInfo(LPCTSTR szOutlookID, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);
	// 获取限值
	bool GetLimit(LIMITOFASM &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFVMAS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFDIS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFFSUNHT &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFLUGDOWN &sLimit, std::wstring &strResult, std::wstring &strMsg);

	// 开始检测
	bool StartTest(NHTestType emTestType, std::wstring &strResult, std::wstring &strMsg);

	// 上传简易瞬态结果
	bool UploadVMAS(SResultOfVMAS &sResultData, std::vector<SRealTimeDataOfVMAS> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// 上传稳态结果
	bool UploadASM(SResultOfASM &sResultData, std::vector<SRealTimeDataOfASM> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// 上传加载减速结果
	bool UploadLUGDOWN(SResultOfLUGDOWN &sResultData, std::vector<SRealTimeDataOfLUGDOWN> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// 上传双怠速结果
	bool UploadDIS(SResultOfDIS &sResultData, std::vector<SRealTimeDataOfDIS> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	// 上传自由加速结果
	bool UploadFSUNHT(SResultOfFSUNHT &sResultData, std::vector<SRealTimeDataOfFSUNHT> &vtRTData, 
					std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);

	// 上传OBD结果
	bool UploadOBD(SResultOfOBD &sResultOfOBD, std::wstring &strResult, std::wstring &strMsg, std::wstring &strReportNumber);
	
	// 标定登录
	bool DemVerify(std::wstring &strResult, std::wstring &strMsg);
	// 测功机加载滑行
	bool DynCoastdown(std::wstring &strResult, std::wstring &strMsg);
	// 测功机寄生功率滑行
	bool DynParasiticLose(std::wstring &strResult, std::wstring &strMsg);
	// 测功机力校准
	bool DynTorqueCal(std::wstring &strResult, std::wstring &strMsg);
	// 测功机速度检查
	bool DynSpeedCal(std::wstring &strResult, std::wstring &strMsg);

	// 分析仪检漏
	bool AnaLeakTest(std::wstring &strResult, std::wstring &strMsg);
	// 分析仪氧量程检查
	bool AnaO2RangeCheck(const float fO2, std::wstring &strResult, std::wstring &strMsg);
	// 分析仪气体校准检查（单点）
	bool AnaCalCheck(std::wstring &strResult, std::wstring &strMsg);
	// 分析仪低标气检查
	bool AnaLowStandardGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// 分析仪气体校准（五点）
	bool AnaFullCal(std::wstring &strResult, std::wstring &strMsg);

	// 不透光校准检查
	bool OpaCalChk(std::wstring &strResult, std::wstring &strMsg);

	// 环境参数自检
	bool SelfcheckResult(float fET, float fRH, float fAP, std::wstring &strResult, std::wstring &strMsg);
	// 设备预热
	bool PreheatResult(std::wstring &strResult, std::wstring &strMsg);

private:
	// 发送接收数据包
	bool SendAndRecvPacket(LPCTSTR szXML, std::wstring &strRecv);
	// 获取限值
	bool GetLimit(LPCTSTR szOutlookID, MapData &mapLimit, std::wstring &strResult, std::wstring &strMsg);

private:
	// VMAS曲线
	std::vector<float> m_vtVSTD;
	std::vector<float> m_vtVSTDMax;
	std::vector<float> m_vtVSTDMin;

	// 联网是否启用
	bool m_bIsUsed;
	// IP地址
	std::wstring m_strIP;
	// 端口
	int m_nPort;

	// 应用服务地址
	std::wstring m_strURL;
	// 检测线编号，对应联网设备编号
	std::wstring m_strDeviceSN;
	// 认证信息，EIS系统版本号
	std::wstring m_strVersion;
	// 认证信息，EIS系统最后修改时间
	std::wstring m_strModifyDateTime;
	// 认证信息，证书序列号
	std::wstring m_strCertificate;
	// 认证信息，硬盘序列号
	std::wstring m_strHardDiskSN;

	// 平台返回的用户列表
	std::vector<USERINFO> m_vtNetUserInfo;
	// 当前登录信息
	std::wstring m_strOperatorName;
	std::wstring m_strOperatorPassword;
	std::wstring m_strDriverName;
	std::wstring m_strDriverPassword;

	// 当前检测流水号，对应联网OutlookID
	std::wstring m_strOutlookID;
	// 记录是否已经开始检测
	MapData m_mapTesting;
};

