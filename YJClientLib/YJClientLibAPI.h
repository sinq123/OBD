/*
* Copyright (c) 2009,佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：YJClientLib.h
* 文件标识：
* 摘要：云景联网接口封装
*
* 版本：1.0.0
* 作者：Sjming
* 日期：2019-06-29
*
* 历史信息：
*
*
* 版本：1.0.0
* 日期：2019-06-29
* 作者：Sjming
* 描述：开发ing
*/
#pragma once

#include <queue>
#include <memory>
#include <string>
#include <map>
#include <windows.h>
#include "YJSocket.h"
#include "json\json.h"

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
typedef std::map<CString, CString>	JsonMap;
typedef std::vector<JsonMap>		JsonMapVec;

class AFX_EXT_CLASS CYJClientLib
{
public:
	static CYJClientLib& GetInstance(void);

private:
	CYJClientLib(void);
	~CYJClientLib(void);
	CYJClientLib(const CYJClientLib &other);
	CYJClientLib& operator=(const CYJClientLib &other);
	friend class std::auto_ptr<CYJClientLib>;

public:
	inline bool IsUsed(void) {	return m_bIsUsed;	}
	// 设置IP地址
	inline void SetIP(LPCTSTR szIP) { m_strIP = szIP; }
	// 设置端口
	inline void SetPort(int nPort)  { m_nPort = nPort; }
	// 设置检测线编号
	inline LPCTSTR GetLineNumber(void) {	return m_strLineNumber;	}
	inline void SetLineNumber(LPCTSTR szLineNumber) {	m_strLineNumber = szLineNumber;	}
	// 设置检测线编号
	inline LPCTSTR GetStationNumber(void) {	return m_strStationNumber;	}
	inline void SetStationNumber(LPCTSTR szStationNumber) {	m_strStationNumber = szStationNumber;	}
	// 设置过程数
	inline void SetPocessCount(int nPocessCount) {	m_nPocessCount = nPocessCount;	}
	// 设置流水号
	inline LPCTSTR GetBusinessID(void) {	return m_strBusinessID;	}
	inline void SetBusinessID(LPCTSTR szBusinessID) {	m_strBusinessID = szBusinessID;	}
	// 设置测功机寄生功率参数,有剩磁
	inline void SetPLHPParams(float fAp, float fBp, float fCp) { m_fAp=fAp;m_fBp=fBp;m_fCp=fCp;	}
	// 计算寄生功率
	inline float CalculatePLHP(const float fV)
	{
		const double dVelocity = fV;
		const double dPLHP = m_fAp*dVelocity/80.5 + m_fBp*dVelocity*dVelocity/80.5/80.5 + m_fCp*dVelocity*dVelocity*dVelocity/80.5/80.5/80.5;
		// 格式化为2位小数,并四舍五入
		const float fPLHP = (int)(dPLHP * 100.0 + 0.5) / 100.0f;
		return fPLHP;
	}
	// 关闭连接
	void CloseConnect(void);
	
public:
	//// 同步OBD时间
	//bool OBDGetTime(std::wstring &strResult, std::wstring &strMsg);
	//// OBD检测登录
	//bool OBDVerify(std::wstring &strResult, std::wstring &strMsg);
	//// OBD待检列表
	//bool GetOBDVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strResult, std::wstring &strMsg);
	//// OBD待检车辆信息
	//bool GetOBDVehicleInfo(LPCTSTR szVIN, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);

	// 排放检测认证\连接注册
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);
	// 同步时间
	bool GetTime(std::wstring &strResult, std::wstring &strMsg);
	// 获取联网用户列表
	bool GetUsers(std::wstring &strResult, std::wstring &strMsg);
	// 清空过程列表
	void ClearProcessData(void);
	//// 检测登录
	//bool EmiVerify(std::wstring &strResult, std::wstring &strMsg);
	//// 用户退出
	//bool UserExit(LPCTSTR szOperator, std::wstring &strResult, std::wstring &strMsg);
	//// 获取环境限制
	//bool GetEvnSet(std::wstring &strResult, std::wstring &strMsg);
	// 待检列表
	bool GetVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strFilterData, std::wstring &strResult, std::wstring &strMsg);
	//// 待检车辆信息
	//bool GetVehicleInfo(LPCTSTR szOutlookID, TESTLOG &sTestLog, std::wstring &strResult, std::wstring &strMsg);
	// 获取限值
	bool GetLimit(LIMITOFASM &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFVMAS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFDIS &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFFSUNHT &sLimit, std::wstring &strResult, std::wstring &strMsg);
	bool GetLimit(LIMITOFLUGDOWN &sLimit, std::wstring &strResult, std::wstring &strMsg);
	

