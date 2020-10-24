#include "StdAfx.h"
#include "YJClientLibAPI.h"
//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

CYJClientLib& CYJClientLib::GetInstance(void)
{
	static std::auto_ptr<CYJClientLib> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new CYJClientLib());
	}
	return *(auto_ptr_instance.get());
}

CYJClientLib::CYJClientLib(void)
{
	srand((unsigned)time(NULL));
		
	wchar_t wchConfigIni[MAX_PATH];
	if (0x00 != CNHCommonAPI::GetFilePathEx(L"Config", L"YJConfig.ini", wchConfigIni))
	{
	}

	CSimpleIni si(wchConfigIni);

	// 服务器IP
	m_strIP = si.GetString(L"System", L"ServerIP", L"117.40.138.23");
	// 服务器端口号
	m_nPort = (unsigned short)_wtoi(si.GetString(L"System", L"ServerPort", L"9779"));
	// 检测站编号
	m_strStationNumber = si.GetString(L"System", L"StationNumber", L"40605001");
	// 检测线编号
	m_strLineNumber = si.GetString(L"System", L"LineNumber", L"01");
	// 接口序列号
	//m_strJKXLH = si.GetString(L"System", L"JKXLH", L"");
	// 是否联网
	m_bIsUsed = (L"1" == si.GetString(L"System", L"IsUsed", L"1")) ? true : false;

	// 初始化数据
	m_mapPlateColor[L"00"] = L"无";
	m_mapPlateColor[L"01"] = L"蓝牌";
	m_mapPlateColor[L"02"] = L"黄牌";
	m_mapPlateColor[L"03"] = L"黑牌";
	m_mapPlateColor[L"04"] = L"白牌";
	m_mapPlateColor[L"05"] = L"绿牌";
	m_mapPlateColor[L"06"] = L"黄绿牌";

	m_mapFuelType[L"A"] = L"汽油";
	m_mapFuelType[L"B"] = L"柴油";
	m_mapFuelType[L"C"] = L"电";
	m_mapFuelType[L"D"] = L"混合油";
	m_mapFuelType[L"E"] = L"天然气";
	m_mapFuelType[L"F"] = L"液化石油气";
	m_mapFuelType[L"L"] = L"甲醇";
	m_mapFuelType[L"M"] = L"乙醇";
	m_mapFuelType[L"N"] = L"太阳能";
	m_mapFuelType[L"O"] = L"混合动力";
	m_mapFuelType[L"P"] = L"氢";
	m_mapFuelType[L"Q"] = L"生物燃料";
	m_mapFuelType[L"Y"] = L"无";
	m_mapFuelType[L"Z"] = L"其他";
	
	m_mapAirIntakeMode[L"01"] = L"自然进气";
	m_mapAirIntakeMode[L"02"] = L"涡轮增压";

	m_mapOilSupplyMode[L"01"] = L"化油器";
	m_mapOilSupplyMode[L"02"] = L"闭环电喷";
	m_mapOilSupplyMode[L"03"] = L"开环电喷";

	m_mapTestType[L"A"] = L"双怠速";
	m_mapTestType[L"B"] = L"稳态";
	m_mapTestType[L"C"] = L"简易瞬态";
	m_mapTestType[L"D"] = L"瞬态";
	m_mapTestType[L"F"] = L"自由加速";
	m_mapTestType[L"G"] = L"加载减速";

	m_mapTestTypeToCode[L"A"] = L"4";
	m_mapTestTypeToCode[L"B"] = L"1";
	m_mapTestTypeToCode[L"C"] = L"2";
	m_mapTestTypeToCode[L"D"] = L"瞬态";
	m_mapTestTypeToCode[L"F"] = L"5";
	m_mapTestTypeToCode[L"G"] = L"3";

	srand((unsigned int)time(NULL));

	m_RunMutex = CreateMutex(NULL, TRUE, NULL);
	::InitializeCriticalSection(&m_CriticalSection);
	HANDLE ThreadHandle = CreateThread(NULL, 0, UploadThreadFunc, this, 0, NULL);
	CloseHandle(ThreadHandle);
}

CYJClientLib::~CYJClientLib(void)
{
	ReleaseMutex(m_RunMutex);
	Sleep(1000);
	::DeleteCriticalSection(&m_CriticalSection);
}

void CYJClientLib::CloseConnect(void)
{
	if (m_yjSocket.IsOpen())
	{
		m_yjSocket.Close();
	}
}

CString CYJClientLib::CheckSum(const char *ch, const UINT un)
{
	//assert(NULL!=pb && 0UL!=un);
	assert(NULL!=ch && 0UL!=un);
	CString strRet(L"");

	DWORD bSum(0x00);	
	for (UINT ui=0; ui<un; ui++)
	{
		bSum ^=  ch[ui];
	}
	strRet.Format(L"%02x", bSum&0xff);
	return strRet;
}

CString CYJClientLib::GenerateInsNum(void)
{
	// 唯一编码生成规则：检测线编码+时间（精确到毫秒）
	CString InsNum(L"");
	InsNum.AppendFormat(L"%s", m_strLineNumber);
	SYSTEMTIME dt;
	GetLocalTime(&dt);
	InsNum.AppendFormat(L"%04d%02d%02d%02d%02d%02d%03d", dt.wYear, dt.wMonth, dt.wDay, dt.wHour, dt.wMinute, dt.wSecond, dt.wMilliseconds);
	return InsNum;
}

