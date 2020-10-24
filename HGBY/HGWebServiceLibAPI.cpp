#include "stdafx.h"
#include "HGWebServiceLibAPI.h"


#include "gsoap/soapVESInspectStd2018SoapProxy.h"
#include "gsoap/VESInspectStd2018Soap.nsmap"

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

// 自用库
#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

// 自用库(MFC扩展DLL)
#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

HGWebServiceLibAPI::HGWebServiceLibAPI(void)
{
	LoadConfig();
}

HGWebServiceLibAPI::~HGWebServiceLibAPI(void)
{
}

HGWebServiceLibAPI& HGWebServiceLibAPI::GetInstance(void)
{
	static std::auto_ptr<HGWebServiceLibAPI> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new HGWebServiceLibAPI());
	}
	return *(auto_ptr_instance.get());	
	//static std::auto_ptr<HGWebServiceLibAPI> auto_ptr_instance;
	//if (auto_ptr_instance.get() == NULL) 
	//{
	//	auto_ptr_instance.reset(new HGWebServiceLibAPI());
	//}
	//return *(auto_ptr_instance.get());
}

void HGWebServiceLibAPI::LoadConfig()
{
	ZeroMemory(m_wchConfigFilePath, sizeof(m_wchConfigFilePath));

	CNHCommonAPI::GetFilePathEx(L"Config", L"HGWebConfig.ini", m_wchConfigFilePath);
	CSimpleIni sio1(m_wchConfigFilePath);
	CString strUseName = sio1.GetString(L"WebServer", L"UseName", L"gmya_shouli");//登录用户id
	CString strPassWord = sio1.GetString(L"WebServer", L"PassWord", L"gmya123456");//登录用户密码
	CString strURL = sio1.GetString(L"WebServer", L"URL", L"http://172.18.3.1:8080/VESLinkService.asmx");//接口地址
	CString strLineNum = sio1.GetString(L"WebServer", L"LineCode", L"");//检测线编号
	bool bIsUsed = (L"1" == sio1.GetString(L"WebServer", L"IsUse", L"1"));//是否联网
	//m_bIsBackTestPrepare = (L"1" == sio1.GetString(L"WebServer", L"NoVehIsBackTestPrepare", L"1"));// OBD检完未获取到车辆信息，是否返回设备准备界面
	m_strKey = sio1.GetString(L"WebServer", L"Key", L"");//调用密钥
	m_strChecker = sio1.GetString(L"WebServer", L"Checker", L"");//设备自检人员
	m_strStnId = sio1.GetString(L"WebServer", L"StnId", L"");//检测站ID
	m_QZR = sio1.GetString(L"WebServer", L"QZR", L"");//签字人
	m_PZR = sio1.GetString(L"WebServer", L"PZR", L"");//批准人
	m_FIRSTAUTHDATE = sio1.GetString(L"WebServer", L"StartUseDate", L"");//启用日期（yyyy-MM-dd）
	m_TESTEXPIREDATE = sio1.GetString(L"WebServer", L"DeadLineDate", L"");//检测线设备检定有效期至（yyyy-MM-dd）
	m_DADATE = sio1.GetString(L"WebServer", L"DynDeadDate", L"");//底盘测功机检定有效期至（yyyy-MM-dd）
	m_AADATE = sio1.GetString(L"WebServer", L"AnaDeadDate", L"");//气体分析仪检定有效期至（yyyy-MM-dd）
	m_FADATE = sio1.GetString(L"WebServer", L"FloDeadDate", L"");//流量计检定有效期至（yyyy-MM-dd）
	m_SADATE = sio1.GetString(L"WebServer", L"OpaDeadDate", L"");//烟度计检定有效期至（yyyy-MM-dd）
	m_TADATE = sio1.GetString(L"WebServer", L"TacDeadDate", L"");//转速计检定有效期至（yyyy-MM-dd）
	m_OADATE = sio1.GetString(L"WebServer", L"OilTempDeadDate", L"");//油温传感器检定有效期至（yyyy-MM-dd）
	m_WSADATE = sio1.GetString(L"WebServer", L"EpDeadDate", L"");//气象站检定有效期至	（yyyy-MM-dd）

	SetUrl(strURL);	 
	m_strUserID = strUseName;
	m_strUserPassWord = strPassWord;
	m_strLineNum = strLineNum;
	m_bIsUsed = bIsUsed;
}

CString HGWebServiceLibAPI::GetLineNumber()
{
	return m_strLineNum;
}

CString HGWebServiceLibAPI::DecodeURI(LPCSTR szURIString)
{
	std::string strSource(szURIString);
	int nSrcLen = strSource.size();

	std::string strDestA;

	int nIndex(0);
	while (nIndex < nSrcLen)
	{
		if (strSource.at(nIndex) == '%'
			&& nIndex+2 <= nSrcLen)
		{
			std::string strValue = strSource.substr(nIndex, 3);
			int nValue(0);
			sscanf(strValue.c_str(), "%%%x", &nValue);
			strDestA += (char)nValue;
			nIndex += 3;
		}
		else if (strSource.at(nIndex) == '+')
		{
			strDestA += ' ';
			nIndex++;
		}
		else
		{
			strDestA += (char)strSource.at(nIndex);
			nIndex++;
		}
	}
	CString strDest(UTF8ToTChar(strDestA.c_str()));
	return strDest;
}

// 设置WebService地址
void HGWebServiceLibAPI::SetUrl(CString strUrl)
{	
	char *pch(NULL);
	pch = CNHCommonAPI::UnicodeToANSI(strUrl);
	m_strUrl = pch;
	if(NULL != pch)
	{
		free(pch);
		pch = NULL;
	}
}

// 数据交换公共接口
HGMsg HGWebServiceLibAPI::RSInfo(CString szFunctionID, CString szParams)
{
	HGMsg hgMsg;
	hgMsg.code = L"-1";
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	_ns1__RSInfo send;
	_ns1__RSInfoResponse res;
	std::wstring strFunctionID(szFunctionID);
	std::wstring strParams(szParams);
	std::wstring strKey(m_strKey);

	szParams.Format(L"接口:%s 信息:%s", szFunctionID,szParams);
	CNHLogAPI::WriteLog(L"HGWebService联网", L"RSInfo发送", szParams);

	send.szFunctionID = &strFunctionID;
	send.szParams = &strParams;
	send.szKeys = &strKey;

	if(SOAP_OK == webService.RSInfo(&send,&res))
	{
		std::wstring strRet = res.RSInfoResult->c_str();
		if (strRet.find(L"%") != -1)
		{
			strRet = DecodeURI(KUTF16ToANSI(strRet.c_str()));
		}
		CString strInfo;
		strInfo.Format(L"接口:%s 信息:%s", szFunctionID,strRet.c_str());
		//if (szFunctionID.Compare(L"GetJobList") == 0)
		//{
		//	CNHLogAPI::WriteLog(L"HGWebService联网", L"RSInfo返回", L"返回信息太长，不显示");
		//}
		//else
		//{
			CNHLogAPI::WriteLog(L"HGWebService联网", L"RSInfo返回", strInfo);
		//}
		CXmlReader xmlReader;
		if (xmlReader.Parse(strRet.c_str()))
		{
			//错误码
			std::wstring strCode(L"");
			//错误提示信息
			std::wstring strMessage(L"");
			if (xmlReader.OpenNode(L"response/head/code"))
			{
				xmlReader.GetNodeContent(strCode);
				hgMsg.code = strCode;
			}
			
			CString strBody = strRet.c_str();
			//strBody = strBody.MakeLower();
			int nBegin = strBody.Find(L"<body>");
			int nEnd = strBody.Find(L"</body>");
			hgMsg.body = strBody.Mid(nBegin+6, nEnd-nBegin-6);


			if (_wtoi(strCode.c_str()) >= 0)
			{
				hgMsg.message =L"成功";
			}
			else
			{
				if (xmlReader.OpenNode(L"response/head/message"))
				{
					xmlReader.GetNodeContent(strMessage);
					xmlReader.GetNodeContent(hgMsg.message);
					m_strErrMessage = hgMsg.message.c_str();
				}
				else
				{
					hgMsg.message =L"返回信息解析时缺少节点message";
				}
			}
		}
		else
		{
			hgMsg.message =L"解析返回值失败";
		}
	}
	else
	{
		hgMsg.message =L"接口访问失败";
	}
	return hgMsg;
}

//Web Service登录接口
bool HGWebServiceLibAPI::Login()
{
	if(m_bIsUsed)
	{
		return Login(m_strUserID, m_strUserPassWord);
	}
	return true;
}
bool HGWebServiceLibAPI::Login(CString userName, CString password)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szUser>%s</szUser>", userName);
	strParam.AppendFormat(L"<szPassword>%s</szPassword>", password);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"Login", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{		
		CXmlReader xmlReader;
		xmlReader.Parse(hgMsg.body.c_str());
		if (xmlReader.OpenNode(L"Token/TokenInfo"))
		{
			JMInspector SJMInspector;
			std::wstring wstr;	
			//用户令牌
			xmlReader.GetNodeAttribute(L"TokenGuid", wstr);
			SJMInspector.szTokenGuid = SysAllocString(wstr.c_str());
			m_strTokenGuid = SysAllocString(wstr.c_str());

			//用户ID
			xmlReader.GetNodeAttribute(L"UserID", wstr);
			SJMInspector.szID = SysAllocString(wstr.c_str());

			//用户组ID
			xmlReader.GetNodeAttribute(L"GroupID", wstr);
			SJMInspector.szGroupID = SysAllocString(wstr.c_str());

			//维修厂ID
			xmlReader.GetNodeAttribute(L"MaiID", wstr);
			SJMInspector.szMaiID = SysAllocString(wstr.c_str());

			//是否具备登录员资格（True：是；False：否）
			xmlReader.GetNodeAttribute(L"bUserLogin", wstr);
			wstr = (L"True" == wstr.c_str())?L"1":L"0";
			SJMInspector.szIsUserLogin = SysAllocString(wstr.c_str());

			//是否具备打印员资格（True：是；False：否）
			xmlReader.GetNodeAttribute(L"bUserPrint", wstr);
			wstr = (L"True" == wstr.c_str())?L"1":L"0";
			SJMInspector.szIsUserPrint = SysAllocString(wstr.c_str());

			//是否具备检测员资格（True：是；False：否）
			xmlReader.GetNodeAttribute(L"bUserInspect", wstr);
			wstr = (L"True" == wstr.c_str())?L"1":L"0";
			SJMInspector.szIsInspector = SysAllocString(wstr.c_str());

			//是否具备引车员资格（True：是；False：否）
			xmlReader.GetNodeAttribute(L"bUserDriver", wstr);
			wstr = (L"True" == wstr.c_str())?L"1":L"0";
			SJMInspector.szIsDriver = SysAllocString(wstr.c_str());

			//是否具备设备管理员资格（True：是；False：否）
			xmlReader.GetNodeAttribute(L"bUserDeviceManager", wstr);
			wstr = (L"True" == wstr.c_str())?L"1":L"0";
			SJMInspector.szIsComperator = SysAllocString(wstr.c_str());

			bRet = true;
		}
		//CString strBody = hgMsg.body.c_str();
		//int nBegin = strBody.Find(L"TokenGuid=\"");
		//int nEnd = strBody.Find(L"\" UserID");
		//m_strTokenGuid = strBody.Mid(nBegin+11, nEnd-nBegin-11);	
	}
	else
	{
		bRet = false;
	}
	
	return bRet;
}

//Web Service登出接口
bool HGWebServiceLibAPI::Logout()
{
	if(m_bIsUsed)
	{
		return Logout(m_strTokenGuid);
	}
	return true;
}
bool HGWebServiceLibAPI::Logout(CString szToken)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"Logout", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//修改登录密码
bool HGWebServiceLibAPI::ChangePassword (CString szUser, CString szNewPass, CString szOldPass)
{
	if(m_bIsUsed)
	{
		return ChangePassword (m_strTokenGuid, szUser, szNewPass, szOldPass);
	}
	return true;
}
bool HGWebServiceLibAPI::ChangePassword (CString szToken, CString szUser, CString szNewPass, CString szOldPass)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szUser>%s</szUser>", szUser);
	strParam.AppendFormat(L"<szNewPass>%s</szNewPass>", szNewPass);
	strParam.AppendFormat(L"<szOldPass>%s</szOldPass>", szOldPass);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"ChangePassword", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
// 时间同步
void HGWebServiceLibAPI::GetSynDateTime()
{
	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", m_strTokenGuid);
	strParam.AppendFormat(L"<szDevSN>%s</szDevSN>", m_strLineNum);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"GetSynDateTime", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		CString strBody = hgMsg.body.c_str();
		//strBody = strBody.MakeLower();
		int nBegin = strBody.Find(L"<CurrentTime>");
		int nEnd = strBody.Find(L"</CurrentTime>");
		CString strNow = strBody.Mid(nBegin + 13, nEnd - nBegin - 13);

		// 同步时间
		HANDLE token;
		//提升权限
		if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&token))
		{
			CNHLogAPI::WriteLog(L"HGWebService联网", L"GetSynDateTime", L"打开进程令牌失败");
			return;
		}
		TOKEN_PRIVILEGES tkp;
		tkp.PrivilegeCount = 1;
		::LookupPrivilegeValue(NULL,SE_SYSTEMTIME_NAME,&tkp.Privileges[0].Luid);
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if(!AdjustTokenPrivileges(token,FALSE,&tkp,sizeof(tkp),NULL,NULL))
		{
			CNHLogAPI::WriteLog(L"HGWebService联网", L"GetSynDateTime", L"调整令牌权限失败");
			return;
		}
		CloseHandle(token);

		// 同步时间
		COleDateTime odtNow;
		if (odtNow.ParseDateTime(strNow))
		{
			SYSTEMTIME st;
			odtNow.GetAsSystemTime(st);
			// 本机
			if (SetLocalTime(&st))
			{
				CNHLogAPI::WriteLog(L"HGWebService联网", L"GetSynDateTime", L"同步本地时间成功");
			}
			else
			{
				CNHLogAPI::WriteLog(L"HGWebService联网", L"GetSynDateTime", L"同步本地时间失败");
			}
		}
	}
	else
	{
		CNHLogAPI::WriteLog(L"HGWebService联网", L"GetSynDateTime", L"获取同步时间失败");
	}
}