// 定义检测信号信息
enum TestSignalInfo
{
	START = 0,		// 开始信号	0
	INTUBE,			// 插管信号	1
	COLSTART,		// 采样开始信号	2
	COLEND,			// 采样结束信号	3
	END,			// 结束信号	4
};
	// 上报检测信号
	bool UploadTestSignal(TestSignalInfo emInfo, std::wstring &strResult, std::wstring &strMsg);
	// 上传OBD检测结果
	bool UploadOBDResult(const LineInfo &sLineInfo,const TestLog &sTestLog, const SResultOfOBD &sResultData);
	// 上传OBD过程数据
	bool UploadOBDProcessData(SOBDRTData &sRTData);
	// 上传ASM过程数据
	bool UploadASMProcessData(const RealTimeDataOfASM &sRTData);
	// 上传ASM检测数据
	bool UploadASMResult(const TestLog &sTestLog, SResultOfASM &sResultData);
	// 上传VMAS过程数据
	bool UploadVMASProcessData(const RealTimeDataOfVMAS &sRTData);
	// 上传VMAS检测数据
	bool UploadVMASResult(const TestLog &sTestLog, const SResultOfVMAS &sResultData);
	// 上传DIS过程数据
	bool UploadDISProcessData(const RealTimeDataOfDIS &sRTData);
	// 上传DIS检测数据
	bool UploadDISResult(const TestLog &sTestLog, const SResultOfDIS &sResultData);
	// 上传自由加速过程数据
	bool UploadFSUNHTProcessData(const RealTimeDataOfFSUNHT &sRTData);
	// 上传自由加速检测数据
	bool UploadFSUNHTResult(const TestLog &sTestLog, const SResultOfFSUNHT &sResultData);
	// 上传加载减速过程数据
	bool UploadLUGDOWNProcessData(const RealTimeDataOfLUGDOWN &sRTData);
	// 上传加载减速检测数据
	bool UploadLUGDOWNResult(const TestLog &sTestLog, const SResultOfLUGDOWN &sResultData);

	// 上传检测线设备状态信息
	bool UploadDeviceStatus(std::wstring strDStatus, std::wstring &strResult, std::wstring &strMsg);
	// 上传检测线设备软件版本更新信息
	bool UploadDeviceSoftwareVerson(std::wstring &strSoftWareVersion, std::wstring &strResult, std::wstring &strMsg);


	// 上传自由加速设备自检信息
	bool UploadFSUSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// 上传加载减速设备自检信息
	bool UploadLUGDOWNSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// 上传稳态设备自检信息
	bool UploadASMSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// 上传双怠速设备自检信息
	bool UploadDISSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);
	// 上传简易瞬态设备自检信息
	bool UploadVMASSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg);


	// 上报汽油线标定信息
	bool UploadGasLineCalInfo(DEMARCATIONRESULTOFANAGASCAL &sResultOfAnaGasCal, std::wstring &strResult, std::wstring &strMsg);
	// 上报柴油线标定信息
	bool UploadDiesLineCalInfo(DEMARCATIONRESULTOFANAGASCAL &sResultOfAnaGasCal, std::wstring &strResult, std::wstring &strMsg);

	// 上报汽油车底盘测功机滑行检查记录表
	bool UploadGasDynCoastdown(SGasDynCoastdownData &sCoastdownData, std::wstring &strResult, std::wstring &strMsg);
	// 上报汽油车底盘测功机附加损失测试记录表
	bool UploadGasDynParasiticLose(DEMARCATIONRESULTOFDYNPLHP &sResultOfDynPLHP, std::wstring &strResult, std::wstring &strMsg);
	// 上报柴油车底盘测功机滑行检查记录表
	bool UploadDiesDynCoastdown(SDiesDynCoastdownData &sCoastdownData, std::wstring &strResult, std::wstring &strMsg);
	// 上报柴油车底盘测功机附加功率损失测试记录表
	bool UploadDiesDynParasiticLose(DEMARCATIONRESULTOFDYNPLHP &sResultOfDynPLHP, std::wstring &strResult, std::wstring &strMsg);

	// 上报单点检查(低标气)记录表
	bool UploadAnaLowStdGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg);
	// 上报单点检查(零气)记录表
	bool UploadAnaZeroGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg);
	// 上报单点检查(高标气)记录表
	bool UploadAnaHighStdGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg);
	// 上报分析仪五点检查记录表
	bool UploadAnaFullCalChk(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, int nCheckType, std::wstring &strResult, std::wstring &strMsg);
	// 上报泄漏检查记录表
	bool UploadAnaLeakTest(std::wstring &strStratTime, std::wstring &strPass, std::wstring &strResult, std::wstring &strMsg);

	// 上报烟度计检查记录表
	bool UploadOpaCalChk(DEMARCATIONRESULTOFOPACALCHK &sResultOfOpaCalChk, std::wstring &strResult, std::wstring &strMsg);

	// 上报设备检查过程记录表
	bool UploadDeviceChkProcess(std::wstring &strJsonData, std::wstring &strResult, std::wstring &strMsg);
	// 上报设备维修保养记录表
	bool UploadDeviceMaintenance(std::wstring &strJsonData, std::wstring &strResult, std::wstring &strMsg);

