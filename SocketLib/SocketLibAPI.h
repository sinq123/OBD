

#pragma once

#include <queue>
#include <memory>
#include <string>
#include <map>
#include <windows.h>
#include "Socket.h"
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

class AFX_EXT_CLASS CSocketLib_API
{
public:
	static CSocketLib_API& GetInstance(void);

private:
	CSocketLib_API(void);
	~CSocketLib_API(void);
	CSocketLib_API(const CSocketLib_API &other);
	CSocketLib_API& operator=(const CSocketLib_API &other);
	friend class std::auto_ptr<CSocketLib_API>;

public:
	// 排放检测认证\连接注册
	bool Authentication(std::wstring &strResult, std::wstring &strMsg);

	// 上报设备检查过程记录表
	bool UploadDeviceChkProcess(const std::wstring& strCheckType, const std::wstring strCheckTimeStart, const std::wstring strCheckTimeEnd, 
		std::wstring &strResult, std::wstring &strMsg);

	// 上报汽油车底盘测功机滑行检查记录表
	bool UploadGasDynCoastdown(std::wstring &strResult, std::wstring &strMsg);
	// 上报汽油车底盘测功机附加损失测试记录表
	bool UploadGasDynParasiticLose(std::wstring &strResult, std::wstring &strMsg);
	// 上报柴油车底盘测功机滑行检查记录表
	bool UploadDiesDynCoastdown(std::wstring &strResult, std::wstring &strMsg);
	// 上报柴油车底盘测功机附加功率损失测试记录表
	bool UploadDiesDynParasiticLose(std::wstring &strResult, std::wstring &strMsg);

	// 上报单点检查(低标气)记录表
	bool UploadAnaLowStdGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// 上报单点检查(零气)记录表
	bool UploadAnaZeroGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// 上报单点检查(高标气)记录表
	bool UploadAnaHighStdGasCheck(std::wstring &strResult, std::wstring &strMsg);
	// 上报分析仪五点检查记录表
	bool UploadAnaFullCalChk(std::wstring &strResult, std::wstring &strMsg);
	// 上报泄漏检查记录表
	bool UploadAnaLeakTest(std::wstring &strResult, std::wstring &strMsg);

	// 上报烟度计检查记录表
	bool UploadOpaCalChk(std::wstring &strResult, std::wstring &strMsg);


	// 关闭连接
	void CloseConnect(void);
private:
	// 生成唯一编码
	CString GenerateInsNum(void);
	// 发送接收数据包
	bool SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv);
	
	// 校验码
	// 计算前n位的校验和
	// 相加，然后取最低字节位
	// (in)p：要进行校验的数据内容指针
	// (in)un：要进行校验的数据长度
	// 返回校验和
	CString CheckSum(const char *ch, const UINT un);
	
private:

	// 发送接收数据类
	CSocket m_Socket;

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
};