//获取检测工作人员列表接口：可以获取检测工作人员列表，内容包括工作人员编号和姓名。
bool HGWebServiceLibAPI::GetInspectorList(vector<JMINSPECTOR>& vecInspector)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return GetInspectorList(m_strTokenGuid, m_strLineNum, vecInspector);
	}
	return true;
}
bool HGWebServiceLibAPI::GetInspectorList(CString szToken, CString szDeviceSN, vector<JMINSPECTOR>& vecInspector)
{
	bool bRet = false;

	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDevSN>%s</szDevSN>", szDeviceSN);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"GetInspectorList", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		//检测相关信息
		CXmlReader xmlReader;
		xmlReader.Parse(hgMsg.body.c_str());
		if (xmlReader.OpenNode(L"ds/INSPECTOR"))
		{
			do
			{
				JMInspector SJMInspector;
				//工作人员编号
				std::wstring strID(L"");
				xmlReader.GetNodeAttribute(L"USR_ID", strID);
				SJMInspector.szID = SysAllocString(strID.c_str());
				//姓名
				std::wstring strName(L"");
				xmlReader.GetNodeAttribute(L"USR_NAME", strName);
				SJMInspector.szName = SysAllocString(strName.c_str());
				//是否具备检测员资格（1：是；0：否）
				std::wstring strIsInspector(L"");
				xmlReader.GetNodeAttribute(L"USR_INSPECT", strIsInspector);
				SJMInspector.szIsInspector = SysAllocString(strIsInspector.c_str());
				//是否具备引车员资格（1：是；0：否）
				std::wstring strIsDriver(L"");
				xmlReader.GetNodeAttribute(L"USR_DRIVER", strIsDriver);
				SJMInspector.szIsDriver = SysAllocString(strIsDriver.c_str());
				//是否具备计算机操作员资格（1：是；0：否）
				std::wstring szIsComperator(L"");
				xmlReader.GetNodeAttribute(L"USR_CMPOPERATOR", szIsComperator);
				SJMInspector.szIsComperator = SysAllocString(szIsComperator.c_str());
				vecInspector.push_back(SJMInspector);
			}
			while(xmlReader.NextSibling());
		}
		bRet = true;
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

//获取待检测车辆列表接口:bIncrement,返回增量列表或者完整列表
bool HGWebServiceLibAPI::GetJobList(void)
{
	if(m_bIsUsed)
	{
		vector<JOB> vecJob;
		return GetJobList(m_strTokenGuid, m_strLineNum, 0, 0, vecJob);
	}
	return true;
}
bool HGWebServiceLibAPI::GetJobList(int nInspectType, int nIncrement, vector<JOB>& vecJob)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return GetJobList(m_strTokenGuid, m_strLineNum, nInspectType, nIncrement, vecJob);
	}
	return true;
}
bool HGWebServiceLibAPI::GetJobList(CString szToken, CString szDeviceSN, int nInspectType, int nIncrement, vector<JOB>& vecJob)
{
	bool bRet = false;

	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<nInspectType>%d</nInspectType>", nInspectType);
	strParam.AppendFormat(L"<nIncrement>%d</nIncrement>", nIncrement);
	strParam.Append(L"</Params>");
	
	HGMsg hgMsg = RSInfo(L"GetJobList", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		//检测相关信息
		CXmlReader xmlReader;
		xmlReader.Parse(hgMsg.body.c_str());
		CString strTemp;
		CString strSql;
		std::wstring wstrTemp;
		bool bIsRecord(false);
		bool bIsUpdataRecord(false);
		
		//检测相关信息
		if (xmlReader.OpenNode(L"VEHICLELST/VEHICLE"))
		{
			do
			{
				JOB SJob;
				TESTLOG sTestLog;
				
				//检测流水号
				xmlReader.GetNodeAttribute(L"DETECT_ID", wstrTemp);
				SJob.lRunningNumber = _wtoi64(wstrTemp.c_str());
				wcscpy_s(sTestLog.wchRunningNumber, wstrTemp.c_str());
				wstrTemp.clear();
				strSql.Format(L"select * from TestLog where RunningNumber = '%s'", sTestLog.wchRunningNumber);
				if(0x01 == GetDboTestLog(strSql, &sTestLog))// 获取本地信息
				{
					bIsRecord = true;
				}
				else
				{
					//无对应记录
					bIsRecord = false;
				}
				//检测状态:只获取状态为0的
				xmlReader.GetNodeAttribute(L"DET_STATUS", wstrTemp);
				if(!bIsRecord && (0 == _wtoi(wstrTemp.c_str()) || 1 == _wtoi(wstrTemp.c_str())))//等待检测/正在检测
				{
					SJob.nState = _wtoi(wstrTemp.c_str());
					wcscpy_s(sTestLog.wchTestState, L"0");
					bIsUpdataRecord = true;
				}
				else if(bIsRecord && wcscmp(L"0", sTestLog.wchTestState) && _wtoi(wstrTemp.c_str()) == 3)
				{
					// 此情况说明平台已退办
					SJob.nState = _wtoi(wstrTemp.c_str());
					wcscpy_s(sTestLog.wchTestState, L"6");
					bIsUpdataRecord = true;
				}
				else
				{
					bIsUpdataRecord = false;
				}
				wstrTemp.clear();
				//号牌号码
				xmlReader.GetNodeAttribute(L"AUT_PLATE", wstrTemp);
				SJob.szPlateNumber = SysAllocString(wstrTemp.c_str());
				wcscpy_s(sTestLog.wchPlateNumber, wstrTemp.c_str());
				wstrTemp.clear();
				//号牌种类
				xmlReader.GetNodeAttribute(L"AUT_PCLASS", wstrTemp);
				SJob.szPlateClass = SysAllocString(wstrTemp.c_str());
				wcscpy_s(sTestLog.wchPlateType, wstrTemp.c_str());
				wstrTemp.clear();
				//检测时间
				xmlReader.GetNodeAttribute(L"DET_DATE", wstrTemp);
				SJob.szTestDate = SysAllocString(wstrTemp.c_str());
				wcscpy_s(sTestLog.wchTestDate, wstrTemp.c_str());
				wcscpy_s(sTestLog.wchBillDate, wstrTemp.c_str());
				//检测项目ID：ASM  稳态工况法；LUGD 加载减速法；TSIC 双怠速法；FRAO自由加速-不透光烟度计法；FRAF 自由加速-滤纸烟度法；
				xmlReader.GetNodeAttribute(L"INSPECT_ID", wstrTemp);
				SJob.szMethod = SysAllocString(wstrTemp.c_str());
				strTemp.Format(L"%s", wstrTemp.c_str());
				wstrTemp.clear();
				if(L"ASM" == strTemp)
				{
					wcscpy_s(sTestLog.wchTestType, L"1");
					wcscpy_s(sTestLog.wchFuelType, L"汽油");
				}
				else if(L"LUGD" == strTemp)
				{
					wcscpy_s(sTestLog.wchTestType, L"3");
					wcscpy_s(sTestLog.wchFuelType, L"柴油");
				}
				else if(L"TSIC" == strTemp)
				{
					wcscpy_s(sTestLog.wchTestType, L"4");
					wcscpy_s(sTestLog.wchFuelType, L"汽油");
				}
				else if(L"FRAO" == strTemp)
				{
					wcscpy_s(sTestLog.wchTestType, L"5");
					wcscpy_s(sTestLog.wchFuelType, L"柴油");
				}
				else if(L"FRAF" == strTemp)
				{
					wcscpy_s(sTestLog.wchTestType, L"6");
					wcscpy_s(sTestLog.wchFuelType, L"柴油");
				}

				// 检测项目
				xmlReader.GetNodeAttribute(L"UNDO_ITEMS", wstrTemp);
				SJob.szUNDO_ITEMS = SysAllocString(wstrTemp.c_str());
				strTemp.Format(L"%s", wstrTemp.c_str());
				wstrTemp.clear();
				//if (strTemp.Find((L"V")) != -1)
				//{
				//	wcscpy_s(sTestLog.wchItemApp, L"1");
				//}
				//else
				//{
				//	wcscpy_s(sTestLog.wchItemApp, L"0");
				//}
				if (strTemp.Find((L"O")) != -1)
				{
					wcscpy_s(sTestLog.wchItemOBD, L"1");
				}
				else
				{
					wcscpy_s(sTestLog.wchItemOBD, L"0");
				}
				if (strTemp.Find((L"G")) != -1)
				{
					wcscpy_s(sTestLog.wchItemEmi, L"1");
				}
				else
				{
					wcscpy_s(sTestLog.wchItemEmi, L"0");
				}

				// 检验类型
				xmlReader.GetNodeAttribute(L"TESTCATEGORY", wstrTemp);
				switch (_wtoi(wstrTemp.c_str()))
				{
				case 1:	wstrTemp = L"定期检验"; break;
				case 2:	wstrTemp = L"注册登记检验"; break;
				case 3:	wstrTemp = L"实验比对"; break;
				case 4:	wstrTemp = L"监督性抽检"; break;
				case 5:	wstrTemp = L"变更登记和转移登记检验"; break;
				default: wstrTemp = L""; break;
				}
				SJob.szTESTCATEGORY = SysAllocString(wstrTemp.c_str());
				wcscpy_s(sTestLog.wchReserved3, wstrTemp.c_str());	
				wstrTemp.clear();
				vecJob.push_back(SJob);	
				
				if(bIsUpdataRecord)
				{					
					SetDboTestLog(sTestLog);
				}
			}
			while(xmlReader.NextSibling());
		}
		bRet = true;
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

//根据输入车辆号牌请求检测作业
bool HGWebServiceLibAPI::RequestJob(CString szPlate, CString szPClass, CString szInspectorID, CString szDriverID,PDETECTINFO pDetectInfo)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return RequestJob(m_strTokenGuid, m_strLineNum, szPlate, szPClass, szInspectorID, szDriverID, pDetectInfo);
	}
	return true;
}
bool HGWebServiceLibAPI::RequestJob(CString szToken, CString szDeviceSN, CString szPlate, CString szPClass, CString szInspectorID, CString szDriverID,PDETECTINFO pDetectInfo)
{
	bool bRet = false;

	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<szPlate>%s</szPlate>", szPlate);
	strParam.AppendFormat(L"<szPClass>%s</szPClass>", szPClass);
	strParam.AppendFormat(L"<szInspectorID>%s</szInspectorID>", szInspectorID);
	strParam.AppendFormat(L"<szDriverID>%s</szDriverID>", szDriverID);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"RequestJob", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		//检测相关信息
		CXmlReader xmlReader;
		xmlReader.Parse(hgMsg.body.c_str());
		if (!xmlReader.OpenNode(L"InspectInfo/Vehicle"))
		{
			return false;
		}
		DeviceCalTimeInfo sDeviceCalTimeInfo;
		//检测流水号
		std::wstring strDetectID(L"");
		xmlReader.GetNodeAttribute(L"DETECT_ID", strDetectID);
		pDetectInfo->lDetectID = _wtoi64(strDetectID.c_str());

		//检测项目ID：ASM  稳态工况法；LUGD 加载减速法；TSIC 双怠速法；FRAO自由加速-不透光烟度计法；FRAF 自由加速-滤纸烟度法
		std::wstring strInspectID(L"");
		xmlReader.GetNodeAttribute(L"INSPECT_ID",strInspectID);
		pDetectInfo->szInspectID = SysAllocString(strInspectID.c_str());

		//号牌号码
		std::wstring strPlateNumber(L"");
		xmlReader.GetNodeAttribute(L"AUT_PLATE", strPlateNumber);
		pDetectInfo->szPlateNumber = SysAllocString(strPlateNumber.c_str());

		//号牌种类
		std::wstring strPlateClass(L"");
		xmlReader.GetNodeAttribute(L"AUT_PCLASS", strPlateClass);
		pDetectInfo->szPlateClass = SysAllocString(strPlateClass.c_str());

		//登记日期，格式：2010/1/1  注：新车无此字段
		std::wstring strRegisterDate(L"");
		xmlReader.GetNodeAttribute(L"AUT_DSERVI", strRegisterDate);
		pDetectInfo->szRegisterDate = SysAllocString(strRegisterDate.c_str());

		//车辆识别号
		std::wstring strVIN(L"");
		xmlReader.GetNodeAttribute(L"AUT_VIN", strVIN);
		pDetectInfo->szVIN = SysAllocString(strVIN.c_str());

		//车辆类别
		std::wstring strVClass(L"");
		xmlReader.GetNodeAttribute(L"AUT_VCLASS", strVClass);
		pDetectInfo->szVClass = SysAllocString(strVClass.c_str());

		//乘员数(单位：人）
		std::wstring strPassenger(L"");
		xmlReader.GetNodeAttribute(L"AUT_PASSENGER", strPassenger);
		pDetectInfo->nPassenger = _wtoi(strPassenger.c_str());

		//基准质量（单位：kg）
		std::wstring strBaseMass(L"");
		xmlReader.GetNodeAttribute(L"AUT_MASSRD", strBaseMass);
		pDetectInfo->nBaseMass = _wtoi(strBaseMass.c_str());

		//最大总质量（单位：kg）
		std::wstring strMaxMass(L"");
		xmlReader.GetNodeAttribute(L"AUT_MASSTT", strMaxMass);
		pDetectInfo->nMaxMass = _wtoi(strMaxMass.c_str());

		//燃料类别
		std::wstring strFuelType(L"");
		xmlReader.GetNodeAttribute(L"AUT_FUEL", strFuelType);
		pDetectInfo->szFuelType = SysAllocString(strFuelType.c_str());

		//供油方式:开环电喷；闭环电喷；非电喷
		std::wstring strFuelSupplyMethod(L"");
		xmlReader.GetNodeAttribute(L"AUT_FUELST", strFuelSupplyMethod);
		pDetectInfo->szFuelSupplyMethod = SysAllocString(strFuelSupplyMethod.c_str());

		//是否安装三元催化转换器:0: 没有；1：有
		std::wstring strTWC(L"");
		xmlReader.GetNodeAttribute(L"AUT_TWC", strTWC);
		pDetectInfo->nTWC = _wtoi(strTWC.c_str());

		//进气方式
		std::wstring strIntakeWay(L"");
		xmlReader.GetNodeAttribute(L"AUT_GASST", strIntakeWay);
		pDetectInfo->szIntakeWay = SysAllocString(strIntakeWay.c_str());

		//额定功率 kW
		std::wstring strRatedPower(L"");
		xmlReader.GetNodeAttribute(L"AUT_EPOWER", strRatedPower);
		pDetectInfo->fRatedPower = _wtof(strRatedPower.c_str());

		//额定转速 r/min
		std::wstring strRatedRev(L"");
		xmlReader.GetNodeAttribute(L"AUT_REV", strRatedRev);
		pDetectInfo->nRatedRev = _wtoi(strRatedRev.c_str());

		//ASM5025工况CO限值
		std::wstring strASM5025CO(L"");
		xmlReader.GetNodeAttribute(L"ASM_COS5025", strASM5025CO);
		pDetectInfo->fASM5025CO = _wtof(strASM5025CO.c_str());

		//ASM5025工况HC限值
		std::wstring strASM5025HC(L"");
		xmlReader.GetNodeAttribute(L"ASM_HCS5025", strASM5025HC);
		pDetectInfo->fASM5025HC = _wtof(strASM5025HC.c_str());

		//ASM5025工况NO限值
		std::wstring strASM5025NO(L"");
		xmlReader.GetNodeAttribute(L"ASM_NOS5025", strASM5025NO);
		pDetectInfo->fASM5025NO = _wtof(strASM5025NO.c_str());

		//ASM2540工况CO限值
		std::wstring strASM2540CO(L"");
		xmlReader.GetNodeAttribute(L"ASM_COS2540", strASM2540CO);
		pDetectInfo->fASM2540CO = _wtof(strASM2540CO.c_str());

		//ASM2540工况HC限值
		std::wstring strASM2540HC(L"");
		xmlReader.GetNodeAttribute(L"ASM_HCS2540", strASM2540HC);
		pDetectInfo->fASM2540HC = _wtof(strASM2540HC.c_str());

		//ASM2540工况NO限值
		std::wstring strASM2540NO(L"");
		xmlReader.GetNodeAttribute(L"ASM_NOS2540", strASM2540NO);
		pDetectInfo->fASM2540NO = _wtof(strASM2540NO.c_str());

		//加载减速法不透光系数限值 m-1
		std::wstring strLUGDK(L"");
		xmlReader.GetNodeAttribute(L"LUGD_KS", strLUGDK);
		pDetectInfo->fLUGDK = _wtof(strLUGDK.c_str());

		//加载减速法氮氧化合物限值
		std::wstring strLUGDNOx(L"");
		xmlReader.GetNodeAttribute(L"LUGD_NOxS", strLUGDNOx);
		pDetectInfo->fLUGDNOx = _wtof(strLUGDNOx.c_str());

		//轮边最大功率限值
		std::wstring strLUGDPowerRatio(L"");
		xmlReader.GetNodeAttribute(L"LUGD_MAXHPS", strLUGDPowerRatio);
		pDetectInfo->fLUGDPowerRatio = _wtof(strLUGDPowerRatio.c_str());

		//双怠速法高怠速工况HC限值
		std::wstring strTSICHCH(L"");
		xmlReader.GetNodeAttribute(L"TSIC_HCS_H", strTSICHCH);
		pDetectInfo->fTSICHCH = _wtof(strTSICHCH.c_str());

		//双怠速法高怠速工况CO限值
		std::wstring strTSICCOH(L"");
		xmlReader.GetNodeAttribute(L"TSIC_COS_H", strTSICCOH);
		pDetectInfo->fTSICCOH = _wtof(strTSICCOH.c_str());

		//双怠速法怠速工况HC限值
		std::wstring strTSICHC(L"");
		xmlReader.GetNodeAttribute(L"TSIC_HCS_L", strTSICHC);
		pDetectInfo->fTSICHC = _wtof(strTSICHC.c_str());

		//双怠速法怠速工况CO限值
		std::wstring strTSICCO(L"");
		xmlReader.GetNodeAttribute(L"TSIC_COS_L", strTSICCO);
		pDetectInfo->fTSICCO = _wtof(strTSICCO.c_str());

		//双怠速法过量空气系数下线
		std::wstring strTSICLambdaL(L"");
		xmlReader.GetNodeAttribute(L"TSIC_AQS_L", strTSICLambdaL);
		pDetectInfo->fTSICLambdaL = _wtof(strTSICLambdaL.c_str());

		//双怠速法过量空气系数上线
		std::wstring strTSICLambdaH(L"");
		xmlReader.GetNodeAttribute(L"TSIC_AQS_H", strTSICLambdaH);
		pDetectInfo->fTSICLambdaH = _wtof(strTSICLambdaH.c_str());

		//不透光系数限值
		std::wstring strFRAOK(L"");
		xmlReader.GetNodeAttribute(L"FRAO_KS", strFRAOK);
		pDetectInfo->fFRAOK = _wtof(strFRAOK.c_str());
		bRet = true;
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

//根据检测流水号请求检测作业
bool HGWebServiceLibAPI::RequestJobByID(__int64 lDetectID, CString szInspectorID, CString szDriverID, PDETECTINFO pDetectInfo)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return RequestJobByID(m_strTokenGuid, m_strLineNum, lDetectID, szInspectorID, szDriverID, pDetectInfo);
	}
	return true;
}
bool HGWebServiceLibAPI::RequestJobByID(CString szToken, CString szDeviceSN, __int64 lDetectID, CString szInspectorID, CString szDriverID, PDETECTINFO pDetectInfo)
{
	bool bRet = false;

	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<szInspectorID>%s</szInspectorID>", szInspectorID);
	strParam.AppendFormat(L"<szDriverID>%s</szDriverID>", szDriverID);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"RequestJobByID", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{	
		//检测相关信息
		CXmlReader xmlReader;		
		if (!xmlReader.Parse(hgMsg.body.c_str()))
		{
			return false;
		}
		if (!xmlReader.OpenNode(L"InspectInfo/Vehicle"))
		{
			return false;
		}
		DeviceCalTimeInfo sDeviceCalTimeInfo;
		//检测流水号
		std::wstring strDetectID(L"");
		xmlReader.GetNodeAttribute(L"DETECT_ID", strDetectID);
		pDetectInfo->lDetectID = _wtoi64(strDetectID.c_str());

		//检测项目ID：ASM  稳态工况法；LUGD 加载减速法；TSIC 双怠速法；FRAO自由加速-不透光烟度计法；FRAF 自由加速-滤纸烟度法
		std::wstring strInspectID(L"");
		xmlReader.GetNodeAttribute(L"INSPECT_ID",strInspectID);
		pDetectInfo->szInspectID = SysAllocString(strInspectID.c_str());

		//号牌号码
		std::wstring strPlateNumber(L"");
		xmlReader.GetNodeAttribute(L"AUT_PLATE", strPlateNumber);
		pDetectInfo->szPlateNumber = SysAllocString(strPlateNumber.c_str());

		//号牌种类
		std::wstring strPlateClass(L"");
		xmlReader.GetNodeAttribute(L"AUT_PCLASS", strPlateClass);
		pDetectInfo->szPlateClass = SysAllocString(strPlateClass.c_str());

		//登记日期，格式：2010/1/1  注：新车无此字段
		std::wstring strRegisterDate(L"");
		xmlReader.GetNodeAttribute(L"AUT_DSERVI", strRegisterDate);
		pDetectInfo->szRegisterDate = SysAllocString(strRegisterDate.c_str());

		//车辆识别号
		std::wstring strVIN(L"");
		xmlReader.GetNodeAttribute(L"AUT_VIN", strVIN);
		pDetectInfo->szVIN = SysAllocString(strVIN.c_str());

		//车辆类别
		std::wstring strVClass(L"");
		xmlReader.GetNodeAttribute(L"AUT_VCLASS", strVClass);
		pDetectInfo->szVClass = SysAllocString(strVClass.c_str());

		//乘员数(单位：人）
		std::wstring strPassenger(L"");
		xmlReader.GetNodeAttribute(L"AUT_PASSENGER", strPassenger);
		pDetectInfo->nPassenger = _wtoi(strPassenger.c_str());

		//基准质量（单位：kg）
		std::wstring strBaseMass(L"");
		xmlReader.GetNodeAttribute(L"AUT_MASSRD", strBaseMass);
		pDetectInfo->nBaseMass = _wtoi(strBaseMass.c_str());

		//最大总质量（单位：kg）
		std::wstring strMaxMass(L"");
		xmlReader.GetNodeAttribute(L"AUT_MASSTT", strMaxMass);
		pDetectInfo->nMaxMass = _wtoi(strMaxMass.c_str());

		//燃料类别
		std::wstring strFuelType(L"");
		xmlReader.GetNodeAttribute(L"AUT_FUEL", strFuelType);
		pDetectInfo->szFuelType = SysAllocString(strFuelType.c_str());

		//供油方式:开环电喷；闭环电喷；非电喷
		std::wstring strFuelSupplyMethod(L"");
		xmlReader.GetNodeAttribute(L"AUT_FUELST", strFuelSupplyMethod);
		pDetectInfo->szFuelSupplyMethod = SysAllocString(strFuelSupplyMethod.c_str());

		//是否安装三元催化转换器:0: 没有；1：有
		std::wstring strTWC(L"");
		xmlReader.GetNodeAttribute(L"AUT_TWC", strTWC);
		pDetectInfo->nTWC = _wtoi(strTWC.c_str());

		//进气方式
		std::wstring strIntakeWay(L"");
		xmlReader.GetNodeAttribute(L"AUT_GASST", strIntakeWay);
		pDetectInfo->szIntakeWay = SysAllocString(strIntakeWay.c_str());

		//额定功率 kW
		std::wstring strRatedPower(L"");
		xmlReader.GetNodeAttribute(L"AUT_EPOWER", strRatedPower);
		pDetectInfo->fRatedPower = _wtof(strRatedPower.c_str());

		//额定转速 r/min
		std::wstring strRatedRev(L"");
		xmlReader.GetNodeAttribute(L"AUT_REV", strRatedRev);
		pDetectInfo->nRatedRev = _wtoi(strRatedRev.c_str());

		//ASM5025工况CO限值
		std::wstring strASM5025CO(L"");
		xmlReader.GetNodeAttribute(L"ASM_COS5025", strASM5025CO);
		pDetectInfo->fASM5025CO = _wtof(strASM5025CO.c_str());

		//ASM5025工况HC限值
		std::wstring strASM5025HC(L"");
		xmlReader.GetNodeAttribute(L"ASM_HCS5025", strASM5025HC);
		pDetectInfo->fASM5025HC = _wtof(strASM5025HC.c_str());

		//ASM5025工况NO限值
		std::wstring strASM5025NO(L"");
		xmlReader.GetNodeAttribute(L"ASM_NOS5025", strASM5025NO);
		pDetectInfo->fASM5025NO = _wtof(strASM5025NO.c_str());

		//ASM2540工况CO限值
		std::wstring strASM2540CO(L"");
		xmlReader.GetNodeAttribute(L"ASM_COS2540", strASM2540CO);
		pDetectInfo->fASM2540CO = _wtof(strASM2540CO.c_str());

		//ASM2540工况HC限值
		std::wstring strASM2540HC(L"");
		xmlReader.GetNodeAttribute(L"ASM_HCS2540", strASM2540HC);
		pDetectInfo->fASM2540HC = _wtof(strASM2540HC.c_str());

		//ASM2540工况NO限值
		std::wstring strASM2540NO(L"");
		xmlReader.GetNodeAttribute(L"ASM_NOS2540", strASM2540NO);
		pDetectInfo->fASM2540NO = _wtof(strASM2540NO.c_str());

		//加载减速法不透光系数限值 m-1
		std::wstring strLUGDK(L"");
		xmlReader.GetNodeAttribute(L"LUGD_KS", strLUGDK);
		pDetectInfo->fLUGDK = _wtof(strLUGDK.c_str());

		//加载减速法氮氧化合物限值
		std::wstring strLUGDNOx(L"");
		xmlReader.GetNodeAttribute(L"LUGD_NOxS", strLUGDNOx);
		pDetectInfo->fLUGDNOx = _wtof(strLUGDNOx.c_str());

		//轮边最大功率限值
		std::wstring strLUGDPowerRatio(L"");
		xmlReader.GetNodeAttribute(L"LUGD_MAXHPS", strLUGDPowerRatio);
		pDetectInfo->fLUGDPowerRatio = _wtof(strLUGDPowerRatio.c_str());

		//双怠速法高怠速工况HC限值
		std::wstring strTSICHCH(L"");
		xmlReader.GetNodeAttribute(L"TSIC_HCS_H", strTSICHCH);
		pDetectInfo->fTSICHCH = _wtof(strTSICHCH.c_str());

		//双怠速法高怠速工况CO限值
		std::wstring strTSICCOH(L"");
		xmlReader.GetNodeAttribute(L"TSIC_COS_H", strTSICCOH);
		pDetectInfo->fTSICCOH = _wtof(strTSICCOH.c_str());

		//双怠速法怠速工况HC限值
		std::wstring strTSICHC(L"");
		xmlReader.GetNodeAttribute(L"TSIC_HCS_L", strTSICHC);
		pDetectInfo->fTSICHC = _wtof(strTSICHC.c_str());

		//双怠速法怠速工况CO限值
		std::wstring strTSICCO(L"");
		xmlReader.GetNodeAttribute(L"TSIC_COS_L", strTSICCO);
		pDetectInfo->fTSICCO = _wtof(strTSICCO.c_str());

		//双怠速法过量空气系数下线
		std::wstring strTSICLambdaL(L"");
		xmlReader.GetNodeAttribute(L"TSIC_AQS_L", strTSICLambdaL);
		pDetectInfo->fTSICLambdaL = _wtof(strTSICLambdaL.c_str());

		//双怠速法过量空气系数上线
		std::wstring strTSICLambdaH(L"");
		xmlReader.GetNodeAttribute(L"TSIC_AQS_H", strTSICLambdaH);
		pDetectInfo->fTSICLambdaH = _wtof(strTSICLambdaH.c_str());

		//不透光系数限值
		std::wstring strFRAOK(L"");
		xmlReader.GetNodeAttribute(L"FRAO_KS", strFRAOK);
		pDetectInfo->fFRAOK = _wtof(strFRAOK.c_str());

		//车主
		std::wstring strOwner(L"");
		xmlReader.GetNodeAttribute(L"AUT_ONAME", strOwner);
		pDetectInfo->szOwner = SysAllocString(strOwner.c_str());

		//型号
		std::wstring strModel(L"");
		xmlReader.GetNodeAttribute(L"AUT_MODEL", strModel);
		pDetectInfo->szModel = SysAllocString(strModel.c_str());

		//生产日期
		std::wstring strProductionDate(L"");
		xmlReader.GetNodeAttribute(L"AUT_DMANUF", strProductionDate);
		pDetectInfo->szProductionDate = SysAllocString(strProductionDate.c_str());

		//驱动方式
		std::wstring strDriveType(L"");
		xmlReader.GetNodeAttribute(L"AUT_PATTERN", strDriveType);
		pDetectInfo->szDriveType = SysAllocString(strDriveType.c_str());
		bRet = true;
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

/************************************************************************************************/
//通知联网监控系统检测开始。
bool HGWebServiceLibAPI::BeginEvent(__int64 lDetectID, int nEventType)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return BeginEvent(m_strTokenGuid, m_strLineNum, lDetectID ,nEventType);
	}
	return true;
}
bool HGWebServiceLibAPI::BeginEvent(CString szToken, CString szDeviceSN, __int64 lDetectID, int nEventType)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<nEventType>%d</nEventType>", nEventType);//1:外观检测,  2:OBD,  3:尾气检测，4:尾气采样开始（与检测过程数据采样时间同步）
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"BeginEvent", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//通知联网监控系统检测结束。
bool HGWebServiceLibAPI::EndEvent(__int64 lDetectID ,int nEventType)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return EndEvent(m_strTokenGuid, m_strLineNum, lDetectID, nEventType);
	}
	return true;
}
bool HGWebServiceLibAPI::EndEvent(CString szToken, CString szDeviceSN, __int64 lDetectID, int nEventType)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<nEventType>%d</nEventType>", nEventType);//1:外观检测,  2:OBD,  3:尾气检测，4:尾气采样结束（与检测过程数据采样时间同步）
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"EndEvent", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//完成当前检测作业。
bool HGWebServiceLibAPI::FinishJob(__int64 lDetectID)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		Sleep(3*1000);
		return FinishJob(m_strTokenGuid, m_strLineNum, lDetectID);
	}
	return true;
}
bool HGWebServiceLibAPI::FinishJob(CString szToken, CString szDeviceSN, __int64 lDetectID)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"FinishJob", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//取消当前检测作业。当检测出现异常情况无法完成检测时，设备必须调用此接口，释放当前检测作业。
bool HGWebServiceLibAPI::CancelJob(__int64 lDetectID)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return CancelJob(m_strTokenGuid, m_strLineNum, lDetectID);
	}
	return true;
}
bool HGWebServiceLibAPI::CancelJob(CString szToken, CString szDeviceSN, __int64 lDetectID)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"CancelJob", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}
	
	// 取消本地数据
	wchar_t wchSql[SQLMAX];
	TESTLOG sTestLog;
	wsprintf(wchSql, L"select * from TestLog where RunningNumber = '%lld'", lDetectID);
	GetDboTestLog(wchSql, &sTestLog);
	wcscpy_s(sTestLog.wchTestState, L"6");
	SetDboTestLog(sTestLog);

	return bRet;
}