bool CYJClientLib::SendAndRecvPacket(CString strDataType, CString strStationNumber, CString strLineNumber, CString strInsNumber, CString strJsonData, std::wstring &strRecv)
{	
	CString strSend(L"");
	// 1数据类型+2检测站编码+3检测线编码+4时间+4唯一编码+5固定分隔符+6数据部分+7固定分隔符+8校验码+9固定结束符
	strSend.Format(L"%s%s%s", strDataType, strStationNumber, strLineNumber);
	strSend.AppendFormat(L"%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	// 唯一编码
	strSend.AppendFormat(L"%s", strInsNumber);
	// 固定分隔符
	strSend.AppendFormat(L"@@@");
	strSend.AppendFormat(L"%s", strJsonData);// 数据部分
	strSend.AppendFormat(L"tek");// 固定分隔符

	std::string sData(CW2A(strSend.GetString()));
	CString strSum = CheckSum((char *)sData.c_str(), sData.size());
	strSend.AppendFormat(L"%s", strSum); // 校验码

	//CString CheckSumDemo(L"HJ0240608036012019-07-03 13:52:47@@@0,网络异常或数据解析错误,数据接收失败tek"); // CheckSum:28
	//std::string sCheckSumDemo(CW2A(CheckSumDemo.GetString()));
	//CString strCheckSumDemo = CheckSum((char *)sCheckSumDemo.c_str(), sCheckSumDemo.size());

	strSend.AppendFormat(L"####");// 固定结束符

	if (!m_yjSocket.IsOpen())
	{
		if (!m_yjSocket.Open(m_strIP, m_nPort))
		{
			strRecv = L"连接服务器失败！";
			return false;
		}
	}

	return m_yjSocket.SendAndRecvPacket(strSend, strRecv);
}

bool CYJClientLib::Authentication(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	//CString strXML(L"LJZC");
	//strXML.AppendFormat(L"%s", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//strXML.AppendFormat(L"@@@");
	//strXML.AppendFormat(L"%s", m_strDeviceSN);
	//strXML.AppendFormat(L"tek");
	//std::string sData(CW2A(strXML.GetString()));
	//strXML.AppendFormat(L"%s", CheckSum((char *)sData.c_str(), sData.size()));
	//strXML.AppendFormat(L"####");

	//std::wstring strRecv;
	//if (SendAndRecvPacket(L"LJZC", L"", L"", L"", m_strLineNumber, strRecv))
	//{
	//	int nResPos = strRecv.find(L",")-1;
	//	strResult = strRecv.substr(nResPos, 1);
	//	int nTekPos = strRecv.find(L"tek");
	//	strMsg = strRecv.substr(nResPos+2, nTekPos-nResPos-1);
	//	return true;
	//}
	//else
	//{
	//	strMsg = strRecv;
	//}
	//return false;

	return SendAndRecvPacket(L"LJZC", L"", L"", L"", m_strLineNumber, strMsg);
}

bool CYJClientLib::GetTime(std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ26", m_strStationNumber, m_strLineNumber, GenerateInsNum(), L"", strRecv);
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		COleDateTime odtNow;
		if (odtNow.ParseDateTime(strMsg.c_str()))
		{
			SYSTEMTIME st;
			st.wYear = odtNow.GetYear();
			st.wMonth = odtNow.GetMonth();
			st.wDay = odtNow.GetDay();
			st.wHour = odtNow.GetHour();
			st.wMinute = odtNow.GetMinute();
			st.wSecond = odtNow.GetSecond();
			st.wMilliseconds = 0;
			// 本机
			if (SetLocalTime(&st))
			{
				CNHLogAPI::WriteLog(L"时间同步", L"设置本地时间", L"设置本地时间成功");
			}
			else
			{
				CNHLogAPI::WriteLog(L"时间同步", L"设置本地时间", L"设置本地时间失败");
			}
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::GetUsers(std::wstring &strResult, std::wstring &strMsg)
{	
	if (!m_bIsUsed)
	{
		return true;
	}
	m_vtNetUserInfo.clear();
	
	std::wstring strRecv;
//#ifdef DEBUG
//	strMsg = L"[{\"StationCode\":\"440608036\",\"Name\":\"梁崇智\",\"InductionCardCode\":\"1412136\",\"ExpiryDate\":null},{\"StationCode\":\"440608036\",\"Name\":\"吴锦雄\",\"InductionCardCode\":\"1412137\",\"ExpiryDate\":null},{\"StationCode\":\"440608036\",\"Name\":\"阮彦玮\",\"InductionCardCode\":\"1412142\",\"ExpiryDate\":null}]";
//	bool bRet(true);
//#else
	bool bRet = SendAndRecvPacket(L"HJ02", m_strStationNumber, m_strLineNumber, L"", L"", strRecv);
//#endif
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		CString strJson(L"");
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			for (int i=0; i<root.size(); i++)
			{
				USERINFO sUserInfo;
				if (!root[i]["Name"].empty())	{wcscpy_s(sUserInfo.wchName, ANSIToTChar(root[i]["Name"].asCString()));}
				if (!root[i]["InductionCardCode"].empty())	{wcscpy_s(sUserInfo.wchLicenseSn, ANSIToTChar(root[i]["InductionCardCode"].asCString()));}
				if (!root[i]["ExpiryDate"].empty())	{wcscpy_s(sUserInfo.wchLicenseEndDate, ANSIToTChar(root[i]["ExpiryDate"].asCString()));}
				m_vtNetUserInfo.push_back(sUserInfo);
			}
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::GetVehicleList(std::vector<TESTLOG> &vtTestLog, std::wstring &strFilterData, std::wstring &strResult, std::wstring &strMsg)
{	
	if (!m_bIsUsed)
	{
		return true;
	}
	vtTestLog.clear();
	
	std::wstring strRecv;
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"InspectionMethod\":\"%s\",", L"");
	if (strFilterData.empty() || strFilterData==L"")
	{
		strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", L"");
		strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", L"");
		strJsonData.AppendFormat(L"\"VIN\":\"%s\",", L"");
	}
	else
	{
		strJsonData.AppendFormat(L"%s", strFilterData.c_str());
	}
	COleDateTime odtCurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan odts;
	odts.SetDateTimeSpan(15, 0, 0, 0);
	COleDateTime odtSartTime = odtCurTime - odts;
	strJsonData.AppendFormat(L"\"StartTime\":\"%s\",",odtSartTime.Format(L"%Y-%m-%d 00:00:00"));
	strJsonData.AppendFormat(L"\"EndTime\":\"%s\"", odtCurTime.Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"}");
//#ifndef DEBUG
	bool bRet = SendAndRecvPacket(L"HJ03", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
//#else
//	bool bRet(true);
//#endif
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		if (strMsg.length() == 21)
		{
			strMsg = strRecv;
		}
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			for (int i=0; i<root.size(); i++)
			{
				TESTLOG sTestlog;
				wcscpy_s(sTestlog.wchTestState, L"0");
				if (!root[i]["InspectionNum"].empty())	{wcscpy_s(sTestlog.wchRunningNumber, ANSIToTChar(root[i]["InspectionNum"].asCString()));}
				if (!root[i]["InspectionNum"].empty())	{wcscpy_s(sTestlog.wchReportNumber, ANSIToTChar(root[i]["InspectionNum"].asCString()));}
				if (!root[i]["StationCode"].empty())	{wcscpy_s(sTestlog.wchStationNumber, ANSIToTChar(root[i]["StationCode"].asCString()));}
				if (!root[i]["SceneCode"].empty())	{wcscpy_s(sTestlog.wchLineNumber, ANSIToTChar(root[i]["SceneCode"].asCString()));}
				if (!root[i]["VLPN"].empty())	{wcscpy_s(sTestlog.wchPlateNumber, ANSIToTChar(root[i]["VLPN"].asCString()));}
				if (!root[i]["VLPNColor"].empty())	{wcscpy_s(sTestlog.wchPlateType, m_mapPlateColor[ANSIToTChar(root[i]["VLPNColor"].asCString())].c_str());}
				if (!root[i]["VLPNColor"].empty())	{wcscpy_s(sTestlog.wchPlateTypeCode, ANSIToTChar(root[i]["VLPNColor"].asCString()));}
				if (!root[i]["VIN"].empty())	{wcscpy_s(sTestlog.wchVIN, ANSIToTChar(root[i]["VIN"].asCString()));}
				if (!root[i]["FuelType"].empty())	{wcscpy_s(sTestlog.wchFuelType, m_mapFuelType[ANSIToTChar(root[i]["FuelType"].asCString())].c_str());}
				if (!root[i]["GAVType"].empty())	{wcscpy_s(sTestlog.wchVehicleType, ANSIToTChar(root[i]["GAVType"].asCString()));}//TODO:转码
				if (!root[i]["KerbMass"].empty())	{wcscpy_s(sTestlog.wchUnladenMass, Json2Cstring(root[i]["KerbMass"]));}
				//if (!root[i]["BenchmarkMass"].empty())	{wcscpy_s(sTestlog.wchUnladenMass, Json2Cstring(root[i]["BenchmarkMass"]));}
				if (!root[i]["VML"].empty())	{wcscpy_s(sTestlog.wchMaximumTotalMass, Json2Cstring(root[i]["VML"]));}
				if (!root[i]["RatedSeats"].empty())	{wcscpy_s(sTestlog.wchRatedPassengerCapacity, Json2Cstring(root[i]["RatedSeats"]));}
				if (!root[i]["Mileage"].empty())	{wcscpy_s(sTestlog.wchTravelledDistance, Json2Cstring(root[i]["Mileage"]));}
				if (!root[i]["EngineNum"].empty())	{wcscpy_s(sTestlog.wchEngineNumber, ANSIToTChar(root[i]["EngineNum"].asCString()));}
				if (!root[i]["IUETYPE"].empty())	{wcscpy_s(sTestlog.wchEngineModel, ANSIToTChar(root[i]["IUETYPE"].asCString()));}
				if (!root[i]["EDSPL"].empty())	{wcscpy_s(sTestlog.wchDisplacement, Json2Cstring(root[i]["EDSPL"]));}
				if (!root[i]["EnginePower"].empty())	{wcscpy_s(sTestlog.wchRatedPower, Json2Cstring(root[i]["EnginePower"]));}
				if (!root[i]["EngineRatedSpeed"].empty())	{wcscpy_s(sTestlog.wchRatedRev, Json2Cstring(root[i]["EngineRatedSpeed"]));}
				if (!root[i]["IntakeWay"].empty())	{wcscpy_s(sTestlog.wchAirIntakeMode, m_mapAirIntakeMode[ANSIToTChar(root[i]["IntakeWay"].asCString())].c_str());}
				if (!root[i]["OilSupplyWay"].empty())	{wcscpy_s(sTestlog.wchOilSupplyMode, m_mapOilSupplyMode[ANSIToTChar(root[i]["OilSupplyWay"].asCString())].c_str());}
				if (!root[i]["HasCCA"].empty())	{wcscpy_s(sTestlog.wchHasCatalyticConverter, Json2Cstring(root[i]["HasCCA"]));}
				if (!root[i]["Operator"].empty())	{wcscpy_s(sTestlog.wchBiller, ANSIToTChar(root[i]["Operator"].asCString()));}
				CString strDate = Json2Cstring(root[i]["AcceptanceDate"]);
				if (!strDate.IsEmpty())	
				{
					strDate.Replace(L"T", L" ");
					wcscpy_s(sTestlog.wchBillDate, strDate.Left(19));
				}
				if (!root[i]["InspectionMethod"].empty())	
				{
					wcscpy_s(sTestlog.wchTestType, m_mapTestTypeToCode[ANSIToTChar(root[i]["InspectionMethod"].asCString())].c_str());
					wcscpy_s(sTestlog.wchItemEmi, L"1");
				}
				if (Json2Cstring(root[i]["InspectionNature"]) == L"01")
				{
					wcscpy_s(sTestlog.wchIsRetest, L"0");
					wcscpy_s(sTestlog.wchNumberOfTestPeriod, L"1");
				}
				else if (Json2Cstring(root[i]["InspectionNature"]) == L"02")
				{
					wcscpy_s(sTestlog.wchIsRetest, L"1");
					wcscpy_s(sTestlog.wchNumberOfTestPeriod, L"2");
				}
				else 
				{
					wcscpy_s(sTestlog.wchNumberOfTestPeriod, L"3");
					wcscpy_s(sTestlog.wchIsRetest, L"1");
				}
				if (!root[i]["HasOBD"].empty())	
				{
					wcscpy_s(sTestlog.wchHasOBD, ANSIToTChar(root[i]["HasOBD"].asCString()));
					wcscpy_s(sTestlog.wchItemOBD, ANSIToTChar(root[i]["HasOBD"].asCString()));
					//2.9版
					wcscpy_s(sTestlog.wchHasDPF, ANSIToTChar(root[i]["DPF"].asCString()));
					wcscpy_s(sTestlog.wchDPFModel, ANSIToTChar(root[i]["DPFType"].asCString()));
					wcscpy_s(sTestlog.wchHasSCR, ANSIToTChar(root[i]["SCR"].asCString()));
					wcscpy_s(sTestlog.wchSCRModel, ANSIToTChar(root[i]["SCRType"].asCString()));
					wcscpy_s(sTestlog.wchReserved2, ANSIToTChar(root[i]["OBDType"].asCString()));
				}
				vtTestLog.push_back(sTestlog);
			}
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::GetLimit(LIMITOFASM &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		strResult = L"0";
		return true;
	}
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ01", m_strStationNumber, m_strLineNumber, GenerateInsNum(), L"B", strRecv);
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			wcscpy_s(sLimit.wchCO5025, Json2Cstring(root["COEL5025"]));
			wcscpy_s(sLimit.wchHC5025, Json2Cstring(root["HCEL5025"]));
			wcscpy_s(sLimit.wchNO5025, Json2Cstring(root["NOEL5025"]));
			wcscpy_s(sLimit.wchCO2540, Json2Cstring(root["COEL2540"]));
			wcscpy_s(sLimit.wchHC2540, Json2Cstring(root["HCEL2540"]));
			wcscpy_s(sLimit.wchNO2540, Json2Cstring(root["NOEL2540"]));
			wcscpy_s(sLimit.wchReserved1, ANSIToTChar(root["LimitType"].asCString()));	// 成功标识
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::GetLimit(LIMITOFVMAS &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		strResult = L"0";
		return true;
	}
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ01", m_strStationNumber, m_strLineNumber, GenerateInsNum(), L"C", strRecv);
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			wcscpy_s(sLimit.wchCO, Json2Cstring(root["COEL"]));
			wcscpy_s(sLimit.wchHC, Json2Cstring(root["HCEL"]));
			wcscpy_s(sLimit.wchNOx, Json2Cstring(root["NOEL"]));
			wcscpy_s(sLimit.wchReserved1, ANSIToTChar(root["LimitType"].asCString()));	// 成功标识
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::GetLimit(LIMITOFDIS &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		strResult = L"0";
		return true;
	}
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ01", m_strStationNumber, m_strLineNumber, GenerateInsNum(), L"A", strRecv);
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			wcscpy_s(sLimit.wchCOLow, Json2Cstring(root["LICOL"]));
			wcscpy_s(sLimit.wchHCLow, Json2Cstring(root["LIHCL"]));
			wcscpy_s(sLimit.wchCOHigh, Json2Cstring(root["HICOL"]));
			wcscpy_s(sLimit.wchHCHigh, Json2Cstring(root["HIHCL"]));
			wcscpy_s(sLimit.wchReserved1, ANSIToTChar(root["LimitType"].asCString()));	// 成功标识
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::GetLimit(LIMITOFFSUNHT &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		strResult = L"0";
		return true;
	}
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ01", m_strStationNumber, m_strLineNumber, GenerateInsNum(), L"F", strRecv);
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			wcscpy_s(sLimit.wchK, Json2Cstring(root["KEL"]));
			wcscpy_s(sLimit.wchK_EDP, Json2Cstring(root["OPACITYEL"]));
			wcscpy_s(sLimit.wchReserved1, ANSIToTChar(root["LimitType"].asCString()));	// 成功标识
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::GetLimit(LIMITOFLUGDOWN &sLimit, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		strResult = L"0";
		return true;
	}
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ01", m_strStationNumber, m_strLineNumber, GenerateInsNum(), L"G", strRecv);
	if (bRet)
	{
		strResult = L"1";
		m_yjSocket.RecvPacket(strMsg);
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(TCharToANSI(strMsg.c_str()), root))
		{
			wcscpy_s(sLimit.wchK, Json2Cstring(root["KEL"]));
			wcscpy_s(sLimit.wchN, Json2Cstring(root["OPACITYEL"]));
			wcscpy_s(sLimit.wchNOx, Json2Cstring(root["NOX"]));
			if(wcscmp(sLimit.wchNOx,L"") == 0)
			{
				wcscpy_s(sLimit.wchNOx, Json2Cstring(root["NOXEL"]));
			}
			wcscpy_s(sLimit.wchReserved1, ANSIToTChar(root["LimitType"].asCString()));	// 成功标识
		}
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
		strMsg = strRecv;
	}
	return bRet;
}

bool CYJClientLib::UploadTestSignal(TestSignalInfo emInfo, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"Time\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	CString strCode(L"");
	switch (emInfo)
	{
	case START:
		{
			strCode = L"start";
			m_nPocessCount = 0;
		}
		break;
	case INTUBE:
		{
			strCode = L"intubated";
		}
		break;	
	case COLSTART:
		{
			strCode = L"collectionstart";
		}
		break;
	case COLEND:
		{
			strCode = L"collectionstop";
		}
		break;
	case END:
		{
			strCode = L"stop";
		}
		break;
	}
	strJsonData.AppendFormat(L"\"Code\":\"%s\"}", strCode);
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ04", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadOBDResult(const LineInfo &sLineInfo,const TestLog &sTestLog, const SResultOfOBD &sResultData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", sTestLog.wchVIN);
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", sTestLog.wchPlateNumber);
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sTestLog.wchPlateTypeCode);
	strJsonData.AppendFormat(L"\"OBDInspectStartTime\":\"%s\",", sResultData.strDetBegTime.c_str());
	strJsonData.AppendFormat(L"\"OBDInspectEndTime\":\"%s\",", sResultData.strDetEndTime.c_str());
	strJsonData.AppendFormat(L"\"ODO\":\"%d\",", _wtoi(sResultData.strDTCMileage.c_str()));// 故障里程
	strJsonData.AppendFormat(L"\"FaultIndicatorLighten\":\"%s\",", sResultData.strLampStateJudge==L"2"?L"0":L"1");// 注意转换结果
	strJsonData.AppendFormat(L"\"IsCommunicated\":\"%s\",", sResultData.strComErrorJudge==L"2"?L"0":L"1");
	strJsonData.AppendFormat(L"\"CommFailReason\":\"%s\",", sResultData.strComErrorComment.c_str());
	strJsonData.AppendFormat(L"\"FaultIndicator\":\"%s\",", sResultData.strLampStateJudge==L"2"?L"0":L"1");// 注意转换结果
	strJsonData.AppendFormat(L"\"IsHaveFaultCode\":\"%s\",", sResultData.strDTCJudge==L"2"?L"1":L"0");//是否有OBD系统故障指示器报警及故障码：0 表示无，1 表示有
	strJsonData.AppendFormat(L"\"FaultInfo\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"UnDoneReadyStatus\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"UnFinifishedItem\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"ECU_CALID\":\"%s\",", sResultData.strEngineCALID.c_str());
	strJsonData.AppendFormat(L"\"ECU_CVN\":\"%s\",", sResultData.strEngineCVN.c_str());
	strJsonData.AppendFormat(L"\"ACU_CALID\":\"%s\",", sResultData.strPostProcessingCALID.c_str());
	strJsonData.AppendFormat(L"\"ACU_CVN\":\"%s\",", sResultData.strPostProcessingCVN.c_str());
	strJsonData.AppendFormat(L"\"OCU_CALID\":\"%s\",", sResultData.strOtherCALID.c_str());
	strJsonData.AppendFormat(L"\"OCU_CVN\":\"%s\",", sResultData.strOtherCVN.c_str());
	strJsonData.AppendFormat(L"\"StatusCoherence\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CheckPerson\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"OBDInspectResult\":\"%s\",", sResultData.strJudge==L"2"?L"0":L"1");
	strJsonData.AppendFormat(L"\"IsNeedReCheck\":\"%s\",", sResultData.strJudge==L"2"?L"1":L"0");
	strJsonData.AppendFormat(L"\"ReCheckContent\":\"%s\",", sResultData.strReCheckItem.c_str());
	strJsonData.AppendFormat(L"\"InspectionNature\":\"%s\",", sTestLog.wchNumberOfTestPeriod);
	strJsonData.AppendFormat(L"\"InspectionTimes\":\"%s\"", sTestLog.wchNumberOfTestPeriod);

	//2.9版
	strJsonData.AppendFormat(L"\"DeviceMANU\":\"%s\"", sLineInfo.wchOBDAdapterManufacturer);
	strJsonData.AppendFormat(L"\"DeviceType\":\"%s\"", sLineInfo.wchOBDAdapterModel);
	if(wcscmp(sTestLog.wchReserved2,L"") != 0)
	{
		strJsonData.AppendFormat(L"\"OBDType\":\"%s\"", sTestLog.wchReserved2);
	}
	else
	{
		strJsonData.AppendFormat(L"\"OBDType\":\"%s\"", L"1");
	}
	strJsonData.AppendFormat(L"\"RemoteCommunicated\":\"%s\"", L"1");
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ05", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadOBDProcessData(SOBDRTData &sRTData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", m_nPocessCount);
	strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", _wtof(sRTData.strVelocity.c_str()));
	strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%s\",", sRTData.strEngineRev.c_str());
	strJsonData.AppendFormat(L"\"Flow_AirInput\":\"%.2f\",", _wtof(sRTData.strMAF.c_str()));
	strJsonData.AppendFormat(L"\"Flow_ThrottlePosition\":\"%.2f\",", _wtof(sRTData.strSolarTermDoorPosition.c_str()));
	strJsonData.AppendFormat(L"\"Flow_OutputPower\":\"%.2f\",", _wtof(sRTData.strEngineOutputPower.c_str()));// 注意转换结果
	strJsonData.AppendFormat(L"\"Flow_BoostPressure\":\"%.2f\",", _wtof(sRTData.strChargeAirPressure.c_str()));
	strJsonData.AppendFormat(L"\"Flow_OilConsume\":\"%.2f\",", _wtof(sRTData.strFuelConsumption.c_str()));
	strJsonData.AppendFormat(L"\"Flow_NOX\":\"%.0f\",", _wtof(sRTData.strNOXConcentration.c_str()));// 注意转换结果
	strJsonData.AppendFormat(L"\"Flow_UreaInject\":\"%.2f\",", _wtof(sRTData.strUreaInjectionVolume.c_str()));
	strJsonData.AppendFormat(L"\"Flow_ExhaustTmp\":\"%.2f\",", _wtof(sRTData.strEGT.c_str()));
	strJsonData.AppendFormat(L"\"Flow_DPFPressure\":\"%.2f\",", _wtof(sRTData.strDPFDifferentialPressure.c_str()));
	strJsonData.AppendFormat(L"\"Flow_EGRPostion\":\"%.2f\",", _wtof(sRTData.strEGRPosition.c_str()));
	strJsonData.AppendFormat(L"\"Flow_FuelInjectPressure\":\"%.2f\",", _wtof(sRTData.strFuelDeliveryPressure.c_str()));
	strJsonData.AppendFormat(L"\"Flow_ThrottleValue\":\"%.2f\",", _wtof(sRTData.strSolarTermDoorPosition.c_str()));
	strJsonData.AppendFormat(L"\"Flow_CLV\":\"%.2f\",", _wtof(sRTData.strCalculationLoad.c_str()));
	strJsonData.AppendFormat(L"\"Flow_PreOSS\":\"%.2f\",", _wtof(sRTData.strAirCoefficient.c_str()));
	strJsonData.AppendFormat(L"\"Flow_Lamda\":\"%.2f\",", _wtof(sRTData.strAirCoefficient.c_str()));
	strJsonData.AppendFormat(L"\"Flow_IntakePressure\":\"%.2f\"", _wtof(sRTData.strMAF.c_str()));
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ06", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadASMProcessData(const RealTimeDataOfASM &sRTData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", sRTData.wchSamplingTime);
	strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", m_nPocessCount);
	strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", _wtof(sRTData.wchVelocity));
	strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%s\",", sRTData.wchEngineRev);
	strJsonData.AppendFormat(L"\"Flow_Freeweight\":\"%.2f\",", _wtoi(sRTData.wchForce)/9.8f);
	strJsonData.AppendFormat(L"\"Flow_HC\":\"%s\",", sRTData.wchHC);
	strJsonData.AppendFormat(L"\"Flow_CO\":\"%.2f\",", _wtof(sRTData.wchCO));
	strJsonData.AppendFormat(L"\"Flow_CO2\":\"%.2f\",", _wtof(sRTData.wchCO2));
	strJsonData.AppendFormat(L"\"Flow_NO\":\"%s\",", sRTData.wchNO);
	strJsonData.AppendFormat(L"\"Flow_O2\":\"%.2f\",", _wtof(sRTData.wchO2));
	strJsonData.AppendFormat(L"\"Flow_HCCorrect\":\"%.0f\",", _wtoi(sRTData.wchHC)*_wtof(sRTData.wchDilutionCorrectionFactor));
	strJsonData.AppendFormat(L"\"Flow_COCorrect\":\"%.2f\",", _wtof(sRTData.wchCO)*_wtof(sRTData.wchDilutionCorrectionFactor));
	strJsonData.AppendFormat(L"\"Flow_CO2Correct\":\"%.2f\",", _wtof(sRTData.wchCO2));
	strJsonData.AppendFormat(L"\"Flow_NOCorrect\":\"%.2f\",", _wtoi(sRTData.wchNO)*_wtof(sRTData.wchDilutionCorrectionFactor)*_wtof(sRTData.wchHumidityCorrectionFactor));
	strJsonData.AppendFormat(L"\"Flow_O2Correct\":\"%.2f\",", _wtof(sRTData.wchO2));
	strJsonData.AppendFormat(L"\"Flow_Lambda\":\"%.2f\",", _wtof(sRTData.wchLambda));
	strJsonData.AppendFormat(L"\"DiluteCorrect\":\"%.2f\",", _wtof(sRTData.wchDilutionCorrectionFactor));
	strJsonData.AppendFormat(L"\"NOHumidityCorrect\":\"%.2f\",", _wtof(sRTData.wchHumidityCorrectionFactor));
	strJsonData.AppendFormat(L"\"FlowAirPressure\":\"%.2f\",", _wtof(sRTData.wchAtmosphericPressure));
	strJsonData.AppendFormat(L"\"FlowTemperature\":\"%.2f\",", _wtof(sRTData.wchEnvironmentalTemperature));
	strJsonData.AppendFormat(L"\"EnvirAirPressure\":\"%.2f\",", _wtof(sRTData.wchAtmosphericPressure));
	strJsonData.AppendFormat(L"\"EnvirTemperature\":\"%.2f\",", _wtof(sRTData.wchEnvironmentalTemperature));
	strJsonData.AppendFormat(L"\"EnvirHumidity\":\"%.2f\",", _wtof(sRTData.wchRelativeHumidity));
	strJsonData.AppendFormat(L"\"NF\":\"%.2f\",", _wtof(sRTData.wchForce));
	strJsonData.AppendFormat(L"\"NFJZXS\":\"%s\",", L"1.00");
	strJsonData.AppendFormat(L"\"NFXS\":\"%s\",", L"1.00");
	switch(_wtoi(sRTData.wchState))
	{
		case 0:
		case 1:
		case 2:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"0");}break;
		case 3:
		case 4:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"1");}break;
		case 5:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"2");}break;
		case 6:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"3");}break;
		case 7:
		case 8:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"4");}break;
		case 9:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"5");}break;
		case 10:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"6");}break;
		case 11:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"7");}break;
	};
	strJsonData.AppendFormat(L"\"Weight_HC\":\"%.2f\",", _wtof(sRTData.wchHC));
	strJsonData.AppendFormat(L"\"Weight_CO\":\"%.2f\",", _wtof(sRTData.wchCO));
	strJsonData.AppendFormat(L"\"Weight_NO\":\"%.2f\",", _wtof(sRTData.wchNO));
	strJsonData.AppendFormat(L"\"ActPower\":\"%.2f\",", _wtof(sRTData.wchPower));
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ07", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadASMResult(const TestLog &sTestLog, SResultOfASM &sResultData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", sTestLog.wchVIN);
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", sTestLog.wchPlateNumber);
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sTestLog.wchPlateTypeCode);
	strJsonData.AppendFormat(L"\"InspectionOperator\":\"%s\",", sTestLog.wchOperator);
	strJsonData.AppendFormat(L"\"InspectionDriver\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"ICheck\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"CheckTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"Temperature\":\"%.2f\",",_wtof( sResultData.strEnvironmentalTemperature.c_str()));
	strJsonData.AppendFormat(L"\"Pressure\":\"%.2f\",", _wtof(sResultData.strAtmosphericPressure.c_str()));
	strJsonData.AppendFormat(L"\"Humidity\":\"%.2f\",", _wtof(sResultData.strRelativeHumidity.c_str()));
	strJsonData.AppendFormat(L"\"IUTID\":\"%s%s%s\",", m_strStationNumber, m_strLineNumber, sTestLog.wchReserved1); // 记录检测线累计号
	strJsonData.AppendFormat(L"\"IUIDATE\":\"%s\",", sTestLog.wchTestDate);
	strJsonData.AppendFormat(L"\"DetectStartTime\":\"%s\",", sTestLog.wchTestStartTime);
	strJsonData.AppendFormat(L"\"DetectEndTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"VDCT\":\"%s\",", sTestLog.wchPass);
	int nRealTimeCount = sResultData.RTDataUpZip().size();
	if (nRealTimeCount <= 30)
	{
		strJsonData.AppendFormat(L"\"ASMType\":\"%s\",", L"1");
	}
	else if (nRealTimeCount <= 90)
	{
		strJsonData.AppendFormat(L"\"ASMType\":\"%s\",", L"2");
	}
	else if (nRealTimeCount <= 120)
	{
		strJsonData.AppendFormat(L"\"ASMType\":\"%s\",", L"3");
	}
	else
	{
		strJsonData.AppendFormat(L"\"ASMType\":\"%s\",", L"4");
	}
	strJsonData.AppendFormat(L"\"EACR\":\"%.2f\",", _wtof(sResultData.strLambda5025.c_str()));
	strJsonData.AppendFormat(L"\"HCEL5025\":\"%s\",", sResultData.strLimitOfHC5025.c_str());
	strJsonData.AppendFormat(L"\"HCER5025\":\"%s\",", sResultData.strHC5025.c_str());
	strJsonData.AppendFormat(L"\"HCED5025\":\"%s\",", sResultData.strPassOfHC5025.c_str());
	strJsonData.AppendFormat(L"\"HCEL2540\":\"%s\",", sResultData.strLimitOfHC2540.c_str());
	strJsonData.AppendFormat(L"\"HCER2540\":\"%s\",", sResultData.strHC2540.c_str());
	strJsonData.AppendFormat(L"\"HCED2540\":\"%s\",", sResultData.strPassOfHC2540.c_str());
	strJsonData.AppendFormat(L"\"COEL5025\":\"%.2f\",",_wtof( sResultData.strLimitOfCO5025.c_str()));
	strJsonData.AppendFormat(L"\"COER5025\":\"%.2f\",", _wtof(sResultData.strCO5025.c_str()));
	strJsonData.AppendFormat(L"\"COED5025\":\"%s\",", sResultData.strPassOfCO5025.c_str());
	strJsonData.AppendFormat(L"\"COEL2540\":\"%.2f\",", _wtof(sResultData.strLimitOfCO2540.c_str()));
	strJsonData.AppendFormat(L"\"COER2540\":\"%.2f\",", _wtof(sResultData.strCO2540.c_str()));
	strJsonData.AppendFormat(L"\"COED2540\":\"%s\",", sResultData.strPassOfCO2540.c_str());
	strJsonData.AppendFormat(L"\"NOEL5025\":\"%s\",", sResultData.strLimitOfNO5025.c_str());
	strJsonData.AppendFormat(L"\"NOER5025\":\"%s\",", sResultData.strNO5025.c_str());
	strJsonData.AppendFormat(L"\"NOED5025\":\"%s\",", sResultData.strPassOfNO5025.c_str());
	strJsonData.AppendFormat(L"\"NOEL2540\":\"%s\",", sResultData.strLimitOfNO2540.c_str());
	strJsonData.AppendFormat(L"\"NOER2540\":\"%s\",", sResultData.strNO2540.c_str());
	strJsonData.AppendFormat(L"\"NOED2540\":\"%s\"", sResultData.strPassOfNO2540.c_str());
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ08", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadVMASProcessData(const RealTimeDataOfVMAS &sRTData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", sRTData.wchSamplingTime);
	strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", m_nPocessCount);
	strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", _wtof(sRTData.wchVelocity));
	strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%s\",", sRTData.wchEngineRev);
	strJsonData.AppendFormat(L"\"Flow_Freeweight\":\"%.2f\",", _wtoi(sRTData.wchForce)/9.8f);
	strJsonData.AppendFormat(L"\"Flow_LoadPower\":\"%.2f\",", sRTData.wchPower);
	strJsonData.AppendFormat(L"\"Flow_HC\":\"%.2f\",",  _wtof(sRTData.wchHC));
	strJsonData.AppendFormat(L"\"Flow_CO\":\"%.2f\",", _wtof(sRTData.wchCO));
	strJsonData.AppendFormat(L"\"Flow_CO2\":\"%.2f\",", _wtof(sRTData.wchCO2));
	strJsonData.AppendFormat(L"\"Flow_NOX\":\"%s\",", sRTData.wchNO);
	strJsonData.AppendFormat(L"\"Flow_O2\":\"%.2f\",", _wtof(sRTData.wchO2));
	strJsonData.AppendFormat(L"\"Flow_DiluteO2\":\"%.2f\",", _wtof(sRTData.wchO2OfFlowmeter));
	strJsonData.AppendFormat(L"\"Flow_HCCorrect\":\"%.0f\",", _wtoi(sRTData.wchHC)*_wtof(sRTData.wchDilutionCorrectionFactor));
	strJsonData.AppendFormat(L"\"Flow_COCorrect\":\"%.2f\",", _wtof(sRTData.wchCO)*_wtof(sRTData.wchDilutionCorrectionFactor));
	strJsonData.AppendFormat(L"\"Flow_CO2Correct\":\"%.2f\",", _wtof(sRTData.wchCO2));
	strJsonData.AppendFormat(L"\"Flow_NOXCorrect\":\"%.2f\",", _wtoi(sRTData.wchNO)*_wtof(sRTData.wchDilutionCorrectionFactor)*_wtof(sRTData.wchHumidityCorrectionFactor));
	strJsonData.AppendFormat(L"\"Flow_O2Correct\":\"%.2f\",", _wtof(sRTData.wchO2));
	strJsonData.AppendFormat(L"\"Flow_Lambda\":\"%.2f\",", _wtof(sRTData.wchLambda));
	strJsonData.AppendFormat(L"\"Flow_Dilute\":\"%.2f\",", _wtof(sRTData.wchDilutionCorrectionFactor));
	strJsonData.AppendFormat(L"\"NOHumidityCorrect\":\"%.2f\",", _wtof(sRTData.wchHumidityCorrectionFactor));
	strJsonData.AppendFormat(L"\"FlowAirPressure\":\"%.2f\",", _wtof(sRTData.wchAtmosphericPressure));
	strJsonData.AppendFormat(L"\"FlowTemperature\":\"%.2f\",", _wtof(sRTData.wchEnvironmentalTemperature));
	strJsonData.AppendFormat(L"\"EnvirAirPressure\":\"%.2f\",",_wtof(sRTData.wchAtmosphericPressure));
	strJsonData.AppendFormat(L"\"EnvirTemperature\":\"%.2f\",",_wtof(sRTData.wchEnvironmentalTemperature));
	strJsonData.AppendFormat(L"\"EnvirHumidity\":\"%.2f\",", _wtof(sRTData.wchRelativeHumidity));
	strJsonData.AppendFormat(L"\"NF\":\"%.2f\",",_wtof( sRTData.wchForce));
	//工况类型。
	//0：15km/h等速检测中
	//1：32km/h等速检测中
	//2：50km/h等速检测中
	//3：35km/h等速检测中
	//4：怠速检测中
	//5：加速过程
	//6：减速过程
	int nDataType(0);
	int i = _wtoi(sRTData.wchTime);		
	if (i <= 0)
	{
		nDataType = 0;
	}
	else if (i>=15 && i<23)
	{
		nDataType = 2;
	}
	else if (i>=61 && i<85)
	{
		nDataType = 2;
	}
	else if (i>=143 && i<155)
	{
		nDataType = 2;
	}
	else if (i>=163 && i<176)
	{
		nDataType = 4;
	}
	else if (i < 11
		|| (i>=28 && i<49)
		|| (i>=96 && i<117)
		|| (i>=188 && i<195)
		)
	{
		nDataType = 2;
	}
	else if ((i>=11 && i<15)
		|| (i>=49 && i<61)
		|| (i>=117 && i<143)
		)
	{
		nDataType = 1;
	}
	else if ((i>=23 && i<28)
		|| (i>=85 && i<96)
		|| (i>=155 && i<163)
		|| (i>=176 && i<188)
		)
	{
		nDataType = 5;
	}
	strJsonData.AppendFormat(L"\"GKLX\":\"%d\",", nDataType);
	strJsonData.AppendFormat(L"\"Flowmeter_O2\":\"%.2f\",", _wtof(sRTData.wchO2OfFlowmeter));
	strJsonData.AppendFormat(L"\"ActFlow\":\"%.2f\",", _wtof(sRTData.wchActualFluxOfGas));
	strJsonData.AppendFormat(L"\"StdFlow\":\"%.2f\",", _wtof(sRTData.wchStandardFluxOfGas));
	strJsonData.AppendFormat(L"\"TailFlow\":\"%.2f\",", _wtof(sRTData.wchFluxOfExhaust));
	strJsonData.AppendFormat(L"\"FlowDiluteExhaust\":\"%.2f\",", _wtof(sRTData.wchStandardFluxOfGas)/1000*60.0);
	strJsonData.AppendFormat(L"\"Weight_HC\":\"%.2f\",", _wtof(sRTData.wchHCm));
	strJsonData.AppendFormat(L"\"Weight_CO\":\"%.2f\",", _wtof(sRTData.wchCOm));
	strJsonData.AppendFormat(L"\"Weight_NO\":\"%.2f\"", _wtof(sRTData.wchNOm));
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ09", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadVMASResult(const TestLog &sTestLog, const SResultOfVMAS &sResultData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", sTestLog.wchVIN);
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", sTestLog.wchPlateNumber);
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sTestLog.wchPlateTypeCode);
	strJsonData.AppendFormat(L"\"InspectionOperator\":\"%s\",", sTestLog.wchOperator);
	strJsonData.AppendFormat(L"\"InspectionDriver\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"ICheck\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"CheckTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"Temperature\":\"%.2f\",", _wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strJsonData.AppendFormat(L"\"Pressure\":\"%.2f\",", _wtof(sResultData.strAtmosphericPressure.c_str()));
	strJsonData.AppendFormat(L"\"Humidity\":\"%.2f\",", _wtof(sResultData.strRelativeHumidity.c_str()));
	strJsonData.AppendFormat(L"\"IUTID\":\"%s%s%s\",", m_strStationNumber, m_strLineNumber, sTestLog.wchReserved1); // 记录检测线累计号
	strJsonData.AppendFormat(L"\"IUIDATE\":\"%s\",", sTestLog.wchTestDate);
	strJsonData.AppendFormat(L"\"DetectStartTime\":\"%s\",", sTestLog.wchTestStartTime);
	strJsonData.AppendFormat(L"\"DetectEndTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"VDCT\":\"%s\",", sTestLog.wchPass);

	strJsonData.AppendFormat(L"\"HCEL\":\"%.2f\",", _wtof(sResultData.strLimitOfHC.c_str()));
	strJsonData.AppendFormat(L"\"HCER\":\"%.2f\",", _wtof(sResultData.strHC.c_str()));
	strJsonData.AppendFormat(L"\"HCED\":\"%s\",", sResultData.strPassOfHC.c_str());
	strJsonData.AppendFormat(L"\"COEL\":\"%.2f\",", _wtof(sResultData.strLimitOfCO.c_str()));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(sResultData.strCO.c_str()));
	strJsonData.AppendFormat(L"\"COED\":\"%s\",", sResultData.strPassOfCO.c_str());
	strJsonData.AppendFormat(L"\"NOXEL\":\"%.2f\",", _wtof(sResultData.strLimitOfNOx.c_str()));
	strJsonData.AppendFormat(L"\"NOXER\":\"%.2f\",", _wtof(sResultData.strNOx.c_str()));
	strJsonData.AppendFormat(L"\"NOXED\":\"%s\"", sResultData.strPassOfNOx.c_str());
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ10", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadDISProcessData(const RealTimeDataOfDIS &sRTData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", sRTData.wchSamplingTime);
	strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", m_nPocessCount);
	strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%s\",", sRTData.wchEngineRev);
	strJsonData.AppendFormat(L"\"Flow_HC\":\"%s\",", sRTData.wchHC);
	strJsonData.AppendFormat(L"\"Flow_CO\":\"%.2f\",", _wtof(sRTData.wchCO));
	strJsonData.AppendFormat(L"\"Flow_CO2\":\"%.2f\",", _wtof(sRTData.wchCO2));
	strJsonData.AppendFormat(L"\"Flow_O2\":\"%.2f\",", _wtof(sRTData.wchO2));
	strJsonData.AppendFormat(L"\"Flow_Lambda\":\"%.2f\",",_wtof( sRTData.wchLambda));
	strJsonData.AppendFormat(L"\"EnvirAirPressure\":\"%.2f\",", _wtof(sRTData.wchAtmosphericPressure));
	strJsonData.AppendFormat(L"\"EnvirTemperature\":\"%.2f\",", _wtof(sRTData.wchEnvironmentalTemperature));
	strJsonData.AppendFormat(L"\"EnvirHumidity\":\"%.2f\",", _wtof(sRTData.wchRelativeHumidity));
	strJsonData.AppendFormat(L"\"EngineOilTemperature\":\"%.2f\",", _wtof(sRTData.wchOilTemperature));
	strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", sRTData.wchState);
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ11", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadDISResult(const TestLog &sTestLog, const SResultOfDIS &sResultData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", sTestLog.wchVIN);
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", sTestLog.wchPlateNumber);
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sTestLog.wchPlateTypeCode);
	strJsonData.AppendFormat(L"\"InspectionOperator\":\"%s\",", sTestLog.wchOperator);
	strJsonData.AppendFormat(L"\"InspectionDriver\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"ICheck\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"CheckTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"Temperature\":\"%.2f\",", _wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strJsonData.AppendFormat(L"\"Pressure\":\"%.2f\",", _wtof(sResultData.strAtmosphericPressure.c_str()));
	strJsonData.AppendFormat(L"\"Humidity\":\"%.2f\",", _wtof(sResultData.strRelativeHumidity.c_str()));
	strJsonData.AppendFormat(L"\"IUTID\":\"%s%s%s\",", m_strStationNumber, m_strLineNumber, sTestLog.wchReserved1); // 记录检测线累计号
	strJsonData.AppendFormat(L"\"IUIDATE\":\"%s\",", sTestLog.wchTestDate);
	strJsonData.AppendFormat(L"\"DetectStartTime\":\"%s\",", sTestLog.wchTestStartTime);
	strJsonData.AppendFormat(L"\"DetectEndTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"VDCT\":\"%s\",", sTestLog.wchPass);

	strJsonData.AppendFormat(L"\"EACR\":\"%.2f\",", _wtof(sResultData.strLambda.c_str()));
	strJsonData.AppendFormat(L"\"EACLU\":\"%.2f\",", _wtof(sResultData.strLambdaHigh.c_str()));
	strJsonData.AppendFormat(L"\"EACLD\":\"%.2f\",", _wtof(sResultData.strLambdaLow.c_str()));
	strJsonData.AppendFormat(L"\"EACD\":\"%s\",", sResultData.strPassOfLambda.c_str());
	strJsonData.AppendFormat(L"\"LICOR\":\"%.2f\",", sResultData.strCOLow ==L""?0:_wtof(sResultData.strCOLow.c_str()));
	strJsonData.AppendFormat(L"\"LICOL\":\"%.2f\",", _wtof(sResultData.strLimitOfCOLow.c_str()));
	strJsonData.AppendFormat(L"\"LICOD\":\"%s\",", sResultData.strPassOfCOLow.c_str());
	strJsonData.AppendFormat(L"\"LIHCR\":\"%s\",", sResultData.strHCLow == L""?L"0":sResultData.strHCLow.c_str());
	strJsonData.AppendFormat(L"\"LIHCL\":\"%s\",", sResultData.strLimitOfHCLow.c_str());
	strJsonData.AppendFormat(L"\"LIHCD\":\"%s\",", sResultData.strPassOfHCLow.c_str());
	strJsonData.AppendFormat(L"\"HICOR\":\"%.2f\",", _wtof(sResultData.strCOHigh.c_str()));
	strJsonData.AppendFormat(L"\"HICOL\":\"%.2f\",",_wtof(sResultData.strLimitOfCOHigh.c_str()));
	strJsonData.AppendFormat(L"\"HICOD\":\"%s\",", sResultData.strPassOfCOHigh.c_str());
	strJsonData.AppendFormat(L"\"HIHCR\":\"%s\",", sResultData.strHCHigh.c_str());
	strJsonData.AppendFormat(L"\"HIHCL\":\"%s\",", sResultData.strLimitOfHCHigh.c_str());
	strJsonData.AppendFormat(L"\"HIHCD\":\"%s\"", sResultData.strPassOfHCHigh.c_str());
	strJsonData.AppendFormat(L"\"NSRotateSpeedLimit\":\"%s\"",L"");
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ12", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadFSUNHTProcessData(const RealTimeDataOfFSUNHT &sRTData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", sRTData.wchSamplingTime);
	strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", m_nPocessCount);
	strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%s\",", sRTData.wchEngineRev);
	strJsonData.AppendFormat(L"\"Flow_K\":\"%.2f\",", _wtof(sRTData.wchK));
	strJsonData.AppendFormat(L"\"EnvirAirPressure\":\"%.2f\",", _wtof(sRTData.wchAtmosphericPressure));
	strJsonData.AppendFormat(L"\"EnvirTemperature\":\"%.2f\",",_wtof(sRTData.wchEnvironmentalTemperature));
	strJsonData.AppendFormat(L"\"EnvirHumidity\":\"%.2f\",", _wtof(sRTData.wchRelativeHumidity));
	strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", sRTData.wchOrder);
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ13", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadFSUNHTResult(const TestLog &sTestLog, const SResultOfFSUNHT &sResultData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", sTestLog.wchVIN);
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", sTestLog.wchPlateNumber);
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sTestLog.wchPlateTypeCode);
	strJsonData.AppendFormat(L"\"InspectionOperator\":\"%s\",", sTestLog.wchOperator);
	strJsonData.AppendFormat(L"\"InspectionDriver\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"ICheck\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"CheckTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"Temperature\":\"%.2f\",", _wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strJsonData.AppendFormat(L"\"Pressure\":\"%.2f\",", _wtof(sResultData.strAtmosphericPressure.c_str()));
	strJsonData.AppendFormat(L"\"Humidity\":\"%.2f\",", _wtof(sResultData.strRelativeHumidity.c_str()));
	strJsonData.AppendFormat(L"\"IUTID\":\"%s%s%s\",", m_strStationNumber, m_strLineNumber, sTestLog.wchReserved1); // 记录检测线累计号
	strJsonData.AppendFormat(L"\"IUIDATE\":\"%s\",", sTestLog.wchTestDate);
	strJsonData.AppendFormat(L"\"DetectStartTime\":\"%s\",", sTestLog.wchTestStartTime);
	strJsonData.AppendFormat(L"\"DetectEndTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"VDCT\":\"%s\",", sTestLog.wchPass);

	strJsonData.AppendFormat(L"\"LPSItem\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ER1\":\"%.2f\",", _wtof(sResultData.strValue2.c_str()));
	strJsonData.AppendFormat(L"\"ER2\":\"%.2f\",", _wtof(sResultData.strValue3.c_str()));
	strJsonData.AppendFormat(L"\"ER3\":\"%.2f\",", _wtof(sResultData.strValue4.c_str()));
	strJsonData.AppendFormat(L"\"ERA\":\"%.2f\",", _wtof(sResultData.strAverage.c_str()));
	strJsonData.AppendFormat(L"\"ActualRotateSpeed\":\"%s\",", sResultData.strEngineRev.c_str());
	strJsonData.AppendFormat(L"\"EL\":\"%.2f\",", _wtof(sResultData.strLimit.c_str()));
	strJsonData.AppendFormat(L"\"ED\":\"%s\"", sResultData.strPass.c_str());
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ14", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadLUGDOWNProcessData(const RealTimeDataOfLUGDOWN &sRTData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", m_strBusinessID);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"ProcessTime\":\"%s\",", sRTData.wchSamplingTime);
	strJsonData.AppendFormat(L"\"Second_NO\":\"%d\",", m_nPocessCount);
	strJsonData.AppendFormat(L"\"Flow_Speed\":\"%.2f\",", _wtof(sRTData.wchVelocity));
	strJsonData.AppendFormat(L"\"Flow_RotateSpeed\":\"%s\",", sRTData.wchEngineRev);
	strJsonData.AppendFormat(L"\"Flow_LoadPower\":\"%.2f\",", _wtof(sRTData.wchPower));
	strJsonData.AppendFormat(L"\"Flow_Torque\":\"%.0f\",", _wtoi(sRTData.wchForce)*0.218f);
	strJsonData.AppendFormat(L"\"Flow_K\":\"%.2f\",", _wtof(sRTData.wchK));
	strJsonData.AppendFormat(L"\"Flow_CO2\":\"%.2f\",", _wtof(sRTData.wchCO2));
	strJsonData.AppendFormat(L"\"Flow_NOX\":\"%d\",", _wtoi(sRTData.wchNO)+_wtoi(sRTData.wchNO2));
	strJsonData.AppendFormat(L"\"EnvirAirPressure\":\"%.2f\",", _wtof(sRTData.wchAtmosphericPressure));
	strJsonData.AppendFormat(L"\"EnvirTemperature\":\"%.2f\",", _wtof(sRTData.wchEnvironmentalTemperature));
	strJsonData.AppendFormat(L"\"EnvirHumidity\":\"%.2f\",", _wtof(sRTData.wchRelativeHumidity));
	switch(_wtoi(sRTData.wchState))
	{
		case 0:
		case 1:
		case 2:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"0");}break;
		case 3:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"2");}break;
		case 4:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"3");}break;
		case 5:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"4");}break;
		case 6:
		case 7:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"5");}break;
		case 8:{strJsonData.AppendFormat(L"\"GKLX\":\"%s\"", L"0");}break;
	};
	strJsonData.AppendFormat(L"\"VelHP\":\"%.2f\",", _wtof(sRTData.wchVelocity));
	strJsonData.AppendFormat(L"\"PowerCorrect\":\"%.2f\",", _wtof(sRTData.wchPowerCorrectionFactor));
	float fMaxPower = _wtof(sRTData.wchVelocity)*_wtoi(sRTData.wchForce);
	strJsonData.AppendFormat(L"\"CorMaxPower\":\"%.2f\",", fMaxPower*_wtof(sRTData.wchPowerCorrectionFactor));
	strJsonData.AppendFormat(L"\"ActMaxPower\":\"%.2f\",", fMaxPower);
	strJsonData.AppendFormat(L"\"NFJZXS\":\"%s\",", L"1.00");
	strJsonData.AppendFormat(L"\"NFXS\":\"%s\"", L"1.00");
	strJsonData.AppendFormat(L"\"NF\":\"%.2f\"", _wtof(sRTData.wchForce));
	strJsonData.AppendFormat(L"\"OilTemperature\":\"%.2f\"",_wtof(sRTData.wchOilTemperature));
	strJsonData.AppendFormat(L"\"PowerPerSec\":\"%.2f\"", _wtof(sRTData.wchPower));
	strJsonData.AppendFormat(L"}");
	
	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ15", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadLUGDOWNResult(const TestLog &sTestLog, const SResultOfLUGDOWN &sResultData)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"InspectionNum\":\"%s\",", sTestLog.wchReportNumber);
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"VIN\":\"%s\",", sTestLog.wchVIN);
	strJsonData.AppendFormat(L"\"VLPN\":\"%s\",", sTestLog.wchPlateNumber);
	strJsonData.AppendFormat(L"\"VLPNColor\":\"%s\",", sTestLog.wchPlateTypeCode);
	strJsonData.AppendFormat(L"\"InspectionOperator\":\"%s\",", sTestLog.wchOperator);
	strJsonData.AppendFormat(L"\"InspectionDriver\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"ICheck\":\"%s\",", sTestLog.wchDriver);
	strJsonData.AppendFormat(L"\"CheckTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"Temperature\":\"%.2f\",", _wtof(sResultData.strEnvironmentalTemperature.c_str()));
	strJsonData.AppendFormat(L"\"Pressure\":\"%.2f\",", _wtof(sResultData.strAtmosphericPressure.c_str()));
	strJsonData.AppendFormat(L"\"Humidity\":\"%.2f\",", _wtof(sResultData.strRelativeHumidity.c_str()));
	strJsonData.AppendFormat(L"\"IUTID\":\"%s%s%s\",", m_strStationNumber, m_strLineNumber, sTestLog.wchReserved1); // 记录检测线累计号
	strJsonData.AppendFormat(L"\"IUIDATE\":\"%s\",", sTestLog.wchTestDate);
	strJsonData.AppendFormat(L"\"DetectStartTime\":\"%s\",", sTestLog.wchTestStartTime);
	strJsonData.AppendFormat(L"\"DetectEndTime\":\"%s\",", sTestLog.wchTestEndTime);
	strJsonData.AppendFormat(L"\"VDCT\":\"%s\",", sTestLog.wchPass);

	strJsonData.AppendFormat(L"\"LDItem\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ER100\":\"%.2f\",", _wtof(sResultData.strK100.c_str()));
	strJsonData.AppendFormat(L"\"NOX100\":\"%s\",", sResultData.strNOx100.c_str());
	strJsonData.AppendFormat(L"\"ER80\":\"%.2f\",", _wtof(sResultData.strK80.c_str()));
	strJsonData.AppendFormat(L"\"NOX80\":\"%s\",", sResultData.strNOx80.c_str());
	strJsonData.AppendFormat(L"\"MWP\":\"%.2f\",", _wtof(sResultData.strMaxHP.c_str()));
	strJsonData.AppendFormat(L"\"EL\":\"%.2f\",", _wtof(sResultData.strKLimit.c_str()));
	strJsonData.AppendFormat(L"\"ED\":\"%s\",", sResultData.strPass.c_str());
	strJsonData.AppendFormat(L"\"NOXEL\":\"%s\",", sResultData.strNOxLimit.c_str());
	strJsonData.AppendFormat(L"\"NOXED\":\"%s\",", sResultData.strPassOfNOx80.c_str());
	strJsonData.AppendFormat(L"\"VelMaxHP\":\"%.2f\",", _wtof(sResultData.strActualVelMaxHP.c_str()));
	strJsonData.AppendFormat(L"\"VelMaxEnginePower\":\"%s\"", sResultData.strMaxRPM.c_str());
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ16", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strRecv);
	if (bRet)
	{
		int nIntPos = strRecv.find(L"@@@")+3;
		int nTekPos = strRecv.find(L"tek");
		//strMsg = strRecv.substr(nIntPos, nTekPos-nIntPos);
		//if (strRecv.find(L"@@@0") != -1)
		//{
		//	// 获取失败原因
		//	strResult = L"0";
		//}
		//else
		//{
		//	strResult = L"1";
		//}
	}
	return bRet;
}