private:
	// 生成唯一编码
	CString GenerateInsNum(void);
	// 发送接收数据包
	bool SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv);
	// 获取限值
	//bool GetLimit(LPCTSTR szOutlookID, MapData &mapLimit, std::wstring &strResult, std::wstring &strMsg);

	// 校验码
	// 计算前n位的校验和
	// 相加，然后取最低字节位
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	CString CheckSum(const char *ch, const UINT un);

	// Json字符串转换格式
	bool Json2Map(LPCTSTR szJson, JsonMap &mapRet);
	CString Map2Json(JsonMap mapPost);
	CString Json2Cstring(Json::Value &value);

private:

	// 发送接收数据类
	CYJSocket m_yjSocket;

	// 联网是否启用
	bool m_bIsUsed;
	// IP地址
	CString m_strIP;
	// 端口
	int m_nPort;

	// 检测站编码
	CString m_strStationNumber;
	// 检测线号
	CString m_strLineNumber;
	// 联网流水号
	CString m_strBusinessID;

public:
	// 平台返回的用户列表
	std::vector<USERINFO> m_vtNetUserInfo;

	// 号牌颜色
	std::map<std::wstring, std::wstring> m_mapPlateColor;
	// 燃油种类
	std::map<std::wstring, std::wstring> m_mapFuelType;
	// 进气方式
	std::map<std::wstring, std::wstring> m_mapAirIntakeMode;
	// 燃料供给系统型式
	std::map<std::wstring, std::wstring> m_mapOilSupplyMode;
	// 检测类型
	std::map<std::wstring, std::wstring> m_mapTestType;
	std::map<std::wstring, std::wstring> m_mapTestTypeToCode;
	
	// 上传过程数据序列计数
	int m_nPocessCount;

private:
	// 测功机寄生功率参数
	float m_fAp;
	float m_fBp;
	float m_fCp;

private:
	// 异步上传检测过程数线程
	static DWORD WINAPI UploadThreadFunc(LPVOID pParam);
	HANDLE m_RunMutex;

public:
	// 过程数据压进队列
	void PushProcessData(const RealTimeDataOfVMAS &sRealTimeDataOfVMAS);
	void PushProcessData(const RealTimeDataOfASM &sRealTimeDataOfASM);
	void PushProcessData(const RealTimeDataOfLUGDOWN &sRealTimeDataOfLUGDOWN);
	void PushProcessData(const RealTimeDataOfDIS &sRealTimeDataOfDIS);
	void PushProcessData(const RealTimeDataOfFSUNHT &sRealTimeDataOfFSUNHT);

	std::queue<RealTimeDataOfVMAS> m_RealTimeDataOfVMASQueue;
	std::queue<RealTimeDataOfLUGDOWN> m_RealTimeDataOfLUGDOWNQueue;
	std::queue<RealTimeDataOfDIS> m_RealTimeDataOfDISQueue;
	std::queue<RealTimeDataOfASM> m_RealTimeDataOfASMQueue;
	std::queue<RealTimeDataOfFSUNHT> m_RealTimeDataOfFSUNHTQueue;

	CRITICAL_SECTION m_CriticalSection;
};