//用于上报[检测设备]当前状态:-1设备故障;0设备关机;1设备预热自检;2空闲;3正在检测
//用于表示[检测作业]当前状态，即检测车辆的状态:当设备状态发生变化时必须调用此接口上报检测状态。
//0等待检测,1正在检测,2检测完成,3检测取消,4其他状态
bool HGWebServiceLibAPI::SetDeviceState(int nState)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return SetDeviceState(m_strTokenGuid, m_strLineNum, nState);
	}
	return true;
}
bool HGWebServiceLibAPI::SetDeviceState(CString szToken, CString szDeviceSN, int nState)
{
	bool bRet = false;

	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDevSN>%s</szDevSN>", szDeviceSN);
	strParam.AppendFormat(L"<nState>%d</nState>", nState);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"SetDeviceState", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

//用于触发联网监控系统拍照。拍照时间为检测员插入采样管后，检测开始前。
bool HGWebServiceLibAPI::TakePhoto(__int64 lDetectID, int nEventType)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return TakePhoto(m_strTokenGuid, m_strLineNum, lDetectID, nEventType);
	}
	return true;
}
bool HGWebServiceLibAPI::TakePhoto(CString szToken, CString szDeviceSN, __int64 lDetectID, int nEventType)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<nEventType>%d</nEventType>", nEventType);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"Snapshoot", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