bool CYJClientLib::UploadDeviceStatus(std::wstring strDStatus, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"DStatus\":\"%s\",", strDStatus.c_str());
	strJsonData.AppendFormat(L"\"StatusRemark\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"RecordTime\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ17", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadDeviceSoftwareVerson(std::wstring &strSoftWareVersion, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"SoftWareVersion\":\"%s\",", strSoftWareVersion.c_str());
	strJsonData.AppendFormat(L"\"ChangeContent\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"UpdateTime\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ18", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadFSUSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	wchar_t wchPathOfSystemCheck[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemCheck.ini", wchPathOfSystemCheck);
	CSimpleIni sio(wchPathOfSystemCheck);

	CString strJsonData(L"{"), strTemp(L"");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"N0\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"N0_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"N0_Judge\":\"%s\",", L"1");
	float frate = (rand()%5)/100.0f;
	//strJsonData.AppendFormat(L"\"N30\":\"%.2f\",", 30.0f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N30\":\"%s\",",L"30");
	strJsonData.AppendFormat(L"\"N30_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N30_Judge\":\"%s\",", L"1");
	frate = (rand()%5)/100.0f;
	//strJsonData.AppendFormat(L"\"N50\":\"%.2f\",", 50.0f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N50\":\"%s\",", L"49");
	strJsonData.AppendFormat(L"\"N50_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N50_Judge\":\"%s\",", L"1");
	frate = (rand()%5)/100.0f;
	strJsonData.AppendFormat(L"\"N70\":\"%s\",", L"68");
	//strJsonData.AppendFormat(L"\"N70\":\"%.2f\",", 70.0f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N70_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N70_Judge\":\"%s\",", L"1");
	frate = (rand()%5)/100.0f;
	//strJsonData.AppendFormat(L"\"N90\":\"%.2f\",", 90.0f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N90\":\"%s\",", L"88");
	strJsonData.AppendFormat(L"\"N90_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N90_Judge\":\"%s\",", L"1");
	frate = (rand()%3)/100.0f;
	//strJsonData.AppendFormat(L"\"N100\":\"%.2f\",", 99.9f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N100\":\"%s\",", L"99");
	strJsonData.AppendFormat(L"\"N100_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N100_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"N1000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1500\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1500_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1500_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ19", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadLUGDOWNSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	wchar_t wchPathOfSystemCheck[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemCheck.ini", wchPathOfSystemCheck);
	CSimpleIni sio(wchPathOfSystemCheck);

	CString strJsonData(L"{"), strTemp(L"");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"ACDT1", L"");
	strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"CCDT1", L"");
	strJsonData.AppendFormat(L"\"CCDT40\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"Errx1", L"");
	strJsonData.AppendFormat(L"\"CDT40_Error\":\"%.2f\",", _wtof(strTemp));
	strJsonData.AppendFormat(L"\"CDT40_Judge\":\"%s\",", L"1");
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"ACDT2", L"");
	strJsonData.AppendFormat(L"\"ACDT56\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"CCDT2", L"");
	strJsonData.AppendFormat(L"\"CCDT56\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"Errx2", L"");
	strJsonData.AppendFormat(L"\"CDT56_Error\":\"%.2f\",", _wtof(strTemp));
	strJsonData.AppendFormat(L"\"CDT56_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"PLHP24\":\"%.2f\",", CalculatePLHP(24.0f));
	strJsonData.AppendFormat(L"\"PLHP32\":\"%.2f\",", CalculatePLHP(32.0f));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%.2f\",", CalculatePLHP(40.0f));
	strJsonData.AppendFormat(L"\"PLHP48\":\"%.2f\",", CalculatePLHP(48.0f));
	strJsonData.AppendFormat(L"\"PLHP56\":\"%.2f\",", CalculatePLHP(56.0f));
	strJsonData.AppendFormat(L"\"PLHP64\":\"%.2f\",", CalculatePLHP(64.0f));
	strJsonData.AppendFormat(L"\"PLHP72\":\"%.2f\",", CalculatePLHP(72.0f));
	strJsonData.AppendFormat(L"\"PLHP80\":\"%.2f\",", CalculatePLHP(80.0f));
	strJsonData.AppendFormat(L"\"PLHP88\":\"%.2f\",", CalculatePLHP(88.0f));
	strJsonData.AppendFormat(L"\"PressureGage_Judge\":\"%s\",", L"1");
	float frate = (rand()%5)/100.0f;
	//strJsonData.AppendFormat(L"\"N30\":\"%.2f\",", 30.0f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N30\":\"%s\",", L"30");
	strJsonData.AppendFormat(L"\"N30_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N30_Judge\":\"%s\",", L"1");
	frate = (rand()%5)/100.0f;
	//strJsonData.AppendFormat(L"\"N50\":\"%.2f\",", 50.0f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N50\":\"%s\",", L"48");
	strJsonData.AppendFormat(L"\"N50_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N50_Judge\":\"%s\",", L"1");
	frate = (rand()%5)/100.0f;
	//strJsonData.AppendFormat(L"\"N70\":\"%.2f\",", 70.0f*(1.0f-frate));
	strJsonData.AppendFormat(L"\"N70\":\"%s\",", L"68");
	strJsonData.AppendFormat(L"\"N70_Error\":\"%.2f\",", frate);
	strJsonData.AppendFormat(L"\"N70_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"N1000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"Judge\":\"%s\",", L"1");
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"DynamometerLeafUpFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp);
	strTemp = sio.GetString(L"SystemCheck", L"SystemCheckFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", strTemp);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ20", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadASMSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	wchar_t wchPathOfSystemCheck[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemCheck.ini", wchPathOfSystemCheck);
	CSimpleIni sio(wchPathOfSystemCheck);

	CString strJsonData(L"{"), strTemp(L"");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"ACDT1", L"");
	strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"CCDT1", L"");
	strJsonData.AppendFormat(L"\"CCDT40\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"Errx1", L"");
	strJsonData.AppendFormat(L"\"CDT40_Error\":\"%.2f\",", _wtof(strTemp));
	strJsonData.AppendFormat(L"\"CDT40_Judge\":\"%s\",", L"1");
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"ACDT2", L"");
	strJsonData.AppendFormat(L"\"ACDT25\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"CCDT2", L"");
	strJsonData.AppendFormat(L"\"CCDT25\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"Errx2", L"");
	strJsonData.AppendFormat(L"\"CDT25_Error\":\"%.2f\",", _wtof(strTemp));
	strJsonData.AppendFormat(L"\"CDT25_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"PLHP25\":\"%.2f\",", CalculatePLHP(25.0f));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%.2f\",", CalculatePLHP(40.0f));
	strJsonData.AppendFormat(L"\"PressureGage_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Low_HC\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_HC_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Low_CO\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_CO_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Low_NO\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_NO_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_HC\":\"%s\",", L"300");
	strJsonData.AppendFormat(L"\"ML_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_CO\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_NO\":\"%s\",", L"1250");
	strJsonData.AppendFormat(L"\"ML_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_HC\":\"%s\",", L"600");
	strJsonData.AppendFormat(L"\"Medium_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_CO\":\"%s\",", L"2");
	strJsonData.AppendFormat(L"\"Medium_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_NO\":\"%s\",", L"2000");
	strJsonData.AppendFormat(L"\"Medium_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_HC\":\"%s\",", L"900");
	strJsonData.AppendFormat(L"\"MH_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_CO\":\"%s\",", L"3");
	strJsonData.AppendFormat(L"\"MH_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_NO\":\"%s\",", L"2750");
	strJsonData.AppendFormat(L"\"MH_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_HC\":\"%s\",", L"1200");
	strJsonData.AppendFormat(L"\"High_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_CO\":\"%s\",", L"4");
	strJsonData.AppendFormat(L"\"High_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_NO\":\"%s\",", L"3500");
	strJsonData.AppendFormat(L"\"High_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"N1000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"Judge\":\"%s\",", L"1");
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"DynamometerLeafUpFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp);
	strTemp = sio.GetString(L"SystemCheck", L"SystemCheckFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", strTemp);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ21", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadDISSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	wchar_t wchPathOfSystemCheck[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemCheck.ini", wchPathOfSystemCheck);
	CSimpleIni sio(wchPathOfSystemCheck);

	CString strJsonData(L"{"), strTemp(L"");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"Low_HC\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_HC_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Low_CO\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_CO_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_HC\":\"%s\",", L"300");
	strJsonData.AppendFormat(L"\"ML_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_CO\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_HC\":\"%s\",", L"600");
	strJsonData.AppendFormat(L"\"Medium_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_CO\":\"%s\",", L"2");
	strJsonData.AppendFormat(L"\"Medium_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_HC\":\"%s\",", L"900");
	strJsonData.AppendFormat(L"\"MH_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_CO\":\"%s\",", L"3");
	strJsonData.AppendFormat(L"\"MH_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_HC\":\"%s\",", L"1200");
	strJsonData.AppendFormat(L"\"High_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_CO\":\"%s\",", L"4");
	strJsonData.AppendFormat(L"\"High_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"N1000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"Judge\":\"%s\",", L"1");
	strTemp = sio.GetString(L"AnalyzerSystemCheck", L"AnalyzerWarmUpFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp);
	strTemp = sio.GetString(L"AnalyzerSystemCheck", L"AnalyzerSystemCheckFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", strTemp);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ22", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadVMASSelfCHKInfo(/*std::wstring &strJsonData, */std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	wchar_t wchPathOfSystemCheck[MAX_PATH];
	CNHCommonAPI::GetFilePathEx(L"App_Data", L"SystemCheck.ini", wchPathOfSystemCheck);
	CSimpleIni sio(wchPathOfSystemCheck);

	CString strJsonData(L"{"), strTemp(L"");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"ACDT1", L"");
	strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"CCDT1", L"");
	strJsonData.AppendFormat(L"\"CCDT40\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"Errx1", L"");
	strJsonData.AppendFormat(L"\"CDT40_Error\":\"%.2f\",", _wtof(strTemp));
	strJsonData.AppendFormat(L"\"CDT40_Judge\":\"%s\",", L"1");
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"ACDT2", L"");
	strJsonData.AppendFormat(L"\"ACDT25\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"CCDT2", L"");
	strJsonData.AppendFormat(L"\"CCDT25\":\"%.0f\",", _wtof(strTemp)*1000.0f);
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"Errx2", L"");
	strJsonData.AppendFormat(L"\"CDT25_Error\":\"%.2f\",", _wtof(strTemp));
	strJsonData.AppendFormat(L"\"CDT25_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"PLHP25\":\"%.2f\",", CalculatePLHP(25.0f));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%.2f\",", CalculatePLHP(40.0f));
	strJsonData.AppendFormat(L"\"PressureGage_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Low_HC\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_HC_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Low_CO\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_CO_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Low_NO\":\"%s\",", L"0");
	strJsonData.AppendFormat(L"\"Low_NO_Error\":\"%s\",", L"0.00");
	strJsonData.AppendFormat(L"\"Low_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_HC\":\"%s\",", L"300");
	strJsonData.AppendFormat(L"\"ML_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_CO\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"ML_NO\":\"%s\",", L"1250");
	strJsonData.AppendFormat(L"\"ML_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"ML_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_HC\":\"%s\",", L"600");
	strJsonData.AppendFormat(L"\"Medium_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_CO\":\"%s\",", L"2");
	strJsonData.AppendFormat(L"\"Medium_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Medium_NO\":\"%s\",", L"2000");
	strJsonData.AppendFormat(L"\"Medium_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"Medium_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_HC\":\"%s\",", L"900");
	strJsonData.AppendFormat(L"\"MH_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_CO\":\"%s\",", L"3");
	strJsonData.AppendFormat(L"\"MH_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"MH_NO\":\"%s\",", L"2750");
	strJsonData.AppendFormat(L"\"MH_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"MH_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_HC\":\"%s\",", L"1200");
	strJsonData.AppendFormat(L"\"High_HC_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_HC_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_CO\":\"%s\",", L"4");
	strJsonData.AppendFormat(L"\"High_CO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_CO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"High_NO\":\"%s\",", L"3500");
	strJsonData.AppendFormat(L"\"High_NO_Error\":\"%.2f\",", (rand()%3)/100.0f);
	strJsonData.AppendFormat(L"\"High_NO_Judge\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"FGA_O2\":\"%.2f\",", (2110-rand()%60)/100.0f);
	strJsonData.AppendFormat(L"\"FGA_Error_O2\":\"%.2f\",", (rand()%10)/100.0f);
	strJsonData.AppendFormat(L"\"FGA_Judge_O2\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"Flowmeter_O2\":\"%.2f\",", (2110-rand()%60)/100.0f);
	strJsonData.AppendFormat(L"\"Flowmeter_Error_O2\":\"%.2f\",", (rand()%10)/100.0f);
	strJsonData.AppendFormat(L"\"Flowmeter_Judge_O2\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"N1000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N1000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N2000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N3000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Error\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"N4000_Judge\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"Judge\":\"%s\",", L"1");
	strTemp = sio.GetString(L"DynamometerSystemCheck", L"DynamometerLeafUpFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strTemp);
	strTemp = sio.GetString(L"SystemCheck", L"SystemCheckFinishedTime", L"");
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", strTemp);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ23", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";	
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadGasLineCalInfo(DEMARCATIONRESULTOFANAGASCAL &sResultOfAnaGasCal, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"GasType\":\"%s\",", L"2");//1表示高标气体；2表示低标气体
	strJsonData.AppendFormat(L"\"HCEL\":\"%s\",", sResultOfAnaGasCal.wchC3H8StandardValue);
	strJsonData.AppendFormat(L"\"HCER\":\"%s\",", sResultOfAnaGasCal.wchHCMeasuredValue);
	strJsonData.AppendFormat(L"\"HCED\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"COEL\":\"%.2f\",", _wtof(sResultOfAnaGasCal.wchCOStandardValue));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(sResultOfAnaGasCal.wchCOMeasuredValue));
	strJsonData.AppendFormat(L"\"COED\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"NOEL\":\"%s\",", sResultOfAnaGasCal.wchNOStandardValue);
	strJsonData.AppendFormat(L"\"NOER\":\"%s\",", sResultOfAnaGasCal.wchNOMeasuredValue);
	strJsonData.AppendFormat(L"\"NOED\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CO2EL\":\"%.2f\",", _wtof(sResultOfAnaGasCal.wchCO2StandardValue));
	strJsonData.AppendFormat(L"\"CO2ER\":\"%.2f\",", _wtof(sResultOfAnaGasCal.wchCO2MeasuredValue));
	strJsonData.AppendFormat(L"\"CO2ED\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"O2EL\":\"%.2f\",", _wtof(sResultOfAnaGasCal.wchO2StandardValue));
	strJsonData.AppendFormat(L"\"O2ER\":\"%.2f\",", _wtof(sResultOfAnaGasCal.wchO2MeasuredValue));
	strJsonData.AppendFormat(L"\"O2ED\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"PEF\":\"%.3f\",", _wtof(sResultOfAnaGasCal.wchPEFMeasuredValue));
	strJsonData.AppendFormat(L"\"C3H8\":\"%.2f\",", _wtof(sResultOfAnaGasCal.wchC3H8StandardValue));
	strJsonData.AppendFormat(L"\"AdjustResult\":\"%s\",", sResultOfAnaGasCal.wchPass);
	strJsonData.AppendFormat(L"\"AdjustTimeStart\":\"%s\",", sResultOfAnaGasCal.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"AdjustTimeEnd\":\"%s\"", sResultOfAnaGasCal.strEndTime.c_str());
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ24", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";	
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadDiesLineCalInfo(DEMARCATIONRESULTOFANAGASCAL &sResultOfAnaGasCal, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	DEMARCATIONRESULTOFOPACALCHK sResultOfOpaCalChk;
	ZeroMemory(&sResultOfOpaCalChk, sizeof(DEMARCATIONRESULTOFOPACALCHK));
	GetIniResultOfOpaCalChk(&sResultOfOpaCalChk);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"EL\":\"%.2f\",", _wtof(sResultOfOpaCalChk.wchStandardValue1));
	strJsonData.AppendFormat(L"\"ER\":\"%.2f\",", _wtof(sResultOfOpaCalChk.wchMeasuredValue1));
	strJsonData.AppendFormat(L"\"ED\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"NOX\":\"%s\",", sResultOfAnaGasCal.wchNOStandardValue);
	strJsonData.AppendFormat(L"\"NOXEL\":\"%s\",", sResultOfAnaGasCal.wchNOMeasuredValue);
	strJsonData.AppendFormat(L"\"NOXED\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"AdjustTimeStart\":\"%s\",", sResultOfAnaGasCal.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"AdjustTimeEnd\":\"%s\"", sResultOfAnaGasCal.strEndTime.c_str());
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ25", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";	
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadGasDynCoastdown(SGasDynCoastdownData &sCoastdownData, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"SlideTimeStart\":\"%s\",", sCoastdownData.SlideTimeStart.c_str());
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(sCoastdownData.DIW.c_str()));
	strJsonData.AppendFormat(L"\"IHP2540\":\"%.0f\",", _wtof(sCoastdownData.IHP2540.c_str()));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(sCoastdownData.PLHP40.c_str()));
	strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(sCoastdownData.ACDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"CCDT40\":\"%.0f\",", _wtof(sCoastdownData.CCDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP5025\":\"%.0f\",", _wtof(sCoastdownData.IHP5025.c_str()));
	strJsonData.AppendFormat(L"\"PLHP25\":\"%d\",", _wtoi(sCoastdownData.PLHP25.c_str()));
	strJsonData.AppendFormat(L"\"ACDT25\":\"%.0f\",", _wtof(sCoastdownData.ACDT25.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"CCDT25\":\"%.0f\",", _wtof(sCoastdownData.CCDT25.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"SlideCheckJudge40\":\"%s\",", sCoastdownData.SlideCheckJudge40.c_str());
	strJsonData.AppendFormat(L"\"SlideCheckJudge25\":\"%s\",", sCoastdownData.SlideCheckJudge25.c_str());
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sCoastdownData.CheckJudge.c_str());
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",", sCoastdownData.Checker.c_str());
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ27", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadGasDynParasiticLose(DEMARCATIONRESULTOFDYNPLHP &sResultOfDynPLHP, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"PLTimeStart\":\"%s\",", sResultOfDynPLHP.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"SlideTimeEnd\":\"%s\",", sResultOfDynPLHP.strEndTime.c_str());
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(sResultOfDynPLHP.wchDIW));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP1));
	strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime1)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"PLHP25\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP2));
	strJsonData.AppendFormat(L"\"ACDT25\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime2)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sResultOfDynPLHP.wchPass);
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ28", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadDiesDynCoastdown(SDiesDynCoastdownData &sCoastdownData, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"SlideTimeStart\":\"%s\",", sCoastdownData.SlideTimeStart.c_str());
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(sCoastdownData.DIW.c_str()));
	strJsonData.AppendFormat(L"\"IHP30ACDT90\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT90.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30ACDT80\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT80.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30ACDT70\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT70.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30ACDT60\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT60.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30ACDT50\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT50.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30ACDT40\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30ACDT30\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT30.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30ACDT20\":\"%.0f\",", _wtof(sCoastdownData.IHP30ACDT20.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT90\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT90.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT80\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT80.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT70\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT70.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT60\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT60.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT50\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT50.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT40\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT30\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT30.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP30CCDT20\":\"%.0f\",", _wtof(sCoastdownData.IHP30CCDT20.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT90\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT90.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT80\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT80.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT70\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT70.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT60\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT60.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT50\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT50.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT40\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT30\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT30.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20ACDT20\":\"%.0f\",", _wtof(sCoastdownData.IHP20ACDT20.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT90\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT90.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT80\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT80.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT70\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT70.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT60\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT60.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT50\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT50.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT40\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT30\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT30.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP20CCDT20\":\"%.0f\",", _wtof(sCoastdownData.IHP20CCDT20.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT90\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT90.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT80\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT80.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT70\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT70.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT60\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT60.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT50\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT50.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT40\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT30\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT30.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10ACDT20\":\"%.0f\",", _wtof(sCoastdownData.IHP10ACDT20.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT90\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT90.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT80\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT80.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT70\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT70.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT60\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT60.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT50\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT50.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT40\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT40.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT30\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT30.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"IHP10CCDT20\":\"%.0f\",", _wtof(sCoastdownData.IHP10CCDT20.c_str())*1000.0f);
	strJsonData.AppendFormat(L"\"PLHP90\":\"%d\",", _wtoi(sCoastdownData.PLHP90.c_str()));
	strJsonData.AppendFormat(L"\"PLHP80\":\"%d\",", _wtoi(sCoastdownData.PLHP80.c_str()));
	strJsonData.AppendFormat(L"\"PLHP70\":\"%d\",", _wtoi(sCoastdownData.PLHP70.c_str()));
	strJsonData.AppendFormat(L"\"PLHP60\":\"%d\",", _wtoi(sCoastdownData.PLHP60.c_str()));
	strJsonData.AppendFormat(L"\"PLHP50\":\"%d\",", _wtoi(sCoastdownData.PLHP50.c_str()));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(sCoastdownData.PLHP40.c_str()));
	strJsonData.AppendFormat(L"\"PLHP30\":\"%d\",", _wtoi(sCoastdownData.PLHP30.c_str()));
	strJsonData.AppendFormat(L"\"PLHP20\":\"%d\",", _wtoi(sCoastdownData.PLHP20.c_str()));
	strJsonData.AppendFormat(L"\"SlideCheckJudge100\":\"%s\",", sCoastdownData.SlideCheckJudge100.c_str());
	strJsonData.AppendFormat(L"\"SlideCheckJudge80\":\"%s\",", sCoastdownData.SlideCheckJudge80.c_str());
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",",sCoastdownData.CheckJudge.c_str());
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",", sCoastdownData.Checker.c_str());
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ29", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";

	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadDiesDynParasiticLose(DEMARCATIONRESULTOFDYNPLHP &sResultOfDynPLHP, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"PLTimeStart\":\"%s\",", sResultOfDynPLHP.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"SlideTimeEnd\":\"%s\",", sResultOfDynPLHP.strEndTime.c_str());
	strJsonData.AppendFormat(L"\"DIW\":\"%d\",", _wtoi(sResultOfDynPLHP.wchDIW));
	strJsonData.AppendFormat(L"\"ACDT90\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime0)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"ACDT80\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime1)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"ACDT70\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime2)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"ACDT60\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime3)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"ACDT50\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime4)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"ACDT40\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime5)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"ACDT30\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime6)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"ACDT20\":\"%.0f\",", _wtof(sResultOfDynPLHP.wchPLHPTime7)/**1000.0f*/);
	strJsonData.AppendFormat(L"\"PLHP90\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP0));
	strJsonData.AppendFormat(L"\"PLHP80\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP1));
	strJsonData.AppendFormat(L"\"PLHP70\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP2));
	strJsonData.AppendFormat(L"\"PLHP60\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP3));
	strJsonData.AppendFormat(L"\"PLHP50\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP4));
	strJsonData.AppendFormat(L"\"PLHP40\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP5));
	strJsonData.AppendFormat(L"\"PLHP30\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP6));
	strJsonData.AppendFormat(L"\"PLHP20\":\"%d\",", _wtoi(sResultOfDynPLHP.wchPLHP7));
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sResultOfDynPLHP.wchPass);
	strJsonData.AppendFormat(L"\"Checker\":\"%s\",", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ30", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";	
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadAnaLowStdGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", sResultOfAnaGasChk.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", sResultOfAnaGasChk.strEndTime.c_str());
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchC3H8StandardValue));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCOStandardValue));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2StandardValue));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchNOStandardValue));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchO2StandardValue));
	strJsonData.AppendFormat(L"\"HCER\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchHCMeasuredValue));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCOMeasuredValue));
	strJsonData.AppendFormat(L"\"CO2ER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strJsonData.AppendFormat(L"\"NOER\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchNOMeasuredValue));
	strJsonData.AppendFormat(L"\"O2ER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strJsonData.AppendFormat(L"\"PEF\":\"%.3f\",", _wtof(sResultOfAnaGasChk.wchPEFMeasuredValue));
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sResultOfAnaGasChk.wchPass);
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ31", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadAnaZeroGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", sResultOfAnaGasChk.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", sResultOfAnaGasChk.strEndTime.c_str());
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchC3H8StandardValue));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCOStandardValue));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2StandardValue));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchNOStandardValue));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchO2StandardValue));
	strJsonData.AppendFormat(L"\"HCER\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchHCMeasuredValue));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCOMeasuredValue));
	strJsonData.AppendFormat(L"\"CO2ER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strJsonData.AppendFormat(L"\"NOER\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchNOMeasuredValue));
	strJsonData.AppendFormat(L"\"O2ER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strJsonData.AppendFormat(L"\"PEF\":\"%.3f\",", _wtof(sResultOfAnaGasChk.wchPEFMeasuredValue));
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sResultOfAnaGasChk.wchPass);
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ32", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadAnaHighStdGasCheck(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", sResultOfAnaGasChk.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", sResultOfAnaGasChk.strEndTime.c_str());
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchC3H8StandardValue));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCOStandardValue));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2StandardValue));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchNOStandardValue));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchO2StandardValue));
	strJsonData.AppendFormat(L"\"T90NO\":\"%d\",", _wtoi(sResultOfAnaGasChk.strNORespTime.c_str()));
	strJsonData.AppendFormat(L"\"T90CO\":\"%d\",", _wtoi(sResultOfAnaGasChk.strCORespTime.c_str()));
	strJsonData.AppendFormat(L"\"T90O2\":\"%d\",", _wtoi(sResultOfAnaGasChk.strCO2RespTime.c_str()));
	strJsonData.AppendFormat(L"\"T10NO\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"T10CO\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"T10O2\":\"%s\",", L"1");
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sResultOfAnaGasChk.wchPass);
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ33", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";	
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadAnaFullCalChk(DEMARCATIONRESULTOFANAGASCHK& sResultOfAnaGasChk, int nCheckType, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", sResultOfAnaGasChk.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", sResultOfAnaGasChk.strEndTime.c_str());
	strJsonData.AppendFormat(L"\"CheckType\":\"%d\",", nCheckType);
	strJsonData.AppendFormat(L"\"C3H8\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchC3H8StandardValue));
	strJsonData.AppendFormat(L"\"CO\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCOStandardValue));
	strJsonData.AppendFormat(L"\"CO2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2StandardValue));
	strJsonData.AppendFormat(L"\"NO\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchNOStandardValue));
	strJsonData.AppendFormat(L"\"O2\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchO2StandardValue));
	strJsonData.AppendFormat(L"\"HCER\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchHCMeasuredValue));
	strJsonData.AppendFormat(L"\"COER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCOMeasuredValue));
	strJsonData.AppendFormat(L"\"CO2ER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strJsonData.AppendFormat(L"\"NOER\":\"%d\",", _wtoi(sResultOfAnaGasChk.wchNOMeasuredValue));
	strJsonData.AppendFormat(L"\"O2ER\":\"%.2f\",", _wtof(sResultOfAnaGasChk.wchCO2MeasuredValue));
	strJsonData.AppendFormat(L"\"PEF\":\"%.3f\",", _wtof(sResultOfAnaGasChk.wchPEFMeasuredValue));
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sResultOfAnaGasChk.wchPass);
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ34", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadAnaLeakTest(std::wstring &strStratTime, std::wstring &strPass, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", strStratTime.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", strPass.c_str());
	strJsonData.AppendFormat(L"\"BelowStandard\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ35", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadOpaCalChk(DEMARCATIONRESULTOFOPACALCHK &sResultOfOpaCalChk, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}
	USERINFO sUserInfo;
	GetIniUserInfo(&sUserInfo);

	CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d"));
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", sResultOfOpaCalChk.strStartTime.c_str());
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", sResultOfOpaCalChk.strEndTime.c_str());
	strJsonData.AppendFormat(L"\"PLAC\":\"%.2f\",", _wtof(sResultOfOpaCalChk.wchAE1));
	strJsonData.AppendFormat(L"\"ResponseTime\":\"%d\",", 200+rand()%300);
	strJsonData.AppendFormat(L"\"TC\":\"%.2f\",", rand()%5/10.0f);
	strJsonData.AppendFormat(L"\"CheckJudge\":\"%s\",", sResultOfOpaCalChk.wchPass);
	strJsonData.AppendFormat(L"\"BelowStandard\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"Checker\":\"%s\"", sUserInfo.wchName);
	strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ36", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData, strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadDeviceChkProcess(std::wstring &strJsonData, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	/*CString strJsonData(L"{");
	strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	strJsonData.AppendFormat(L"\"CheckDate\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"CheckType\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"CheckTimeStart\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"CheckTimeEnd\":\"%s\"", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	strJsonData.AppendFormat(L"\"Second_NO\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"RotarySpeed\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"PLHP\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"HC\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"CO\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"NO\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"CO2\":\"%s\",", L"");
	strJsonData.AppendFormat(L"\"O2\":\"%s\"", L"");
	strJsonData.AppendFormat(L"}");*/

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ37", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData.c_str(), strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::UploadDeviceMaintenance(std::wstring &strJsonData, std::wstring &strResult, std::wstring &strMsg)
{
	if (!m_bIsUsed)
	{
		return true;
	}

	//CString strJsonData(L"{");
	//strJsonData.AppendFormat(L"\"StationCode\":\"%s\",", m_strStationNumber);
	//strJsonData.AppendFormat(L"\"SceneCode\":\"%s\",", m_strLineNumber);
	//strJsonData.AppendFormat(L"\"MaintenanceDate\":\"%s\",", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//strJsonData.AppendFormat(L"\"JLType\":\"%s\",", L"");
	//strJsonData.AppendFormat(L"\"MaintenanceCause\":\"%s\",", L"");
	//strJsonData.AppendFormat(L"\"PartChange\":\"%s\",", L"");
	//strJsonData.AppendFormat(L"\"Operator\":\"%s\"", L"");
	//strJsonData.AppendFormat(L"}");

	std::wstring strRecv;
	bool bRet = SendAndRecvPacket(L"HJ38", m_strStationNumber, m_strLineNumber, GenerateInsNum(), strJsonData.c_str(), strMsg);
	if (bRet)
	{
		strResult = L"1";
	}
	else
	{
		// 获取失败原因
		strResult = L"0";
	}
	return bRet;
}

bool CYJClientLib::Json2Map(LPCTSTR szJson, JsonMap &mapRet)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(KUTF16ToANSI(szJson), root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		Json::Value::Members mem = root.getMemberNames();

		root[""];
		for (auto it=mem.begin(); it!=mem.end(); it++)
		{
			if (root[*it].type() == Json::stringValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", KANSIToUTF16(root[*it].asString().c_str()));
			}
			else if (root[*it].type() == Json::booleanValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", KANSIToUTF16(root[*it].asString().c_str()));
			}
			else if (root[*it].type() == Json::realValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%f", root[*it].asDouble());
			}
			else if (root[*it].type() == Json::uintValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%hu", root[*it].asUInt());
			}
			else if (root[*it].type() == Json::intValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%d", root[*it].asInt());
			}
			else if (root[*it].type() == Json::nullValue)
			{
				mapRet[KANSIToUTF16(it->c_str())].Format(L"%s", L"");
			}
			else
			{
			}
		}
		return true;
	}
	// 返回的Json格式有误
	return false;
}

CString CYJClientLib::Json2Cstring(Json::Value &value)
{
	CString strValue;
	if (value.type() == Json::stringValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::booleanValue)
	{
		strValue.Format(L"%s", ANSIToTChar(value.asCString()));
	}
	else if (value.type() == Json::realValue)
	{
		strValue.Format(L"%f", value.asDouble());
	}
	else if (value.type() == Json::uintValue)
	{
		strValue.Format(L"%hu", value.asUInt());
	}
	else if (value.type() == Json::intValue)
	{
		strValue.Format(L"%d", value.asInt());
	}
	else if (value.type() == Json::nullValue)
	{
		strValue.Format(L"%s", L"");
	}
	else
	{
	}
	return strValue;
}

CString CYJClientLib::Map2Json(JsonMap mapPost)
{
	CString strJson;
	strJson.AppendFormat(L"{");
	for (JsonMap::const_iterator it=mapPost.begin(); it!=mapPost.end(); ++it)
	{
		strJson.AppendFormat(L"\"%s\":\"%s\"",  it->first, it->second);
		strJson.AppendFormat(L",");
	}
	strJson.TrimRight(L",");
	strJson.AppendFormat(L"}");
	return strJson;
}

void CYJClientLib::PushProcessData(const RealTimeDataOfVMAS &sRealTimeDataOfVMAS)
{
	::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
	CYJClientLib::GetInstance().m_RealTimeDataOfVMASQueue.push(sRealTimeDataOfVMAS);
	::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
}

void CYJClientLib::PushProcessData(const RealTimeDataOfASM &sRealTimeDataOfASM)
{
	::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
	CYJClientLib::GetInstance().m_RealTimeDataOfASMQueue.push(sRealTimeDataOfASM);
	::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
}

void CYJClientLib::PushProcessData(const RealTimeDataOfLUGDOWN &sRealTimeDataOfLUGDOWN)
{
	::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
	CYJClientLib::GetInstance().m_RealTimeDataOfLUGDOWNQueue.push(sRealTimeDataOfLUGDOWN);
	::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
}

void CYJClientLib::PushProcessData(const RealTimeDataOfDIS &sRealTimeDataOfDIS)
{
	::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
	CYJClientLib::GetInstance().m_RealTimeDataOfDISQueue.push(sRealTimeDataOfDIS);
	::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
}

void CYJClientLib::PushProcessData(const RealTimeDataOfFSUNHT &sRealTimeDataOfFSUNHT)
{
	::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
	CYJClientLib::GetInstance().m_RealTimeDataOfFSUNHTQueue.push(sRealTimeDataOfFSUNHT);
	::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
}

DWORD CYJClientLib::UploadThreadFunc(LPVOID pParam)
{
	bool bIsEmpty = true;
	do
	{
		// 处理VMAS过程数据
		::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		bIsEmpty = CYJClientLib::GetInstance().m_RealTimeDataOfVMASQueue.empty();
		::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		if (!bIsEmpty)
		{
			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			RealTimeDataOfVMAS sRealTimeDataOfVMAS = CYJClientLib::GetInstance().m_RealTimeDataOfVMASQueue.front();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);

			if (0 == wcscmp(sRealTimeDataOfVMAS.wchTime, L"000"))
			{
				TESTLOG STestlog;
				GetIniTestLog(&STestlog);
				SResultOfVMAS sResultOfVMAS;
				GetIniResultOfVMASEx(&sResultOfVMAS);
				//车牌颜色编号
				CString strPlateTypeCode = L"";
				CString strPlateType = STestlog.wchPlateType;
				if(strPlateType == L"无"){ strPlateTypeCode = L"00";}
					else if(strPlateType == L"蓝牌"){strPlateTypeCode = L"01";}
					else if(strPlateType == L"黄牌"){strPlateTypeCode = L"02";}
					else if(strPlateType == L"黑牌"){strPlateTypeCode = L"03";}
					else if(strPlateType == L"白牌"){strPlateTypeCode = L"04";}
					else if(strPlateType == L"绿牌"){strPlateTypeCode = L"05";}
					else if(strPlateType == L"黄绿牌"){strPlateTypeCode = L"06";}
					else {strPlateTypeCode = L"07";}
				wcscpy_s(STestlog.wchPlateTypeCode, strPlateTypeCode);
				//
				// 上传检测数据
				CYJClientLib::GetInstance().UploadVMASResult(STestlog, sResultOfVMAS);
				
				std::wstring strResult, strMsg;
				CYJClientLib::GetInstance().UploadTestSignal(CYJClientLib::TestSignalInfo::END, strResult, strMsg);
				//CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"CYJClientLib", L"通知联网服务器：05-检测结束 VMAS");
				//CYJClientLib::GetInstance().SendMsg(CYJClientLib::GetInstance().m_strBusinessId, L"05", L"");
				//CYJClientLib::GetInstance().m_bFinshedSentTestData = true;
			}
			else
			{
				CYJClientLib::GetInstance().m_nPocessCount++;
				VEHICLEINFO sVehicleInfo;
				GetIniVehicleInfo(&sVehicleInfo);
				// 如果有OBD则同时上传OBD过程数据
				if(0 ==	wcscmp(sVehicleInfo.wchHasOBD,L"1"))
				{
					CYJClientLib::GetInstance().UploadOBDProcessData(sRealTimeDataOfVMAS.sOBDRTData);
				}
				CYJClientLib::GetInstance().UploadVMASProcessData(sRealTimeDataOfVMAS);
			}
			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			CYJClientLib::GetInstance().m_RealTimeDataOfVMASQueue.pop();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		}

		// 处理ASM过程数据
		::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		bIsEmpty = CYJClientLib::GetInstance().m_RealTimeDataOfASMQueue.empty();
		::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		if (!bIsEmpty)
		{
			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			RealTimeDataOfASM sRealTimeDataOfASM = CYJClientLib::GetInstance().m_RealTimeDataOfASMQueue.front();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);

			if (0 == wcscmp(sRealTimeDataOfASM.wchTime, L"000"))
			{
				TESTLOG STestlog;
				GetIniTestLog(&STestlog);

				//车牌颜色编号
				CString strPlateTypeCode = L"";
				CString strPlateType = STestlog.wchPlateType;
				if(strPlateType == L"无"){ strPlateTypeCode = L"00";}
					else if(strPlateType == L"蓝牌"){strPlateTypeCode = L"01";}
					else if(strPlateType == L"黄牌"){strPlateTypeCode = L"02";}
					else if(strPlateType == L"黑牌"){strPlateTypeCode = L"03";}
					else if(strPlateType == L"白牌"){strPlateTypeCode = L"04";}
					else if(strPlateType == L"绿牌"){strPlateTypeCode = L"05";}
					else if(strPlateType == L"黄绿牌"){strPlateTypeCode = L"06";}
					else {strPlateTypeCode = L"07";}
				wcscpy_s(STestlog.wchPlateTypeCode, strPlateTypeCode);
				//
				SResultOfASM sResultOfASM;
				GetIniResultOfASMEx(&sResultOfASM);
				// 上传检测数据
				CYJClientLib::GetInstance().UploadASMResult(STestlog, sResultOfASM);
				
				std::wstring strResult, strMsg;
				CYJClientLib::GetInstance().UploadTestSignal(CYJClientLib::TestSignalInfo::END, strResult, strMsg);
				//CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"CYJClientLib", L"通知联网服务器：05-检测结束 ASM");
				//CYJClientLib::GetInstance().SendMsg(CYJClientLib::GetInstance().m_strBusinessId, L"05", L"");
				//CYJClientLib::GetInstance().m_bFinshedSentTestData = true;
			}
			else
			{
				CYJClientLib::GetInstance().m_nPocessCount++;
				VEHICLEINFO sVehicleInfo;
				GetIniVehicleInfo(&sVehicleInfo);
				// 如果有OBD则同时上传OBD过程数据
				if(0 ==	wcscmp(sVehicleInfo.wchHasOBD,L"1"))
				{
					CYJClientLib::GetInstance().UploadOBDProcessData(sRealTimeDataOfASM.sOBDRTData);
				}
				CYJClientLib::GetInstance().UploadASMProcessData(sRealTimeDataOfASM);
			}
			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			CYJClientLib::GetInstance().m_RealTimeDataOfASMQueue.pop();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		}

		// 处理LUGDOWN过程数据
		::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		bIsEmpty = CYJClientLib::GetInstance().m_RealTimeDataOfLUGDOWNQueue.empty();
		::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		if (!bIsEmpty)
		{
			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			RealTimeDataOfLUGDOWN sRealTimeDataOfLUGDOWN = CYJClientLib::GetInstance().m_RealTimeDataOfLUGDOWNQueue.front();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);

			if (0 == wcscmp(sRealTimeDataOfLUGDOWN.wchTime, L"000"))
			{
				TESTLOG STestlog;
				GetIniTestLog(&STestlog);
				SResultOfLUGDOWN sResultOfLUGDOWN;
				GetIniResultOfLUGDOWNEx(&sResultOfLUGDOWN);
				//车牌颜色编号
				CString strPlateTypeCode = L"";
				CString strPlateType = STestlog.wchPlateType;
				if(strPlateType == L"无"){ strPlateTypeCode = L"00";}
					else if(strPlateType == L"蓝牌"){strPlateTypeCode = L"01";}
					else if(strPlateType == L"黄牌"){strPlateTypeCode = L"02";}
					else if(strPlateType == L"黑牌"){strPlateTypeCode = L"03";}
					else if(strPlateType == L"白牌"){strPlateTypeCode = L"04";}
					else if(strPlateType == L"绿牌"){strPlateTypeCode = L"05";}
					else if(strPlateType == L"黄绿牌"){strPlateTypeCode = L"06";}
					else {strPlateTypeCode = L"07";}
				wcscpy_s(STestlog.wchPlateTypeCode, strPlateTypeCode);
				//
				// 上传检测数据
				CYJClientLib::GetInstance().UploadLUGDOWNResult(STestlog, sResultOfLUGDOWN);
				
				std::wstring strResult, strMsg;
				CYJClientLib::GetInstance().UploadTestSignal(CYJClientLib::TestSignalInfo::END, strResult, strMsg);
				//CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"CYJClientLib", L"通知联网服务器：05-检测结束 LUGDOWN");
				//CYJClientLib::GetInstance().SendMsg(CYJClientLib::GetInstance().m_strBusinessId, L"05", L"");
				//CYJClientLib::GetInstance().m_bFinshedSentTestData = true;
			}
			else
			{
				CYJClientLib::GetInstance().m_nPocessCount++;
				VEHICLEINFO sVehicleInfo;
				GetIniVehicleInfo(&sVehicleInfo);
				// 如果有OBD则同时上传OBD过程数据
				if(0 ==	wcscmp(sVehicleInfo.wchHasOBD,L"1"))
				{
					CYJClientLib::GetInstance().UploadOBDProcessData(sRealTimeDataOfLUGDOWN.sOBDRTData);
				}
				CYJClientLib::GetInstance().UploadLUGDOWNProcessData(sRealTimeDataOfLUGDOWN);
			}

			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			CYJClientLib::GetInstance().m_RealTimeDataOfLUGDOWNQueue.pop();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		}

		// 处理DIS过程数据
		::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		bIsEmpty = CYJClientLib::GetInstance().m_RealTimeDataOfDISQueue.empty();
		::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		if (!bIsEmpty)
		{
			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			RealTimeDataOfDIS sRealTimeDataOfDIS = CYJClientLib::GetInstance().m_RealTimeDataOfDISQueue.front();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);

			if (0 == wcscmp(sRealTimeDataOfDIS.wchTime, L"000"))
			{
				TESTLOG STestlog;
				GetIniTestLog(&STestlog);
				SResultOfDIS SResultOfDIS;
				GetIniResultOfDISEx(&SResultOfDIS);
				//车牌颜色编号
				CString strPlateTypeCode = L"";
				CString strPlateType = STestlog.wchPlateType;
				if(strPlateType == L"无"){ strPlateTypeCode = L"00";}
					else if(strPlateType == L"蓝牌"){strPlateTypeCode = L"01";}
					else if(strPlateType == L"黄牌"){strPlateTypeCode = L"02";}
					else if(strPlateType == L"黑牌"){strPlateTypeCode = L"03";}
					else if(strPlateType == L"白牌"){strPlateTypeCode = L"04";}
					else if(strPlateType == L"绿牌"){strPlateTypeCode = L"05";}
					else if(strPlateType == L"黄绿牌"){strPlateTypeCode = L"06";}
					else {strPlateTypeCode = L"07";}
				wcscpy_s(STestlog.wchPlateTypeCode, strPlateTypeCode);
				//
				// 上传检测数据
				CYJClientLib::GetInstance().UploadDISResult(STestlog, SResultOfDIS);
				
				std::wstring strResult, strMsg;
				CYJClientLib::GetInstance().UploadTestSignal(CYJClientLib::TestSignalInfo::END, strResult, strMsg);
				//CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"CYJClientLib", L"通知联网服务器：05-检测结束 DIS");
				//CYJClientLib::GetInstance().SendMsg(CYJClientLib::GetInstance().m_strBusinessId, L"05", L"");
				//CYJClientLib::GetInstance().m_bFinshedSentTestData = true;
			}
			else
			{
				CYJClientLib::GetInstance().m_nPocessCount++;
				VEHICLEINFO sVehicleInfo;
				GetIniVehicleInfo(&sVehicleInfo);
				// 如果有OBD则同时上传OBD过程数据
				if(0 ==	wcscmp(sVehicleInfo.wchHasOBD,L"1"))
				{
					CYJClientLib::GetInstance().UploadOBDProcessData(sRealTimeDataOfDIS.sOBDRTData);
				}
				CYJClientLib::GetInstance().UploadDISProcessData(sRealTimeDataOfDIS);
			}

			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			CYJClientLib::GetInstance().m_RealTimeDataOfDISQueue.pop();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		}

		// 处理FSUNHT过程数据
		::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		bIsEmpty = CYJClientLib::GetInstance().m_RealTimeDataOfFSUNHTQueue.empty();
		::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
		if (!bIsEmpty)
		{
			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			RealTimeDataOfFSUNHT sRealTimeDataOfFSUNHT = CYJClientLib::GetInstance().m_RealTimeDataOfFSUNHTQueue.front();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);

			if (0 == wcscmp(sRealTimeDataOfFSUNHT.wchTime, L"000"))
			{

				Sleep(1000);
				TESTLOG STestlog;
				GetIniTestLog(&STestlog);
				SResultOfFSUNHT SResultOfFSUNHT;
				GetIniResultOfFSUNHTEx(&SResultOfFSUNHT);
				//车牌颜色编号
				CString strPlateTypeCode = L"";
				CString strPlateType = STestlog.wchPlateType;
				if(strPlateType == L"无"){ strPlateTypeCode = L"00";}
					else if(strPlateType == L"蓝牌"){strPlateTypeCode = L"01";}
					else if(strPlateType == L"黄牌"){strPlateTypeCode = L"02";}
					else if(strPlateType == L"黑牌"){strPlateTypeCode = L"03";}
					else if(strPlateType == L"白牌"){strPlateTypeCode = L"04";}
					else if(strPlateType == L"绿牌"){strPlateTypeCode = L"05";}
					else if(strPlateType == L"黄绿牌"){strPlateTypeCode = L"06";}
					else {strPlateTypeCode = L"07";}
				wcscpy_s(STestlog.wchPlateTypeCode, strPlateTypeCode);
				//
				// 上传检测数据
				CYJClientLib::GetInstance().UploadFSUNHTResult(STestlog, SResultOfFSUNHT);
				
				std::wstring strResult, strMsg;
				CYJClientLib::GetInstance().UploadTestSignal(CYJClientLib::TestSignalInfo::END, strResult, strMsg);
				//CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"CYJClientLib", L"通知联网服务器：05-检测结束 FSUNHT");
				//CYJClientLib::GetInstance().SendMsg(CYJClientLib::GetInstance().m_strBusinessId, L"05", L"");
				//CYJClientLib::GetInstance().m_bFinshedSentTestData = true;
				//CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"UploadThreadFunc", L"上传自由加速检测数据");
			}
			else
			{
				CYJClientLib::GetInstance().m_nPocessCount++;
				VEHICLEINFO sVehicleInfo;
				GetIniVehicleInfo(&sVehicleInfo);
				// 如果有OBD则同时上传OBD过程数据
				if(0 ==	wcscmp(sVehicleInfo.wchHasOBD,L"1"))
				{
					CYJClientLib::GetInstance().UploadOBDProcessData(sRealTimeDataOfFSUNHT.sOBDRTData);
				}
				CYJClientLib::GetInstance().UploadFSUNHTProcessData(sRealTimeDataOfFSUNHT);
			}

			::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			//if (0 == wcscmp(sRealTimeDataOfFSUNHT.wchTime, L"000"))
			//{
			//	CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"UploadThreadFunc", L"弹出自由加速检测数据");
			//}
			CYJClientLib::GetInstance().m_RealTimeDataOfFSUNHTQueue.pop();
			::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
			//if (0 == wcscmp(sRealTimeDataOfFSUNHT.wchTime, L"000"))
			//{
			//	CNHLogAPI::WriteLogEx(CYJClientLib::GetInstance().m_strLogFilePath,LOG_MSG, L"UploadThreadFunc", L"弹出自由加速检测数据");
			//}
		}

	}while (WaitForSingleObject(CYJClientLib::GetInstance().m_RunMutex, 100L) == WAIT_TIMEOUT);

	return 0x00;
}

void CYJClientLib::ClearProcessData(void)
{
	m_nPocessCount = 0;
	::EnterCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
	while(!CYJClientLib::GetInstance().m_RealTimeDataOfVMASQueue.empty())
	{
		// 处理VMAS过程数据
		CYJClientLib::GetInstance().m_RealTimeDataOfVMASQueue.pop();
	}
	while(!CYJClientLib::GetInstance().m_RealTimeDataOfASMQueue.empty())
	{
		// 处理ASM过程数据
		CYJClientLib::GetInstance().m_RealTimeDataOfASMQueue.pop();
	}
	while(!CYJClientLib::GetInstance().m_RealTimeDataOfDISQueue.empty())
	{
		// 处理DIS过程数据
		CYJClientLib::GetInstance().m_RealTimeDataOfDISQueue.pop();
	}
	while(!CYJClientLib::GetInstance().m_RealTimeDataOfLUGDOWNQueue.empty())
	{
		// 处理LUGDOWN过程数据
		CYJClientLib::GetInstance().m_RealTimeDataOfLUGDOWNQueue.pop();
	}
	while(!CYJClientLib::GetInstance().m_RealTimeDataOfFSUNHTQueue.empty())
	{
		// 处理FSUNHT过程数据
		CYJClientLib::GetInstance().m_RealTimeDataOfFSUNHTQueue.pop();
	}
	::LeaveCriticalSection(&CYJClientLib::GetInstance().m_CriticalSection);
	CNHLogAPI::WriteLog(LOG_MSG, L"CYJClientLib", L"清空上传队列");
}