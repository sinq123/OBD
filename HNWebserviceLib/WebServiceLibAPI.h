#pragma once

#include <queue>
#include <memory>
#include <list>
using std::list;
#include"atlstr.h"
#include "IocpModeSvr.h"
#include "json\json.h"
#include "SocketNetLibAPI.h"

#include <Windows.h>

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT
//#include "../NHDef/NHDef.h"
//#include "../NHDef/NHStructDef.h"

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

//#include "..\NHNetWork\NHNet.h"
//#ifdef _DEBUG
//#pragma comment(lib, "..\\Debug\\NHNetWork_D.lib")
//#else
//#pragma comment(lib, "..\\Release\\NHNetWork.lib")
//#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

class AFX_EXT_CLASS WebServiceLibAPI
{
public:
	static WebServiceLibAPI& GetInstance(void);

private:
	WebServiceLibAPI(void);
	~WebServiceLibAPI(void);
	WebServiceLibAPI(const WebServiceLibAPI &other);
	WebServiceLibAPI& operator=(const WebServiceLibAPI &other);
	friend class std::auto_ptr<WebServiceLibAPI>;
	
	UINT m_unTimerID;
	static void CALLBACK TimePro(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
public:
	//开始检测并录像
	bool StartDetAndVideo();
	//结束检测，停止录像
	bool EndDetAndVideo(int nTestType);
	//上传过程数据
	bool UploadProcessData(int nTestType);

	//上传OBD数据
	bool UploadOBDTestData();
	//上传燃油蒸发数据
	bool UploadFuelEvaporationData(bool bAirInlet, bool bAirOutlet, bool bPass);
	// 自检开始并录像 BeginSelfTestRecord
	bool BeginSelfTestRecord(CString strTestType);
	////上传自检结果（要求自检录像的地区暂时保留！后续会删除！无要求录像地区继续使用）
	//bool UploadSelfTestData(CString strTestType, bool bResult = false, CString strResultData = NULL);
	/* 自检结束并停止录像 EndSelfTestRecordAndUpload
	 * strTestType: 检查项目
	 * bResult: 检查结果
	 * strResultData: 上传结果xml信息 ,有保存临时文件的，此参数传NULL
	 * strProcessData：上传过程json信息
	 */
	bool EndSelfTestRecordAndUpload(CString strTestType, bool bResult = false, CString strResultData= NULL, CString strProcessData= NULL);
	
	bool GetIsUsedNetTest(){return m_bUsed;};
	//工况通信
	bool ESToEP(int nNumber, int nOrder);

	//设置车辆类型
	CString SetVehicleType(CString strVehicleType);

	bool IsNeedTestOBD(const VehicleInfo &sVehicleInfo);

private:
	 //WebService地址
	CString m_strURL;
	//监控IP
	CString m_strIP;
	//监控端口
	CString m_strPort;
	
	 //用户名
	CString m_strName;
	//密码
	CString m_strPassWord;
	//是否使用联网(标准版默认非联网且应为不可配置)
	bool m_bUsed;
public:
	//车型号
	//CString m_strVehicleType;
	int m_nServerPort;
	//自检是否拍照
	//bool m_bSelfCheckUsedVideo;
public:
	static void __stdcall RecvData(unsigned long sIP,SOCKET sClient,char * pData,unsigned long DataLength);
	// 服务器对象
	CIocpModeSvr m_IocpModeSvr;
	RealTimeDataOfLUGDOWN *m_RealTimeDataOfLUGDOWN;
	RealTimeDataOfDIS *m_RealTimeDataOfDIS;
    RealTimeDataOfVMAS *m_RealTimeDataOfVMAS;
	RealTimeDataOfFSUNHT *m_RealTimeDataOfFSUNHT;

	CSocketNetLibAPI m_sSocketNet;
	// 检测状态
	int m_nState; 
	// 工况类型
	int m_nOrder;
	// 工况通信发送频率
	int m_nSendTime;
	// 是否使用工况通信
	bool m_bIsUsedSendState;
	// 气体单点检查是否只上传合格数据
	bool m_bIsChkPassData;
	
//	// 启动网络主线程
//	void StartNHServer(void);
//	// 停止网络主线程
//	void StopNHServer(void);
//
//private:
//	// 网络对象
//	CNHNetServer m_NHNetServer;
//	// 返回消息的客户端IP
//	CString m_strClientIP;
//	// 数据处理回调函数
//	static void __stdcall DataSolutionCallBack(const wchar_t* pwchIP, const BYTE* pBytes, const size_t size);
};