/************************************************************************************************/
//ASM检测
bool HGWebServiceLibAPI::PutASMData(__int64 lDetectID, ASMDATA sASMData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutASMData(m_strTokenGuid, m_strLineNum, m_QZR, m_PZR, lDetectID, sASMData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutASMData(CString szToken, CString szDeviceSN, CString szQZR, CString szPZR, __int64 lDetectID, ASMDATA sASMData)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", sASMData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", sASMData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", sASMData.fEnvPressure);
	strParam.AppendFormat(L"<fASM5025CO>%f</fASM5025CO>", sASMData.fASM5025CO);
	strParam.AppendFormat(L"<fASM5025HC>%f</fASM5025HC>", sASMData.fASM5025HC);
	strParam.AppendFormat(L"<fASM5025NO>%f</fASM5025NO>", sASMData.fASM5025NO);
	strParam.AppendFormat(L"<fASM5025Lambda>%f</fASM5025Lambda>", sASMData.fASM5025Lambda);
	strParam.AppendFormat(L"<fASM5025PowTotal>%f</fASM5025PowTotal>", sASMData.fASM5025PowTotal);
	strParam.AppendFormat(L"<fASM5025Rev>%f</fASM5025Rev>", sASMData.fASM5025Rev);
	strParam.AppendFormat(L"<fASM5025Speed>%f</fASM5025Speed>", sASMData.fASM5025Speed);
	strParam.AppendFormat(L"<fASM2540CO>%f</fASM2540CO>", sASMData.fASM2540CO);
	strParam.AppendFormat(L"<fASM2540HC>%f</fASM2540HC>", sASMData.fASM2540HC);
	strParam.AppendFormat(L"<fASM2540NO>%f</fASM2540NO>", sASMData.fASM2540NO);
	strParam.AppendFormat(L"<fASM2540Lambda>%f</fASM2540Lambda>", sASMData.fASM2540Lambda);
	strParam.AppendFormat(L"<fASM2540PowTotal>%f</fASM2540PowTotal>", sASMData.fASM2540PowTotal);
	strParam.AppendFormat(L"<fASM2540Rev>%f</fASM2540Rev>", sASMData.fASM2540Rev);
	strParam.AppendFormat(L"<fASM2540Speed>%f</fASM2540Speed>", sASMData.fASM2540Speed);
	strParam.AppendFormat(L"<szQZR>%s</szQZR>", szQZR);
	strParam.AppendFormat(L"<szPZR>%s</szPZR>", szPZR);
	strParam.AppendFormat(L"<nFERESULT>%d</nFERESULT>", (sASMData.nFERESULT == 0) ? 2 : 1);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutASMData", strParam);
	if (((L"0" == hgMsg.code)||(L"1" == hgMsg.code))||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutASMLog(__int64 lDetectID, ASMInfo SASMInfo, vector<ASMLOG>& vecASMLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutASMLog(m_strTokenGuid, m_strLineNum, lDetectID, SASMInfo,vecASMLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutASMLog(CString szToken,CString szDeviceSN, __int64 lDetectID, ASMInfo SASMInfo, vector<ASMLOG>& vecASMLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.Append(L"<szDetectLog>");
	strParam.Append(L"<DetectLog>");
	strParam.AppendFormat(L"<DetectID>%lld</DetectID>", SASMInfo.DetectID);
	strParam.AppendFormat(L"<InspectID>%s</InspectID>", SASMInfo.InspectID.c_str());
	strParam.Append(L"<ASM>");
	strParam.Append(L"<ASM5025>");
	strParam.AppendFormat(L"<StageBegin>%d</StageBegin>", SASMInfo.ASM5025SampleBegin);
	strParam.AppendFormat(L"<StageEnd>%d</StageEnd>", SASMInfo.ASM5025StageEnd);
	strParam.AppendFormat(L"<SampleBegin>%d</SampleBegin>", SASMInfo.ASM5025SampleBegin);
	strParam.Append(L"</ASM5025>");
	strParam.Append(L"<ASM2540>");
	strParam.AppendFormat(L"<StageBegin>%d</StageBegin>", SASMInfo.ASM2540StageBegin);
	strParam.AppendFormat(L"<StageEnd>%d</StageEnd>", SASMInfo.ASM2540StageEnd);
	strParam.AppendFormat(L"<SampleBegin>%d</SampleBegin>", SASMInfo.ASM2540SampleBegin);
	strParam.Append(L"</ASM2540>");
	strParam.AppendFormat(L"<LogData Count=\"%d\">", vecASMLog.size());
	int nIndex = 0;
	for(vector<ASMLOG>::iterator ir=vecASMLog.begin(); ir!=vecASMLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		//strParam.AppendFormat(L"TimeNo=\"%d\" ", ++nIndex);
		strParam.AppendFormat(L"V=\"%f\" ", ir->fV);
		strParam.AppendFormat(L"F=\"%f\" ", ir->fForce);
		strParam.AppendFormat(L"REV=\"%d\" ", ir->nREV);
		strParam.AppendFormat(L"TP=\"%f\" ", ir->fTP);
		strParam.AppendFormat(L"PP=\"%f\" ", ir->fPP);
		strParam.AppendFormat(L"IP=\"%f\" ", ir->fIP);  
		strParam.AppendFormat(L"HC=\"%f\" ", ir->fHC);   
		strParam.AppendFormat(L"CO=\"%f\" ", ir->fCO);   
		strParam.AppendFormat(L"NO=\"%f\" ", ir->fNO);  
		strParam.AppendFormat(L"O2=\"%f\" ", ir->fO2);   
		strParam.AppendFormat(L"CO2=\"%f\" ", ir->fCO2);   
		strParam.AppendFormat(L"ET=\"%f\" ", ir->fET);   
		strParam.AppendFormat(L"EP=\"%f\" ", ir->fEP);  
		strParam.AppendFormat(L"EH=\"%f\" ", ir->fEH);  
		strParam.AppendFormat(L"DM=\"%f\" ", ir->fDM);
		strParam.AppendFormat(L"HM=\"%f\" ", ir->fHM);   
		strParam.AppendFormat(L"Time=\"%s\" ", ir->szNT);
		strParam.AppendFormat(L"Lumbda=\"%f\" ", ir->fLumbda);
		strParam.AppendFormat(L"NL=\"%d\" ", int(ir->fForce));
		strParam.AppendFormat(L"KW=\"%d\" ", int(ir->fTP));
		strParam.AppendFormat(L"KG=\"%d\" ", int(ir->fForce / 9.8f));
		strParam.AppendFormat(L"XZHCO=\"%f\" ", ir->fCO);
		strParam.AppendFormat(L"XZHHC=\"%f\" ", ir->fHC);
		strParam.AppendFormat(L"XZHNO=\"%f\" ", ir->fNO);
		strParam.AppendFormat(L"Type=\"%d\" ", (ir->Type < 3) ? 0 : (ir->Type < 7 ? 1 : 2));
		strParam.Append(L"/>");
		count++;
	}
	strParam.Append(L"</LogData>");
	strParam.Append(L"</ASM>");
	strParam.Append(L"</DetectLog>");
	strParam.Append(L"</szDetectLog>");
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutASMLog", strParam);
	if (((L"0" == hgMsg.code)||(L"1" == hgMsg.code))||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//LUGDOWN检测
bool HGWebServiceLibAPI::PutLUGDData(__int64 lDetectID, LUGDDATA SLUGDData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutLUGDData(m_strTokenGuid, m_strLineNum, m_QZR, m_PZR, lDetectID, SLUGDData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutLUGDData(CString szToken, CString szDeviceSN, CString szQZR, CString szPZR, __int64 lDetectID, LUGDDATA SLUGDData)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", SLUGDData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", SLUGDData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", SLUGDData.fEnvPressure);
	strParam.AppendFormat(L"<fK100>%f</fK100>", SLUGDData.fK100);
	strParam.AppendFormat(L"<fK80>%f</fK80>", SLUGDData.fK80);
	strParam.AppendFormat(L"<fNOx80>%f</fNOx80>", SLUGDData.fNOx80);
	strParam.AppendFormat(L"<fMaxHP>%f</fMaxHP>", SLUGDData.fMaxHP);
	strParam.AppendFormat(L"<fMaxHREV>%f</fMaxHREV>", SLUGDData.fMaxHREV);
	strParam.AppendFormat(L"<fVehMaxHP>%f</fVehMaxHP>", SLUGDData.fVehMaxHP); //平台要求传发动机最大转速
	strParam.AppendFormat(L"<szQZR>%s</szQZR>", szQZR);
	strParam.AppendFormat(L"<szPZR>%s</szPZR>", szPZR);
	strParam.AppendFormat(L"<nFERESULT>%d</nFERESULT>", (SLUGDData.nFERESULT == 0) ? 2 : 1);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutLUGDData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutLUGDLog(__int64 lDetectID, LUGDINFO SLUGDInfo, vector<LUGDLog>& vecLUGDLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutLUGDLog(m_strTokenGuid, m_strLineNum, lDetectID, SLUGDInfo, vecLUGDLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutLUGDLog(CString szToken, CString szDeviceSN, __int64 lDetectID, LUGDINFO SLUGDInfo, vector<LUGDLog>& vecLUGDLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.Append(L"<szDetectLog>");
	strParam.Append(L"<DetectLog>");
	strParam.AppendFormat(L"<DetectID>%lld</DetectID>", SLUGDInfo.DetectID);
	strParam.AppendFormat(L"<InspectID>%s</InspectID>", SLUGDInfo.InspectID);
	strParam.AppendFormat(L"<InspectTime>%d</InspectTime>", SLUGDInfo.InspectTime);
	strParam.AppendFormat(L"<WorkTime>%d</WorkTime>", SLUGDInfo.WorkTime);
	strParam.Append(L"<LUGD>");
	strParam.AppendFormat(L"<IdleRev>%d</IdleRev>", SLUGDInfo.IdleRev);
	strParam.AppendFormat(L"<MaxHP>%f</MaxHP>", SLUGDInfo.MaxHP);
	strParam.AppendFormat(L"<CorrMaxHP>%f</CorrMaxHP>", SLUGDInfo.CorrMaxHP);
	strParam.AppendFormat(L"<CalVelMaxHP>%f</CalVelMaxHP>", SLUGDInfo.CalVelMaxHP);
	strParam.AppendFormat(L"<RealVelMaxHP>%f</RealVelMaxHP>", SLUGDInfo.RealVelMaxHP);
	strParam.AppendFormat(L"<PowerScanStart>%d</PowerScanStart>", SLUGDInfo.PowerScanStart);
	strParam.AppendFormat(L"<PowerScanEnd>%d</PowerScanEnd>", SLUGDInfo.PowerScanEnd);
	strParam.AppendFormat(L"<VelMaxHP100Start>%d</VelMaxHP100Start>", SLUGDInfo.VelMaxHP100Start);
	strParam.AppendFormat(L"<VelMaxHP100End>%d</VelMaxHP100End>", SLUGDInfo.VelMaxHP100End);
	strParam.AppendFormat(L"<VelMaxHP80Start>%d</VelMaxHP80Start>", SLUGDInfo.VelMaxHP80Start);
	strParam.AppendFormat(L"<VelMaxHP80End>%d</VelMaxHP80End>", SLUGDInfo.VelMaxHP80End);
	strParam.AppendFormat(L"<LogData Count=\"%d\">", vecLUGDLog.size());
	for(vector<LUGDLog>::iterator ir=vecLUGDLog.begin(); ir!=vecLUGDLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"V=\"%f\" ", ir->fV);
		strParam.AppendFormat(L"F=\"%f\" ", ir->fF);
		strParam.AppendFormat(L"REV=\"%d\" ", ir->nREV);
		strParam.AppendFormat(L"K=\"%f\" ", ir->fK);   
		strParam.AppendFormat(L"N=\"%f\" ", ir->fN);  
		strParam.AppendFormat(L"CO2=\"%f\" ", ir->fCO2);   
		strParam.AppendFormat(L"NOx=\"%f\" ", ir->fNOx);   
		strParam.AppendFormat(L"OilTemp=\"%f\" ", ir->fOilTemp);  
		strParam.AppendFormat(L"TP=\"%f\" ", ir->fTP);
		strParam.AppendFormat(L"PP=\"%f\" ", ir->fPP);
		strParam.AppendFormat(L"IP=\"%f\" ", ir->fIP);
		strParam.AppendFormat(L"PF=\"%f\" ", ir->fPF);  
		strParam.AppendFormat(L"ET=\"%f\" ", ir->fET);  
		strParam.AppendFormat(L"EP=\"%f\" ", ir->fEP);  
		strParam.AppendFormat(L"EH=\"%f\" ", ir->fEH);
		strParam.AppendFormat(L"Time=\"%s\" ", ir->szTime);
		strParam.AppendFormat(L"NL=\"%d\" ", int(ir->fF));
		strParam.AppendFormat(L"KG=\"%d\" ", int(ir->fF / 9.8f));
		int nType = 0;
		if (ir->Type == 2)
		{
			nType = 0; //0-功率扫描中
		}
		else if (ir->Type == 3)
		{
			nType = 2; //2-100%VelMaxHP 点检验过程
		}
		else if (ir->Type == 5)
		{
			nType = 3; //3-80%VelMaxHP 点检验过程
		}
		strParam.AppendFormat(L"Type=\"%d\" ", nType);
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</LogData>");
	strParam.Append(L"</LUGD>");
	strParam.Append(L"</DetectLog>");
	strParam.Append(L"</szDetectLog>");
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutLUGDLog", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//DIS检测
bool HGWebServiceLibAPI::PutTSICData(__int64 lDetectID, TSICDATA STSICData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutTSICData(m_strTokenGuid, m_strLineNum, m_QZR, m_PZR, lDetectID, STSICData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutTSICData(CString szToken, CString szDeviceSN, CString szQZR, CString szPZR, __int64 lDetectID, TSICDATA STSICData)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", STSICData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", STSICData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", STSICData.fEnvPressure);
	strParam.AppendFormat(L"<fHCH>%f</fHCH>", STSICData.fHCH);
	strParam.AppendFormat(L"<fCOH>%f</fCOH>", STSICData.fCOH);
	strParam.AppendFormat(L"<fHC>%f</fHC>", STSICData.fHC);
	strParam.AppendFormat(L"<fCO>%f</fCO>", STSICData.fCO);
	strParam.AppendFormat(L"<fLambda>%f</fLambda>", STSICData.fLambda);
	strParam.AppendFormat(L"<szQZR>%s</szQZR>", szQZR);
	strParam.AppendFormat(L"<szPZR>%s</szPZR>", szPZR);
	strParam.AppendFormat(L"<nFERESULT>%d</nFERESULT>", (STSICData.nFERESULT == 0) ? 2 : 1);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutTSICData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutTSICLog(__int64 lDetectID, TSICInfo STSICInfo, vector<TSICLOG>& vecTSICLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutTSICLog(m_strTokenGuid, m_strLineNum, lDetectID, STSICInfo,vecTSICLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutTSICLog(CString szToken, CString szDeviceSN, __int64 lDetectID, TSICInfo STSICInfo, vector<TSICLOG>& vecTSICLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.Append(L"<szDetectLog>");
	strParam.Append(L"<DetectLog>");
	strParam.AppendFormat(L"<DetectID>%lld</DetectID>", STSICInfo.DetectID);
	strParam.AppendFormat(L"<InspectID>%s</InspectID>", STSICInfo.InspectID);
	strParam.AppendFormat(L"<InspectTime>%d</InspectTime>", STSICInfo.InspectTime);
	strParam.AppendFormat(L"<WorkTime>%d</WorkTime>", STSICInfo.WorkTime);
	strParam.AppendFormat(L"<OilTemp>%f</OilTemp>", STSICInfo.OilTemp);
	strParam.AppendFormat(L"<EngRev>%d</EngRev>", STSICInfo.EngRev);
	strParam.Append(L"<TSIC>");
	if(120 == vecTSICLog.size())
	{
		strParam.AppendFormat(L"<LogData Count=\"%d\">", vecTSICLog.size()-30);
	}
	else
	{
		strParam.AppendFormat(L"<LogData Count=\"%d\">", vecTSICLog.size());
	}
	for(vector<TSICLOG>::iterator ir=vecTSICLog.begin(); ir!=vecTSICLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"REV=\"%d\" ", ir->nREV);
		strParam.AppendFormat(L"HC=\"%f\" ", ir->fHC);
		strParam.AppendFormat(L"CO=\"%f\" ", ir->fCO);
		strParam.AppendFormat(L"CO2=\"%f\" ", ir->fCO2);  
		strParam.AppendFormat(L"O2=\"%f\" ", ir->fO2);   
		strParam.AppendFormat(L"Lambda=\"%f\" ", ir->fLambda);   
		strParam.AppendFormat(L"Time=\"%s\" ", ir->szTime); 
		strParam.AppendFormat(L"No=\"%f\" ", ir->fNo);   
		strParam.AppendFormat(L"OilTemp=\"%f\" ", ir->fOilTemp);   
		strParam.AppendFormat(L"Temp=\"%f\" ", ir->fTemp);   
		strParam.AppendFormat(L"Humi=\"%f\" ", ir->fHumi);  
		strParam.AppendFormat(L"Prss=\"%f\" ", ir->fPrss);  
		strParam.AppendFormat(L"Stage=\"%d\" ", ir->nStage);//1:高怠速，2:怠速
		strParam.AppendFormat(L"Type=\"%d\" ", ir->Type);//工况类型,0-70%额定转速、1-高怠速准备、2-高怠速检 测,3-怠速准备、4-怠速检测
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</LogData>");
	strParam.Append(L"</TSIC>");
	strParam.Append(L"</DetectLog>");
	strParam.Append(L"</szDetectLog>");
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutTSICLog", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//FSUNHT检测
bool HGWebServiceLibAPI::PutFRAOData(__int64 lDetectID, FRAODATA SFRAOData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutFRAOData(m_strTokenGuid, m_strLineNum, m_QZR, m_PZR, lDetectID, SFRAOData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutFRAOData(CString szToken, CString szDeviceSN, CString szQZR, CString szPZR, __int64 lDetectID, FRAODATA SFRAOData)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", SFRAOData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", SFRAOData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", SFRAOData.fEnvPressure);
	strParam.AppendFormat(L"<fIdleRev>%f</fIdleRev>", SFRAOData.fIdleRev);
	strParam.AppendFormat(L"<fK1>%f</fK1>", SFRAOData.fSmoke1);
	strParam.AppendFormat(L"<fK2>%f</fK2>", SFRAOData.fSmoke2);
	strParam.AppendFormat(L"<fK3>%f</fK3>", SFRAOData.fSmoke3);
	strParam.AppendFormat(L"<fK4>%f</fK4>", SFRAOData.fSmoke4);
	strParam.AppendFormat(L"<fKAvg>%f</fKAvg>", SFRAOData.fKAvg);
	strParam.AppendFormat(L"<fIdleRev>%f</fIdleRev>", SFRAOData.fIdleRev);
	strParam.AppendFormat(L"<szQZR>%s</szQZR>", szQZR);
	strParam.AppendFormat(L"<szPZR>%s</szPZR>", szPZR);
	strParam.AppendFormat(L"<nFERESULT>%d</nFERESULT>", (SFRAOData.nFERESULT == 0) ? 2 : 1);
	strParam.AppendFormat(L"<nRatedSpeed>%d</nRatedSpeed>", SFRAOData.nRatedSpeed);
	strParam.AppendFormat(L"<nLowSpeed>%d</nLowSpeed>", SFRAOData.nLowSpeed);
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutFRAOData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutFRAOLog(__int64 lDetectID, FRAOINFO SFRAOInfo, vector<FRAOLog>& vecFRAOLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutFRAOLog(m_strTokenGuid, m_strLineNum, lDetectID, SFRAOInfo, vecFRAOLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutFRAOLog(CString szToken, CString szDeviceSN, __int64 lDetectID, FRAOInfo SFRAOInfo, vector<FRAOLog>& vecFRAOLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.Append(L"<szDetectLog>");
	strParam.Append(L"<DetectLog>");
	strParam.AppendFormat(L"<DetectID>%lld</DetectID>", SFRAOInfo.DetectID);
	strParam.AppendFormat(L"<InspectID>%s</InspectID>", SFRAOInfo.InspectID);
	strParam.AppendFormat(L"<EngRev>%d</EngRev>", SFRAOInfo.EngRev);
	strParam.AppendFormat(L"<InspectTime>%d</InspectTime>", SFRAOInfo.InspectTime);
	strParam.AppendFormat(L"<WorkTime>%d</WorkTime>", SFRAOInfo.WorkTime);
	strParam.Append(L"<FRAO>");
	strParam.AppendFormat(L"<LogData Count=\"%d\">", vecFRAOLog.size());
	for(vector<FRAOLog>::iterator ir=vecFRAOLog.begin(); ir!=vecFRAOLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"REV=\"%d\" ", ir->nREV);
		strParam.AppendFormat(L"K=\"%f\" ", ir->fK);
		strParam.AppendFormat(L"Type=\"%d\" ", (ir->Type-3));
		strParam.AppendFormat(L"Time=\"%s\" ", ir->Time);
		strParam.Append(L"/>");
		count++;
	}
	strParam.Append(L"</LogData>");
	strParam.Append(L"</FRAO>");
	strParam.Append(L"</DetectLog>");
	strParam.Append(L"</szDetectLog>");
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutFRAOLog", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//外观检查检测结果
bool HGWebServiceLibAPI::PutVISInspectData(VISInspectData &sVISInspectData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutVISInspectData(m_strTokenGuid, m_strLineNum, sVISInspectData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutVISInspectData(CString szToken, CString szDeviceSN, VISInspectData &sVISInspectData)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", sVISInspectData.lDetectID);
	strParam.AppendFormat(L"<nODOMETER>%d</nODOMETER>", sVISInspectData.nODOMETER);
	strParam.AppendFormat(L"<nVIN_OK>%d</nVIN_OK>", sVISInspectData.nVIN_OK);
	strParam.AppendFormat(L"<nEngineNO_OK>%d</nEngineNO_OK>", sVISInspectData.nEngineNO_OK);
	strParam.AppendFormat(L"<nFakePlate_OK>%d</nFakePlate_OK>", sVISInspectData.nFakePlate_OK);
	strParam.AppendFormat(L"<nMechanicalStatus_OK>%d</nMechanicalStatus_OK>", sVISInspectData.nMechanicalStatus_OK);
	strParam.AppendFormat(L"<nPollutionDevice_OK>%d</nPollutionDevice_OK>", sVISInspectData.nPollutionDevice_OK);
	strParam.AppendFormat(L"<nOilBurningBSmoke_Is>%d</nOilBurningBSmoke_Is>", sVISInspectData.nOilBurningBSmoke_Is);
	strParam.AppendFormat(L"<nCrankCaseVentilation_OK>%d</nCrankCaseVentilation_OK>", sVISInspectData.nCrankCaseVentilation_OK);
	strParam.AppendFormat(L"<nFuelEvaporation_OK>%d</nFuelEvaporation_OK>", sVISInspectData.nFuelEvaporation_OK);
	strParam.AppendFormat(L"<nInstrumentation_OK>%d</nInstrumentation_OK>", sVISInspectData.nInstrumentation_OK);	
	strParam.AppendFormat(L"<nMechanicalProblem_Is>%d</nMechanicalProblem_Is>", sVISInspectData.nMechanicalProblem_Is);
	strParam.AppendFormat(L"<nInletOrExhaustLeak_Is>%d</nInletOrExhaustLeak_Is>", sVISInspectData.nInletOrExhaustLeak_Is);
	strParam.AppendFormat(L"<nEngGearCLiquidLeak_Is>%d</nEngGearCLiquidLeak_Is>", sVISInspectData.nEngGearCLiquidLeak_Is);
	strParam.AppendFormat(L"<nHasOBD_OK>%d</nHasOBD_OK>", sVISInspectData.nHasOBD_OK);
	strParam.AppendFormat(L"<nTirePressure_OK>%d</nTirePressure_OK>", sVISInspectData.nTirePressure_OK);
	strParam.AppendFormat(L"<nAirConditionerClose_OK>%d</nAirConditionerClose_OK>", sVISInspectData.nAirConditionerClose_OK);
	strParam.AppendFormat(L"<nInterruptAEEA_OK>%d</nInterruptAEEA_OK>", sVISInspectData.nInterruptAEEA_OK);
	strParam.AppendFormat(L"<nIPUMPBYELECCONTROL>%d</nIPUMPBYELECCONTROL>", sVISInspectData.nIPUMPBYELECCONTROL);
	strParam.AppendFormat(L"<nOils_OK>%d</nOils_OK>", sVISInspectData.nOils_OK);
	strParam.AppendFormat(L"<nCheckOnDynamometer_OK>%d</nCheckOnDynamometer_OK>", sVISInspectData.nCheckOnDynamometer_OK);
	strParam.AppendFormat(L"<szBEGINTIME>%s</szBEGINTIME>", sVISInspectData.szBEGINTIME.c_str());
	strParam.AppendFormat(L"<szENDTIME>%s</szENDTIME>", sVISInspectData.szENDTIME.c_str());
	strParam.AppendFormat(L"<nCheckResult>%d</nCheckResult>", sVISInspectData.nCheckResult);
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", sVISInspectData.szChecker.c_str());
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutVISInspectData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//OBD检查检测结果
bool HGWebServiceLibAPI::PutOBDInspectData(OBDInspectData &sOBDInspectData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutOBDInspectData(m_strTokenGuid, m_strLineNum,sOBDInspectData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutOBDInspectData(CString szToken, CString szDeviceSN, OBDInspectData &sOBDInspectData)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", sOBDInspectData.lDetectID);
	strParam.AppendFormat(L"<szVIN>%s</szVIN>", sOBDInspectData.szVIN.c_str());
	strParam.AppendFormat(L"<nMILOK>%d</nMILOK>", sOBDInspectData.nMILOK);
	strParam.AppendFormat(L"<nFaultIndicatorOK>%d</nFaultIndicatorOK>", sOBDInspectData.nFaultIndicatorOK);
	strParam.AppendFormat(L"<nFaultMsgExist>%d</nFaultMsgExist>", sOBDInspectData.nFaultMsgExist);
	strParam.AppendFormat(L"<nConnect_OK>%d</nConnect_OK>", sOBDInspectData.nConnect_OK);
	strParam.AppendFormat(L"<nConnectKOREASON>%d</nConnectKOREASON>", sOBDInspectData.nConnect_OK == 1 ? 0 :sOBDInspectData.nConnectKOREASON);
	strParam.AppendFormat(L"<nODOAfterMIL>%d</nODOAfterMIL>", sOBDInspectData.nODOAfterMIL);
	strParam.AppendFormat(L"<nNotReadyItemExist>%d</nNotReadyItemExist>", sOBDInspectData.nNotReadyItemExist);
	strParam.AppendFormat(L"<szNotReadyItems>%s</szNotReadyItems>", sOBDInspectData.szNotReadyItems.c_str());
	strParam.AppendFormat(L"<szENGCALID>%s</szENGCALID>", sOBDInspectData.szENGCALID.c_str());	
	strParam.AppendFormat(L"<szENGCVN>%s</szENGCVN>", sOBDInspectData.szENGCVN.c_str());
	strParam.AppendFormat(L"<szPSTPRCSCALID>%s</szPSTPRCSCALID>", sOBDInspectData.szPSTPRCSCALID.c_str());
	strParam.AppendFormat(L"<szPSTPRCSCVN>%s</szPSTPRCSCVN>", sOBDInspectData.szPSTPRCSCVN.c_str());
	strParam.AppendFormat(L"<szOTHERCALID>%s</szOTHERCALID>", sOBDInspectData.szOTHERCALID.c_str());
	strParam.AppendFormat(L"<szOTHERCVN>%s</szOTHERCVN>", sOBDInspectData.szOTHERCVN.c_str());
	strParam.AppendFormat(L"<szOBD_Checker>%s</szOBD_Checker>", sOBDInspectData.szOBD_Checker.c_str());
	strParam.AppendFormat(L"<szCheckResult>%s</szCheckResult>", sOBDInspectData.szCheckResult.c_str());
	strParam.AppendFormat(L"<szOBD_CheckRemark>%s</szOBD_CheckRemark>", sOBDInspectData.szOBD_CheckRemark.c_str());

	strParam.AppendFormat(L"<szOBDTYPE>%s</szOBDTYPE>", sOBDInspectData.szOBDTYPE.c_str());
	strParam.AppendFormat(L"<szYROBD>%s</szYROBD>", sOBDInspectData.szYROBD.c_str());
	strParam.AppendFormat(L"<szFaultCode>%s</szFaultCode>", sOBDInspectData.szFaultCode.c_str());
	strParam.AppendFormat(L"<szFaultCodeMsg>%s</szFaultCodeMsg>", sOBDInspectData.szFaultCodeMsg.c_str());
	strParam.AppendFormat(L"<nFaultMIL>%d</nFaultMIL>", sOBDInspectData.nFaultMIL);
	strParam.AppendFormat(L"<szFaultDiagnosisMsg>%s</szFaultDiagnosisMsg>", sOBDInspectData.szFaultDiagnosisMsg.c_str());
	strParam.AppendFormat(L"<szMonitorItems>%s</szMonitorItems>", sOBDInspectData.szMonitorItems.c_str());
	strParam.AppendFormat(L"<szMonitorTime>%s</szMonitorTime>", sOBDInspectData.szMonitorTime.c_str());
	strParam.AppendFormat(L"<szMonitorOKTime>%s</szMonitorOKTime>", sOBDInspectData.szMonitorOKTime.c_str());
	strParam.AppendFormat(L"<szIUPR>%s</szIUPR>", sOBDInspectData.szIUPR.c_str());
	strParam.AppendFormat(L"<nODO>%d</nODO>", sOBDInspectData.nODO);
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutOBDInspectData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//OBD过程数据
bool HGWebServiceLibAPI::PutOBDLineData(__int64 lDetectID, vector<OBDLineDataCheckLogs>& vecOBDLineDataCheckLogs, const CString strFuelType)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutOBDLineData(m_strTokenGuid, m_strLineNum, lDetectID, vecOBDLineDataCheckLogs, strFuelType);
	}
	return true;
}
bool HGWebServiceLibAPI::PutOBDLineData(CString szToken, CString szDeviceSN, __int64 lDetectID, vector<OBDLineDataCheckLogs>& vecOBDLineDataCheckLogs, const CString strFuelType)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.Append(L"<szCheckLog>");
	strParam.Append(L"<CheckLog>");
	strParam.AppendFormat(L"<FuelType>%s</FuelType>", strFuelType);
	strParam.Append(L"<Result>");
	//strParam.AppendFormat(L"<CheckLog Count=\"%d\">",vecOBDLineDataCheckLogs.size());
	
	if(L"0" == strFuelType)
	{	
		strParam.AppendFormat(L"<Petrol Count=\"%d\">\n",vecOBDLineDataCheckLogs.size());
		for(vector<OBDLineDataCheckLogs>::iterator ir=vecOBDLineDataCheckLogs.begin(); ir!=vecOBDLineDataCheckLogs.end(); ++ir)
		{	
			//strParam.AppendFormat(L"<Data ID=\"%d\">", count);			
			//strParam.AppendFormat(L"<ObdAirPressure>%s</ObdAirPressure>", ir->PetrolObdAirPressure.c_str());
			//strParam.AppendFormat(L"<ObdAirInput>%s</ObdAirInput>", ir->PetrolObdAirInput.c_str());
			//strParam.AppendFormat(L"<ObdRPM>%s</ObdRPM>", ir->PetrolObdRPM.c_str());
			//strParam.AppendFormat(L"<ObdSpeed>%s</ObdSpeed>", ir->PetrolObdSpeed.c_str());
			//strParam.AppendFormat(L"<ObdExcessAirFactor>%s</ObdExcessAirFactor>", ir->PetrolObdExcessAirFactor.c_str());
			//strParam.AppendFormat(L"<ObdPreOxygen>%s</ObdPreOxygen>", ir->PetrolObdPreOxygen.c_str());
			//strParam.AppendFormat(L"<ObdCLV>%s</ObdCLV>", ir->PetrolObdCLV.c_str());
			//strParam.AppendFormat(L"<ObdThrottle>%s</ObdThrottle>", ir->PetrolObdThrottle.c_str());
			//strParam.AppendFormat(L"<ObdPower>%s</ObdPower>", ir->PetrolObdPower.c_str());
			//strParam.AppendFormat(L"<NowTime>%s</NowTime>", ir->NowTime.c_str());
			//strParam.Append(L"<Diesel>");
			//strParam.AppendFormat(L"<ObdThrottle>%s</ObdThrottle>", ir->DieselObdThrottle.c_str());
			//strParam.AppendFormat(L"<ObdSpeed>%s</ObdSpeed>", ir->DieselObdSpeed.c_str());
			//strParam.AppendFormat(L"<ObdPower>%s</ObdPower>", ir->DieselObdPower.c_str());
			//strParam.AppendFormat(L"<ObdRPM>%s</ObdRPM>", ir->DieselObdRPM.c_str());
			//strParam.AppendFormat(L"<ObdAirInput>%s</ObdAirInput>", ir->DieselObdAirInput.c_str());
			//strParam.AppendFormat(L"<ObdBoostPressure>%s</ObdBoostPressure>", ir->DieselObdBoostPressure.c_str());
			//strParam.AppendFormat(L"<ObdFuelConsumption>%s</ObdFuelConsumption>", ir->DieselObdFuelConsumption.c_str());
			//strParam.AppendFormat(L"<ObdNOx>%s</ObdNOx>", ir->DieselObdNOx.c_str());
			//strParam.AppendFormat(L"<ObdSCR>%s</ObdSCR>", ir->DieselObdSCR.c_str());
			//strParam.AppendFormat(L"<ObdEGT>%s</ObdEGT>", ir->DieselObdEGT.c_str());
			//strParam.AppendFormat(L"<ObdDPF>%s</ObdDPF>", ir->DieselObdDPF.c_str());
			//strParam.AppendFormat(L"<ObdEGR>%s</ObdEGR>", ir->DieselObdEGR.c_str());
			//strParam.AppendFormat(L"<ObdEFI>%s</ObdEFI>", ir->DieselObdEFI.c_str());
			//strParam.AppendFormat(L"<NowTime>%s</NowTime>", ir->NowTime.c_str());
			//strParam.Append(L"</Diesel>");

			strParam.Append(L"<Data "); 
			strParam.AppendFormat(L"ID=\"%d\" ", count);
			//strParam.AppendFormat(L"TimeNo=\"%d\" ", ++nIndex);
			strParam.AppendFormat(L"ObdAirPressure=\"%s\" ", ir->PetrolObdAirPressure.c_str());
			strParam.AppendFormat(L"ObdAirInput=\"%s\" ", ir->PetrolObdAirInput.c_str());
			strParam.AppendFormat(L"ObdRPM=\"%s\" ", ir->PetrolObdRPM.c_str());
			strParam.AppendFormat(L"ObdSpeed=\"%s\" ", ir->PetrolObdSpeed.c_str());
			strParam.AppendFormat(L"ObdExcessAirFactor=\"%s\" ", ir->PetrolObdExcessAirFactor.c_str());
			strParam.AppendFormat(L"ObdPreOxygen=\"%s\" ", ir->PetrolObdPreOxygen.c_str());  
			strParam.AppendFormat(L"ObdCLV=\"%s\" ", ir->PetrolObdCLV.c_str());   
			strParam.AppendFormat(L"ObdThrottle=\"%s\" ", ir->PetrolObdThrottle.c_str());   
			strParam.AppendFormat(L"ObdPower=\"%s\" ", ir->PetrolObdPower.c_str());  
			strParam.AppendFormat(L"NowTime=\"%s\" ", ir->NowTime.c_str());   
			strParam.Append(L"/>\n");
	
			count++;
		}
		strParam.Append(L"</Petrol>");
	}
	else
	{
		strParam.AppendFormat(L"<Diesel Count=\"%d\">\n",vecOBDLineDataCheckLogs.size());
		for(vector<OBDLineDataCheckLogs>::iterator ir=vecOBDLineDataCheckLogs.begin(); ir!=vecOBDLineDataCheckLogs.end(); ++ir)
		{	
			//strParam.AppendFormat(L"<Data ID=\"%d\">", count);

			//strParam.AppendFormat(L"<ObdThrottle>%s</ObdThrottle>", ir->DieselObdThrottle.c_str());
			//strParam.AppendFormat(L"<ObdSpeed>%s</ObdSpeed>", ir->DieselObdSpeed.c_str());
			//strParam.AppendFormat(L"<ObdPower>%s</ObdPower>", ir->DieselObdPower.c_str());
			//strParam.AppendFormat(L"<ObdRPM>%s</ObdRPM>", ir->DieselObdRPM.c_str());
			//strParam.AppendFormat(L"<ObdAirInput>%s</ObdAirInput>", ir->DieselObdAirInput.c_str());
			//strParam.AppendFormat(L"<ObdBoostPressure>%s</ObdBoostPressure>", ir->DieselObdBoostPressure.c_str());
			//strParam.AppendFormat(L"<ObdFuelConsumption>%s</ObdFuelConsumption>", ir->DieselObdFuelConsumption.c_str());
			//strParam.AppendFormat(L"<ObdNOx>%s</ObdNOx>", ir->DieselObdNOx.c_str());
			//strParam.AppendFormat(L"<ObdSCR>%s</ObdSCR>", ir->DieselObdSCR.c_str());
			//strParam.AppendFormat(L"<ObdEGT>%s</ObdEGT>", ir->DieselObdEGT.c_str());
			//strParam.AppendFormat(L"<ObdDPF>%s</ObdDPF>", ir->DieselObdDPF.c_str());
			//strParam.AppendFormat(L"<ObdEGR>%s</ObdEGR>", ir->DieselObdEGR.c_str());
			//strParam.AppendFormat(L"<ObdEFI>%s</ObdEFI>", ir->DieselObdEFI.c_str());
			//strParam.AppendFormat(L"<NowTime>%s</NowTime>", ir->NowTime.c_str());

			strParam.Append(L"<Data "); 
			strParam.AppendFormat(L"ID=\"%d\" ", count);
			strParam.AppendFormat(L"ObdThrottle=\"%s\" ", ir->DieselObdThrottle.c_str());
			strParam.AppendFormat(L"ObdSpeed=\"%s\" ", ir->DieselObdSpeed.c_str());
			strParam.AppendFormat(L"ObdPower=\"%s\" ", ir->DieselObdPower.c_str());
			strParam.AppendFormat(L"ObdRPM=\"%s\" ", ir->DieselObdRPM.c_str());
			strParam.AppendFormat(L"ObdAirInput=\"%s\" ", ir->DieselObdAirInput.c_str());
			strParam.AppendFormat(L"ObdBoostPressure=\"%s\" ", ir->DieselObdBoostPressure.c_str());
			strParam.AppendFormat(L"ObdFuelConsumption=\"%s\" ", ir->DieselObdFuelConsumption.c_str());  
			strParam.AppendFormat(L"ObdNOx=\"%s\" ", ir->DieselObdNOx.c_str());   
			strParam.AppendFormat(L"ObdSCR=\"%s\" ", ir->DieselObdSCR.c_str());   
			strParam.AppendFormat(L"ObdEGT=\"%s\" ", ir->DieselObdEGT.c_str());  
			strParam.AppendFormat(L"ObdDPF=\"%s\" ", ir->DieselObdDPF.c_str());   
			strParam.AppendFormat(L"ObdEGR=\"%s\" ", ir->DieselObdEGR.c_str());   
			strParam.AppendFormat(L"ObdEFI=\"%s\" ", ir->DieselObdEFI.c_str());  
			strParam.AppendFormat(L"NowTime=\"%s\" ", ir->NowTime.c_str());   
			strParam.Append(L"/>\n");
	
			count++;
		}
		strParam.Append(L"</Diesel>");
	}
	
	strParam.Append(L"</Result>");
	strParam.Append(L"</CheckLog>");
	strParam.Append(L"</szCheckLog>");

	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutOBDLineData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 5.14燃油蒸发检验接口
bool HGWebServiceLibAPI::PutFuelEvaporationData(__int64 lDetectID, FuelEvaporationData &sFuelEvaporationData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutFuelEvaporationData(m_strTokenGuid, m_strLineNum, lDetectID, sFuelEvaporationData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutFuelEvaporationData(CString szToken, CString szDeviceSN, __int64 lDetectID, FuelEvaporationData &sFuelEvaporationData)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<nTESTFILLER>%d</nTESTFILLER>", sFuelEvaporationData.nTESTFILLER);
	strParam.AppendFormat(L"<nTESTTCAP>%d</nTESTTCAP>", sFuelEvaporationData.nTESTTCAP);
	strParam.AppendFormat(L"<nRESULT>%d</nRESULT>", sFuelEvaporationData.nRESULT);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutFuelEvaporationData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 5.15林格曼黑度法检验接口
bool HGWebServiceLibAPI::PutLingermanBlacknessData(__int64 lDetectID, LingermanBlacknessData &sLingermanBlacknessData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutLingermanBlacknessData(m_strTokenGuid, m_strLineNum, lDetectID, sLingermanBlacknessData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutLingermanBlacknessData(CString szToken, CString szDeviceSN, __int64 lDetectID, LingermanBlacknessData &sLingermanBlacknessData)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<nVSOMKE>%d</nVSOMKE>", sLingermanBlacknessData.nVSOMKE);
	strParam.AppendFormat(L"<nBLACKNESS>%d</nBLACKNESS>", sLingermanBlacknessData.nBLACKNESS);
	strParam.AppendFormat(L"<nRESULT>%d</nRESULT>", sLingermanBlacknessData.nRESULT);
	strParam.AppendFormat(L"<dtTESTDATE>%s</dtTESTDATE>", sLingermanBlacknessData.dtTESTDATE.c_str());
	strParam.AppendFormat(L"<dtTESTDATEEND>%s</dtTESTDATEEND>", sLingermanBlacknessData.dtTESTDATEEND.c_str());
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutLingermanBlacknessData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
/************************************************************************************************/

//获取设备信息:下次标定/校准时间
bool HGWebServiceLibAPI::GetDevicesInfo()
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return GetDevicesInfo(m_strTokenGuid, m_strLineNum);
	}
	return true;
}
bool HGWebServiceLibAPI::GetDevicesInfo(CString szToken, CString szDevSN)
{
	bool bRet = false;

	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDevSN>%s</szDevSN>", szDevSN);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"GetDevicesInfo", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		//检测相关信息
		CXmlReader xmlReader;
		xmlReader.Parse(hgMsg.body.c_str());
		DeviceCalTimeInfo sDeviceCalTimeInfo;
		//排放检测设备编号
		std::wstring strLineNum(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_SN"))
		{
			xmlReader.GetNodeContent(strLineNum);
			m_strLineNum = strLineNum.c_str();
			sDeviceCalTimeInfo.LineNum = SysAllocString(strLineNum.c_str());
		}

		//力传感器下次标定/校准时间
		std::wstring strForceCalTime(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_FORCE_CALITIME"))
		{
			xmlReader.GetNodeContent(strForceCalTime);
			sDeviceCalTimeInfo.ForceCalTime = SysAllocString(strForceCalTime.c_str());
		}

		//车速传感器下次标定/校准时间
		std::wstring strSpeedCalTime(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_SPEED_CALITIME"))
		{
			xmlReader.GetNodeContent(strSpeedCalTime);
			sDeviceCalTimeInfo.SpeedCalTime = SysAllocString(strSpeedCalTime.c_str());
		}

		//不透光烟度计下次标定/校准时间
		std::wstring strOpaCalTime(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_OPACIMETER_CALITIME"))
		{
			xmlReader.GetNodeContent(strOpaCalTime);
			sDeviceCalTimeInfo.OpaCalTime = SysAllocString(strOpaCalTime.c_str());
		}

		//废气分析仪下次标定/校准时间
		std::wstring strGasAnaCalTime(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_GASANALYZER_CALITIME"))
		{
			xmlReader.GetNodeContent(strGasAnaCalTime);
			sDeviceCalTimeInfo.GasAnaCalTime = SysAllocString(strGasAnaCalTime.c_str());
		}

		//滤纸式烟度计下次标定/校准时间
		std::wstring strSmokeCalTime(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_SMOKEMETER_CALITIME"))
		{
			xmlReader.GetNodeContent(strSmokeCalTime);
			sDeviceCalTimeInfo.SmokeCalTime = SysAllocString(strSmokeCalTime.c_str());
		}

		//环境参数下次标定/校准时间
		std::wstring strEnvironment(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_ENVIRONMENT"))
		{
			xmlReader.GetNodeContent(strEnvironment);
			sDeviceCalTimeInfo.Environment = SysAllocString(strEnvironment.c_str());
		}

		//氮氧分析仪下次标定/校准时间
		std::wstring strNoxAnalyzer(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_NOXANALYZER_CALITIME"))
		{
			xmlReader.GetNodeContent(strNoxAnalyzer);
			sDeviceCalTimeInfo.NoxAnalyzer = SysAllocString(strNoxAnalyzer.c_str());
		}

		//转速计下次标定/校准时间
		std::wstring strTechometer(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_TECHOMETER_CALITIME"))
		{
			xmlReader.GetNodeContent(strTechometer);
			sDeviceCalTimeInfo.Techometer = SysAllocString(strTechometer.c_str());
		}

		//加载滑行下次滑行时间
		std::wstring strPwslip(L"");
		if (xmlReader.OpenNode(L"Devices/DeviceInfo/DEV_PWSLIP_CALITIME"))
		{
			xmlReader.GetNodeContent(strPwslip);
			sDeviceCalTimeInfo.Pwslip = SysAllocString(strPwslip.c_str());
		}

		ExportDeviceCalTimeInfo(sDeviceCalTimeInfo);
		bRet = true;
	}
	else
	{
		bRet = false;
	}
	return bRet;

}

//设备校准信息的上传;用于设备检定(计量检定)信息的上传。 不用
bool HGWebServiceLibAPI::PutCalibrationInfo()
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutCalibrationInfo(m_strTokenGuid, m_strLineNum);
	}
	return true;
}
bool HGWebServiceLibAPI::PutCalibrationInfo(CString szToken, CString szDeviceSN)
{
	bool bRet = false;
	LINEINFO SiniLineInfo;
	GetIniLineInfo(&SiniLineInfo);

	CString strParam(L"");
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtFIRSTAUTHDATE>%s</dtFIRSTAUTHDATE>", m_FIRSTAUTHDATE);
	strParam.AppendFormat(L"<dtTESTEXPIREDATE>%s</dtTESTEXPIREDATE>", m_TESTEXPIREDATE);
	strParam.AppendFormat(L"<dtDADATE>%s</dtDADATE>", m_DADATE);
	strParam.AppendFormat(L"<dtAADATE>%s</dtAADATE>", m_AADATE);
	strParam.AppendFormat(L"<dtFADATE>%s</dtFADATE>", m_FADATE);
	strParam.AppendFormat(L"<dtSADATE>%s</dtSADATE>", m_SADATE);
	strParam.AppendFormat(L"<dtTADATE>%s</dtTADATE>", m_TADATE);
	strParam.AppendFormat(L"<dtOADATE>%s</dtOADATE>", m_OADATE);
	strParam.AppendFormat(L"<dtWSADATE>%s</dtWSADATE>", m_WSADATE);
	strParam.AppendFormat(L"<szDYNAMOMETER>%s</szDYNAMOMETER>", SiniLineInfo.wchDynamometerModel);
	strParam.AppendFormat(L"<szDPROVIDER>%s</szDPROVIDER>", SiniLineInfo.wchDynamometerManufacturer);
	strParam.AppendFormat(L"<szANALYSER>%s</szANALYSER>", SiniLineInfo.wchAnalyzerModel);
	strParam.AppendFormat(L"<szAPROVIDER>%s</szAPROVIDER>", SiniLineInfo.wchAnalyzerManufacturer);
	strParam.AppendFormat(L"<szFLOWMETER>%s</szFLOWMETER>", SiniLineInfo.wchFlowmeterModel);
	strParam.AppendFormat(L"<szFPROVIDER>%s</szFPROVIDER>", SiniLineInfo.wchFlowmeterManufacturer);
	strParam.AppendFormat(L"<szSMOKEMETER>%s</szSMOKEMETER>", SiniLineInfo.wchSmokemeterModel);
	strParam.AppendFormat(L"<szSPROVIDER>%s</szSPROVIDER>", SiniLineInfo.wchSmokemeterManufacturer);
	strParam.AppendFormat(L"<szTACHOMETER>%s</szTACHOMETER>", SiniLineInfo.wchTachometerModel);
	strParam.AppendFormat(L"<szTPROVIDER>%s</szTPROVIDER>", SiniLineInfo.wchTachometerManufacturer);
	strParam.AppendFormat(L"<szOTSENSOR>%s</szOTSENSOR>", SiniLineInfo.wchEnvParamMeasurerModel);
	strParam.AppendFormat(L"<szOPROVIDER>%s</szOPROVIDER>", SiniLineInfo.wchEnvParamMeasurerManufacturer);
	strParam.AppendFormat(L"<szWSTYPE>%s</szWSTYPE>", SiniLineInfo.wchEnvParamMeasurerModel);
	strParam.AppendFormat(L"<szWSROVIDER>%s</szWSROVIDER>", SiniLineInfo.wchEnvParamMeasurerManufacturer);
	strParam.AppendFormat(L"<szODTYPE>%s</szODTYPE>", SiniLineInfo.wchOBDAdapterModel);
	strParam.AppendFormat(L"<szODPROVIDER>%s</szODPROVIDER>", SiniLineInfo.wchOBDAdapterManufacturer);
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutCalibrationInfo", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

// 测功机滑行检查
bool HGWebServiceLibAPI::PutDynConstLoad(PWSlip &sPWSlip, CheckLog &sCheckLog, vector<SlipLoadingPWLog>& vecSlipLoadingPWLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutDynConstLoad(m_strTokenGuid, m_strLineNum, m_strChecker, sPWSlip, sCheckLog, vecSlipLoadingPWLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutDynConstLoad(CString szToken, CString szDeviceSN, CString szChecker, PWSlip &sPWSlip, CheckLog &sCheckLog, vector<SlipLoadingPWLog>& vecSlipLoadingPWLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<szDeviceType>%s</szDeviceType>", sPWSlip.szDeviceType.c_str());
	strParam.AppendFormat(L"<szSlipType>%s</szSlipType>", sPWSlip.szSlipType.c_str());
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sPWSlip.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sPWSlip.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sPWSlip.dtEndTime.c_str());
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<DailySlipLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sCheckLog.CheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", sCheckLog.DeviceSN.c_str());
	strParam.AppendFormat(L"<fDIW>%f</fDIW>", sCheckLog.fDIW);
	strParam.AppendFormat(L"<fPowerI>%f</fPowerI>", sCheckLog.fPowerI);
	strParam.AppendFormat(L"<SlipLoadingPWLog Count=\"%d\">", vecSlipLoadingPWLog.size());
	for(vector<SlipLoadingPWLog>::iterator ir=vecSlipLoadingPWLog.begin(); ir!=vecSlipLoadingPWLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"Speed=\"%f\" ", ir->fSpeed);
		strParam.AppendFormat(L"BeginV=\"%f\" ", ir->fBeginV);
		strParam.AppendFormat(L"EndV=\"%f\" ", ir->fEndV);
		strParam.AppendFormat(L"LossPW=\"%f\" ", ir->fLossPW);		
		strParam.AppendFormat(L"LoadPW=\"%f\" ", ir->fLoadPW);
		strParam.AppendFormat(L"TheoretialTime=\"%f\" ", ir->fTheoretialTime);
		strParam.AppendFormat(L"ActualTime=\"%f\" ", ir->fActualTime);
		strParam.AppendFormat(L"RatioResult=\"%f\" ", ir->fRatioResult);
		strParam.AppendFormat(L"Limit=\"%f\" ", ir->fLimit);
		strParam.AppendFormat(L"JudgeResult=\"%d\" ", ir->nJudgeResult);
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</SlipLoadingPWLog>");
	strParam.Append(L"</DailySlipLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sPWSlip.nJudgeResult);
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.AppendFormat(L"<nIHP2540>%d</nIHP2540>", sPWSlip.nIHP2540);
	strParam.AppendFormat(L"<nIHP5025>%d</nIHP5025>", sPWSlip.nIHP5025);
	strParam.AppendFormat(L"<nPLHP90>%d</nPLHP90>", sPWSlip.nPLHP90);
	strParam.AppendFormat(L"<nPLHP80>%d</nPLHP80>", sPWSlip.nPLHP80);
	strParam.AppendFormat(L"<nPLHP70>%d</nPLHP70>", sPWSlip.nPLHP70);
	strParam.AppendFormat(L"<nPLHP60>%d</nPLHP60>", sPWSlip.nPLHP60);
	strParam.AppendFormat(L"<nPLHP50>%d</nPLHP50>", sPWSlip.nPLHP50);
	strParam.AppendFormat(L"<nPLHP40>%d</nPLHP40>", sPWSlip.nPLHP40);
	strParam.AppendFormat(L"<nPLHP30>%d</nPLHP30>", sPWSlip.nPLHP30);
	strParam.AppendFormat(L"<nPLHP25>%d</nPLHP25>", sPWSlip.nPLHP25);
	strParam.AppendFormat(L"<nPLHP20>%d</nPLHP20>", sPWSlip.nPLHP20);
	strParam.AppendFormat(L"<nJCJG10010>%d</nJCJG10010>", sPWSlip.nJCJG10010);
	strParam.AppendFormat(L"<nJCJG8010>%d</nJCJG8010>", sPWSlip.nJCJG8010);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckDynamometerLoadingPWSlip", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 5.5.2测功机附加损失测试
bool HGWebServiceLibAPI::PutDynPLHP(LossSlip &sLossSlip, LossSlipCheck &sLossSlipCheck, vector<SlipLossPWLog>& vecSlipLossPWLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutDynPLHP(m_strTokenGuid, m_strLineNum, m_strChecker, sLossSlip, sLossSlipCheck, vecSlipLossPWLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutDynPLHP(CString szToken, CString szDeviceSN, CString szChecker, LossSlip &sLossSlip, LossSlipCheck &sLossSlipCheck, vector<SlipLossPWLog>& vecSlipLossPWLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<szDeviceType>%s</szDeviceType>", sLossSlip.szDeviceType.c_str());
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sLossSlip.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sLossSlip.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sLossSlip.dtEndTime.c_str());
	strParam.AppendFormat(L"<fLossPW40>%f</fLossPW40>", sLossSlip.fLossPW40);
	strParam.AppendFormat(L"<fLossPW25>%f</fLossPW25>", sLossSlip.fLossPW25);
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sLossSlip.nJudgeResult);
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<LossSlipLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sLossSlipCheck.CheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", sLossSlipCheck.DeviceSN.c_str());
	strParam.AppendFormat(L"<fDIW>%f</fDIW>", sLossSlipCheck.fDIW);
	strParam.AppendFormat(L"<fSlipTime90>%f</fSlipTime90>", sLossSlipCheck.fSlipTime90);
	strParam.AppendFormat(L"<fSlipTime80>%f</fSlipTime80>", sLossSlipCheck.fSlipTime80);
	strParam.AppendFormat(L"<fSlipTime70>%f</fSlipTime70>", sLossSlipCheck.fSlipTime70);
	strParam.AppendFormat(L"<fSlipTime60>%f</fSlipTime60>", sLossSlipCheck.fSlipTime60);
	strParam.AppendFormat(L"<fSlipTime50>%f</fSlipTime50>", sLossSlipCheck.fSlipTime50);
	strParam.AppendFormat(L"<fSlipTime40>%f</fSlipTime40>", sLossSlipCheck.fSlipTime40);
	strParam.AppendFormat(L"<fSlipTime30>%f</fSlipTime30>", sLossSlipCheck.fSlipTime30);
	strParam.AppendFormat(L"<fSlipTime25>%f</fSlipTime25>", sLossSlipCheck.fSlipTime25);
	strParam.AppendFormat(L"<fSlipTime20>%f</fSlipTime20>", sLossSlipCheck.fSlipTime20);
	strParam.AppendFormat(L"<fLossPW40>%f</fLossPW40>", sLossSlipCheck.fLossPW40);
	strParam.AppendFormat(L"<fLossPW25>%f</fLossPW25>", sLossSlipCheck.fLossPW25);
	strParam.AppendFormat(L"<SlipLossPWLog Count=\"%d\">", vecSlipLossPWLog.size());
	for(vector<SlipLossPWLog>::iterator ir=vecSlipLossPWLog.begin(); ir!=vecSlipLossPWLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"V=\"%f\" ", ir->fV);
		strParam.AppendFormat(L"LossPW=\"%f\" ", ir->fLossPW);
		strParam.AppendFormat(L"VS=\"%f\" ", ir->fVS);
		strParam.AppendFormat(L"VE=\"%f\" ", ir->fVE);		
		strParam.AppendFormat(L"VTime=\"%f\" ", ir->fVTime);		
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</SlipLossPWLog>");
	strParam.Append(L"</LossSlipLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckDynamometerLossSlip", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 5.5.3测功机变载荷滑行测试
bool HGWebServiceLibAPI::PutDynVarLoad(VariableLoadingPWSlip &VariableLoadingPWSlip, VariableLoadingPWSlipLog &sVariableLoadingPWSlipLog
	, Slip &sSlip805_800, Slip &Slip724_161, Slip &sSlip611_434, vector<SlipLog>& vecSlipLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutDynVarLoad(m_strTokenGuid, m_strLineNum, m_strChecker,VariableLoadingPWSlip, sVariableLoadingPWSlipLog, sSlip805_800, Slip724_161, sSlip611_434, vecSlipLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutDynVarLoad(CString szToken, CString szDeviceSN, CString szChecker, VariableLoadingPWSlip &VariableLoadingPWSlip, VariableLoadingPWSlipLog &sVariableLoadingPWSlipLog
	, Slip &sSlip805_800, Slip &Slip724_161, Slip &sSlip611_434, vector<SlipLog>& vecSlipLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<szDeviceType>%s</szDeviceType>", VariableLoadingPWSlip.szDeviceType.c_str());
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", VariableLoadingPWSlip.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", VariableLoadingPWSlip.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", VariableLoadingPWSlip.dtEndTime.c_str());
	strParam.AppendFormat(L"<JudgeResult>%d</JudgeResult>", VariableLoadingPWSlip.nJudgeResult);
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<VariableLoadingPWSlipLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sVariableLoadingPWSlipLog.CheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", sVariableLoadingPWSlipLog.DeviceSN.c_str());
	strParam.AppendFormat(L"<fDIW>%f</fDIW>", sVariableLoadingPWSlipLog.fDIW);
	strParam.Append(L"<Slip80.5-8.0>");
	strParam.AppendFormat(L"<BeginV>%f</BeginV>", sSlip805_800.BeginV);
	strParam.AppendFormat(L"<EndV>%f</EndV>", sSlip805_800.EndV);
	strParam.AppendFormat(L"<TheoretialTime>%f</TheoretialTime>", sSlip805_800.TheoretialTime);
	strParam.AppendFormat(L"<ActualTime>%f</ActualTime>", sSlip805_800.ActualTime);
	strParam.AppendFormat(L"<RatioResult>%f</RatioResult>", sSlip805_800.RatioResult);
	strParam.AppendFormat(L"<Limit>%f</Limit>", sSlip805_800.Limit);
	strParam.AppendFormat(L"<JudgeResult>%d</JudgeResult>", sSlip805_800.JudgeResult);
	strParam.Append(L"</Slip80.5-8.0>");
	strParam.Append(L"<Slip72.4-16.1>");
	strParam.AppendFormat(L"<BeginV>%f</BeginV>", Slip724_161.BeginV);
	strParam.AppendFormat(L"<EndV>%f</EndV>", Slip724_161.EndV);
	strParam.AppendFormat(L"<TheoretialTime>%f</TheoretialTime>", Slip724_161.TheoretialTime);
	strParam.AppendFormat(L"<ActualTime>%f</ActualTime>", Slip724_161.ActualTime);
	strParam.AppendFormat(L"<RatioResult>%f</RatioResult>", Slip724_161.RatioResult);
	strParam.AppendFormat(L"<Limit>%f</Limit>", Slip724_161.Limit);
	strParam.AppendFormat(L"<JudgeResult>%d</JudgeResult>", Slip724_161.JudgeResult);
	strParam.Append(L"</Slip72.4-16.1>");
	strParam.Append(L"<Slip61.1-43.4>");
	strParam.AppendFormat(L"<BeginV>%f</BeginV>", sSlip611_434.BeginV);
	strParam.AppendFormat(L"<EndV>%f</EndV>", sSlip611_434.EndV);
	strParam.AppendFormat(L"<TheoretialTime>%f</TheoretialTime>", sSlip611_434.TheoretialTime);
	strParam.AppendFormat(L"<ActualTime>%f</ActualTime>", sSlip611_434.ActualTime);
	strParam.AppendFormat(L"<RatioResult>%f</RatioResult>", sSlip611_434.RatioResult);
	strParam.AppendFormat(L"<Limit>%f</Limit>", sSlip611_434.Limit);
	strParam.AppendFormat(L"<JudgeResult>%d</JudgeResult>", sSlip611_434.JudgeResult);
	strParam.Append(L"</Slip61.1-43.4>");
	strParam.AppendFormat(L"<SlipLog  Count=\"%d\">", vecSlipLog.size());
	for(vector<SlipLog>::iterator ir=vecSlipLog.begin(); ir!=vecSlipLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"Speed=\"%f\" ", ir->fSpeed);
		strParam.AppendFormat(L"LossPW=\"%f\" ", ir->fLossPW);
		strParam.AppendFormat(L"LoadPW=\"%f\" ", ir->fLoadPW);
		strParam.AppendFormat(L"SlipTime=\"%d\" ", ir->nSlipTime);		
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</SlipLog>");
	strParam.Append(L"</VariableLoadingPWSlipLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckDynamometerVariableLoadingPWSlip", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 测功机响应时间滑行检查
bool HGWebServiceLibAPI::PutDynResponseTime(ResponseTimeSlip &sResponseTimeSlip, ResponseTimeSlipLog &sResponseTimeSlipLog, vector<ResponseTimeSlipLogs>& vecSlipLog)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutDynResponseTime(m_strTokenGuid, m_strLineNum, m_strChecker,sResponseTimeSlip, sResponseTimeSlipLog, vecSlipLog);
	}
	return true;
}
bool HGWebServiceLibAPI::PutDynResponseTime(CString szToken, CString szDeviceSN, CString szChecker, ResponseTimeSlip &sResponseTimeSlip, ResponseTimeSlipLog &sResponseTimeSlipLog, vector<ResponseTimeSlipLogs>& vecSlipLog)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<szDeviceType>%s</szDeviceType>", sResponseTimeSlip.szDeviceType.c_str());
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sResponseTimeSlip.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sResponseTimeSlip.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sResponseTimeSlip.dtEndTime.c_str());
	strParam.Append(L"<szCheckLog>");
	strParam.Append(L"<ResponseTimeSlipLog>");

	strParam.AppendFormat(L"<CheckDate>%f</CheckDate>", sResponseTimeSlipLog.CheckDate);
	strParam.AppendFormat(L"<DeviceSN>%f</DeviceSN>", sResponseTimeSlipLog.DeviceSN);
	strParam.AppendFormat(L"<fDIW>%f</fDIW>", sResponseTimeSlipLog.fDIW);
	strParam.AppendFormat(L"<SpeedStart>%f</SpeedStart>", sResponseTimeSlipLog.SpeedStart);
	strParam.AppendFormat(L"<Speed>%f</Speed>", sResponseTimeSlipLog.Speed);
	strParam.AppendFormat(L"<ForceC>%f</ForceC>", sResponseTimeSlipLog.ForceC);
	strParam.AppendFormat(L"<ForceC90>%f</ForceC90>", sResponseTimeSlipLog.ForceC90);
	strParam.AppendFormat(L"<ForceC95>%f</ForceC95>", sResponseTimeSlipLog.ForceC95);
	strParam.AppendFormat(L"<ForceC125>%f</ForceC125>", sResponseTimeSlipLog.ForceC125);
	strParam.AppendFormat(L"<Time90>%d</Time90>", sResponseTimeSlipLog.Time90);
	strParam.AppendFormat(L"<Time95>%d</Time95>", sResponseTimeSlipLog.Time95);
	strParam.AppendFormat(L"<JudgeResult>%d</JudgeResult>", sResponseTimeSlipLog.JudgeResult);
	strParam.AppendFormat(L"<SlipLog Count=\"%d\">", vecSlipLog.size());
	for(vector<ResponseTimeSlipLogs>::iterator ir=vecSlipLog.begin(); ir!=vecSlipLog.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"Speed=\"%f\" ", ir->fSpeed);
		strParam.AppendFormat(L"Force=\"%f\" ", ir->fForce);
		strParam.AppendFormat(L"Time=\"%d\" ", ir->nTime);
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</SlipLog>");
	strParam.Append(L"</ResponseTimeSlipLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sResponseTimeSlip.nJudgeResult);
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckDynamometerResponseTimeSlip", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutGasAnaLeak(GasAnalyMeterLeak &GasAnalyMeterLeak)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutGasAnaLeak(m_strTokenGuid, m_strLineNum, m_strChecker, GasAnalyMeterLeak);
	}
	return true;
}
bool HGWebServiceLibAPI::PutGasAnaLeak(CString szToken, CString szDeviceSN, CString szChecker, GasAnalyMeterLeak &GasAnalyMeterLeak)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", GasAnalyMeterLeak.dtCheckDate);
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", GasAnalyMeterLeak.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", GasAnalyMeterLeak.dtEndTime.c_str());
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", GasAnalyMeterLeak.nJudgeResult);
	strParam.AppendFormat(L"<szRemark>%s</szRemark>", GasAnalyMeterLeak.szRemark.c_str());
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckGasAnalyMeterLeak", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutGasAnaCheckO2(GasAnalyMeterCheckO2 &sGasAnalyMeterCheckO2)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutGasAnaCheckO2(m_strTokenGuid, m_strLineNum, m_strChecker, sGasAnalyMeterCheckO2);
	}
	return true;
}
bool HGWebServiceLibAPI::PutGasAnaCheckO2(CString szToken, CString szDeviceSN, CString szChecker, GasAnalyMeterCheckO2 &sGasAnalyMeterCheckO2)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sGasAnalyMeterCheckO2.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sGasAnalyMeterCheckO2.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sGasAnalyMeterCheckO2.dtEndTime.c_str());
	strParam.AppendFormat(L"<CheckVal>%f</CheckVal>", sGasAnalyMeterCheckO2.CheckVal);
	strParam.AppendFormat(L"<StdVal>%f</StdVal>", sGasAnalyMeterCheckO2.StdVal);
	strParam.AppendFormat(L"<DivVal>%f</DivVal>", sGasAnalyMeterCheckO2.DivVal);
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sGasAnalyMeterCheckO2.nJudgeResult);
	strParam.AppendFormat(L"<szRemark>%s</szRemark>", sGasAnalyMeterCheckO2.szRemark.c_str());
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutDailyCheckGasAnalyMeterCheckO2", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutGasAnaSinglePointCheck(AnalyMeterSinglePoint &AnalyMeterSinglePoint, GasAnalyzerCheckLog &sGasAnalyzerCheckLog, vector<GasAnalyzerPointLogs>& vecGasAnalyzerPointLogs)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutGasAnaSinglePointCheck(m_strTokenGuid, m_strLineNum, m_strChecker, AnalyMeterSinglePoint, sGasAnalyzerCheckLog, vecGasAnalyzerPointLogs);
	}
	return true;
}
bool HGWebServiceLibAPI::PutGasAnaSinglePointCheck(CString szToken, CString szDeviceSN, CString szChecker, AnalyMeterSinglePoint &AnalyMeterSinglePoint, GasAnalyzerCheckLog &sGasAnalyzerCheckLog, vector<GasAnalyzerPointLogs>& vecGasAnalyzerPointLogs)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", AnalyMeterSinglePoint.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", AnalyMeterSinglePoint.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", AnalyMeterSinglePoint.dtEndTime.c_str());
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<GasAnalyzerCheckLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sGasAnalyzerCheckLog.CheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", sGasAnalyzerCheckLog.DeviceSN.c_str());
	strParam.AppendFormat(L"<CheckType>%s</CheckType>", sGasAnalyzerCheckLog.CheckType.c_str());
	strParam.AppendFormat(L"<PEF>%s</PEF>", sGasAnalyzerCheckLog.PEF.c_str());
	strParam.AppendFormat(L"<CheckPointLog Count=\"d\">", vecGasAnalyzerPointLogs.size());
	for(vector<GasAnalyzerPointLogs>::iterator ir=vecGasAnalyzerPointLogs.begin(); ir!=vecGasAnalyzerPointLogs.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"ID=\"%d\" ", count);
		strParam.AppendFormat(L"GasType=\"%s\" ", ir->szGasType.c_str());
		strParam.AppendFormat(L"CheckVal=\"%f\" ", ir->fCheckVal);
		strParam.AppendFormat(L"StdVal=\"%f\" ", ir->fStdVal);
		strParam.AppendFormat(L"RateVal=\"%f\" ", ir->fRateVal);
		strParam.AppendFormat(L"DivVal=\"%f\" ", ir->fDivVal);
		strParam.AppendFormat(L"CheckJudge=\"%d\" ", ir->nCheckJudge);
		strParam.AppendFormat(L"T10Val=\"%f\" ", ir->fT10Val);
		strParam.AppendFormat(L"T10Std=\"%f\" ", ir->fT10Std);
		strParam.AppendFormat(L"T10Judge=\"%d\" ", ir->nT10Judge);
		strParam.AppendFormat(L"T90Val=\"%f\" ", ir->fT90Val);
		strParam.AppendFormat(L"T90Std=\"%f\" ", ir->fT90Std);
		strParam.AppendFormat(L"T90Judge=\"%d\" ", ir->nT90Judge);
		strParam.AppendFormat(L"JudgeResult=\"%d\" ", ir->nJudgeResult);
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</CheckPointLog>");
	strParam.Append(L"</GasAnalyzerCheckLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", AnalyMeterSinglePoint.nCheckJudgeResult);
	strParam.AppendFormat(L"<szChecker>%d</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckGasAnalyMeterSinglePoint", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutGasAnaFivePointCheck(FivePointCheck &sFivePointCheck)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutGasAnaFivePointCheck(m_strTokenGuid, m_strLineNum, m_strChecker, sFivePointCheck);
	}
	return true;
}
bool HGWebServiceLibAPI::PutGasAnaFivePointCheck(CString szToken, CString szDeviceSN, CString szChecker, FivePointCheck &sFivePointCheck)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sFivePointCheck.dtCheckDate.c_str());
	strParam.AppendFormat(L"<szCheckType>%d</szCheckType>", sFivePointCheck.nCheckType);
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sFivePointCheck.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sFivePointCheck.dtEndTime.c_str());
	strParam.AppendFormat(L"<fC3H8Std>%.2f</fC3H8Std>", sFivePointCheck.fC3H8Std);
	strParam.AppendFormat(L"<fCOStd>%.2f</fCOStd>", sFivePointCheck.fCOStd);
	strParam.AppendFormat(L"<fCO2Std>%.2f</fCO2Std>", sFivePointCheck.fCO2Std);
	strParam.AppendFormat(L"<fNOStd>%.2f</fNOStd>", sFivePointCheck.fNOStd);
	strParam.AppendFormat(L"<fO2Std>%.2f</fO2Std>", sFivePointCheck.fO2Std);
	strParam.AppendFormat(L"<fHCVal>%.2f</fHCVal>", sFivePointCheck.fHCVal);
	strParam.AppendFormat(L"<fCOVal>%.2f</fCOVal>", sFivePointCheck.fCOVal);
	strParam.AppendFormat(L"<fCO2Val>%.2f</fCO2Val>", sFivePointCheck.fCO2Val);
	strParam.AppendFormat(L"<fNOVal>%.2f</fNOVal>", sFivePointCheck.fNOVal);
	strParam.AppendFormat(L"<fO2Val>%.2f</fO2Val>", sFivePointCheck.fO2Val);
	strParam.AppendFormat(L"<nPEF>%d</nPEF>", sFivePointCheck.nPEF);
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sFivePointCheck.nJudgeResult);
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckFivePointCheck", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutNOxGasAnaLeakCheck(NOxGasAnalyMeterLeak &sNOxGasAnalyMeterLeak)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutNOxGasAnaLeakCheck(m_strTokenGuid, m_strLineNum, m_strChecker, sNOxGasAnalyMeterLeak);
	}
	return true;
}
bool HGWebServiceLibAPI::PutNOxGasAnaLeakCheck(CString szToken, CString szDeviceSN, CString szChecker, NOxGasAnalyMeterLeak &sNOxGasAnalyMeterLeak)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sNOxGasAnalyMeterLeak.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sNOxGasAnalyMeterLeak.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sNOxGasAnalyMeterLeak.dtEndTime.c_str());
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sNOxGasAnalyMeterLeak.nJudgeResult);
	strParam.AppendFormat(L"<szRemark>%s</szRemark>", sNOxGasAnalyMeterLeak.szRemark.c_str());
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckNOxGasAnalyMeterLeak", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutNOxAnaSinglePointCheck(NOxAnalyMeterSinglePoint &sNOxAnalyMeterSinglePoint, NOxAnalyzerCheckLog &sNOxAnalyzerCheckLog, vector<NOxAnalyzerCheckLogs>& vecNOxAnalyzerCheckLogs)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutNOxAnaSinglePointCheck(m_strTokenGuid, m_strLineNum, m_strChecker, sNOxAnalyMeterSinglePoint, sNOxAnalyzerCheckLog, vecNOxAnalyzerCheckLogs);
	}
	return true;
}
bool HGWebServiceLibAPI::PutNOxAnaSinglePointCheck(CString szToken, CString szDeviceSN, CString szChecker, NOxAnalyMeterSinglePoint &sNOxAnalyMeterSinglePoint, NOxAnalyzerCheckLog &sNOxAnalyzerCheckLog, vector<NOxAnalyzerCheckLogs>& vecNOxAnalyzerCheckLogs)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sNOxAnalyMeterSinglePoint.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sNOxAnalyMeterSinglePoint.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sNOxAnalyMeterSinglePoint.dtEndTime.c_str());
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<NOxAnalyzerCheckLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sNOxAnalyzerCheckLog.CheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", sNOxAnalyzerCheckLog.DeviceSN.c_str());
	strParam.AppendFormat(L"<CheckType>%s</CheckType>", sNOxAnalyzerCheckLog.CheckType.c_str());//0:低标气 3：高标气
	//strParam.AppendFormat(L"<CheckPointLog Count=\"d\">",vecNOxAnalyzerCheckLogs.size());
	strParam.AppendFormat(L"<CheckPointLog Count=\"5\">");
	for(vector<NOxAnalyzerCheckLogs>::iterator ir=vecNOxAnalyzerCheckLogs.begin(); ir!=vecNOxAnalyzerCheckLogs.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"GasType=\"%s\" ", ir->szGasType);
		strParam.AppendFormat(L"CheckVal=\"%f\" ", ir->fCheckVal);
		strParam.AppendFormat(L"StdVal=\"%f\" ", ir->fStdVal);
		strParam.AppendFormat(L"RateVal=\"%f\" ", ir->fRateVal);
		strParam.AppendFormat(L"DivVal=\"%f\" ", ir->fDivVal);
		strParam.AppendFormat(L"CheckJudge=\"%d\" ", ir->nCheckJudge);
		strParam.AppendFormat(L"T10Val=\"%f\" ", ir->fT10Val);
		strParam.AppendFormat(L"T10Std=\"%f\" ", ir->fT10Std);
		strParam.AppendFormat(L"T10Judge=\"%d\" ", ir->nT10Judge);
		strParam.AppendFormat(L"T90Val=\"%f\" ", ir->fT90Val);
		strParam.AppendFormat(L"T90Std=\"%f\" ", ir->fT90Std);
		strParam.AppendFormat(L"T90Judge=\"%d\" ", ir->nT90Judge);
		strParam.AppendFormat(L"JudgeResult=\"%d\" ", ir->nJudgeResult);
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</CheckPointLog>");
	strParam.Append(L"</NOxAnalyzerCheckLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sNOxAnalyMeterSinglePoint.nCheckJudgeResult);
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckNOxAnalyMeterSinglePoint", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutOpaCheck(OpaciMeterSinglePoint &sOpaciMeterSinglePoint, OpaciMeterCheckLog &sOpaciMeterCheckLog, vector<OpaciMeterCheckLogs>& vecOpaciMeterCheckLogs)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutOpaCheck(m_strTokenGuid, m_strLineNum, m_strChecker, sOpaciMeterSinglePoint, sOpaciMeterCheckLog, vecOpaciMeterCheckLogs);
	}
	return true;
}
bool HGWebServiceLibAPI::PutOpaCheck(CString szToken, CString szDeviceSN, CString szChecker, OpaciMeterSinglePoint &sOpaciMeterSinglePoint, OpaciMeterCheckLog &sOpaciMeterCheckLog, vector<OpaciMeterCheckLogs>& vecOpaciMeterCheckLogs)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sOpaciMeterSinglePoint.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sOpaciMeterSinglePoint.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sOpaciMeterSinglePoint.dtEndTime.c_str());
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<OpaciMeterCheckLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sOpaciMeterCheckLog.CheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", sOpaciMeterCheckLog.DeviceSN.c_str());
	strParam.AppendFormat(L"<CheckV0>%f</CheckV0>", sOpaciMeterCheckLog.CheckV0);
	strParam.AppendFormat(L"<DivV0>%f</DivV0>", sOpaciMeterCheckLog.DivV0);
	strParam.AppendFormat(L"<CheckV0Judge>%d</CheckV0Judge>", sOpaciMeterCheckLog.CheckV0Judge);
	strParam.AppendFormat(L"<CheckV100>%f</CheckV100>", sOpaciMeterCheckLog.CheckV100);
	strParam.AppendFormat(L"<DivV100>%f</DivV100>", sOpaciMeterCheckLog.DivV100);
	strParam.AppendFormat(L"<CheckV100Judge>%d</CheckV100Judge>", sOpaciMeterCheckLog.CheckV100Judge);
	//strParam.AppendFormat(L"<KDC>%f</KDC>", sOpaciMeterCheckLog.KDC);
	strParam.AppendFormat(L"<RTime>%f</RTime>", sOpaciMeterCheckLog.RTime);
	strParam.AppendFormat(L"<Fgt>%f</Fgt>", sOpaciMeterCheckLog.Fgt);
	strParam.AppendFormat(L"<CheckPointLog Count=\"d\">",vecOpaciMeterCheckLogs.size());
	for(vector<OpaciMeterCheckLogs>::iterator ir=vecOpaciMeterCheckLogs.begin(); ir!=vecOpaciMeterCheckLogs.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"CheckType=\"%s\" ", ir->szCheckType.c_str());
		strParam.AppendFormat(L"CheckVal=\"%f\" ", ir->fCheckVal);
		strParam.AppendFormat(L"StdVal=\"%f\" ", ir->fStdVal);
		strParam.AppendFormat(L"AbsoluteVal=\"%f\" ", ir->fAbsoluteVal);
		strParam.AppendFormat(L"AbsoluteJudge=\"%d\" ", ir->nAbsoluteJudge);
		strParam.Append(L"/>");

		count++;
	}
	strParam.Append(L"</CheckPointLog>");
	strParam.Append(L"</OpaciMeterCheckLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sOpaciMeterSinglePoint.nJudgeResult);
	strParam.AppendFormat(L"<szRemark>%s</szRemark>", sOpaciMeterSinglePoint.szRemark.c_str());
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDailyCheckOpaciMeterSinglePoint", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutEPCheck(WeatherStation &sWeatherStation, WeatherStationCheckLog &sWeatherStationCheckLoge)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutEPCheck(m_strTokenGuid, m_strLineNum, m_strChecker, sWeatherStation, sWeatherStationCheckLoge);
	}
	return true;
}
bool HGWebServiceLibAPI::PutEPCheck(CString szToken, CString szDeviceSN, CString szChecker, WeatherStation &sWeatherStation, WeatherStationCheckLog &sWeatherStationCheckLoge)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sWeatherStation.dtCheckDate.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sWeatherStation.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sWeatherStation.dtEndTime.c_str());
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<WeatherStationCheckLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sWeatherStationCheckLoge.CheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", sWeatherStationCheckLoge.DeviceSN.c_str());
	strParam.AppendFormat(L"<CheckTempVal>%f</CheckTempVal>", sWeatherStationCheckLoge.CheckTempVal);
	strParam.AppendFormat(L"<StdTempVal>%f</StdTempVal>", sWeatherStationCheckLoge.StdTempVal);
	strParam.AppendFormat(L"<RateTempVal>%f</RateTempVal>", sWeatherStationCheckLoge.RateTempVal);
	strParam.AppendFormat(L"<RateJudgeTemp>%d</RateJudgeTemp>", sWeatherStationCheckLoge.RateJudgeTemp);
	strParam.AppendFormat(L"<CheckPressVal>%f</CheckPressVal>", sWeatherStationCheckLoge.CheckPressVal);
	strParam.AppendFormat(L"<StdPressVal>%f</StdPressVal>", sWeatherStationCheckLoge.StdPressVal);
	strParam.AppendFormat(L"<RatePressVal>%f</RatePressVal>", sWeatherStationCheckLoge.RatePressVal);
	strParam.AppendFormat(L"<RateJudgePress>%d</RateJudgePress>", sWeatherStationCheckLoge.RateJudgePress);
	strParam.AppendFormat(L"<CheckHumiVal>%f</CheckHumiVal>", sWeatherStationCheckLoge.CheckHumiVal);
	strParam.AppendFormat(L"<StdHumiVal>%f</StdHumiVal>", sWeatherStationCheckLoge.StdHumiVal);
	strParam.AppendFormat(L"<RateHumiVal>%f</RateHumiVal>", sWeatherStationCheckLoge.RateHumiVal);
	strParam.AppendFormat(L"<RateJudgeHumi>%d</RateJudgeHumi>", sWeatherStationCheckLoge.RateJudgeHumi);
	strParam.Append(L"</WeatherStationCheckLog>");

	strParam.Append(L"</szCheckLog>");
	strParam.AppendFormat(L"<nJudgeResult>%d</nJudgeResult>", sWeatherStation.nJudgeResult);
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", szChecker);
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutDailyCheckWeatherStation", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

/************************************************************************************************/
bool HGWebServiceLibAPI::PutASMSelfCheckData(ASMSelfCheckData &ASMSelfCheckData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutASMSelfCheckData(m_strTokenGuid, m_strLineNum, ASMSelfCheckData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutASMSelfCheckData(CString szToken, CString szDeviceSN, ASMSelfCheckData &ASMSelfCheckData)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", ASMSelfCheckData.lDetectID);
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", ASMSelfCheckData.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", ASMSelfCheckData.dtEndTime.c_str());
	strParam.AppendFormat(L"<nCheckResult>%d</nCheckResult>", ASMSelfCheckData.nCheckResult);
	strParam.Append(L"<CheckLog>");

	strParam.Append(L"<EnvCheckResult>");
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", ASMSelfCheckData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", ASMSelfCheckData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", ASMSelfCheckData.fEnvPressure);
	strParam.AppendFormat(L"<nEnvTemperatureOK>%d</nEnvTemperatureOK>", ASMSelfCheckData.nEnvTemperatureOK);
	strParam.AppendFormat(L"<nEnvHumidityOK>%d</nEnvHumidityOK>", ASMSelfCheckData.nEnvHumidityOK);
	strParam.AppendFormat(L"<nEnvPressureOK>%d</nEnvPressureOK>", ASMSelfCheckData.nEnvPressureOK);
	strParam.Append(L"</EnvCheckResult>");
	strParam.Append(L"<GasAnalyMeterZeroCheckResult>");
	strParam.AppendFormat(L"<dtZeroBeginTime>%s</dtZeroBeginTime>", ASMSelfCheckData.dtZeroBeginTime.c_str());	
	strParam.AppendFormat(L"<dtZeroEndTime>%s</dtZeroEndTime>", ASMSelfCheckData.dtZeroEndTime.c_str());
	strParam.Append(L"</GasAnalyMeterZeroCheckResult>");
	strParam.Append(L"<GasAnalyMeterEnvCheckResult>");
	strParam.AppendFormat(L"<fEnviHC>%f</fEnviHC>", ASMSelfCheckData.fEnviHC);
	strParam.AppendFormat(L"<fEnviCO>%f</fEnviCO>", ASMSelfCheckData.fEnviCO);
	strParam.AppendFormat(L"<fEnviNOx>%f</fEnviNOx>", ASMSelfCheckData.fEnviNOx);
	strParam.AppendFormat(L"<nEnviCheckOK>%d</nEnviCheckOK>", ASMSelfCheckData.nEnviCheckOK);
	strParam.Append(L"</GasAnalyMeterEnvCheckResult>");
	strParam.Append(L"<GasAnalyMeterLeftGasCheckResult>");
	strParam.AppendFormat(L"<fGasHC>%f</fGasHC>", ASMSelfCheckData.fGasHC);
	strParam.AppendFormat(L"<fGasCO>%f</fGasCO>", ASMSelfCheckData.fGasCO);
	strParam.AppendFormat(L"<fGasNOx>%f</fGasNOx>", ASMSelfCheckData.fGasNOx);
	strParam.AppendFormat(L"<nGasCheckOK>%d</nGasCheckOK>", ASMSelfCheckData.nGasCheckOK);
	strParam.Append(L"</GasAnalyMeterLeftGasCheckResult>");
	strParam.Append(L"<GasAnalyMeterFlowCheckResult>");
	strParam.AppendFormat(L"<nFlowCheckOK>%d</nFlowCheckOK>", ASMSelfCheckData.nFlowCheckOK);
	strParam.Append(L"</GasAnalyMeterFlowCheckResult>");

	strParam.Append(L"</CheckLog>");
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutASMSelfCheckData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutLUGDSelfCheckData(LUGDSelfCheckData &LUGDSelfCheckData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutLUGDSelfCheckData(m_strTokenGuid, m_strLineNum, LUGDSelfCheckData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutLUGDSelfCheckData(CString szToken, CString szDeviceSN, LUGDSelfCheckData &LUGDSelfCheckData)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", LUGDSelfCheckData.lDetectID);
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", LUGDSelfCheckData.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", LUGDSelfCheckData.dtEndTime.c_str());
	strParam.AppendFormat(L"<nCheckResult>%d</nCheckResult>", LUGDSelfCheckData.nCheckResult);
	strParam.Append(L"<CheckLog>");

	strParam.Append(L"<EnvCheckResult>");
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", LUGDSelfCheckData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", LUGDSelfCheckData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", LUGDSelfCheckData.fEnvPressure);
	strParam.AppendFormat(L"<nEnvTemperatureOK>%d</nEnvTemperatureOK>", LUGDSelfCheckData.nEnvTemperatureOK);
	strParam.AppendFormat(L"<nEnvHumidityOK>%d</nEnvHumidityOK>", LUGDSelfCheckData.nEnvHumidityOK);
	strParam.AppendFormat(L"<nEnvPressureOK>%d</nEnvPressureOK>", LUGDSelfCheckData.nEnvPressureOK);
	strParam.Append(L"</EnvCheckResult>");
	strParam.Append(L"<OilTemperatureCheckResult>");
	strParam.AppendFormat(L"<dtOilTmpCheckBeginTime>%s</dtOilTmpCheckBeginTime>", LUGDSelfCheckData.dtOilTmpCheckBeginTime.c_str());	
	strParam.AppendFormat(L"<dtOilTmpCheckEndTime>%s</dtOilTmpCheckEndTime>", LUGDSelfCheckData.dtOilTmpCheckEndTime.c_str());
	strParam.AppendFormat(L"<fOilTemperature>%f</fOilTemperature>", LUGDSelfCheckData.fOilTemperature);	
	strParam.AppendFormat(L"<nOilTemperatureResult>%d</nOilTemperatureResult>", LUGDSelfCheckData.nOilTemperatureResult);
	strParam.Append(L"</OilTemperatureCheckResult>");
	strParam.Append(L"<OpaciMeterCheckResult>");
	strParam.AppendFormat(L"<fKCheckP0Point>%f</fKCheckP0Point>", LUGDSelfCheckData.fKCheckP0Point);
	strParam.AppendFormat(L"<fKCheckP100Point>%f</fKCheckP100Point>", LUGDSelfCheckData.fKCheckP100Point);
	strParam.AppendFormat(L"<nKCheckP0PointOK>%d</nKCheckP0PointOK>", LUGDSelfCheckData.nKCheckP0PointOK);
	strParam.AppendFormat(L"<nKCheckP100PointOK>%d</nKCheckP100PointOK>", LUGDSelfCheckData.nKCheckP100PointOK);
	strParam.Append(L"</OpaciMeterCheckResult>");

	strParam.Append(L"</CheckLog>");
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutLUGDSelfCheckData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutTSICSelfCheckData(TSICSelfCheckData &TSICSelfCheckData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutTSICSelfCheckData(m_strTokenGuid, m_strLineNum, TSICSelfCheckData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutTSICSelfCheckData(CString szToken, CString szDeviceSN, TSICSelfCheckData &TSICSelfCheckData)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", TSICSelfCheckData.lDetectID);
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", TSICSelfCheckData.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", TSICSelfCheckData.dtEndTime.c_str());
	strParam.AppendFormat(L"<nCheckResult>%d</nCheckResult>", TSICSelfCheckData.nCheckResult);
	strParam.Append(L"<CheckLog>");

	strParam.Append(L"<EnvCheckResult>");
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", TSICSelfCheckData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", TSICSelfCheckData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", TSICSelfCheckData.fEnvPressure);
	strParam.AppendFormat(L"<nEnvTemperatureOK>%d</nEnvTemperatureOK>", TSICSelfCheckData.nEnvTemperatureOK);
	strParam.AppendFormat(L"<nEnvHumidityOK>%d</nEnvHumidityOK>", TSICSelfCheckData.nEnvHumidityOK);
	strParam.AppendFormat(L"<nEnvPressureOK>%d</nEnvPressureOK>", TSICSelfCheckData.nEnvPressureOK);
	strParam.Append(L"</EnvCheckResult>");
	strParam.Append(L"<GasAnalyMeterZeroCheckResult>");
	strParam.AppendFormat(L"<dtZeroBeginTime>%s</dtZeroBeginTime>", TSICSelfCheckData.dtZeroBeginTime.c_str());	
	strParam.AppendFormat(L"<dtZeroEndTime>%s</dtZeroEndTime>", TSICSelfCheckData.dtZeroEndTime.c_str());
	strParam.Append(L"</GasAnalyMeterZeroCheckResult>");
	strParam.Append(L"<GasAnalyMeterEnvCheckResult>");
	strParam.AppendFormat(L"<fEnviHC>%f</fEnviHC>", TSICSelfCheckData.fEnviHC);
	strParam.AppendFormat(L"<fEnviCO>%f</fEnviCO>", TSICSelfCheckData.fEnviCO);
	strParam.AppendFormat(L"<fEnviNOx>%f</fEnviNOx>", TSICSelfCheckData.fEnviNOx);
	strParam.AppendFormat(L"<nEnviCheckOK>%d</nEnviCheckOK>", TSICSelfCheckData.nEnviCheckOK);
	strParam.Append(L"</GasAnalyMeterEnvCheckResult>");
	strParam.Append(L"<GasAnalyMeterLeftGasCheckResult>");
	strParam.AppendFormat(L"<fGasHC>%f</fGasHC>", TSICSelfCheckData.fGasHC);
	strParam.AppendFormat(L"<fGasCO>%f</fGasCO>", TSICSelfCheckData.fGasCO);
	strParam.AppendFormat(L"<fGasNOx>%f</fGasNOx>", TSICSelfCheckData.fGasNOx);
	strParam.AppendFormat(L"<nGasCheckOK>%d</nGasCheckOK>", TSICSelfCheckData.nGasCheckOK);
	strParam.Append(L"</GasAnalyMeterLeftGasCheckResult>");
	strParam.Append(L"<GasAnalyMeterFlowCheckResult>");
	strParam.AppendFormat(L"<nFlowCheckOK>%d</nFlowCheckOK>", TSICSelfCheckData.nFlowCheckOK);
	strParam.Append(L"</GasAnalyMeterFlowCheckResult>");

	strParam.Append(L"</CheckLog>");
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutTSICSelfCheckData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HGWebServiceLibAPI::PutFRAOSelfCheckData(FRAOSelfCheckData &FRAOSelfCheckData)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutFRAOSelfCheckData(m_strTokenGuid, m_strLineNum, FRAOSelfCheckData);
	}
	return true;
}
bool HGWebServiceLibAPI::PutFRAOSelfCheckData(CString szToken, CString szDeviceSN, FRAOSelfCheckData &FRAOSelfCheckData)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", FRAOSelfCheckData.lDetectID);
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", FRAOSelfCheckData.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", FRAOSelfCheckData.dtEndTime.c_str());
	strParam.AppendFormat(L"<nCheckResult>%d</nCheckResult>", FRAOSelfCheckData.nCheckResult);
	strParam.Append(L"<CheckLog>");

	strParam.Append(L"<EnvCheckResult>");
	strParam.AppendFormat(L"<fEnvTemperature>%f</fEnvTemperature>", FRAOSelfCheckData.fEnvTemperature);
	strParam.AppendFormat(L"<fEnvHumidity>%f</fEnvHumidity>", FRAOSelfCheckData.fEnvHumidity);
	strParam.AppendFormat(L"<fEnvPressure>%f</fEnvPressure>", FRAOSelfCheckData.fEnvPressure);
	strParam.AppendFormat(L"<nEnvTemperatureOK>%d</nEnvTemperatureOK>", FRAOSelfCheckData.nEnvTemperatureOK);
	strParam.AppendFormat(L"<nEnvHumidityOK>%d</nEnvHumidityOK>", FRAOSelfCheckData.nEnvHumidityOK);
	strParam.AppendFormat(L"<nEnvPressureOK>%d</nEnvPressureOK>", FRAOSelfCheckData.nEnvPressureOK);
	strParam.Append(L"</EnvCheckResult>");
	strParam.Append(L"<OilTemperatureCheckResult>");
	strParam.AppendFormat(L"<dtOilTmpCheckBeginTime>%s</dtOilTmpCheckBeginTime>", FRAOSelfCheckData.dtOilTmpCheckBeginTime.c_str());	
	strParam.AppendFormat(L"<dtOilTmpCheckEndTime>%s</dtOilTmpCheckEndTime>", FRAOSelfCheckData.dtOilTmpCheckEndTime.c_str());
	strParam.AppendFormat(L"<fOilTemperature>%f</fOilTemperature>", FRAOSelfCheckData.fOilTemperature);	
	strParam.AppendFormat(L"<nOilTemperatureResult>%d</nOilTemperatureResult>", FRAOSelfCheckData.nOilTemperatureResult);
	strParam.Append(L"</OilTemperatureCheckResult>");
	strParam.Append(L"<OpaciMeterCheckResult>");
	strParam.AppendFormat(L"<fKCheckP0Point>%f</fKCheckP0Point>", FRAOSelfCheckData.fKCheckP0Point);
	strParam.AppendFormat(L"<fKCheckP100Point>%f</fKCheckP100Point>", FRAOSelfCheckData.fKCheckP100Point);
	strParam.AppendFormat(L"<nKCheckP0PointOK>%d</nKCheckP0PointOK>", FRAOSelfCheckData.nKCheckP0PointOK);
	strParam.AppendFormat(L"<nKCheckP100PointOK>%d</nKCheckP100PointOK>", FRAOSelfCheckData.nKCheckP100PointOK);
	strParam.Append(L"</OpaciMeterCheckResult>");

	strParam.Append(L"</CheckLog>");
	strParam.Append(L"</Params>");
	HGMsg hgMsg = RSInfo(L"PutFRAOSelfCheckData", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
/************************************************************************************************/
// 5.12设备检查过程记录接口
bool HGWebServiceLibAPI::PutDeviceCheckRecord(DeviceCheckRecord &sDeviceCheckRecord, vector<DeviceCheckRecordLogs>& vecDeviceCheckRecordLogs)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutDeviceCheckRecord(m_strTokenGuid, m_strStnId, m_strLineNum, sDeviceCheckRecord, vecDeviceCheckRecordLogs);
	}
	return true;
}
bool HGWebServiceLibAPI::PutDeviceCheckRecord(CString szToken, CString szStnId, CString szDeviceSN, DeviceCheckRecord &sDeviceCheckRecord, vector<DeviceCheckRecordLogs>& vecDeviceCheckRecordLogs)
{
	bool bRet = false;
	int count = 1;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szStnId>%s</szStnId>", szStnId);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sDeviceCheckRecord.dtCheckDate.c_str());
	strParam.AppendFormat(L"<szCheckType>%s</szCheckType>", sDeviceCheckRecord.szCheckType.c_str());
	strParam.AppendFormat(L"<dtBeginTime>%s</dtBeginTime>", sDeviceCheckRecord.dtBeginTime.c_str());
	strParam.AppendFormat(L"<dtEndTime>%s</dtEndTime>", sDeviceCheckRecord.dtEndTime.c_str());
	strParam.Append(L"<szCheckLog>");

	strParam.Append(L"<DeviceCheckLog>");
	strParam.AppendFormat(L"<CheckDate>%s</CheckDate>", sDeviceCheckRecord.dtCheckDate.c_str());
	strParam.AppendFormat(L"<DeviceSN>%s</DeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<CheckPointLog Count=\"d\">",vecDeviceCheckRecordLogs.size());
	for(vector<DeviceCheckRecordLogs>::iterator ir=vecDeviceCheckRecordLogs.begin(); ir!=vecDeviceCheckRecordLogs.end(); ++ir)
	{
		strParam.Append(L"<Data "); 
		strParam.AppendFormat(L"Rpt=\"%d\" ", ir->nRpt);
		strParam.AppendFormat(L"LodVal=\"%f\" ", ir->fLodVal);
		strParam.AppendFormat(L"HC=\"%f\" ", ir->fHC);
		strParam.AppendFormat(L"CO=\"%f\" ", ir->fCO);
		strParam.AppendFormat(L"NO=\"%f\" ", ir->fNO);  
		strParam.AppendFormat(L"CO2=\"%f\" ", ir->fCO2);   
		strParam.AppendFormat(L"O2=\"%f\" ", ir->fO2);   
		strParam.Append(L"/>");
		count++;
	}
	strParam.Append(L"</DeviceCheckLog>");
	strParam.Append(L"</szCheckLog>");
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDeviceCheckRecord", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 5.13设备维修保养记录接口
bool HGWebServiceLibAPI::PutDeviceFixRecord(DeviceFixRecord &sDeviceFixRecord)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return PutDeviceFixRecord(m_strTokenGuid, m_strStnId, m_strLineNum, sDeviceFixRecord);
	}
	return true;
}
bool HGWebServiceLibAPI::PutDeviceFixRecord(CString szToken, CString szStnId, CString szDeviceSN, DeviceFixRecord &sDeviceFixRecord)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szStnId>%s</szStnId>", szStnId);
	strParam.AppendFormat(L"<szDeviceSN>%s</szDeviceSN>", szDeviceSN);
	strParam.AppendFormat(L"<dtCheckDate>%s</dtCheckDate>", sDeviceFixRecord.dtCheckDate.c_str());
	strParam.AppendFormat(L"<szCheckType>%s</szCheckType>", sDeviceFixRecord.szCheckType.c_str());
	strParam.AppendFormat(L"<szReason>%s</szReason>", sDeviceFixRecord.szReason.c_str());
	strParam.AppendFormat(L"<szFixModel>%s</szFixModel>", sDeviceFixRecord.szFixModel.c_str());
	strParam.AppendFormat(L"<szChecker>%s</szChecker>", sDeviceFixRecord.szChecker.c_str());
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"PutDeviceFixRecord", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 5.16是否存在obd通信成功记录接口
bool HGWebServiceLibAPI::IsCommunicationSuccess(bool &bIsExist, CommunicationSuccess& sCommunicationSuccess)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return IsCommunicationSuccess(m_strTokenGuid, bIsExist, sCommunicationSuccess);
	}
	return true;
}
bool HGWebServiceLibAPI::IsCommunicationSuccess(CString szToken, bool &bIsExist, CommunicationSuccess& sCommunicationSuccess)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<szPlate>%s</szPlate>", sCommunicationSuccess.szPlate.c_str());
	strParam.AppendFormat(L"<szModel>%s</szModel>", sCommunicationSuccess.szModel.c_str());
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"IsCommunicationSuccess", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		//检测相关信息
		CXmlReader xmlReader;
		xmlReader.Parse(hgMsg.body.c_str());
		//是否存在obd通信成功记录
		std::wstring strIsExist(L"");
		if (xmlReader.OpenNode(L"IsExist"))
		{
			xmlReader.GetNodeContent(strIsExist);
			bIsExist = (_wtoi(strIsExist.c_str()) == 1);
		}
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

// 5.17新车上牌（注册登记业务）随车清单查验功能接口
bool HGWebServiceLibAPI::UpdateRegisterBusiness(__int64 lDetectID, RegisterBusiness& sRegisterBusiness)
{
	if(m_bIsUsed)
	{
		m_strErrMessage = L"";
		return UpdateRegisterBusiness(m_strTokenGuid, m_strLineNum, lDetectID, sRegisterBusiness);
	}
	return true;
}
bool HGWebServiceLibAPI::UpdateRegisterBusiness(CString szToken, CString szDeviceSN, __int64 lDetectID, RegisterBusiness& sRegisterBusiness)
{
	bool bRet = false;
	VESInspectStd2018SoapProxy webService(SOAP_C_UTFSTRING);
	webService.soap_endpoint = m_strUrl.c_str();
	CString strParam;
	strParam.Append(L"<Params>");
	strParam.AppendFormat(L"<szToken>%s</szToken>", szToken);
	strParam.AppendFormat(L"<lDetectID>%lld</lDetectID>", lDetectID);
	strParam.AppendFormat(L"<szINFORMATION_OK>%s</szINFORMATION_OK>", sRegisterBusiness.szINFORMATION_OK.c_str());
	strParam.AppendFormat(L"<szVEHICLEDEVICE_OK>%s</szVEHICLEDEVICE_OK>", sRegisterBusiness.szVEHICLEDEVICE_OK.c_str());
	strParam.AppendFormat(L"<szAUT_PLATE>%s</szAUT_PLATE>", sRegisterBusiness.szAUT_PLATE.c_str());
	strParam.AppendFormat(L"<szAUT_PCLASS>%s</szAUT_PCLASS>", sRegisterBusiness.szAUT_PCLASS.c_str());
	strParam.AppendFormat(L"<szPicBase64>%s</szPicBase64>", sRegisterBusiness.szPicBase64.c_str());
	strParam.Append(L"</Params>");

	HGMsg hgMsg = RSInfo(L"UpdateRegisterBusiness", strParam);
	if ((L"0" == hgMsg.code)||(L"1" == hgMsg.code))
	{
		bRet = true;	
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

//导出设备下次标定/校准时间
void HGWebServiceLibAPI::ExportDeviceCalTimeInfo(DEVICECALTIMEINFO sDeviceCalTimeInfo)
{
	// 删除设备下次标定/校准时间信息ini文件
	ClearDeviceCalTimeInfo();

	CString strPath;
	// 获取exe(dll)文件绝对路径
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	// 向上2层
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	// 配置文件路径
	strPath = strPath + L"\\App_Data";

	// 判断路径是否存在
	if (_taccess(strPath, 06))
	{
		// 路径不存在

		// 创建路径
		if (!CreateDirectory(strPath, NULL)) 
		{ 
			// 创建路径失败,返回,不保存
			return;
		} 
	}

	CString str;
	str.Format(L"[DeviceCalTimeInfo]\r\nLineNum=%s\r\nForceCalTime=%s\r\nSpeedCalTime=%s\r\nOpaCalTime=%s\r\nGasAnaCalTime=%s\r\nSmokeCalTime=%s\r\nEnvironment=%s",
		sDeviceCalTimeInfo.LineNum,  // 排放检测设备编号
		sDeviceCalTimeInfo.ForceCalTime,  // 力传感器下次标定/校准时间
		sDeviceCalTimeInfo.SpeedCalTime, //车速传感器下次标定/校准时间
		sDeviceCalTimeInfo.OpaCalTime, //不透光烟度计下次标定/校准时间
		sDeviceCalTimeInfo.GasAnaCalTime, //废气分析仪下次标定/校准时间
		sDeviceCalTimeInfo.SmokeCalTime, //滤纸式烟度计下次标定/校准时间
		sDeviceCalTimeInfo.Environment //环境参数下次标定/校准时间
		);

	// 写入到ini文件
	strPath = strPath + L"\\DeviceCalTimeInfo.ini";
	SetFileAttributes(strPath, 0);
	CStdioFileEx sfe;
	sfe.Open(strPath, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode);
	sfe.WriteString(str);
	sfe.Close();
}	

// 删除设备下次标定/校准时间信息ini文件
void HGWebServiceLibAPI::ClearDeviceCalTimeInfo(void)
{
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"DeviceCalTimeInfo.ini", wchPath))
	{
		// 暂不做任何操作 
		return;
	}
	CSimpleIni si(wchPath);

	DeleteFile(wchPath);
}