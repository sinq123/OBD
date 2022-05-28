
#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef JinBenTengOBD_H
#define JinBenTengOBD_H

#include <vector>
#include "OBDAdapter.h"

/*
接口返回值（int）定义：
	0：成功
	1：诊断库初始化失败
	2：诊断库加载失败
	3：串口连接失败/未连接
	4：VCI不在线
	5：调用StartTest激活失败
	6：未调用StartTest
	7：输入错误，命令不支持
	8：执行失败（动态库内部特殊标志，外部调用客户忽略）
	9：返回数据超过外部缓存容量
	10：未知错误（仅占位，默认错误状态）
	11：激活扫描中
	12：参数缺失/错误
	13：当前车辆协议不支持此功能调用
	14：VCI硬件非法，非Windows开发包产品硬件，请向供应商确认； 
	15：安全校验失败
	16：怠速控制参数为空
	17：返还怠速控制失败
	18：怠速控制失败
	19：VCI设置失败
	20：超时退出
*/

class CJinBenTengOBD : public COBDAdapter
{
public:
	CJinBenTengOBD(void);
	~CJinBenTengOBD(void);

public:
	////////////////////////////////////////////////////////////////////////////////////串口操作start
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// 参数：
	// bPort：串口号(1~255)
	// nBaudRate：波特率 (110、300、600、1200、2400、4800、9600、14400、19200、38400、56000、57600、115200、128000、256000)
	// 返回：
	// COM_OPEN_SUCCEED
	// COM_OPEN_FAIL
	DWORD Open(const BYTE bPort, const int nBaudRate = CBR_115200){ return COM_OPEN_FAIL;}
	DWORD Open(std::wstring strIP, const int nPort){ return COM_OPEN_FAIL;}
	DWORD Open(const BYTE bPort, const wchar_t* wchPath);
	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// 返回：
	// COM_CLOSE_SUCCEED
	// COM_CLOSE_FAIL_WIN32API
	// COM_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);
	
	bool IsOpen(void) const;
	////////////////////////////////////////////////////////////////////////////////////串口操作end

public:
	// 适配器是否在线
	bool IsAdapterOnline(void);
	// 初始化适配器
	bool Init(DWORD &dwProtocol, const int nFuelType);
	// 初始化的状态
	bool GetInitState(OBDInitState &enState, DWORD &dwProtocol);
	// 取故障码
	bool GetDTC(std::vector<SDTCData> &vtDTC);
	// 取故障指示灯状态
	bool GetLampState(bool &bIsOn);
	// 取系统检测结果
	bool GetSystemCheck(SOBDSystemCheckItem &sSystemCheckItem);
	// 清除故障码
	bool ClearDTC(void);
	// 取实时数据
	bool GetRTData(int &nRev, float &fV, float &fTemp);
	bool GetRTData(std::map<std::wstring, std::wstring> &mapData);
	// 获取故障里程，单位km
	bool GetDTCMileage(DWORD &dwMileage);
	// 获取冻结数据
	bool GetFreezeData(std::wstring &strData);
	// 取VIN
	bool GetVIN(std::wstring &strVIN, std::wstring &strVINHex, int *const pnCode=NULL);
	// 取OBD型式检验要求
	bool GetOBDType(std::wstring &strType);
	// 取行驶里程，单位km
	bool GetMileage(DWORD &dwMileage);
	// 取ECU信息
	bool GetECUInfo(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// 取软件版本
	bool GetSoftwareVer(std::wstring &strVer);
	// 取IUPR数据
	bool GetIUPR(std::map<std::wstring, std::wstring> &mapData, int *const pnCode=NULL);
	// 反初始化
	bool UnInit(void);
	// 取OBD总线协议
	std::wstring GetOBDProtocolName(DWORD dwProtocol);

	/*
	初始化诊断接口
	int OBDDiagnosisInit (char*);
	返回：int，参见前面的接口返回值定义
	参数：char*：诊断程序保存目录
	导出函数名：OBDDiagnosisInit
	功能：初始化 OBD 诊断，程序启动时执行一次即可
	*/
	int JBT_OBDDiagnosisInit(void);

	int JBT_CommInit(void);

	bool GetScanStartTest(std::wstring& strProgress, DWORD &dwProtocol);

	// 释放加载的资源
	// 释放加载和占用的资源，为避免内容泄露，一定要在诊断结束后调用此方法
	int JBT_OBDDiagnosisRelease(void);

private:
	bool SendAndRecvPacket(const std::wstring& strSend, std::wstring& strRecv);

	bool Xml2Map(LPCTSTR szXML, 
		std::wstring &strCmd, std::wstring &strCode, std::wstring &strMsg, 
		std::map<std::wstring, std::wstring> &mapData);
private:
	// 加载DLL库判断
	HINSTANCE m_hDll;
	// 端口
	BYTE m_bPort;
	// Dll 目录
	CString m_strDllPath;

private:
	int m_nFuelType;	// 燃油类型，0汽油；1柴油

	// 缓存数据
	int m_nRev;		// 发动机转速
	float m_fV;		// 车速
	float m_fTemp;	// 冷却液温度
	std::map<std::wstring, std::wstring> m_mapRTData;
	
	// 上次操作时刻
	clock_t m_LastOPTime;
	// 取实时数据
	int m_nGetRTDataTimeInterval;

private:
	 // 日志文件路径（主程序）
	CStringW m_strLogFilePath;
	// 本模块名
	CStringW m_strModName;
	// 生成日志文件
	void GenLogFile(void);
};

#endif