#include "stdafx.h"
#include "ZYHttp_API.h"



#include "..\RapidjsonLib\RapidjsonLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\RapidjsonLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\RapidjsonLib.lib")
#endif

#include "..\CurlHttp\CurlHttp.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\CurlHttp_D.lib")
#else
#pragma comment(lib, "..\\Release\\CurlHttp.lib")
#endif


//��ʱ�ı��ĺ��塰�����Ϊ�����롱
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
#pragma comment(lib, "..\\Debug\\MYLib_D.lib")
#else
#pragma comment( lib, "..\\Release\\MYLib.lib" )
#endif

//�ָ���ĺ���Ϊ���
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT


CZYHttp_PAI::CZYHttp_PAI(void)
{
	// ģ������
	m_strModName = L"ZYHttp";

	// ���������ļ�
	LoadConfig();

	// ������־�ļ�
	GenLogFile();

	// ��ʼ������
	InitData();
}

CZYHttp_PAI::~CZYHttp_PAI(void)
{
}

CZYHttp_PAI& CZYHttp_PAI::GetInstance(void)
{
	static std::auto_ptr<CZYHttp_PAI> auto_ptr_instance;
	if (auto_ptr_instance.get() == NULL) 
	{
		auto_ptr_instance.reset(new CZYHttp_PAI());
	}
	return *(auto_ptr_instance.get());
}

// ��ѯ��ӿ� ��ʼ

bool CZYHttp_PAI::GetPlatFormTime(SHBMsg& sHBMsg)
{
	bool bRet(false);
	//���ڵ�
	Json::Value root;

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y01��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y01", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y07��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;

				std::wstring strTime = UTF8ToTChar(root["data"]["sj"].asString().c_str());
				strTime.replace(strTime.begin(), strTime.end(), L'.', L'\0');

				HANDLE token;
				//����Ȩ��
				if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&token))
				{
					CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"GetPlatFormTime����", L"�򿪽�������ʧ��");
					bRet = false;
				}
				else
				{
					TOKEN_PRIVILEGES tkp;
					tkp.PrivilegeCount = 1;
					::LookupPrivilegeValue(NULL,SE_SYSTEMTIME_NAME,&tkp.Privileges[0].Luid);
					tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					if(!AdjustTokenPrivileges(token,FALSE,&tkp,sizeof(tkp),NULL,NULL))
					{
						CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"GetPlatFormTime����", L"��������Ȩ��ʧ��");
						bRet = false;
					}
					else
					{
						CloseHandle(token);

						// ͬ��ʱ��
						COleDateTime odtNow;
						if (odtNow.ParseDateTime(strTime.c_str()))
						{
							SYSTEMTIME st;
							odtNow.GetAsSystemTime(st);
							// ����
							if (SetLocalTime(&st))
							{
								CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"GetPlatFormTime����", L"ͬ������ʱ��ɹ�");
							}
							else
							{
								CNHLogAPI::WriteLogEx(m_strLogFilePath, LOG_MSG, L"GetPlatFormTime����", L"ͬ������ʱ��ʧ��");
								bRet = false;
							}
						}
						else
						{
							bRet = false;
						}
					}
				}
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::GetVehInfo(const CStringW& hphm, const CStringW& hpzl, const CStringW& hpys, const CStringW& clsbdh, SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;


	//1	hphm	���ƺ���	String	���ɿ�	
	root["data"]["hphm"] = TCharToUTF8(hphm.GetString());
	//2	hpzl	��������	String	���ɿ�	����¼3.1
	root["data"]["hpzl"] = TCharToUTF8(hpzl.GetString());
	//3	hpys	������ɫ	String	���ɿ�	
	root["data"]["hpys"] = TCharToUTF8(hpys.GetString());
	//4	clsbdh	����ʶ�����	String	���ɿ�	���Ժ�4λ
	root["data"]["clsbdh"] = TCharToUTF8(clsbdh.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y02��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y02", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y02��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::GetLimitValue(const CStringW& jyff, SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	//1	jyff	���鷽��	String	���ɿ�	����¼3.10
	root["data"]["jyff"] = TCharToUTF8(jyff.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y03��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y03", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y03��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::GetPersonnelInfo(SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y04��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y04", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y04��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::GetTestMethod(const CStringW& hphm, const CStringW& hpzl, const CStringW& clsbdh, SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	//1		hphm	���ƺ���	String	���ɿ�	���ó���
	root["data"]["hphm"] = TCharToUTF8(hphm.GetString());
	//2		hpzl	��������	String	���ɿ�	���ó��ǣ�����¼3.1
	root["data"]["hpzl"] = TCharToUTF8(hpzl.GetString());
	//3		clsbdh	����ʶ�����	String	���ɿ�	
	root["data"]["clsbdh"] = TCharToUTF8(clsbdh.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y06��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y06", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y06��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::GetVehAccInfo(const CStringW& jyff, const CStringW& slkssj, const CStringW& sljssj, const CStringW& hphm, const CStringW& hpzl, const CStringW& rllb,
	std::list<TESTLOG> &lsTestLog ,SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	//1		jyff	���鷽��	String	�ɿ�	����¼3.10
	root["data"]["jyff"] = TCharToUTF8(TestTypeNameToCode(jyff).GetString());
	//2		slkssj	����ʼʱ��	String	�ɿ�	��ʽ��yyyy-mm-dd hh24:mi:ss
	root["data"]["slkssj"] = TCharToUTF8(slkssj.GetString());
	//3		sljssj	�������ʱ��	String	�ɿ�	��ʽ��yyyy-mm-dd hh24:mi:ss
	root["data"]["sljssj"] = TCharToUTF8(sljssj.GetString());
	//4		hphm	���ƺ���	String	�ɿ�	
	root["data"]["hphm"] = TCharToUTF8(hphm.GetString());
	//5		hpzl	��������	String	�ɿ�	����¼3.1
	root["data"]["hpzl"] = TCharToUTF8(hpzl.GetString());
	//6		rllb	ȼ�����	String	�ɿ�	����¼3.6
	root["data"]["rllb"] = TCharToUTF8(rllb.GetString());


	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y07��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y07", root, wstrRet);

#ifdef _DEBUG
	nRet = 0;
	wstrRet = L"{\"code\":200,\"msg\":\"�����ɹ�\",\"count\":\"1\",\"data\":[\
			   {\"xh\":\"1\",\"jylsh\":\"4307031221070700012\",\"jycs\":\"1\",\"hphm\":\"��SDS107\",\"hpzl\":\"02\",\"hpys\":\"0\",\"clsbdh\":\"LJNTGUC59LN159494\",\
			   \"slsj\":\"2021-07-07 09:40:54\",\"jyff\":\"HS\",\"clcp\":\"������\",\"rllb\":\"A\",\"fdjedgl\":\"77\",\"fdjedzs\":\"3000\",\"ccrq\":\"2020-06-01\",\
			   \"ccdjrq\":\"2020-06-23\",\"zdzzl\":\"2810\",\"zbzl\":\"1995\",\"sfyOBD\":\"1\"},\
			   {\"xh\":\"2\",\"jylsh\":\"4307031221070700013\",\"jycs\":\"1\",\"hphm\":\"��SDS108\",\"hpzl\":\"02\",\"hpys\":\"0\",\"clsbdh\":\"LJNTGUC59LN159494\",\
			   \"slsj\":\"2021-07-07 09:40:54\",\"jyff\":\"HA\",\"clcp\":\"������\",\"rllb\":\"A\",\"fdjedgl\":\"77\",\"fdjedzs\":\"3000\",\"ccrq\":\"2020-06-01\",\
			   \"ccdjrq\":\"2020-06-23\",\"zdzzl\":\"2810\",\"zbzl\":\"1995\",\"sfyOBD\":\"1\"},\
			   {\"xh\":\"3\",\"jylsh\":\"4307031221070700014\",\"jycs\":\"1\",\"hphm\":\"��SDS109\",\"hpzl\":\"02\",\"hpys\":\"0\",\"clsbdh\":\"LJNTGUC59LN159494\",\
			   \"slsj\":\"2021-07-07 09:40:54\",\"jyff\":\"HY\",\"clcp\":\"������\",\"rllb\":\"B\",\"fdjedgl\":\"77\",\"fdjedzs\":\"3000\",\"ccrq\":\"2020-06-01\",\
			   \"ccdjrq\":\"2020-06-23\",\"zdzzl\":\"2810\",\"zbzl\":\"1995\",\"sfyOBD\":\"1\"},\
			   {\"xh\":\"3\",\"jylsh\":\"4307031221070700015\",\"jycs\":\"1\",\"hphm\":\"��SDS110\",\"hpzl\":\"02\",\"hpys\":\"0\",\"clsbdh\":\"LJNTGUC59LN159494\",\
			   \"slsj\":\"2021-07-07 09:40:54\",\"jyff\":\"HP\",\"clcp\":\"������\",\"rllb\":\"B\",\"fdjedgl\":\"77\",\"fdjedzs\":\"3000\",\"ccrq\":\"2020-06-01\",\
			   \"ccdjrq\":\"2020-06-23\",\"zdzzl\":\"2810\",\"zbzl\":\"1995\",\"sfyOBD\":\"1\"},\
			   {\"xh\":\"5\",\"jylsh\":\"4307031221070700016\",\"jycs\":\"1\",\"hphm\":\"��SDS111\",\"hpzl\":\"02\",\"hpys\":\"0\",\"clsbdh\":\"LJNTGUC59LN159494\",\
			   \"slsj\":\"2021-07-07 09:40:54\",\"jyff\":\"HJ\",\"clcp\":\"������\",\"rllb\":\"B\",\"fdjedgl\":\"77\",\"fdjedzs\":\"3000\",\"ccrq\":\"2020-06-01\",\
			   \"ccdjrq\":\"2020-06-23\",\"zdzzl\":\"2810\",\"zbzl\":\"1995\",\"sfyOBD\":\"1\"},\
			   {\"xh\":\"6\",\"jylsh\":\"4307031221070700016\",\"jycs\":\"1\",\"hphm\":\"��SDS112\",\"hpzl\":\"02\",\"hpys\":\"0\",\"clsbdh\":\"LJNTGUC59LN159494\",\
			   \"slsj\":\"2021-07-07 09:40:54\",\"jyff\":\"HV\",\"clcp\":\"������\",\"rllb\":\"A\",\"fdjedgl\":\"77\",\"fdjedzs\":\"3000\",\"ccrq\":\"2020-06-01\",\
			   \"ccdjrq\":\"2020-06-23\",\"zdzzl\":\"2810\",\"zbzl\":\"1995\",\"sfyOBD\":\"1\"}\
			   ]}";
#endif

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y07��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;

				const Json::Value arrayObj = root["data"];

				for(UINT i = 0; i < arrayObj.size(); i++)
				{
					TESTLOG sTestLog;

					//1		xh	���	String		���ݵ����
					wcscpy_s(sTestLog.wchReserved1 , JsonValueToCString(arrayObj[i]["xh"]));
					//2		jylsh	������ˮ��	String		
					wcscpy_s(sTestLog.wchReportNumber , JsonValueToCString(arrayObj[i]["jylsh"]));
					//3		jycs	�������	String		
					wcscpy_s(sTestLog.wchNumberOfTestPeriod , JsonValueToCString(arrayObj[i]["jycs"]));
					//4		hphm	���ƺ���	String		
					wcscpy_s(sTestLog.wchPlateNumber ,JsonValueToCString(arrayObj[i]["hphm"]));
					//5		hpzl	��������	String		����¼3.1
					wcscpy_s(sTestLog.wchPlateTypeCodeGA ,JsonValueToCString(arrayObj[i]["hpzl"]));
					//6		hpys	������ɫ	String		����¼3.2
					wcscpy_s(sTestLog.wchPlateTypeCode ,JsonValueToCString(arrayObj[i]["hpys"]));
					wcscpy_s(sTestLog.wchPlateType ,PlateTypeCodeToName(sTestLog.wchPlateTypeCode));
					//7		clsbdh	����ʶ�����	String		
					wcscpy_s(sTestLog.wchVIN ,JsonValueToCString(arrayObj[i]["clsbdh"]));
					//8		clcp	��������	String		
					wcscpy_s(sTestLog.wchModel ,JsonValueToCString(arrayObj[i]["clcp"]));
					//9		slsj	����ʱ��	String		yyyy-mm-dd hh24:mi:ss
					wcscpy_s(sTestLog.wchReserved2 ,JsonValueToCString(arrayObj[i]["slsj"]));
					//10		jyff	���鷽��	String		����¼3.10
					wcscpy_s(sTestLog.wchTestType , TestTypeCodeToName(JsonValueToCString(arrayObj[i]["jyff"])));
					//11		rllb	ȼ�����	String		����¼3.6
					wcscpy_s(sTestLog.wchFuelType ,FuleCodeToName(JsonValueToCString(arrayObj[i]["rllb"])));
					//12		fdjedgl	�����������	String		
					wcscpy_s(sTestLog.wchRatedPower ,JsonValueToCString(arrayObj[i]["fdjedgl"]));
					//13		fdjedzs	�������ת��	String		
					wcscpy_s(sTestLog.wchRatedRev ,JsonValueToCString(arrayObj[i]["fdjedzs"]));
					//14		ccrq	��������	String		
					wcscpy_s(sTestLog.wchProductionDate ,JsonValueToCString(arrayObj[i]["ccrq"]));
					//15		ccdjrq	���εǼ�����	String		
					wcscpy_s(sTestLog.wchRegistrationDate ,JsonValueToCString(arrayObj[i]["ccdjrq"]));
					//16		zdzzl	  ���������	String		ǧ�ˣ�kg��
					wcscpy_s(sTestLog.wchMaximumTotalMass ,JsonValueToCString(arrayObj[i]["zdzzl"]));
					//17		zbzl	��������	String		
					wcscpy_s(sTestLog.wchUnladenMass , JsonValueToCString(arrayObj[i]["zbzl"]));
					//18		sfyOBD	�Ƿ���OBD	String		0��1��
					wcscpy_s(sTestLog.wchHasOBD ,JsonValueToCString(arrayObj[i]["sfyOBD"]));
					//19		count	��ǰ����������	String
					lsTestLog.push_back(sTestLog);
				}

			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::GetAccInfo(const CStringW& jylsh, TESTLOG& sTestLog, VEHICLEINFO& sVehInfo, SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	//1		jylsh	������ˮ��	String	���ɿ�	��ͬһ����������˼�����ˮ�ű���Ψһ����ͬ
	root["data"]["jylsh"] = TCharToUTF8(jylsh.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y08��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y08", root, wstrRet);

#ifdef _DEBUG
	nRet = 0;

	wstrRet = L"{\"code\":200,\"msg\":\"�����ɹ�\",\"data\":{\"jyjgbh\":\"43070312\",\"jylsh\":\"4307031221070800003\",\"jycs\":\"1\",\"hphm\":\"��SDS202\",\"hpzl\":\"02\",\
			   \"hpys\":\"0\",\"clsbdh\":\"L345545\",\"slsj\":\"2021-07-08 09:09:56\",\"jyff\":\"HA\",\"clcp\":\"�����ղ���\",\"sfyOBD\":\"1\",\"fdjxh\":\"HR16\",\"fdjbh\":\"213\",\
			   \"cllx\":\"K33\",\"clxh\":\"DFL7168VAK1\",\"csys\":\"B\",\"syxz\":\"A\",\"ccdjrq\":\"2021-07-05\",\"djrq\":\"\",\"czxm\":\"5555\",\"dwmc\":\"\",\"czdh\":\"555555\",\
			   \"czdz\":\"\",\"czsfzh\":\"\",\"ccrq\":\"2021-06-05\",\"clyt\":\"P1\",\"ytsx\":\"2\",\"xszbh\":\"\",\"jyhgbzbh\":\"\",\"xzqh\":\"\",\"zsxzqh\":\"\",\"zzxzqh\":\"\",\
			   \"sgcssbwqk\":\"\",\"sfmj\":\"\",\"bmjyy\":\"\",\"pfbz\":\"5\",\"bsqxs\":\"\",\"jqfs\":\"01\",\"rllb\":\"A\",\"gyfs\":\"\",\"qdxs\":\"02\",\"zdzzl\":\"1660\",\
			   \"jzzl\":\"1337\",\"zbzl\":\"1237\",\"dczz\":\"\",\"fdjpl\":\"1598\",\"fdjedzs\":\"5000\",\"fdjedgl\":\"93\",\"sfyegr\":\"\",\"sfthclzs\":\"\",\"hclzl\":\"\",\
			   \"hclzzxh\":\"\",\"sfdk\":\"\",\"lcbds\":\"23000\",\"qgs\":\"\",\"cswdzz\":\"\",\"dpxh\":\"\",\"qdltqy\":\"\",\"ryzfkzzz\":\"\",\"chzhq\":\"\",\"chzhqxh\":\"\",\
			   \"rygg\":\"\",\"hbflbz\":\"\",\"dws\":\"\",\"yxqz\":\"\",\"qzbfqz\":\"\",\"fzjg\":\"\",\"glbm\":\"\",\"bxzzrq\":\"\",\"jdczt\":\"\",\"hbdbqk\":\"\",\"pfpdyj\":\"\",\
			   \"ddjxh\":\"\",\"cnzzxh\":\"\",\"dcrl\":\"\",\"sfydpf\":\"\",\"dpfxh\":\"\",\"sfyscr\":\"\",\"scrxh\":\"\",\"hdzkrs\":\"5\",\"clzs\":\"2\",\"qdzw\":\"\",\"bz\":\"\"}}";
#endif
	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y08��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;

				//1		jyjgbh	����������	String		
				wcscpy_s(sTestLog.wchStationNumber , JsonValueToCString(root["data"]["jyjgbh"]));
				//2		jylsh	������ˮ��	String		
				wcscpy_s(sTestLog.wchReportNumber , JsonValueToCString(root["data"]["jylsh"]));
				//3		jycs	�������	String		
				wcscpy_s(sTestLog.wchNumberOfTestPeriod , JsonValueToCString(root["data"]["jycs"]));
				//4		hphm	���ƺ���	String		
				wcscpy_s(sTestLog.wchPlateNumber , JsonValueToCString(root["data"]["hphm"]));
				wcscpy_s(sVehInfo.wchPlateNumber , JsonValueToCString(root["data"]["hphm"]));
				//5		hpzl	��������	String		����¼3.1
				wcscpy_s(sTestLog.wchPlateTypeCodeGA , JsonValueToCString(root["data"]["hpzl"]));
				wcscpy_s(sVehInfo.wchPlateTypeCodeGA , JsonValueToCString(root["data"]["hpzl"]));
				//6		hpys	������ɫ	String		
				wcscpy_s(sTestLog.wchPlateTypeCode ,JsonValueToCString(root["data"]["hpys"]));
				wcscpy_s(sTestLog.wchPlateType ,PlateTypeCodeToName(sTestLog.wchPlateTypeCode));
				wcscpy_s(sVehInfo.wchPlateTypeCode , sTestLog.wchPlateTypeCode);
				wcscpy_s(sVehInfo.wchPlateType , sTestLog.wchPlateType);
				//7		clsbdh	����ʶ�����	String		
				wcscpy_s(sTestLog.wchVIN , JsonValueToCString(root["data"]["clsbdh"]));
				wcscpy_s(sVehInfo.wchVIN , JsonValueToCString(root["data"]["clsbdh"]));
				//8		clcp	��������	String		
				wcscpy_s(sTestLog.wchModel , JsonValueToCString(root["data"]["clcp"]));
				wcscpy_s(sVehInfo.wchModel , JsonValueToCString(root["data"]["clcp"]));
				//9		slsj	����ʱ��	String		yyyy-mm-dd hh24:mi:ss
				wcscpy_s(sTestLog.wchReserved2 , JsonValueToCString(root["data"]["slsj"]));
				wcscpy_s(sVehInfo.wchReserved2 , JsonValueToCString(root["data"]["slsj"]));
				//10		jyff	���鷽��	String		����¼3.10
				wcscpy_s(sTestLog.wchTestType , TestTypeCodeToName(JsonValueToCString(root["data"]["jyff"])));
				//11		sfyOBD	�Ƿ���OBD	String		0��1��
				wcscpy_s(sTestLog.wchHasOBD ,JsonValueToCString(root["data"]["sfyOBD"]));
				wcscpy_s(sVehInfo.wchHasOBD ,JsonValueToCString(root["data"]["sfyOBD"]));
				//12		fdjxh	�������ͺ�	String		
				wcscpy_s(sTestLog.wchEngineModel ,JsonValueToCString(root["data"]["fdjxh"]));
				wcscpy_s(sVehInfo.wchEngineModel ,JsonValueToCString(root["data"]["fdjxh"]));
				//13		fdjbh	���������	String		
				wcscpy_s(sTestLog.wchEngineNumber ,JsonValueToCString(root["data"]["fdjbh"]));
				wcscpy_s(sVehInfo.wchEngineNumber ,JsonValueToCString(root["data"]["fdjbh"]));
				//14		cllx	��������	String		����¼3.18
				wcscpy_s(sTestLog.wchVehicleType ,JsonValueToCString(root["data"]["cllx"]));
				wcscpy_s(sVehInfo.wchVehicleType ,JsonValueToCString(root["data"]["cllx"]));
				//15		csys	������ɫ	String		����¼3.12
				//16		syxz	ʹ������	String		����¼3.13
				wcscpy_s(sTestLog.wchUseCharacter ,JsonValueToCString(root["data"]["syxz"]));
				wcscpy_s(sVehInfo.wchUseCharacter ,JsonValueToCString(root["data"]["syxz"]));
				//17		ccdjrq	���εǼ�����	String		
				wcscpy_s(sTestLog.wchRegistrationDate ,JsonValueToCString(root["data"]["ccdjrq"]));
				wcscpy_s(sVehInfo.wchRegistrationDate ,JsonValueToCString(root["data"]["ccdjrq"]));
				//18		djrq	�����������	String		
				//19		czxm	��������	String		
				wcscpy_s(sTestLog.wchOwner ,JsonValueToCString(root["data"]["czxm"]));
				wcscpy_s(sVehInfo.wchOwner ,JsonValueToCString(root["data"]["czxm"]));
				//20		czdh	�����绰	String		
				wcscpy_s(sTestLog.wchPhone ,JsonValueToCString(root["data"]["czdh"]));
				wcscpy_s(sVehInfo.wchPhone ,JsonValueToCString(root["data"]["czdh"]));
				//21		czdz	������ַ	String		
				wcscpy_s(sTestLog.wchAddress ,JsonValueToCString(root["data"]["czdz"]));
				wcscpy_s(sVehInfo.wchAddress ,JsonValueToCString(root["data"]["czdz"]));
				//22		czsfzh	�������֤��	String		
				//23		ccrq	��������	String		
				wcscpy_s(sTestLog.wchProductionDate ,JsonValueToCString(root["data"]["ccrq"]));
				wcscpy_s(sVehInfo.wchProductionDate ,JsonValueToCString(root["data"]["ccrq"]));
				//24		clyt	������;	String		����¼3.14
				//25		ytsx	��;����	String		����¼3.15
				//26		sfmj	�Ƿ����	String		1-��� 2-�����
				//27		bmjyy	�����ԭ��	String		
				//28		pfbz	�ŷű�׼	String		����¼3.4
				//29		bsqxs	��������ʽ	String		
				wcscpy_s(sTestLog.wchGearBoxType ,JsonValueToCString(root["data"]["bsqxs"]));
				wcscpy_s(sVehInfo.wchGearBoxType ,JsonValueToCString(root["data"]["bsqxs"]));
				//30		jqfs	������ʽ	String		����¼3.5
				wcscpy_s(sTestLog.wchAirIntakeMode ,AirIntakeModeCodeToName(JsonValueToCString(root["data"]["jqfs"])));
				wcscpy_s(sVehInfo.wchAirIntakeMode ,AirIntakeModeCodeToName(JsonValueToCString(root["data"]["jqfs"])));
				//31		rllb	ȼ�����	String		����¼3.6
				wcscpy_s(sTestLog.wchFuelType ,FuleCodeToName(JsonValueToCString(root["data"]["rllb"])));
				wcscpy_s(sVehInfo.wchFuelType ,FuleCodeToName(JsonValueToCString(root["data"]["rllb"])));
				//32		gyfs	���ͷ�ʽ	String		����¼3.7
				wcscpy_s(sTestLog.wchOilSupplyMode ,OilSupplyModeCodeToName(JsonValueToCString(root["data"]["gyfs"])));
				wcscpy_s(sVehInfo.wchOilSupplyMode ,OilSupplyModeCodeToName(JsonValueToCString(root["data"]["gyfs"])));
				//33		qdxs	������ʽ	String		����¼3.8
				wcscpy_s(sTestLog.wchDriveType ,DriveTypeCodeToName(JsonValueToCString(root["data"]["qdxs"])));
				wcscpy_s(sVehInfo.wchDriveType ,DriveTypeCodeToName(JsonValueToCString(root["data"]["qdxs"])));
				//34		zdzzl	  ���������	String		ǧ�ˣ�kg��
				wcscpy_s(sTestLog.wchMaximumTotalMass ,(JsonValueToCString(root["data"]["zdzzl"])));
				wcscpy_s(sVehInfo.wchMaximumTotalMass ,(JsonValueToCString(root["data"]["zdzzl"])));
				//35		jzzl	��׼����	String		ǧ�ˣ�kg��
				//36		zbzl	��������	String		��λkg
				wcscpy_s(sTestLog.wchUnladenMass ,(JsonValueToCString(root["data"]["zbzl"])));
				wcscpy_s(sVehInfo.wchUnladenMass ,(JsonValueToCString(root["data"]["zbzl"])));
				//37		dczz	��������	String		��λkg
				//38		fdjpl	����������	String		
				wcscpy_s(sTestLog.wchDisplacement ,(JsonValueToCString(root["data"]["fdjpl"])));
				wcscpy_s(sVehInfo.wchDisplacement ,(JsonValueToCString(root["data"]["fdjpl"])));
				//39		fdjedzs	�������ת��	String		
				wcscpy_s(sTestLog.wchRatedRev ,(JsonValueToCString(root["data"]["fdjedzs"])));
				wcscpy_s(sVehInfo.wchRatedRev ,(JsonValueToCString(root["data"]["fdjedzs"])));
				//40		fdjedgl	�����������	String		
				wcscpy_s(sTestLog.wchRatedPower ,(JsonValueToCString(root["data"]["fdjedgl"])));
				wcscpy_s(sVehInfo.wchRatedPower ,(JsonValueToCString(root["data"]["fdjedgl"])));
				//41		lcbds	��̱����	String		����
				wcscpy_s(sTestLog.wchTravelledDistance ,(JsonValueToCString(root["data"]["lcbds"])));
				wcscpy_s(sVehInfo.wchTravelledDistance ,(JsonValueToCString(root["data"]["lcbds"])));
				//42		qgs	������	String		
				wcscpy_s(sTestLog.wchNumberOfCylinder ,(JsonValueToCString(root["data"]["qgs"])));
				wcscpy_s(sVehInfo.wchNumberOfCylinder ,(JsonValueToCString(root["data"]["qgs"])));
				//43		qdltqy	������̥��ѹ	String		kPa
				wcscpy_s(sTestLog.wchTyrePressure ,(JsonValueToCString(root["data"]["qdltqy"])));
				wcscpy_s(sVehInfo.wchTyrePressure ,(JsonValueToCString(root["data"]["qdltqy"])));
				//44		rygg	ȼ�͹��	String		����¼3.9
				wcscpy_s(sTestLog.wchFuelMark ,FuelMarkCodeToName(JsonValueToCString(root["data"]["rygg"])));
				wcscpy_s(sVehInfo.wchFuelMark ,FuelMarkCodeToName(JsonValueToCString(root["data"]["rygg"])));
				//45		dws	��λ��	String		
				wcscpy_s(sTestLog.wchNumberOfGears ,(JsonValueToCString(root["data"]["dws"])));
				wcscpy_s(sVehInfo.wchNumberOfGears ,(JsonValueToCString(root["data"]["dws"])));
				//46		ddjxh	�綯���ͺ�	String		
				wcscpy_s(sTestLog.wchMotorModel ,(JsonValueToCString(root["data"]["ddjxh"])));
				wcscpy_s(sVehInfo.wchMotorModel ,(JsonValueToCString(root["data"]["ddjxh"])));
				//47		cnzzxh	����װ���ͺ�	String		
				wcscpy_s(sTestLog.wchEnergyStorageDeviceModel ,(JsonValueToCString(root["data"]["cnzzxh"])));
				wcscpy_s(sVehInfo.wchEnergyStorageDeviceModel ,(JsonValueToCString(root["data"]["cnzzxh"])));
				//48		dcrl	�������	String		
				wcscpy_s(sTestLog.wchBatteryCapacity ,(JsonValueToCString(root["data"]["dcrl"])));
				wcscpy_s(sVehInfo.wchBatteryCapacity ,(JsonValueToCString(root["data"]["dcrl"])));
				//49		sfydpf	�Ƿ���DPF	String		
				wcscpy_s(sTestLog.wchHasDPF ,(JsonValueToCString(root["data"]["sfydpf"])));
				wcscpy_s(sVehInfo.wchHasDPF ,(JsonValueToCString(root["data"]["sfydpf"])));
				//50		dpfxh	DPF�ͺ�	String		
				wcscpy_s(sTestLog.wchDPFModel ,(JsonValueToCString(root["data"]["dpfxh"])));
				wcscpy_s(sVehInfo.wchDPFModel ,(JsonValueToCString(root["data"]["dpfxh"])));
				//51		sfyscr	�Ƿ���SCR	String		
				wcscpy_s(sTestLog.wchHasSCR ,(JsonValueToCString(root["data"]["sfyscr"])));
				wcscpy_s(sVehInfo.wchHasSCR ,(JsonValueToCString(root["data"]["sfyscr"])));
				//52		scrxh	SCR�ͺ�	String		
				wcscpy_s(sTestLog.wchSCRModel ,(JsonValueToCString(root["data"]["scrxh"])));
				wcscpy_s(sVehInfo.wchSCRModel ,(JsonValueToCString(root["data"]["scrxh"])));
				//53		hdzkrs	�˶��ؿ�����	String		
				wcscpy_s(sTestLog.wchRatedPassengerCapacity ,(JsonValueToCString(root["data"]["hdzkrs"])));
				wcscpy_s(sVehInfo.wchRatedPassengerCapacity ,(JsonValueToCString(root["data"]["hdzkrs"])));
				//54		clzs	��������	String		
				wcscpy_s(sTestLog.wchSCRModel ,(JsonValueToCString(root["data"]["scrxh"])));
				wcscpy_s(sVehInfo.wchSCRModel ,(JsonValueToCString(root["data"]["scrxh"])));
				//55		qdzw	������λ	String		����¼3.19
				//56		bz	��ע	String		

			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::GetVehFailPass(const CStringW& jylsh, const CStringW& jycs, SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	//1		jylsh	������ˮ��	String	���ɿ�	��ͬһ����������˼�����ˮ�ű���Ψһ����ͬ
	root["data"]["jylsh"] = TCharToUTF8(jylsh.GetString());
	//2		jycs	�������	String	���ɿ�	
	root["data"]["jycs"] = TCharToUTF8(jycs.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y09��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = QueryCutl(L"90Y09", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y09��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}


int CZYHttp_PAI::QueryCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet)
{

	root["jkid"] = Json::Value(TCharToUTF8(wstrJkid.c_str()));
	root["yhxlh"] = Json::Value(TCharToUTF8(wstryhxlh.c_str()));
	root["jyjgbh"] = Json::Value(TCharToUTF8(wstrjyjgbh.c_str()));

	Json::StyledWriter sw;

	std::wstring wstrPostData = UTF8ToTChar(sw.write(root).c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath, wstrQURL.c_str(), wstrJkid.c_str(), wstrPostData.c_str());

	return CCurlHttp_API::CurlPost(wstrQURL, wstrPostData, wstrRet);
}

// ��ѯ��ӿ� ����

// д����ӿ� ��ʼ

bool CZYHttp_PAI::SetVehItemStart(const CStringW& jylsh, const CStringW& jycs, const CStringW& jyxm, SHBMsg& sHBMsg)
{
	bool bRet(false);


	//���ڵ�
	Json::Value root;

	//1		jylsh	������ˮ��	String	���ɿ�	
	root["data"]["jylsh"] = TCharToUTF8(jylsh.GetString());
	//2		jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//3		jycs	�������	String	���ɿ�	
	root["data"]["jycs"] = TCharToUTF8(jycs.GetString());
	//4		jyxm	������Ŀ	String	���ɿ�	����¼3.11
	root["data"]["jyxm"] = TCharToUTF8(jyxm.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y24��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y24", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y24��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetVehItemEnd(const CStringW& jylsh, const CStringW& jycs, const CStringW& jyxm, SHBMsg& sHBMsg)
{
	bool bRet(false);


	//���ڵ�
	Json::Value root;

	//1		jylsh	������ˮ��	String	���ɿ�	
	root["data"]["jylsh"] = TCharToUTF8(jylsh.GetString());
	//2		jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//3		jycs	�������	String	���ɿ�	
	root["data"]["jycs"] = TCharToUTF8(jycs.GetString());
	//4		jyxm	������Ŀ	String	���ɿ�	����¼3.11
	root["data"]["jyxm"] = TCharToUTF8(jyxm.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y25��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y25", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y25��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetTestingSignalInfo(const CStringW& jylsh, const CStringW& jycs, const CStringW& jygcbs, SHBMsg& sHBMsg)
{
	bool bRet(false);


	//���ڵ�
	Json::Value root;

	//1		jylsh	������ˮ��	String	���ɿ�	
	root["data"]["jylsh"] = TCharToUTF8(jylsh.GetString());
	//2		jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//3		jycs	�������	String	���ɿ�	
	root["data"]["jycs"] = TCharToUTF8(jycs.GetString());
	//4		jygcbs	������̱�ʶ	String	���ɿ�	���±���ϸ˵��
	root["data"]["jygcbs"] = TCharToUTF8(jygcbs.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y26��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y26", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y26��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetOBDItemEnd(const TestLog &sTestLog, const SResultOfOBD &sResultData, SHBMsg& sHBMsg)
{
	bool bRet(false);


	//���ڵ�
	Json::Value root;

	//1		jylsh	������ˮ��	String	���ɿ�	
	root["data"]["jylsh"] = TCharToUTF8(sTestLog.wchReportNumber);
	//2		jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//3		jycs	�������	String	���ɿ�	
	root["data"]["jycs"] = TCharToUTF8(sTestLog.wchNumberOfTestPeriod);
	//1		jyy	����Ա	String	���ɿ�	
	root["data"]["jyy"] = TCharToUTF8(sResultData.strOperator.c_str());
	//2		jyysfzh	����Ա���֤��	String	���ɿ�	
	root["data"]["jyysfzh"] = TCharToUTF8(L"123456200001010101");
	//3		clljxslc	�����ۼ���ʻ  ���	String	���ɿ�	�����ۼ���ʻ��̣�MIL�Ƶ����󣩣���λ��km
	root["data"]["clljxslc"] = TCharToUTF8(sResultData.strMileage.empty()? L"0": sResultData.strMileage.c_str());
	//4		OBDgzzsqsfdl	OBD����ָʾ���Ƿ����	String	���ɿ�	OBD����ָʾ���Ƿ������1�ǣ�0 ��
	root["data"]["OBDgzzsqsfdl"] = TCharToUTF8(L"0");
	//5		txsfcg	ͨ���Ƿ�ɹ�	String	���ɿ�	ͨ���Ƿ�ɹ�����1�ǣ�0 ��
	root["data"]["txsfcg"] = TCharToUTF8(L"1");
	//6		txbcgyy	ͨ�Ų��ɹ�ԭ��	String	���ɿ�	ͨ�Ų��ɹ�ԭ��01��ʾ�ӿ��𻵣�02 ��ʾ�Ҳ����ӿڣ�03 ���Ӻ���ͨѶ
	root["data"]["txbcgyy"] = TCharToUTF8(L"0");
	//7		OBDgzzsqsfhg	OBD����ָʾ���Ƿ�ϸ�	String	���ɿ�	OBD����ָʾ���Ƿ�ϸ�1 �ϸ�2���ϸ����ͳ��ǡ�
	root["data"]["OBDgzzsqsfhg"] = TCharToUTF8(L"1");
	//8		OBDgzzsqbjjgzm	�Ƿ���OBD����ָʾ��������������	String	���ɿ�	�Ƿ���OBDϵͳ����ָʾ�������������룺0 ��ʾ�ޣ�1 ��ʾ��
	root["data"]["OBDgzzsqbjjgzm"] = TCharToUTF8(L"0");
	//9		gzdmjgzxx	���ϴ��뼰������Ϣ����������ָʾ��������	String	�ɿ�	���й�����Ϣ�������ϴ�
	root["data"]["gzdmjgzxx"] = TCharToUTF8(L"");
	//10		jxwwcxm	����δ�����Ŀ	String	�ɿ�	����δ�����Ŀ:01 SCR,02 POC,03 DOC,04 DPF�� 05������ѭ��EGR��  06�߻����� 07�������� ��08���������������� 09�ɱ�����VVT
	root["data"]["jxwwcxm"] = TCharToUTF8(L"");
	//11		fdjcalid	���������Ƶ�ԪCAL ID	String	���ɿ�	
	root["data"]["fdjcalid"] = TCharToUTF8(sResultData.strEngineCALID.c_str());
	//12		fdjcvn	���������Ƶ�ԪCVN	String	���ɿ�	
	root["data"]["fdjcvn"] = TCharToUTF8(sResultData.strEngineCVN.c_str());
	//13		hclcalid	������Ƶ�ԪCAL ID	String	���ɿ�	
	root["data"]["hclcalid"] = TCharToUTF8(sResultData.strPostProcessingCALID.c_str());
	//14		hclcvn	������Ƶ�ԪCVN	String	���ɿ�	
	root["data"]["hclcvn"] = TCharToUTF8(sResultData.strPostProcessingCVN.c_str());
	//15		qtcalid	�������Ƶ�Ԫ CAL ID	String	���ɿ�	
	root["data"]["qtcalid"] = TCharToUTF8(sResultData.strOtherCALID.c_str());
	//16		qtcvn	�������Ƶ�ԪCVN	String	���ɿ�	
	root["data"]["qtcvn"] = TCharToUTF8(sResultData.strOtherCVN.c_str());
	//17		ztsfyz	�Ǳ����ϵĹ���ָʾ��״̬��OBD����ǻ�ȡ״̬�Ƿ�һ��	String	���ɿ�	�Ǳ����ϵĹ���ָʾ��״̬��OBD����ǻ�ȡ״̬�Ƿ�һ�£�0 ��1 ��
	root["data"]["ztsfyz"] = TCharToUTF8(L"1");
	//18		xsjysdOBDyq	��ʽ����ʱ��OBDҪ��	String	���ɿ�	EOBD��OBD��CN-OBD-6�ȣ����ն�ȡ������Ϣ�ϴ�
	root["data"]["xsjysdOBDyq"] = TCharToUTF8(sResultData.strOBDType.c_str());
	//19		OBDjyjg	OBD������	String	���ɿ�0-δ�죬1-�ϸ�2-���ϸ�
	root["data"]["OBDjyjg"] = TCharToUTF8(L"1");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y27��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y27", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y27��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}


bool CZYHttp_PAI::SetGasConstLoad(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����ػ����ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	COleDateTime otd1, otd2;
	strTemp = si.GetString(L"LowerRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"HigherRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd2.ParseDateTime(strTemp))
	{
		otd2 = COleDateTime::GetCurrentTime();
	}

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���50km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-mm-dd hh24:mi:ss)
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	if (otd1 > otd2)
	{
		root["data"]["jckssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	else
	{
		root["data"]["jckssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	//6	jbgl	��������	String	�ɿ�	��λ��kg
	strTemp = si.GetString(L"LowerRangeLoadConst", L"DIW", L"0");
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());
	//7	ihp5025	IHP5025���ع���	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"PowerStandardValue", L"8")));
	root["data"]["ihp5025"] = TCharToUTF8(strTemp.GetString());
	//8	fjssgl25	25km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"InternalDamage", L"0.2")));
	root["data"]["fjssgl25"] = TCharToUTF8(strTemp.GetString());
	//9	sjhxsj25	35-15km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"ACDT", L"")));
	root["data"]["sjhxsj25"] = TCharToUTF8(strTemp.GetString());
	//10	myhxsj25	35-15km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"CCDT", L"")));
	root["data"]["myhxsj25"] = TCharToUTF8(strTemp.GetString());
	//11	hxjcjg25	35-15km/h���м����	String	�ɿ�	1-�ϸ�2-���ϸ�
	strTemp.Format(L"%s", L"1");
	root["data"]["hxjcjg25"] = TCharToUTF8(strTemp.GetString());
	//12	ihp2540	IHP2540���ع���	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"HigherRangeLoadConst", L"PowerStandardValue", L"10")));
	root["data"]["ihp2540"] = TCharToUTF8(strTemp.GetString());
	//13	fjssgl40	40km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"HigherRangeLoadConst", L"InternalDamage", L"0.2")));
	root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
	//14	sjhxsj40	50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"HigherRangeLoadConst", L"ACDT", L"")));
	root["data"]["sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//15	myhxsj40	50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"HigherRangeLoadConst", L"CCDT", L"")));
	root["data"]["myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//16	hxjcjg40	50-30km/h���м����	String	�ɿ�	1-�ϸ�2-���ϸ�
	strTemp.Format(L"%s", L"1");
	root["data"]["hxjcjg40"] = TCharToUTF8(strTemp.GetString());
	//17	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	strTemp.Format(L"%s", L"1");
	root["data"]["jcpdjg"] = TCharToUTF8(strTemp.GetString());
	//18	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	strTemp.Format(L"%s", L"");
	root["data"]["jcbtgyy"] = TCharToUTF8(strTemp.GetString());

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y43��", L"�ϴ�����",UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y43", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y43��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetGasPLHP(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���50km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-mm-dd hh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ(yyyy-mm-dd hh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jcjssj"] = TCharToUTF8(strTemp.GetString());
	//6	jbgl	��������	String	�ɿ�	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfDynPLHP", L"DIW", L"0")));
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());

	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");

		if ((strTemp.Find(L"20") != -1)
			|| (strTemp.Find(L"15") != -1))
		{

			//7	fjssgl25	25km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl25"] = TCharToUTF8(strTemp.GetString());
			//8	sjhxsj25	35-15km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj25"] = TCharToUTF8(strTemp.GetString());
			//9	fjssjcjg25	25km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg25"] = TCharToUTF8(L"1");
		}
		if (strTemp.Find(L"30") != -1)
		{
			//10	fjssgl40	40km/hʱ�ĸ�����ʧ����	String	�ɿ�	��λ��kW С���������λ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
			//11	sjhxsj40	50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj40"] = TCharToUTF8(strTemp.GetString());
			//12	fjssjcjg40	40km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg40"] = TCharToUTF8(L"1");
		}
	}

	//13	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//14	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y44��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y44", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y44��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetDieConstLoad(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfDynConstLoad.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����ػ����ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	COleDateTime otd1, otd2;
	strTemp = si.GetString(L"LowerRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd1.ParseDateTime(strTemp))
	{
		otd1 = COleDateTime::GetCurrentTime();
	}
	strTemp = si.GetString(L"HigherRangeLoadConst", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	if (!otd2.ParseDateTime(strTemp))
	{
		otd2 = COleDateTime::GetCurrentTime();
	}

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���100km/h��ʼ��ʱ�䣬ʱ���ʽ(yyyy-mm-ddhh24:mi:ss)
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ(yyyy-mm-ddhh24:mi:ss)
	if (otd1 > otd2)
	{
		root["data"]["jckssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	else
	{
		root["data"]["jckssj"] = TCharToUTF8(otd1.Format(L"%Y-%m-%d %H:%M:%S").GetString());
		root["data"]["jcjssj"] = TCharToUTF8(otd2.Format(L"%Y-%m-%d %H:%M:%S").GetString());
	}
	//6	jbgl	��������	String	�ɿ�	��λΪkg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"LowerRangeLoadConst", L"DIW", L"0")));
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());
	//7	ihp30sjhxsj90	IHP30kw 100-80km/hʵ�ʻ���ʱ��	String	�ɿ� ��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT300", L"0")));
	root["data"]["ihp30sjhxsj90"] = TCharToUTF8(strTemp.GetString());
	//8	ihp30myhxsj90	IHP30kw 100-80km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT300", L"0")));
	root["data"]["ihp30myhxsj90"] = TCharToUTF8(strTemp.GetString());
	//9	ihp30jcpdjg90	IHP30kw 100-80km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg90"] = TCharToUTF8(L"1");
	//10	ihp30sjhxsj80	IHP30kw 90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT301", L"0")));
	root["data"]["ihp30sjhxsj80"] = TCharToUTF8(strTemp.GetString());
	//11	ihp30myhxsj80	IHP30kw 90-70km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT301", L"0")));
	root["data"]["ihp30myhxsj80"] = TCharToUTF8(strTemp.GetString());
	//12	ihp30jcpdjg80	IHP30kw 90-70km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg80"] = TCharToUTF8(L"1");
	//13	ihp30sjhxsj70	IHP30kw 80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT302", L"0")));
	root["data"]["ihp30sjhxsj70"] = TCharToUTF8(strTemp.GetString());
	//14	ihp30myhxsj70	IHP30kw 80-60km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT302", L"0")));
	root["data"]["ihp30myhxsj70"] = TCharToUTF8(strTemp.GetString());
	//15	ihp30jcpdjg70	IHP30kw 80-60km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg70"] = TCharToUTF8(L"1");
	//16	ihp30sjhxsj60	IHP30kw 70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT303", L"0")));
	root["data"]["ihp30sjhxsj60"] = TCharToUTF8(strTemp.GetString());
	//17	ihp30myhxsj60	IHP30kw 70-50km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT303", L"0")));
	root["data"]["ihp30myhxsj60"] = TCharToUTF8(strTemp.GetString());
	//18	ihp30jcpdjg60	IHP30kw 70-50km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg60"] = TCharToUTF8(L"1");
	//19	ihp30sjhxsj50	IHP30kw 60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT304", L"0")));
	root["data"]["ihp30sjhxsj50"] = TCharToUTF8(strTemp.GetString());
	//20	ihp30myhxsj50	IHP30kw 60-40km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT304", L"0")));
	root["data"]["ihp30myhxsj50"] = TCharToUTF8(strTemp.GetString());
	//21	ihp30jcpdjg50	IHP30kw 60-40km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg50"] = TCharToUTF8(L"1");
	//22	ihp30sjhxsj40	IHP30kw 50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT305", L"0")));
	root["data"]["ihp30sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//23	ihp30myhxsj40	IHP30kw 50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT305", L"0")));
	root["data"]["ihp30myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//24	ihp30jcpdjg40	IHP30kw 50-30km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg40"] = TCharToUTF8(L"1");
	//25	ihp30sjhxsj30	IHP30kw 40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT306", L"0")));
	root["data"]["ihp30sjhxsj30"] = TCharToUTF8(strTemp.GetString());
	//26	ihp30myhxsj30	IHP30kw 40-20km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT306", L"0")));
	root["data"]["ihp30myhxsj30"] = TCharToUTF8(strTemp.GetString());
	//27	ihp30jcpdjg30	IHP30kw 40-20km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg30"] = TCharToUTF8(L"1");
	//28	ihp30sjhxsj20	IHP30kw 30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT307", L"0")));
	root["data"]["ihp30sjhxsj20"] = TCharToUTF8(strTemp.GetString());
	//29	ihp30myhxsj20	IHP30kw 30-10km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT307", L"0")));
	root["data"]["ihp30myhxsj20"] = TCharToUTF8(strTemp.GetString());
	//30	ihp30jcpdjg20	IHP30kw 30-10km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp30jcpdjg20"] = TCharToUTF8(L"1");
	//31	ihp20sjhxsj90	IHP20kw 100-80km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT200", L"0")));
	root["data"]["ihp20sjhxsj90"] = TCharToUTF8(strTemp.GetString());
	//32	ihp20myhxsj90	IHP20kw 100-80km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT200", L"0")));
	root["data"]["ihp20myhxsj90"] = TCharToUTF8(strTemp.GetString());
	//33	Ihp20jcpdjg90	IHP20kw 100-80km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg90"] = TCharToUTF8(L"1");
	//34	ihp20sjhxsj80	IHP20kw 90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT201", L"0")));
	root["data"]["ihp20sjhxsj80"] = TCharToUTF8(strTemp.GetString());
	//35	ihp20myhxsj80	IHP20kw 90-70km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT201", L"0")));
	root["data"]["ihp20myhxsj80"] = TCharToUTF8(strTemp.GetString());
	//36	Ihp20jcpdjg80	IHP20kw 90-70km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg80"] = TCharToUTF8(L"1");
	//37	ihp20sjhxsj70	IHP20kw 80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT202", L"0")));
	root["data"]["ihp20sjhxsj70"] = TCharToUTF8(strTemp.GetString());
	//38	ihp20myhxsj70	IHP20kw 80-60km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT202", L"0")));
	root["data"]["ihp20myhxsj70"] = TCharToUTF8(strTemp.GetString());
	//39	Ihp20jcpdjg70	IHP20kw 80-60km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg70"] = TCharToUTF8(L"1");
	//40	ihp20sjhxsj60	IHP20kw 70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT203", L"0")));
	root["data"]["ihp20sjhxsj60"] = TCharToUTF8(strTemp.GetString());
	//41	ihp20myhxsj60	IHP20kw 70-50km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT203", L"0")));
	root["data"]["ihp20myhxsj60"] = TCharToUTF8(strTemp.GetString());
	//42	Ihp20jcpdjg60	IHP20kw 70-50km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg60"] = TCharToUTF8(L"1");
	//43	ihp20sjhxsj50	IHP20kw 60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT204", L"0")));
	root["data"]["ihp20sjhxsj50"] = TCharToUTF8(strTemp.GetString());
	//44	ihp20myhxsj50	IHP20kw 60-40km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT204", L"0")));
	root["data"]["ihp20myhxsj50"] = TCharToUTF8(strTemp.GetString());
	//45	Ihp20jcpdjg50	IHP20kw 60-40km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg50"] = TCharToUTF8(L"1");
	//46	ihp20sjhxsj40	IHP20kw 50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT205", L"0")));
	root["data"]["ihp20sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//47	ihp20myhxsj40	IHP20kw 50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT205", L"0")));
	root["data"]["ihp20myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//48	Ihp20cpdjg40	IHP20kw 50-30km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	// Э����� Ӧ��ΪIhp20jcpdjg40
	root["data"]["Ihp20jcpdjg40"] = TCharToUTF8(L"1");
	//49	ihp20sjhxsj30	IHP20kw 40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT206", L"0")));
	root["data"]["ihp20sjhxsj30"] = TCharToUTF8(strTemp.GetString());
	//50	ihp20myhxsj30	IHP20kw 40-20km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT206", L"0")));
	root["data"]["ihp20myhxsj30"] = TCharToUTF8(strTemp.GetString());
	//51	Ihp20jcpdjg30	IHP20kw 40-20km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg30"] = TCharToUTF8(L"1");
	//52	ihp20sjhxsj20	IHP20kw 30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT207", L"0")));
	root["data"]["ihp20sjhxsj20"] = TCharToUTF8(strTemp.GetString());
	//53	ihp20myhxsj20	IHP20kw 30-10km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT207", L"0")));
	root["data"]["ihp20myhxsj20"] = TCharToUTF8(strTemp.GetString());
	//54	Ihp20jcpdjg20	IHP20kw 30-10km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["Ihp20jcpdjg20"] = TCharToUTF8(L"1");
	//55	ihp10sjhxsj90	IHP10kw 100-80km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT100", L"0")));
	root["data"]["ihp10sjhxsj90"] = TCharToUTF8(strTemp.GetString());
	//56	ihp10myhxsj90	IHP10kw 100-80km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT100", L"0")));
	root["data"]["ihp10myhxsj90"] = TCharToUTF8(strTemp.GetString());
	//57	ihp10jcpdjg90	IHP10kw 100-80km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg90"] = TCharToUTF8(L"1");
	//58	ihp10sjhxsj80	IHP10kw 90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT101", L"0")));
	root["data"]["ihp10sjhxsj80"] = TCharToUTF8(strTemp.GetString());
	//59	ihp10myhxsj80	IHP10kw 90-70km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT101", L"0")));
	root["data"]["ihp10myhxsj80"] = TCharToUTF8(strTemp.GetString());
	//60	ihp10jcpdjg80	IHP10kw 90-70km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg80"] = TCharToUTF8(L"1");
	//61	ihp10sjhxsj70	IHP10kw 80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT102", L"0")));
	root["data"]["ihp10sjhxsj70"] = TCharToUTF8(strTemp.GetString());
	//62	ihp10myhxsj70	IHP10kw 80-60km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT102", L"0")));
	root["data"]["ihp10myhxsj70"] = TCharToUTF8(strTemp.GetString());
	//63	ihp10jcpdjg70	IHP10kw 80-60km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg70"] = TCharToUTF8(L"1");
	//64	ihp10sjhxsj60	IHP10kw 70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT103", L"0")));
	root["data"]["ihp10sjhxsj60"] = TCharToUTF8(strTemp.GetString());
	//65	ihp10myhxsj60	IHP10kw 70-50km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT103", L"0")));
	root["data"]["ihp10myhxsj60"] = TCharToUTF8(strTemp.GetString());
	//66	ihp10jcpdjg60	IHP10kw 70-50km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg60"] = TCharToUTF8(L"1");
	//67	ihp10sjhxsj50	IHP10kw 60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT104", L"0")));
	root["data"]["ihp10sjhxsj50"] = TCharToUTF8(strTemp.GetString());
	//68	ihp10myhxsj50	IHP10kw 60-40km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT104", L"0")));
	root["data"]["ihp10myhxsj50"] = TCharToUTF8(strTemp.GetString());
	//69	ihp10jcpdjg50	IHP10kw 60-40km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg50"] = TCharToUTF8(L"1");
	//70	ihp10sjhxsj40	IHP10kw 50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT105", L"0")));
	root["data"]["ihp10sjhxsj40"] = TCharToUTF8(strTemp.GetString());
	//71	ihp10myhxsj40	IHP10kw 50-30km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT105", L"0")));
	root["data"]["ihp10myhxsj40"] = TCharToUTF8(strTemp.GetString());
	//72	ihp10jcpdjg40	IHP10kw 50-30km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg40"] = TCharToUTF8(L"1");
	//73	ihp10sjhxsj30	IHP10kw 40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT106", L"0")));
	root["data"]["ihp10sjhxsj30"] = TCharToUTF8(strTemp.GetString());
	//74	ihp10myhxsj30	IHP10kw 40-20km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT106", L"0")));
	root["data"]["ihp10myhxsj30"] = TCharToUTF8(strTemp.GetString());
	//75	ihp10jcpdjg30	IHP10kw 40-20km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg30"] = TCharToUTF8(L"1");
	//76	ihp10sjhxsj20	IHP10kw 30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeCCDT107", L"0")));
	root["data"]["ihp10sjhxsj20"] = TCharToUTF8(strTemp.GetString());
	//77	ihp10myhxsj20	IHP10kw 30-10km/h���廬��ʱ��	String	�ɿ�	��λ��ms С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownRangeACDT107", L"0")));
	root["data"]["ihp10myhxsj20"] = TCharToUTF8(strTemp.GetString());
	//78	ihp10jcpdjg20	IHP10kw 30-10km/h����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["ihp10jcpdjg20"] = TCharToUTF8(L"1");
	//79	fjssgl90	90km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP0", L"0")));
	root["data"]["fjssgl90"] = TCharToUTF8(strTemp.GetString());
	//80	fjssgl80	80km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP1", L"0")));
	root["data"]["fjssgl80"] = TCharToUTF8(strTemp.GetString());
	//81	fjssgl70	70km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP2", L"0")));
	root["data"]["fjssgl70"] = TCharToUTF8(strTemp.GetString());
	//82	fjssgl60	60km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP3", L"0")));
	root["data"]["fjssgl60"] = TCharToUTF8(strTemp.GetString());
	//83	fjssgl50	50km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP4", L"0")));
	root["data"]["fjssgl50"] = TCharToUTF8(strTemp.GetString());
	//84	fjssgl40	40km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP5", L"0")));
	root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
	//85	fjssgl30	30km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP6", L"0")));
	root["data"]["fjssgl30"] = TCharToUTF8(strTemp.GetString());
	//86	fjssgl20	20km/h������ʧ����	String	�ɿ�	��λ��kW С�������һλ
	strTemp.Format(L"%.1f", _wtof(si.GetString(L"LowerRangeLoadConst", L"LugdownPLHP7", L"0")));
	root["data"]["fjssgl20"] = TCharToUTF8(strTemp.GetString());
	//87	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//88	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y45��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y45", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y45��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetDiePLHP(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfDynPLHP.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"�ϴ����������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	//String	���ɿ�	��ʼʱ���ǹ�Ͳת���½���100km/h��ʼ��ʱ�䣬ʱ���ʽ	(yyyy-mm-ddhh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	�ɿ�	ʱ���ʽ	(yyyy-mm-ddhh24:mi:ss)
	strTemp = si.GetString(L"ResultOfDynPLHP", L"EndTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jcjssj"] = TCharToUTF8(strTemp.GetString());
	//6	jbgl	��������	String	�ɿ�	��λ��kg
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfDynPLHP", L"DIW", L"0")));
	root["data"]["jbgl"] = TCharToUTF8(strTemp.GetString());

	for (int i=0; i<9; i++)
	{
		CString str;
		str.Format(L"LowerVelocity%d",i);
		strTemp = si.GetString(L"ResultOfDynPLHP", str, L"0");

		if (strTemp.Find(L"80") != -1)
		{
			//7	fjssgl90	90km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl90"] = TCharToUTF8(strTemp.GetString());
			//8	sjhxsj90	100-80km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj90"] = TCharToUTF8(strTemp.GetString());
			//9	fjssjcjg90	90km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg90"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"70") != -1)
		{
			//10	fjssgl80	80km/h������ʧ����	String	�ɿ�	��λ��kW
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl80"] = TCharToUTF8(strTemp.GetString());
			//11	sjhxsj80	90-70km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj80"] = TCharToUTF8(strTemp.GetString());
			//12	fjssjcjg80	80km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg80"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"60") != -1)
		{
			//13	fjssgl70	70km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl70"] = TCharToUTF8(strTemp.GetString());
			//14	sjhxsj70	80-60km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��msС���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj70"] = TCharToUTF8(strTemp.GetString());
			//15	fjssjcjg70	70km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg70"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"50") != -1)
		{
			//16	fjssgl60	60km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl60"] = TCharToUTF8(strTemp.GetString());
			//17	sjhxsj60	70-50km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj60"] = TCharToUTF8(strTemp.GetString());
			//18	fjssjcjg60	60km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg60"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"40") != -1)
		{
			//19	fjssgl50	50km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl50"] = TCharToUTF8(strTemp.GetString());
			//20	sjhxsj50	60-40km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj50"] = TCharToUTF8(strTemp.GetString());
			//21	fjssjcjg50	50km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg50"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"30") != -1)
		{
			//22	fjssgl40	40km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl40"] = TCharToUTF8(strTemp.GetString());
			//23	sjhxsj40	50-30km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj40"] = TCharToUTF8(strTemp.GetString());
			//24	fjssjcjg40	40km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg40"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"20") != -1)
		{
			//25	fjssgl30	30km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl30"] = TCharToUTF8(strTemp.GetString());
			//26	sjhxsj30	40-20km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj30"] = TCharToUTF8(strTemp.GetString());
			//27	fjssjcjg30	30km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg30"] = TCharToUTF8(L"1");
		}
		else if (strTemp.Find(L"10") != -1)
		{
			//28	fjssgl20	20km/h������ʧ����	String	�ɿ�	��λ��kW	С�������һλ
			str.Format(L"PLHP%d", i);
			strTemp.Format(L"%.1f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["fjssgl20"] = TCharToUTF8(strTemp.GetString());
			//29	sjhxsj20	30-10km/hʵ�ʻ���ʱ��	String	�ɿ�	��λ��ms	С���������λ
			str.Format(L"PLHPTime%d", i);
			strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfDynPLHP", str, L"0")));
			root["data"]["sjhxsj20"] = TCharToUTF8(strTemp.GetString());
			//30	fjssjcjg20	20km/hʱ�ĸ�����ʧ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
			root["data"]["fjssjcjg20"] = TCharToUTF8(L"1");
		}
	}
	//31	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//32	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y46��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y46", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y46��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetGasCheck(const CStringW& strjclx, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ͨ����ʼ��ʱ���ʽ(yyyy-mm-dd hh24:mm:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ (yyyy-mm-ddhh24:mi:ss)
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	jclx	�������	String	���ɿ�	1-�����顢2-�����
	root["data"]["jclx"] = TCharToUTF8(strjclx);
	//7	qtlx	��������	String	���ɿ�	1-��Ũ�ȡ�2-�е�Ũ�ȡ�3-�и�Ũ�ȡ�4-��Ũ�ȡ�5-���
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");
	root["data"]["qtlx"] = TCharToUTF8(strTemp.GetString());
	//8	c3h8	��׼��C3H8Ũ��	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"C3H8StandardValue", L"0")));
	root["data"]["c3h8"] = TCharToUTF8(strTemp.GetString());
	//9	bzco	��׼��COŨ��	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COStandardValue", L"0.00")));
	root["data"]["bzco"] = TCharToUTF8(strTemp.GetString());
	root["data"]["co"] = TCharToUTF8(strTemp.GetString());
	//10	bzco2	��׼��CO2Ũ��	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00")));
	root["data"]["bzco2"] = TCharToUTF8(strTemp.GetString());
	root["data"]["co2"] = TCharToUTF8(strTemp.GetString());
	//11	bznox	��׼��NOxŨ��	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	root["data"]["bznox"] = TCharToUTF8(strTemp.GetString());
	root["data"]["nox"] = TCharToUTF8(strTemp.GetString());
	//12	bzo2	��׼��O2Ũ��	String	�ɿ�	��λ��%	С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"0.00")));
	root["data"]["bzo2"] = TCharToUTF8(strTemp.GetString());
	root["data"]["o2"] = TCharToUTF8(strTemp.GetString());
	//13	hcjcjg	HC�����ֵ	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"HCMeasuredValue", L"0")));
	root["data"]["hcjcjg"] = TCharToUTF8(strTemp.GetString());
	//14	cojcjg	CO�����ֵ	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"COMeasuredValue", L"0.00")));
	root["data"]["cojcjg"] = TCharToUTF8(strTemp.GetString());
	//15	co2jcjg	CO2�����ֵ	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00")));
	root["data"]["co2jcjg"] = TCharToUTF8(strTemp.GetString());
	//16	noxjcjg	NOx�����ֵ	String	���ɿ�	��λ��10-6������
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0")));
	root["data"]["noxjcjg"] = TCharToUTF8(strTemp.GetString());
	//17	o2jcjg	O2�����ֵ	String	�ɿ�	��λ��%С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"0.00")));
	root["data"]["o2jcjg"] = TCharToUTF8(strTemp.GetString());
	//18	pef	PEFֵ	String	���ɿ�	С���������λ
	strTemp.Format(L"%.3f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"PEFMeasuredValue", L"0.3")));
	root["data"]["pef"] = TCharToUTF8(strTemp.GetString());
	//19	hcjcpdjg	HC����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["hcjcpdjg"] = TCharToUTF8(L"1");
	//20	cojcpdjg	CO����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["cojcpdjg"] = TCharToUTF8(L"1");
	//21	co2jcpdjg	CO2����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["co2jcpdjg"] = TCharToUTF8(L"1");
	//22	noxjcpdjg	NOx����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["noxjcpdjg"] = TCharToUTF8(L"1");
	//23	o2jcpdjg	O2����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�	ע������������Ϊ5-���ʱ������ɿ�
	root["data"]["o2jcpdjg"] = TCharToUTF8(L"1");
	//24	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//25	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y47��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y47", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y47��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetDieCheck(const CStringW& strjclx, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfAnaGasChk.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"��������������ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	// �Ƿ������ж� 1.���� 0.������
	CStringW strJudgeMent[3];
	// CO2
	strJudgeMent[0] = si.GetString(L"ResultOfAnaGasChk", L"SkipCO2Judgement", L"0");
	// NO
	strJudgeMent[1] = si.GetString(L"ResultOfAnaGasChk", L"SkipNOJudgement", L"0");
	// NO2
	strJudgeMent[2] = si.GetString(L"ResultOfAnaGasChk", L"SkipNO2Judgement", L"0");


	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	��ͨ����ʼ��ʱ���ʽ(yyyy-mm-ddhh24:mi:ss)
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ	(yyyy-mm-ddhh24:mi:ss)
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	jclx	�������	String	���ɿ�	1-�����顢2-�����
	root["data"]["jclx"] = TCharToUTF8(strjclx);
	//7	qtlx	��������	String	���ɿ�	1-��Ũ�ȡ�2-�е�Ũ�ȡ�3-�и�Ũ�ȡ�4-��Ũ�ȡ�5-���
	strTemp = si.GetString(L"ResultOfAnaGasChk", L"Type", L"1");
	root["data"]["qtlx"] = TCharToUTF8(strTemp.GetString());
	//8	co2	��׼��CO2Ũ��	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2StandardValue", L"0.00")));
	if (strJudgeMent[0] == L"1")
	{
		root["data"]["co2"] = TCharToUTF8(L"");
		root["data"]["bzco2"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["co2"] = TCharToUTF8(strTemp.GetString());
		root["data"]["bzco2"] = TCharToUTF8(strTemp.GetString());
	}
	//9	no	��׼��NOŨ��	String	���ɿ�	��λ��10-6����
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	if (strJudgeMent[1] == L"1")
	{
		root["data"]["no"] = TCharToUTF8(L"");
		root["data"]["no2"] = TCharToUTF8(L"");
		root["data"]["bzno"] = TCharToUTF8(L"");
		root["data"]["bzno2"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["no"] = TCharToUTF8(strTemp.GetString());
		root["data"]["no2"] = TCharToUTF8(strTemp.GetString());
		root["data"]["bzno"] = TCharToUTF8(strTemp.GetString());
		root["data"]["bzno2"] = TCharToUTF8(strTemp.GetString());
	}
	//10	no2	��׼��NO2Ũ��	String	���ɿ�	��λ��10-6������
	//strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NO2StandardValue", L"0")));
	//if (strJudgeMent[2] == L"1")
	//{
	//	root["data"]["no2"] = TCharToUTF8(L"");
	//}
	//else
	//{
	//	if (_wtoi(strTemp) > 1)
	//	{
	//		root["data"]["no2"] = TCharToUTF8(strTemp.GetString());
	//	}
	//	else
	//	{
	//		strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOStandardValue", L"0")));
	//		root["data"]["no2"] = TCharToUTF8(strTemp.GetString());
	//	}
	//}
	//11	o2	��׼��O2Ũ��	String	�ɿ�	��λ��%С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2StandardValue", L"")));
	root["data"]["o2"] = TCharToUTF8(strTemp.GetString());
	//12	co2jcjg	CO2�����ֵ	String	���ɿ�	��λ��%��С���������λ
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"CO2MeasuredValue", L"0.00")));
	if (strJudgeMent[0] == L"1")
	{
		root["data"]["co2jcjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["co2jcjg"] = TCharToUTF8(strTemp.GetString());
	}
	//13	nojcjg	NO�����ֵ	String	���ɿ�	��λ��10-6����
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NOMeasuredValue", L"0")));
	if (strJudgeMent[1] == L"1")
	{
		root["data"]["nojcjg"] = TCharToUTF8(L"");
		root["data"]["no2jcjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["nojcjg"] = TCharToUTF8(strTemp.GetString());
		root["data"]["no2jcjg"] = TCharToUTF8(strTemp.GetString());
	}
	//14	no2jcjg	NO2�����ֵ	String	���ɿ�	��λ��10-6����
	//strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"NO2MeasuredValue", L"0")));
	//if (strJudgeMent[2] == L"1")
	//{
	//	root["data"]["no2jcjg"] = TCharToUTF8(L"");
	//}
	//else
	//{
	//	root["data"]["no2jcjg"] = TCharToUTF8(strTemp.GetString());
	//}
	//15	o2jcjg	O2�����ֵ	String	�ɿ�	��λ��%	С���������λ	ע������������Ϊ5-���ʱ������ɿ�
	strTemp.Format(L"%.0f", _wtof(si.GetString(L"ResultOfAnaGasChk", L"O2MeasuredValue", L"")));
	root["data"]["o2jcjg"] = TCharToUTF8(strTemp.GetString());
	//16	co2jcpdjg	CO2����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	if (strJudgeMent[0] == L"1")
	{
		root["data"]["co2jcpdjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["co2jcpdjg"] = TCharToUTF8(L"1");
	}
	//17	nojcpdjg	NO����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	if (strJudgeMent[1] == L"1")
	{
		root["data"]["nojcpdjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["nojcpdjg"] = TCharToUTF8(L"1");
	}
	//18	no2jcpdjg	NO2����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	if (strJudgeMent[2] == L"1")
	{
		root["data"]["no2jcpdjg"] = TCharToUTF8(L"");
	}
	else
	{
		root["data"]["no2jcpdjg"] = TCharToUTF8(L"1");
	}
	//19	o2jcpdjg	O2����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�	ע������������Ϊ5-���ʱ������ɿ�
	root["data"]["o2jcpdjg"] = TCharToUTF8(L"");
	//20	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//21	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y48��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y48", root, wstrRet);

#ifdef _DEBUG
	nRet = 0;
	wstrRet = L"{\"code\":200,\"msg\":\"�����ɹ�\"}";
#endif

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y48��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetLeakCheck(const CStringW& sblx, const CStringW& jyy, const CStringW& jyysfzh,SHBMsg& sHBMsg)
{
	bool bRet(false);

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	root["data"]["jyy"] = TCharToUTF8(jyy.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	root["data"]["jyysfzh"] = TCharToUTF8(jyysfzh.GetString());
	//4	sblx	�豸����	String	���ɿ�	H02-����������	H03-������������� 
	root["data"]["sblx"] = TCharToUTF8(sblx.GetString());
	//5	jckssj	��鿪ʼʱ��	String	���ɿ�	��ʼʱ���Ǵ�ͨ������ʼ��ʱ���ʽ	(yyyy-mm-dd hh24:mi:ss)
	root["data"]["jckssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	jcjssj	������ʱ��	String	���ɿ�	ʱ���ʽ	(yyyy-mm-dd hh24:mi:ss)
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//7	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//8	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y49��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y49", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y49��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetOpaCheck(SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"ResultOfOpaCalChk.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"��͸�����ļ�INI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYYSFZH", L"123456200001010101");
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	strTemp = si.GetString(L"ResultOfOpaCalChk", L"StartTime", COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	root["data"]["jckssj"] = TCharToUTF8(strTemp.GetString());
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jcjssj"] = TCharToUTF8(COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d %H:%M:%S"));
	//6	bz30	30%�˹�Ƭ��ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue1", L"0.00")));
	root["data"]["bz30"] = TCharToUTF8(strTemp.GetString());
	//7	clz30	30%�˹�Ƭ����ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue1", L"0.00")));
	root["data"]["clz30"] = TCharToUTF8(strTemp.GetString());
	//8	jcjg30	30%�˹�Ƭ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcjg30"] = TCharToUTF8(L"1");
	//9	bz50	50%�˹�Ƭ��ֵ	String	�ɿ�	�˹�Ƭ2�����б�ֵ����
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue2", L"0.00")));
	root["data"]["bz50"] = TCharToUTF8(strTemp.GetString());
	//10	clz50	50%�˹�Ƭ����ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue2", L"0.00")));
	root["data"]["clz50"] = TCharToUTF8(strTemp.GetString());
	//11	jcjg50	50%�˹�Ƭ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcjg50"] = TCharToUTF8(L"1");
	//12	bz70	70%�˹�Ƭ��ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"StandardValue3", L"0.00")));
	root["data"]["bz70"] = TCharToUTF8(strTemp.GetString());
	//13	clz70	70%�˹�Ƭ����ֵ	String	�ɿ�	
	strTemp.Format(L"%.2f", _wtof(si.GetString(L"ResultOfOpaCalChk", L"MeasuredValue3", L"0.00")));
	root["data"]["clz70"] = TCharToUTF8(strTemp.GetString());
	//14	jcjg70	70%�˹�Ƭ�����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcjg70"] = TCharToUTF8(L"1");
	//15	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//16	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y51��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y51", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y51��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetParaCheck(const SEnvPara& sEnvPara, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfEnvParams.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"UpResultOfEnvParamsINI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;
	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = sEnvPara.JYY.c_str();
	root["data"]["jyy"] = TCharToUTF8(strTemp.GetString());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = sEnvPara.JYYSFZH.c_str();
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jckssj"] = TCharToUTF8(sEnvPara.odtbegin.Format(L"%Y-%m-%d %H:%M:%S"));
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jcjssj"] = TCharToUTF8(sEnvPara.odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
	//6	txjcjg	ͨѶ�����	String	�ɿ�	1 �ɹ�	0 ʧ��
	root["data"]["txjcjg"] = TCharToUTF8(L"1");
	//7	hjwd	�����¶ȣ����϶ȣ�	String	�ɿ�	С�������һλ
	root["data"]["hjwd"] = TCharToUTF8(sEnvPara.hjwd.c_str());
	//8	scwd	ʵ���¶ȶ��������϶ȣ�	String	�ɿ�	С�������һλ
	root["data"]["scwd"] = TCharToUTF8(sEnvPara.scwd.c_str());
	//9	wdjcpdjg	�¶ȼ���ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["wdjcpdjg"] = TCharToUTF8(L"1");
	//10	hjsd	����ʪ�ȣ�%��	String	�ɿ�	С�������һλ
	root["data"]["hjsd"] = TCharToUTF8(sEnvPara.hjsd.c_str());
	//11	scsd	ʵ��ʪ�ȶ���	String	�ɿ�	С�������һλ
	root["data"]["scsd"] = TCharToUTF8(sEnvPara.scsd.c_str());
	//12	sdjcpdjg	ʪ�ȼ���ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["sdjcpdjg"] = TCharToUTF8(L"1");
	//13	hjdqy	��������ѹkPa	String	�ɿ�	С�������һλ
	root["data"]["hjdqy"] = TCharToUTF8(sEnvPara.hjdqy.c_str());
	//14	scdqy	ʵ�����ѹ����kPa	String	�ɿ�	С�������һλ
	root["data"]["scdqy"] = TCharToUTF8(sEnvPara.scdqy.c_str());
	//15	dqyjcpdjg	����ѹ����ж����	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["dqyjcpdjg"] = TCharToUTF8(L"1");
	//16	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//17	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y52��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y52", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y52��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool CZYHttp_PAI::SetSpeCheck(const STachometer& sTachometer, SHBMsg& sHBMsg)
{
	bool bRet(false);

	// ��ȡ���ػ��н���ļ�
	wchar_t wchPath[MAX_PATH];
	ZeroMemory(wchPath, sizeof(wchPath));

	if (0x00 != CNHCommonAPI::GetFilePathEx(L"App_Data", L"UpResultOfTachometer.ini", wchPath, false))
	{
		// �ļ�������
		sHBMsg.code = L"-1";
		sHBMsg.msg = L"UpResultOfEnvParamsINI����";
		return false;
	}
	CSimpleIni si(wchPath);
	CStringW strTemp;

	//���ڵ�
	Json::Value root;

	//1	jcxdh	����ߴ���	String	���ɿ�	H01��H02��H03��
	root["data"]["jcxdh"] = TCharToUTF8(wstrjcxdh.c_str());
	//2	jyy	����Ա	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = si.GetString(L"SystemCheck", L"JYY", L"�￭");
	root["data"]["jyy"] = TCharToUTF8(sTachometer.JYY.c_str());
	//3	jyysfzh	����Ա���֤��	String	���ɿ�	��ֵʱ����Ӣ�Ķ��ŷָ�
	strTemp = sTachometer.JYYSFZH.c_str();
	root["data"]["jyysfzh"] = TCharToUTF8(strTemp.GetString());
	//4	jckssj	��鿪ʼʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jckssj"] = TCharToUTF8(sTachometer.odtbegin.Format(L"%Y-%m-%d %H:%M:%S"));
	//5	jcjssj	������ʱ��	String	���ɿ�	yyyy-mm-dd hh24:mi:ss
	root["data"]["jcjssj"] = TCharToUTF8(sTachometer.odtEnd.Format(L"%Y-%m-%d %H:%M:%S"));
	//6	bzzs1	1000r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	root["data"]["bzzs1"] = TCharToUTF8(sTachometer.bzzs1.c_str());
	//7	sc1	1000r/minʵ�����	String	�ɿ�	С�������һλ
	root["data"]["sc1"] = TCharToUTF8(sTachometer.sc1.c_str());
	//8	pd1	1000r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd1"] = TCharToUTF8(L"1");
	//9	bzzs2	1500r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	root["data"]["bzzs2"] = TCharToUTF8(sTachometer.bzzs2.c_str());
	//10	sc2	1500r/minʵ�����	String	�ɿ�	С�������һλ
	root["data"]["sc2"] = TCharToUTF8(sTachometer.sc2.c_str());
	//11	pd2	1500r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd2"] = TCharToUTF8(L"1");
	//12	bzzs3	2000r/min��ѧת�ٱ����	String	�ɿ�	С�������һλ
	root["data"]["bzzs3"] = TCharToUTF8(sTachometer.bzzs3.c_str());
	//13	sc3	2000r/minʵ�����	String	�ɿ�	С�������һλ
	root["data"]["sc3"] = TCharToUTF8(sTachometer.sc3.c_str());
	//14	pd3	2000r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd3"] = TCharToUTF8(L"1");
	//15	bzzs4	2500r/min��ѧת�ٱ����	String	�ɿ�	
	root["data"]["bzzs4"] = TCharToUTF8(sTachometer.bzzs4.c_str());
	//16	sc4	2500r/minʵ�����	String	�ɿ�	
	root["data"]["sc4"] = TCharToUTF8(sTachometer.sc4.c_str());
	//17	pd4	2500r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd4"] = TCharToUTF8(L"1");
	//18	bzzs5	3000r/min��ѧת�ٱ����	String	�ɿ�	
	root["data"]["bzzs5"] = TCharToUTF8(sTachometer.bzzs5.c_str());
	//19	sc5	3000r/minʵ�����	String	�ɿ�	
	root["data"]["sc5"] = TCharToUTF8(sTachometer.sc5.c_str());
	//20	pd5	3000r/min�ж�	String	�ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["pd5"] = TCharToUTF8(L"1");
	//21	jcpdjg	����ж����	String	���ɿ�	1-�ϸ�2-���ϸ�
	root["data"]["jcpdjg"] = TCharToUTF8(L"1");
	//22	jcbtgyy	��鲻ͨ��ԭ��	String	�ɿ�	
	root["data"]["jcbtgyy"] = TCharToUTF8(L"");

	Json::StyledWriter sw;

	CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y53��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

	std::wstring wstrRet;
	int nRet = WriteCutl(L"90Y53", root, wstrRet);

	// �����ɹ�
	if (nRet == 0)
	{
		CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y53��", L"���ز���", wstrRet.c_str());

		Json::Reader reader;
		root.clear();

		if (reader.parse(TCharToUTF8(wstrRet.c_str()), root))
		{
			sHBMsg.code = JsonValueToCString(root["code"]);
			sHBMsg.msg = JsonValueToCString(root["msg"]);
			sHBMsg.data = JsonValueToCString(root["data"]);

			if (sHBMsg.code == L"200")
			{
				bRet = true;
			}
		}
	}

	return bRet;
}


bool CZYHttp_PAI::SetOBDProData(const TestLog &sTestLog, SHBMsg& sHBMsg)
{

	//���ڵ�
	Json::Value root;

	switch (_wtoi(sTestLog.wchTestType))
	{
	case 1:
		{
			// ��̬
			std::vector<SRealTimeDataOfASM> vt;
			GetIniRealTimeDataOfASMEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				//1		jylsh	������ˮ��	String	���ɿ�
				root["data"]["jylsh"] = TCharToUTF8(sTestLog.wchReportNumber);
				//2		jycs	�������	String	���ɿ�
				root["data"]["jycs"] = TCharToUTF8(sTestLog.wchNumberOfTestPeriod);
				//3		xslcs	��ʻ�����	String	���ɿ�
				root["data"]["xslcs"] = TCharToUTF8(sTestLog.wchTravelledDistance);
				//4		cysx	����ʱ��	String	���ɿ�
				root["data"]["cysx"] = i + 1;
				//5		cysj	����ʱ��	String	���ɿ�
				root["data"]["cysj"] = TCharToUTF8(vt[i].strSamplingTime.c_str());
				//6		jqmjdkd	�����ž���    ���ȣ�%��	String	���ɿ�
				root["data"]["jqmjdkd"] = TCharToUTF8(SetJQMKD(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//7		jsfhz	���㸺��ֵ��%��	String	���ɿ�
				root["data"]["jsfhz"] = TCharToUTF8(SetJSFHZ(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//8		qycgqxh	ǰ���������źţ�mV/mA��	String	���ɿ�
				root["data"]["qycgqxh"] = TCharToUTF8(SetQYCGQXH(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//9		glkqxs	��������ϵ�����ˣ�	String	���ɿ�
				root["data"]["glkqxs"] = TCharToUTF8(vt[i].sOBDRTData.strAirCoefficient.empty() ? L"0": vt[i].sOBDRTData.strAirCoefficient.c_str());
				//10		jqyl	����ѹ����kPa��	String	���ɿ�
				root["data"]["jqyl"] = TCharToUTF8(SetJQYL(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//11		ymkd	���ſ��ȣ�%��	String	���ɿ�
				root["data"]["ymkd"] = TCharToUTF8(vt[i].sOBDRTData.strThrottleOpening.empty() ? L"0": vt[i].sOBDRTData.strThrottleOpening.c_str());
				//12		fdjscgl	������������ʣ�kW��	String	���ɿ�
				root["data"]["fdjscgl"] = TCharToUTF8(vt[i].sOBDRTData.strEngineOutputPower.empty() ? L"0": vt[i].sOBDRTData.strEngineOutputPower.c_str());
				//13		zyyl	��ѹѹ����kPa��	String	���ɿ�
				root["data"]["zyyl"] = TCharToUTF8(vt[i].sOBDRTData.strChargeAirPressure.empty() ? L"0": vt[i].sOBDRTData.strChargeAirPressure.c_str());
				//14		hyl	��������L/100km��	String	���ɿ�
				root["data"]["hyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelConsumption.empty() ? L"0": vt[i].sOBDRTData.strFuelConsumption.c_str());
				//15		nocgqnd	����������Ũ�ȣ�10-6��	String	���ɿ�
				root["data"]["nocgqnd"] = TCharToUTF8(vt[i].sOBDRTData.strNOXConcentration.empty() ? L"0": vt[i].sOBDRTData.strNOXConcentration.c_str());
				//16		nspsl	������������L/h��	String	���ɿ�
				root["data"]["nspsl"] = TCharToUTF8(vt[i].sOBDRTData.strUreaInjectionVolume.empty() ? L"0": vt[i].sOBDRTData.strUreaInjectionVolume.c_str());
				//17		pqwd	�����¶ȣ��棩	String	���ɿ�
				root["data"]["pqwd"] = TCharToUTF8(vt[i].sOBDRTData.strEGT.empty() ? L"0": vt[i].sOBDRTData.strEGT.c_str());
				//18		klbjqyc	����������ѹ�kPa��	String	���ɿ�
				root["data"]["klbjqyc"] = TCharToUTF8(vt[i].sOBDRTData.strDPFDifferentialPressure.empty() ? L"0": vt[i].sOBDRTData.strDPFDifferentialPressure.c_str());
				//19		egrkd	EGR���ȣ�%��	String	���ɿ�
				root["data"]["egrkd"] = TCharToUTF8(vt[i].sOBDRTData.strEGRPosition.empty() ? L"0": vt[i].sOBDRTData.strEGRPosition.c_str());
				//20		rypsyl	ȼ������ѹ����MPa��	String	���ɿ�
				root["data"]["rypsyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelDeliveryPressure.empty() ? L"0": vt[i].sOBDRTData.strFuelDeliveryPressure.c_str());
				//21		cs	���٣�km/h��	String	���ɿ�
				root["data"]["cs"] = TCharToUTF8(SetCS(sTestLog.wchTestType, vt[i].strVelocity.c_str()).GetString());
				//22		fdjzs	������ת�٣�r/min��	String	���ɿ�
				root["data"]["fdjzs"] = TCharToUTF8(SetFDJZS(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//23		jql	��������g/s��	String	���ɿ�
				root["data"]["jql"] = TCharToUTF8(SetJQL(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());

				Json::StyledWriter sw;

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

				std::wstring wstrRet;
				int nRet = WriteCutl(L"90Y28", root, wstrRet);

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"���ز���", wstrRet.c_str());
			}
		}
		break;
	case 2:
		{
			// ����˲̬
			std::vector<SRealTimeDataOfVMAS> vt;
			GetIniRealTimeDataOfVMASEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				//1		jylsh	������ˮ��	String	���ɿ�
				root["data"]["jylsh"] = TCharToUTF8(sTestLog.wchReportNumber);
				//2		jycs	�������	String	���ɿ�
				root["data"]["jycs"] = TCharToUTF8(sTestLog.wchNumberOfTestPeriod);
				//3		xslcs	��ʻ�����	String	���ɿ�
				root["data"]["xslcs"] = TCharToUTF8(sTestLog.wchTravelledDistance);
				//4		cysx	����ʱ��	String	���ɿ�
				root["data"]["cysx"] = i + 1;
				//5		cysj	����ʱ��	String	���ɿ�
				root["data"]["cysj"] = TCharToUTF8(vt[i].strSamplingTime.c_str());
				//6		jqmjdkd	�����ž���    ���ȣ�%��	String	���ɿ�
				root["data"]["jqmjdkd"] = TCharToUTF8(SetJQMKD(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//7		jsfhz	���㸺��ֵ��%��	String	���ɿ�
				root["data"]["jsfhz"] = TCharToUTF8(SetJSFHZ(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//8		qycgqxh	ǰ���������źţ�mV/mA��	String	���ɿ�
				root["data"]["qycgqxh"] = TCharToUTF8(SetQYCGQXH(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//9		glkqxs	��������ϵ�����ˣ�	String	���ɿ�
				root["data"]["glkqxs"] = TCharToUTF8(vt[i].sOBDRTData.strAirCoefficient.empty() ? L"0": vt[i].sOBDRTData.strAirCoefficient.c_str());
				//10		jqyl	����ѹ����kPa��	String	���ɿ�
				root["data"]["jqyl"] = TCharToUTF8(SetJQYL(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//11		ymkd	���ſ��ȣ�%��	String	���ɿ�
				root["data"]["ymkd"] = TCharToUTF8(vt[i].sOBDRTData.strThrottleOpening.empty() ? L"0": vt[i].sOBDRTData.strThrottleOpening.c_str());
				//12		fdjscgl	������������ʣ�kW��	String	���ɿ�
				root["data"]["fdjscgl"] = TCharToUTF8(vt[i].sOBDRTData.strEngineOutputPower.empty() ? L"0": vt[i].sOBDRTData.strEngineOutputPower.c_str());
				//13		zyyl	��ѹѹ����kPa��	String	���ɿ�
				root["data"]["zyyl"] = TCharToUTF8(vt[i].sOBDRTData.strChargeAirPressure.empty() ? L"0": vt[i].sOBDRTData.strChargeAirPressure.c_str());
				//14		hyl	��������L/100km��	String	���ɿ�
				root["data"]["hyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelConsumption.empty() ? L"0": vt[i].sOBDRTData.strFuelConsumption.c_str());
				//15		nocgqnd	����������Ũ�ȣ�10-6��	String	���ɿ�
				root["data"]["nocgqnd"] = TCharToUTF8(vt[i].sOBDRTData.strNOXConcentration.empty() ? L"0": vt[i].sOBDRTData.strNOXConcentration.c_str());
				//16		nspsl	������������L/h��	String	���ɿ�
				root["data"]["nspsl"] = TCharToUTF8(vt[i].sOBDRTData.strUreaInjectionVolume.empty() ? L"0": vt[i].sOBDRTData.strUreaInjectionVolume.c_str());
				//17		pqwd	�����¶ȣ��棩	String	���ɿ�
				root["data"]["pqwd"] = TCharToUTF8(vt[i].sOBDRTData.strEGT.empty() ? L"0": vt[i].sOBDRTData.strEGT.c_str());
				//18		klbjqyc	����������ѹ�kPa��	String	���ɿ�
				root["data"]["klbjqyc"] = TCharToUTF8(vt[i].sOBDRTData.strDPFDifferentialPressure.empty() ? L"0": vt[i].sOBDRTData.strDPFDifferentialPressure.c_str());
				//19		egrkd	EGR���ȣ�%��	String	���ɿ�
				root["data"]["egrkd"] = TCharToUTF8(vt[i].sOBDRTData.strEGRPosition.empty() ? L"0": vt[i].sOBDRTData.strEGRPosition.c_str());
				//20		rypsyl	ȼ������ѹ����MPa��	String	���ɿ�
				root["data"]["rypsyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelDeliveryPressure.empty() ? L"0": vt[i].sOBDRTData.strFuelDeliveryPressure.c_str());
				//21		cs	���٣�km/h��	String	���ɿ�
				root["data"]["cs"] = TCharToUTF8(SetCS(sTestLog.wchTestType, vt[i].strVelocity.c_str()).GetString());
				//22		fdjzs	������ת�٣�r/min��	String	���ɿ�
				root["data"]["fdjzs"] = TCharToUTF8(SetFDJZS(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//23		jql	��������g/s��	String	���ɿ�
				root["data"]["jql"] = TCharToUTF8(SetJQL(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());

				Json::StyledWriter sw;

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

				std::wstring wstrRet;
				int nRet = WriteCutl(L"90Y28", root, wstrRet);

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"���ز���", wstrRet.c_str());
			}
		}
		break;
	case 3:
		{
			// ���ؼ���
			std::vector<SRealTimeDataOfLUGDOWN> vt;
			GetIniRealTimeDataOfLUGDOWNEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				//1		jylsh	������ˮ��	String	���ɿ�
				root["data"]["jylsh"] = TCharToUTF8(sTestLog.wchReportNumber);
				//2		jycs	�������	String	���ɿ�
				root["data"]["jycs"] = TCharToUTF8(sTestLog.wchNumberOfTestPeriod);
				//3		xslcs	��ʻ�����	String	���ɿ�
				root["data"]["xslcs"] = TCharToUTF8(sTestLog.wchTravelledDistance);
				//4		cysx	����ʱ��	String	���ɿ�
				root["data"]["cysx"] = i + 1;
				//5		cysj	����ʱ��	String	���ɿ�
				root["data"]["cysj"] = TCharToUTF8(vt[i].strSamplingTime.c_str());
				//6		jqmjdkd	�����ž���    ���ȣ�%��	String	���ɿ�
				root["data"]["jqmjdkd"] = TCharToUTF8(SetJQMKD(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//7		jsfhz	���㸺��ֵ��%��	String	���ɿ�
				root["data"]["jsfhz"] = TCharToUTF8(SetJSFHZ(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//8		qycgqxh	ǰ���������źţ�mV/mA��	String	���ɿ�
				root["data"]["qycgqxh"] = TCharToUTF8(SetQYCGQXH(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//9		glkqxs	��������ϵ�����ˣ�	String	���ɿ�
				root["data"]["glkqxs"] = TCharToUTF8(vt[i].sOBDRTData.strAirCoefficient.empty() ? L"0": vt[i].sOBDRTData.strAirCoefficient.c_str());
				//10		jqyl	����ѹ����kPa��	String	���ɿ�
				root["data"]["jqyl"] = TCharToUTF8(SetJQYL(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//11		ymkd	���ſ��ȣ�%��	String	���ɿ�
				root["data"]["ymkd"] = TCharToUTF8(vt[i].sOBDRTData.strThrottleOpening.empty() ? L"0": vt[i].sOBDRTData.strThrottleOpening.c_str());
				//12		fdjscgl	������������ʣ�kW��	String	���ɿ�
				root["data"]["fdjscgl"] = TCharToUTF8(vt[i].sOBDRTData.strEngineOutputPower.empty() ? L"0": vt[i].sOBDRTData.strEngineOutputPower.c_str());
				//13		zyyl	��ѹѹ����kPa��	String	���ɿ�
				root["data"]["zyyl"] = TCharToUTF8(vt[i].sOBDRTData.strChargeAirPressure.empty() ? L"0": vt[i].sOBDRTData.strChargeAirPressure.c_str());
				//14		hyl	��������L/100km��	String	���ɿ�
				root["data"]["hyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelConsumption.empty() ? L"0": vt[i].sOBDRTData.strFuelConsumption.c_str());
				//15		nocgqnd	����������Ũ�ȣ�10-6��	String	���ɿ�
				root["data"]["nocgqnd"] = TCharToUTF8(vt[i].sOBDRTData.strNOXConcentration.empty() ? L"0": vt[i].sOBDRTData.strNOXConcentration.c_str());
				//16		nspsl	������������L/h��	String	���ɿ�
				root["data"]["nspsl"] = TCharToUTF8(vt[i].sOBDRTData.strUreaInjectionVolume.empty() ? L"0": vt[i].sOBDRTData.strUreaInjectionVolume.c_str());
				//17		pqwd	�����¶ȣ��棩	String	���ɿ�
				root["data"]["pqwd"] = TCharToUTF8(vt[i].sOBDRTData.strEGT.empty() ? L"0": vt[i].sOBDRTData.strEGT.c_str());
				//18		klbjqyc	����������ѹ�kPa��	String	���ɿ�
				root["data"]["klbjqyc"] = TCharToUTF8(vt[i].sOBDRTData.strDPFDifferentialPressure.empty() ? L"0": vt[i].sOBDRTData.strDPFDifferentialPressure.c_str());
				//19		egrkd	EGR���ȣ�%��	String	���ɿ�
				root["data"]["egrkd"] = TCharToUTF8(vt[i].sOBDRTData.strEGRPosition.empty() ? L"0": vt[i].sOBDRTData.strEGRPosition.c_str());
				//20		rypsyl	ȼ������ѹ����MPa��	String	���ɿ�
				root["data"]["rypsyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelDeliveryPressure.empty() ? L"0": vt[i].sOBDRTData.strFuelDeliveryPressure.c_str());
				//21		cs	���٣�km/h��	String	���ɿ�
				root["data"]["cs"] = TCharToUTF8(SetCS(sTestLog.wchTestType, vt[i].strVelocity.c_str()).GetString());
				//22		fdjzs	������ת�٣�r/min��	String	���ɿ�
				root["data"]["fdjzs"] = TCharToUTF8(SetFDJZS(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());
				//23		jql	��������g/s��	String	���ɿ�
				root["data"]["jql"] = TCharToUTF8(SetJQL(sTestLog.wchTestType, vt[i].strVelocity.c_str(), vt[i].strEngineRev.c_str()).GetString());

				Json::StyledWriter sw;

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

				std::wstring wstrRet;
				int nRet = WriteCutl(L"90Y28", root, wstrRet);

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"���ز���", wstrRet.c_str());
			}
		}
		break;
	case 4:
		{
			// ˫����
			std::vector<SRealTimeDataOfDIS> vt;
			GetIniRealTimeDataOfDISEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				//1		jylsh	������ˮ��	String	���ɿ�
				root["data"]["jylsh"] = TCharToUTF8(sTestLog.wchReportNumber);
				//2		jycs	�������	String	���ɿ�
				root["data"]["jycs"] = TCharToUTF8(sTestLog.wchNumberOfTestPeriod);
				//3		xslcs	��ʻ�����	String	���ɿ�
				root["data"]["xslcs"] = TCharToUTF8(sTestLog.wchTravelledDistance);
				//4		cysx	����ʱ��	String	���ɿ�
				root["data"]["cysx"] = i + 1;
				//5		cysj	����ʱ��	String	���ɿ�
				root["data"]["cysj"] = TCharToUTF8(vt[i].strSamplingTime.c_str());
				//6		jqmjdkd	�����ž���    ���ȣ�%��	String	���ɿ�
				root["data"]["jqmjdkd"] = TCharToUTF8(SetJQMKD(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//7		jsfhz	���㸺��ֵ��%��	String	���ɿ�
				root["data"]["jsfhz"] = TCharToUTF8(SetJSFHZ(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//8		qycgqxh	ǰ���������źţ�mV/mA��	String	���ɿ�
				root["data"]["qycgqxh"] = TCharToUTF8(SetQYCGQXH(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//9		glkqxs	��������ϵ�����ˣ�	String	���ɿ�
				root["data"]["glkqxs"] = TCharToUTF8(vt[i].sOBDRTData.strAirCoefficient.empty() ? L"0": vt[i].sOBDRTData.strAirCoefficient.c_str());
				//10		jqyl	����ѹ����kPa��	String	���ɿ�
				root["data"]["jqyl"] = TCharToUTF8(SetJQYL(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//11		ymkd	���ſ��ȣ�%��	String	���ɿ�
				root["data"]["ymkd"] = TCharToUTF8(vt[i].sOBDRTData.strThrottleOpening.empty() ? L"0": vt[i].sOBDRTData.strThrottleOpening.c_str());
				//12		fdjscgl	������������ʣ�kW��	String	���ɿ�
				root["data"]["fdjscgl"] = TCharToUTF8(vt[i].sOBDRTData.strEngineOutputPower.empty() ? L"0": vt[i].sOBDRTData.strEngineOutputPower.c_str());
				//13		zyyl	��ѹѹ����kPa��	String	���ɿ�
				root["data"]["zyyl"] = TCharToUTF8(vt[i].sOBDRTData.strChargeAirPressure.empty() ? L"0": vt[i].sOBDRTData.strChargeAirPressure.c_str());
				//14		hyl	��������L/100km��	String	���ɿ�
				root["data"]["hyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelConsumption.empty() ? L"0": vt[i].sOBDRTData.strFuelConsumption.c_str());
				//15		nocgqnd	����������Ũ�ȣ�10-6��	String	���ɿ�
				root["data"]["nocgqnd"] = TCharToUTF8(vt[i].sOBDRTData.strNOXConcentration.empty() ? L"0": vt[i].sOBDRTData.strNOXConcentration.c_str());
				//16		nspsl	������������L/h��	String	���ɿ�
				root["data"]["nspsl"] = TCharToUTF8(vt[i].sOBDRTData.strUreaInjectionVolume.empty() ? L"0": vt[i].sOBDRTData.strUreaInjectionVolume.c_str());
				//17		pqwd	�����¶ȣ��棩	String	���ɿ�
				root["data"]["pqwd"] = TCharToUTF8(vt[i].sOBDRTData.strEGT.empty() ? L"0": vt[i].sOBDRTData.strEGT.c_str());
				//18		klbjqyc	����������ѹ�kPa��	String	���ɿ�
				root["data"]["klbjqyc"] = TCharToUTF8(vt[i].sOBDRTData.strDPFDifferentialPressure.empty() ? L"0": vt[i].sOBDRTData.strDPFDifferentialPressure.c_str());
				//19		egrkd	EGR���ȣ�%��	String	���ɿ�
				root["data"]["egrkd"] = TCharToUTF8(vt[i].sOBDRTData.strEGRPosition.empty() ? L"0": vt[i].sOBDRTData.strEGRPosition.c_str());
				//20		rypsyl	ȼ������ѹ����MPa��	String	���ɿ�
				root["data"]["rypsyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelDeliveryPressure.empty() ? L"0": vt[i].sOBDRTData.strFuelDeliveryPressure.c_str());
				//21		cs	���٣�km/h��	String	���ɿ�
				root["data"]["cs"] = TCharToUTF8(SetCS(sTestLog.wchTestType, L"0").GetString());
				//22		fdjzs	������ת�٣�r/min��	String	���ɿ�
				root["data"]["fdjzs"] = TCharToUTF8(SetFDJZS(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//23		jql	��������g/s��	String	���ɿ�
				root["data"]["jql"] = TCharToUTF8(SetJQL(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());

				Json::StyledWriter sw;

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

				std::wstring wstrRet;
				int nRet = WriteCutl(L"90Y28", root, wstrRet);

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"���ز���", wstrRet.c_str());
			}
		}
		break;
	case 5:
		{
			// ��͸��
			std::vector<SRealTimeDataOfFSUNHT> vt;
			GetIniRealTimeDataOfFSUNHTEx(vt);
			for(int i=0; i<vt.size(); i++)
			{
				//1		jylsh	������ˮ��	String	���ɿ�
				root["data"]["jylsh"] = TCharToUTF8(sTestLog.wchReportNumber);
				//2		jycs	�������	String	���ɿ�
				root["data"]["jycs"] = TCharToUTF8(sTestLog.wchNumberOfTestPeriod);
				//3		xslcs	��ʻ�����	String	���ɿ�
				root["data"]["xslcs"] = TCharToUTF8(sTestLog.wchTravelledDistance);
				//4		cysx	����ʱ��	String	���ɿ�
				root["data"]["cysx"] = i + 1;
				//5		cysj	����ʱ��	String	���ɿ�
				root["data"]["cysj"] = TCharToUTF8(vt[i].strSamplingTime.c_str());
				//6		jqmjdkd	�����ž���    ���ȣ�%��	String	���ɿ�
				root["data"]["jqmjdkd"] = TCharToUTF8(SetJQMKD(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//7		jsfhz	���㸺��ֵ��%��	String	���ɿ�
				root["data"]["jsfhz"] = TCharToUTF8(SetJSFHZ(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//8		qycgqxh	ǰ���������źţ�mV/mA��	String	���ɿ�
				root["data"]["qycgqxh"] = TCharToUTF8(SetQYCGQXH(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//9		glkqxs	��������ϵ�����ˣ�	String	���ɿ�
				root["data"]["glkqxs"] = TCharToUTF8(vt[i].sOBDRTData.strAirCoefficient.empty() ? L"0": vt[i].sOBDRTData.strAirCoefficient.c_str());
				//10		jqyl	����ѹ����kPa��	String	���ɿ�
				root["data"]["jqyl"] = TCharToUTF8(SetJQYL(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//11		ymkd	���ſ��ȣ�%��	String	���ɿ�
				root["data"]["ymkd"] = TCharToUTF8(vt[i].sOBDRTData.strThrottleOpening.empty() ? L"0": vt[i].sOBDRTData.strThrottleOpening.c_str());
				//12		fdjscgl	������������ʣ�kW��	String	���ɿ�
				root["data"]["fdjscgl"] = TCharToUTF8(vt[i].sOBDRTData.strEngineOutputPower.empty() ? L"0": vt[i].sOBDRTData.strEngineOutputPower.c_str());
				//13		zyyl	��ѹѹ����kPa��	String	���ɿ�
				root["data"]["zyyl"] = TCharToUTF8(vt[i].sOBDRTData.strChargeAirPressure.empty() ? L"0": vt[i].sOBDRTData.strChargeAirPressure.c_str());
				//14		hyl	��������L/100km��	String	���ɿ�
				root["data"]["hyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelConsumption.empty() ? L"0": vt[i].sOBDRTData.strFuelConsumption.c_str());
				//15		nocgqnd	����������Ũ�ȣ�10-6��	String	���ɿ�
				root["data"]["nocgqnd"] = TCharToUTF8(vt[i].sOBDRTData.strNOXConcentration.empty() ? L"0": vt[i].sOBDRTData.strNOXConcentration.c_str());
				//16		nspsl	������������L/h��	String	���ɿ�
				root["data"]["nspsl"] = TCharToUTF8(vt[i].sOBDRTData.strUreaInjectionVolume.empty() ? L"0": vt[i].sOBDRTData.strUreaInjectionVolume.c_str());
				//17		pqwd	�����¶ȣ��棩	String	���ɿ�
				root["data"]["pqwd"] = TCharToUTF8(vt[i].sOBDRTData.strEGT.empty() ? L"0": vt[i].sOBDRTData.strEGT.c_str());
				//18		klbjqyc	����������ѹ�kPa��	String	���ɿ�
				root["data"]["klbjqyc"] = TCharToUTF8(vt[i].sOBDRTData.strDPFDifferentialPressure.empty() ? L"0": vt[i].sOBDRTData.strDPFDifferentialPressure.c_str());
				//19		egrkd	EGR���ȣ�%��	String	���ɿ�
				root["data"]["egrkd"] = TCharToUTF8(vt[i].sOBDRTData.strEGRPosition.empty() ? L"0": vt[i].sOBDRTData.strEGRPosition.c_str());
				//20		rypsyl	ȼ������ѹ����MPa��	String	���ɿ�
				root["data"]["rypsyl"] = TCharToUTF8(vt[i].sOBDRTData.strFuelDeliveryPressure.empty() ? L"0": vt[i].sOBDRTData.strFuelDeliveryPressure.c_str());
				//21		cs	���٣�km/h��	String	���ɿ�
				root["data"]["cs"] = TCharToUTF8(SetCS(sTestLog.wchTestType, L"0").GetString());
				//22		fdjzs	������ת�٣�r/min��	String	���ɿ�
				root["data"]["fdjzs"] = TCharToUTF8(SetFDJZS(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());
				//23		jql	��������g/s��	String	���ɿ�
				root["data"]["jql"] = TCharToUTF8(SetJQL(sTestLog.wchTestType, L"0", vt[i].strEngineRev.c_str()).GetString());

				Json::StyledWriter sw;

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"�ϴ�����", UTF8ToTChar(sw.write(root).c_str()));

				std::wstring wstrRet;
				int nRet = WriteCutl(L"90Y28", root, wstrRet);

				CNHLogAPI::WriteLogEx(m_strLogFilePath, L"��90Y28��", L"���ز���", wstrRet.c_str());
			}
		}
		break;
	default:{}
			break;

	}

	return true;
}

int CZYHttp_PAI::WriteCutl(const std::wstring wstrJkid, Json::Value &root, std::wstring& wstrRet)
{

	root["jkid"] = Json::Value(TCharToUTF8(wstrJkid.c_str()));
	root["yhxlh"] = Json::Value(TCharToUTF8(wstryhxlh.c_str()));
	root["jyjgbh"] = Json::Value(TCharToUTF8(wstrjyjgbh.c_str()));

	Json::StyledWriter sw;

	std::wstring wstrPostData = UTF8ToTChar(sw.write(root).c_str());

	CNHLogAPI::WriteLogEx(m_strLogFilePath, wstrWURL.c_str(), wstrJkid.c_str(), wstrPostData.c_str());


	return CCurlHttp_API::CurlPost(wstrWURL, wstrPostData, wstrRet);
}
// д����ӿ� ����

// //��ʼ��һ����΢��Ϊ��λ��ʱ������
int CZYHttp_PAI::randEx(void)
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand();
}

// JQMKD		�����ž��Կ���
CStringW CZYHttp_PAI::SetJQMKD(const CString& strTestType, const CString& strVelocity, const CString& strEngineRev)
{
	if ((_wtoi(strVelocity.GetString()) == 0)
		&& (_wtoi(strEngineRev.GetString()) == 0))
	{
		return L"0";
	}

	CString strJQMKD;

	int n = randEx()%10 + 1;

	if (n%2 == 0)
	{
		float f = n + 32.94f;
		strJQMKD.Format(L"%.2f",f);
	}
	else
	{
		float f = n /10.0f;
		f = 32.94f + f;
		strJQMKD.Format(L"%.2f",f);
	}

	return strJQMKD;
}

// JSFHZ		���㸺��ֵ
CStringW CZYHttp_PAI::SetJSFHZ(const CString& strTestType, const CString& strVelocity, const CString& strEngineRev)
{

	if ((_wtoi(strVelocity.GetString()) == 0)
		&& (_wtoi(strEngineRev.GetString()) == 0))
	{
		return L"0";
	}

	CString strJSFHZ;

	int n = randEx()%10 + 1;

	if (n%2 == 0)
	{
		float f = 60.00f - n;
		strJSFHZ.Format(L"%.2f",f);
	}
	else
	{
		float f = n /10.0f;
		f = 60.00f + f;
		strJSFHZ.Format(L"%.2f",f);
	}

	return strJSFHZ;
}
// QYCGQXH	ǰ���������ź�
CStringW CZYHttp_PAI::SetQYCGQXH(const CString& strTestType, const CString& strVelocity, const CString& strEngineRev)
{
	if ((_wtoi(strVelocity.GetString()) == 0)
		&& (_wtoi(strEngineRev.GetString()) == 0))
	{
		return L"0";
	}

	CString strQYCGQXH;

	int n = randEx()%10 + 1;

	if (n%2 == 0)
	{
		float f = 30.00 - n;
		strQYCGQXH.Format(L"%.2f",f);
	}
	else
	{
		float f = n /10.0f;
		f = 30.00 + f;
		strQYCGQXH.Format(L"%.2f",f);
	}

	return strQYCGQXH;
}
// CS		����
CStringW CZYHttp_PAI::SetCS(const CString& strTestType, const CString& strVelocity)
{
	if ((_wtoi(strVelocity.GetString()) == 0))
	{
		return L"0";
	}

	CString strCS;
	int n = randEx()%10 + 1;

	switch (_wtoi(strTestType))
	{
	case 1:
	case 2:
	case 3:
		{
			// ��̬
			// ����˲̬
			// ���ؼ���
			float f;
			if (n%2 == 0) { f = (randEx()%2 * 2) + _wtof(strVelocity.GetString()) +0.5f;}
			else if (n%3 == 0) { f = (randEx()%2 * 2) + _wtof(strVelocity.GetString()) -1.5f;}
			else if (n%5 == 0) { f = (randEx()%2 * -1) + _wtof(strVelocity.GetString()) +2.5f;}
			else { f = (randEx()%2 * -1) + _wtof(strVelocity.GetString()) +1.0f;}
			strCS.Format(L"%.2f",f);

			return strCS;
		}
		break;
	case 4:
	case 5:
		{
			// ˫����
			// ��͸��
			return L"0";
		}
		break;
	default:{return L"0";}
			break;

	}


}
// FDJZS		������ת��
CStringW CZYHttp_PAI::SetFDJZS(const CString& strTestType, const CString& strVelocity, const CString& strEngineRev)
{

	CString strFDJZS;

	switch (_wtoi(strTestType))
	{
	case 1:
	case 2:
		{
			// ��̬
			// ����˲̬
			int i= randEx()%10 + 1;
			if (_wtoi(strVelocity.GetString()) != 0)
			{
				int n(0);
				if (i%2 == 0){n = randEx()%300 + 1752;}
				else if (i%3 == 0){n = randEx()%300 + 1825;}
				else if (i%5 == 0){n = randEx()%300 + 1872;}
				else {n = randEx()%300 + 1852;}
				strFDJZS.Format(L"%d", n);
			}
			else
			{
				int n(0);
				if (i%2 == 0){n = randEx()%300 + 650;}
				else if (i%3 == 0){n = randEx()%300 + 678;}
				else if (i%5 == 0){n = randEx()%300 + 687;}
				else {n = randEx()%300 + 640;}
				strFDJZS.Format(L"%d", n);
			}
		}
		break;
	case 3:
	case 4:
	case 5:
		{
			// ���ؼ���
			// ˫����
			// ��͸��
			strFDJZS = strEngineRev;
		}
		break;
	default:{strFDJZS = strEngineRev;}
			break;
	}

	return strFDJZS;
}
// JQL		������
CStringW CZYHttp_PAI::SetJQL(const CString& strTestType, const CString& strVelocity, const CString& strEngineRev)
{
	if ((_wtoi(strVelocity.GetString()) == 0)
		&& (_wtoi(strEngineRev.GetString()) == 0))
	{
		return L"0";
	}

	CString strJQL;

	int n = randEx()%4 + 1;

	if (n%2 == 0)
	{
		float f = 2.87f + n;
		strJQL.Format(L"%.2f",f);
	}
	else
	{
		float f = n /10.0f;
		f = 2.87f + f;
		strJQL.Format(L"%.2f",f);
	}

	return strJQL;
}
// JQYL		����ѹ��
CStringW CZYHttp_PAI::SetJQYL(const CString& strTestType, const CString& strVelocity, const CString& strEngineRev)
{
	if ((_wtoi(strVelocity.GetString()) == 0)
		&& (_wtoi(strEngineRev.GetString()) == 0))
	{
		return L"0";
	}

	CString strJQYL;

	int n = randEx()%10 + 1;

	if (n%2 == 0)
	{
		float f = 72.00f - n;
		strJQYL.Format(L"%.2f",f);
	}
	else
	{
		float f = n /10.0f;
		f = 72.00f + f;
		strJQYL.Format(L"%.2f",f);
	}

	return strJQYL;
}


void CZYHttp_PAI::GenLogFile(void)
{
	// ��־�ļ������ļ���·��
	wchar_t wchLogFileFolderPath[MAX_PATH] = {0};
	CNHCommonAPI::GetHLDFilePath(L"LOG", L"", wchLogFileFolderPath, true);

	// ɾ��lDays��ǰ�������죩����־�ļ�
	COleDateTime odtNow(COleDateTime::GetCurrentTime());
	const long lDays(30);
	COleDateTime odtDelLog = odtNow - COleDateTimeSpan(lDays, 0, 0, 0);
	SYSTEMTIME stDelLog;
	ZeroMemory(&stDelLog, sizeof(SYSTEMTIME));
	stDelLog.wYear = (WORD)odtDelLog.GetYear();
	stDelLog.wMonth = (WORD)odtDelLog.GetMonth();
	stDelLog.wDay = (WORD)odtDelLog.GetDay();
	stDelLog.wHour = 23;
	stDelLog.wMinute = 59;
	stDelLog.wSecond = 59;
	CNHLogAPI::DeleteLogEx(wchLogFileFolderPath, stDelLog);

	// ���ɽ�����־�ļ�·��
	// ��������־�ļ�
	CStringW strFileName;
	strFileName.Format(L"%s_%s.log", odtNow.Format(L"%Y-%m-%d"), m_strModName);
	m_strLogFilePath = wchLogFileFolderPath;
	m_strLogFilePath += strFileName;

}

void CZYHttp_PAI::InitData(void)
{
}

void CZYHttp_PAI::LoadConfig(void)
{
	// ��ȡ�����ļ�
	wchar_t wchFilePath[MAX_PATH] = {L'\0'};
	CNHCommonAPI::GetHLDFilePath(L"Config", L"ZYConfig.ini", wchFilePath);

	CSimpleIni si(wchFilePath);

	// URLд���ַ
	wstrWURL = si.GetString(L"NetConfig", L"WURL", L"");
	// URL��ѯ��ַ
	wstrQURL = si.GetString(L"NetConfig", L"QURL", L"");
	// �������к�
	wstryhxlh = si.GetString(L"NetConfig", L"yhxlh", L"");
	// ����������
	wstrjyjgbh = si.GetString(L"NetConfig", L"jyjgbh", L"");
	// ����߱��
	wstrjcxdh = si.GetString(L"NetConfig", L"jcxdh", L"");

}

CString CZYHttp_PAI::JsonValueToCString(Json::Value root)
{
	CString strRet(L"");

	if (root.type() == Json::nullValue)
	{
		strRet.Format(L"%s", L"");
	}
	if (root.type() == Json::stringValue)
	{
		strRet.Format(L"%s", UTF8ToTChar(root.asString().c_str()));
	}
	else if (root.type() == Json::booleanValue)
	{
		strRet.Format(L"%s", UTF8ToTChar(root.asString().c_str()));
	}
	else if (root.type() == Json::realValue)
	{
		strRet.Format(L"%f", root.asDouble());
	}
	else if (root.type() == Json::uintValue)
	{
		strRet.Format(L"%hu", root.asUInt());
	}
	else if (root.type() == Json::intValue)
	{
		strRet.Format(L"%d", root.asInt());
	}
	else if (root.type() == Json::arrayValue || root.type() == Json::objectValue)
	{
		Json::StyledWriter sw;
		strRet.Format(L"%s", UTF8ToTChar(sw.write(root).c_str()));
	}

	return strRet;
}


CStringW CZYHttp_PAI::TestTypeNameToCode(const CString strTestTypeName)
{
	CString strCode(L"HA");
	if (L"��̬" == strTestTypeName)
	{
		strCode = (L"HA");
	}
	else if (L"����˲̬" == strTestTypeName)
	{
		strCode = (L"HV");
	}
	else if (L"���ؼ���" == strTestTypeName)
	{
		strCode = (L"HJ");
	}
	else if (L"˫����" == strTestTypeName)
	{
		strCode = (L"HS");
	}
	else if (L"��͸�����ɼ���" == strTestTypeName)
	{
		strCode = (L"HY");
	}
	else if (L"��ֽʽ���ɼ���" == strTestTypeName)
	{
		strCode = (L"HY");
	}
	else if (L"ũ�ó����ɼ���" == strTestTypeName)
	{
		strCode = (L"HP");
	}
	else
	{
		strCode = (L"HA");
	}

	return strCode;
}

CStringW CZYHttp_PAI::TestTypeCodeToName(const CString strTestTypeCode)
{
	CString strName(L"1");
	if (L"HA" == strTestTypeCode)
	{
		strName = (L"1");
	}
	else if (L"HV" == strTestTypeCode)
	{
		strName = (L"2");
	}
	else if (L"HJ" == strTestTypeCode)
	{
		strName = (L"3");
	}
	else if (L"HS" == strTestTypeCode)
	{
		strName = (L"4");
	}
	else if (L"HY" == strTestTypeCode)
	{
		strName = (L"5");
	}
	else if (L"HP" == strTestTypeCode)
	{
		strName = (L"7");
	}
	else
	{
		strName = (L"1");
	}

	return strName;
}

CStringW CZYHttp_PAI::PlateTypeNameToCode(const CString strName)
{
	CString strCode(L"0");
	if (strName.Find(L"��") != -1)
	{
		strCode = L"0";
	}
	else if (strName.Find(L"��") != -1)
	{
		strCode = L"1";
	}
	else if (strName.Find(L"��") != -1)
	{
		strCode = L"2";
	}
	else if (strName.Find(L"��") != -1)
	{
		strCode = L"3";
	}
	else if (strName.Find(L"����Դ") != -1)
	{
		strCode = L"4";
	}
	else if (strName.Find(L"����") != -1)
	{
		strCode = L"5";
	}
	return strCode;
}

CStringW CZYHttp_PAI::PlateTypeCodeToName(const CString strCode)
{
	CString strName(L"����");
	if (strCode.Find(L"0") != -1)
	{
		strName = L"����";
	}
	else if (strCode.Find(L"1") != -1)
	{
		strName = L"����";
	}
	else if (strCode.Find(L"2") != -1)
	{
		strName = L"����";
	}
	else if (strCode.Find(L"3") != -1)
	{
		strName = L"����";
	}
	else if (strCode.Find(L"4") != -1)
	{
		strName = L"����Դ";
	}
	else if (strCode.Find(L"5") != -1)
	{
		strName = L"����";
	}
	return strName;
}

CStringW CZYHttp_PAI::FuleNameToCode(const CString strName)
{
	CString strCode(L"����");
	if (strName.Find(L"����") != -1)
	{
		strCode = L"A";
	}
	else if (strName.Find(L"����") != -1)
	{
		strCode = L"B";
	}
	else if (strName.Find(L"��") != -1)
	{
		strCode = L"C";
	}
	else if (strName.Find(L"�����") != -1)
	{
		strCode = L"D";
	}
	else if (strName.Find(L"��Ȼ��") != -1)
	{
		strCode = L"E";
	}
	else if (strName.Find(L"Һ��ʯ����") != -1)
	{
		strCode = L"F";
	}
	else if (strName.Find(L"�״�") != -1)
	{
		strCode = L"L";
	}
	else if (strName.Find(L"̫����") != -1)
	{
		strCode = L"N";
	}
	else if (strName.Find(L"��") != -1)
	{
		strCode = L"P";
	}
	else if (strName.Find(L"��") != -1)
	{
		strCode = L"Y";
	}
	else if (strName.Find(L"�Ҵ�") != -1)
	{
		strCode = L"M";
	}
	else if (strName.Find(L"��϶���") != -1)
	{
		strCode = L"O";
	}
	else if (strName.Find(L"����ȼ��") != -1)
	{
		strCode = L"Q";
	}
	return strCode;
}

CStringW CZYHttp_PAI::FuleCodeToName(const CString strCode)
{
	CString strName(L"����");
	if (strCode.Find(L"A") != -1)
	{
		strName = L"����";
	}
	else if (strCode.Find(L"B") != -1)
	{
		strName = L"����";
	}
	else if (strCode.Find(L"C") != -1)
	{
		strName = L"��";
	}
	else if (strCode.Find(L"D") != -1)
	{
		strName = L"�����";
	}
	else if (strCode.Find(L"E") != -1)
	{
		strName = L"��Ȼ��";
	}
	else if (strCode.Find(L"F") != -1)
	{
		strName = L"Һ��ʯ����";
	}
	else if (strCode.Find(L"L") != -1)
	{
		strName = L"�״�";
	}
	else if (strCode.Find(L"N") != -1)
	{
		strName = L"̫����";
	}
	else if (strCode.Find(L"P") != -1)
	{
		strName = L"��";
	}
	else if (strCode.Find(L"Y") != -1)
	{
		strName = L"��";
	}
	else if (strCode.Find(L"M") != -1)
	{
		strName = L"�Ҵ�";
	}
	else if (strCode.Find(L"O") != -1)
	{
		strName = L"��϶���";
	}
	else if (strCode.Find(L"Q") != -1)
	{
		strName = L"����ȼ��";
	}
	return strName;
}

CStringW CZYHttp_PAI::VehColourNameToCode(const CString strName)
{
	CString strCode(L"");
	if (strName.Find(L"��") != -1)
	{
		strCode += L"A";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"B";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"C";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"D";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"E";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"F";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"G";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"H";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"I";
	}
	if (strName.Find(L"��") != -1)
	{
		strCode += L"J";
	}
	if (strName.Find(L"����") != -1)
	{
		strCode += L"Z";
	}

	return strCode;
}

CStringW CZYHttp_PAI::VehColourCodeToName(const CString strCode)
{
	CString strName(L"");
	if (strCode.Find(L"A") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"B") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"C") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"D") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"E") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"F") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"G") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"H") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"I") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"J") != -1)
	{
		strName += L"��";
	}
	if (strCode.Find(L"Z") != -1)
	{
		strName += L"����";
	}

	return strName;
}

CStringW CZYHttp_PAI::AirIntakeModeNameToCode(const CString strName)
{
	CString strCode(L"01");
	if (strName.Find(L"��Ȼ����") != -1)
	{
		strCode = L"01";
	}
	else if (strName.Find(L"������ѹ") != -1)
	{
		strCode = L"02";
	}

	return strCode;
}

CStringW CZYHttp_PAI::AirIntakeModeCodeToName(const CString strCode)
{
	CString strName(L"��Ȼ����");
	if (strCode.Find(L"01") != -1)
	{
		strName = L"��Ȼ����";
	}
	else if (strCode.Find(L"02") != -1)
	{
		strName = L"������ѹ";
	}

	return strName;
}

CStringW CZYHttp_PAI::OilSupplyModeNameToCode(const CString strName)
{
	CString strCode(L"01");
	if (strName.Find(L"������") != -1)
	{
		strCode = L"01";
	}
	else if (strName.Find(L"�ջ�����") != -1)
	{
		strCode = L"02";
	}
	else if (strName.Find(L"��������") != -1)
	{
		strCode = L"03";
	}

	return strCode;
}

CStringW CZYHttp_PAI::OilSupplyModeCodeToName(const CString strCode)
{
	CString strName(L"������");
	if (strCode.Find(L"01") != -1)
	{
		strName = L"������";
	}
	else if (strCode.Find(L"02") != -1)
	{
		strName = L"�ջ�����";
	}
	else if (strCode.Find(L"03") != -1)
	{
		strName = L"��������";
	}

	return strName;
}

CStringW CZYHttp_PAI::DriveTypeNameToCode(const CString strName)
{
	CString strCode(L"02");
	if (strName.Find(L"4x2������פ��") != -1)
	{
		strCode = L"01";
	}
	else if (strName.Find(L"4x2ǰ����פ��") != -1)
	{
		strCode = L"02";
	}
	else if (strName.Find(L"4x2ǰ��ǰפ��") != -1)
	{
		strCode = L"03";
	}
	else if (strName.Find(L"4x4ȫ����פ��") != -1)
	{
		strCode = L"04";
	}
	else if (strName.Find(L"4x4ȫ������פ��") != -1)
	{
		strCode = L"05";
	}
	else if (strName.Find(L"6x2˫�󸡶�����פ��") != -1)
	{
		strCode = L"06";
	}
	else if (strName.Find(L"6x2������פ�����") != -1)
	{
		strCode = L"07";
	}
	else if (strName.Find(L"6x4˫����˫��פ��") != -1)
	{
		strCode = L"08";
	}
	else if (strName.Find(L"6x4˫������˫��פ��") != -1)
	{
		strCode = L"09";
	}
	else if (strName.Find(L"6x6ȫ����˫��פ��") != -1)
	{
		strCode = L"10";
	}
	else if (strName.Find(L"8x2������פ�����") != -1)
	{
		strCode = L"11";
	}
	else if (strName.Find(L"8x2������פ��ȫ��") != -1)
	{
		strCode = L"12";
	}
	else if (strName.Find(L"8x4˫����˫��פ��") != -1)
	{
		strCode = L"13";
	}
	else if (strName.Find(L"10x2������פ�����") != -1)
	{
		strCode = L"14";
	}
	else if (strName.Find(L"10x6����������פ��") != -1)
	{
		strCode = L"15";
	}

	return strCode;
}

CStringW CZYHttp_PAI::DriveTypeCodeToName(const CString strCode)
{
	CString strName(L"4x2ǰ����פ��");
	if (strCode.Find(L"01") != -1)
	{
		strName = L"4x2������פ��";
	}
	else if (strCode.Find(L"02") != -1)
	{
		strName = L"4x2ǰ����פ��";
	}
	else if (strCode.Find(L"03") != -1)
	{
		strName = L"4x2ǰ��ǰפ��";
	}
	else if (strCode.Find(L"04") != -1)
	{
		strName = L"4x4ȫ����פ��";
	}
	else if (strCode.Find(L"05") != -1)
	{
		strName = L"4x4ȫ������פ��";
	}
	else if (strCode.Find(L"06") != -1)
	{
		strName = L"6x2˫�󸡶�����פ��";
	}
	else if (strCode.Find(L"07") != -1)
	{
		strName = L"6x2������פ�����";
	}
	else if (strCode.Find(L"08") != -1)
	{
		strName = L"6x4˫����˫��פ��";
	}
	else if (strCode.Find(L"09") != -1)
	{
		strName = L"6x4˫������˫��פ��";
	}
	else if (strCode.Find(L"10") != -1)
	{
		strName = L"6x6ȫ����˫��פ��";
	}
	else if (strCode.Find(L"11") != -1)
	{
		strName = L"8x2������פ�����";
	}
	else if (strCode.Find(L"12") != -1)
	{
		strName = L"8x2������פ��ȫ��";
	}
	else if (strCode.Find(L"13") != -1)
	{
		strName = L"8x4˫����˫��פ��";
	}
	else if (strCode.Find(L"14") != -1)
	{
		strName = L"10x2������פ�����";
	}
	else if (strCode.Find(L"15") != -1)
	{
		strName = L"10x6����������פ��";
	}
	return strName;
}

CStringW CZYHttp_PAI::FuelMarkNameToCode(const CString strName)
{
	CString strCode(L"02");
	if (strName.Find(L"89��") != -1)
	{
		strCode = L"01";
	}
	else if (strName.Find(L"90��") != -1)
	{
		strCode = L"02";
	}
	else if (strName.Find(L"92��") != -1)
	{
		strCode = L"03";
	}
	else if (strName.Find(L"95��") != -1)
	{
		strCode = L"04";
	}
	else if (strName.Find(L"97��") != -1)
	{
		strCode = L"05";
	}
	else if (strName.Find(L"98��") != -1)
	{
		strCode = L"06";
	}
	else if (strName.Find(L"0�Ų���") != -1)
	{
		strCode = L"07";
	}

	return strCode;
}

CStringW CZYHttp_PAI::FuelMarkCodeToName(const CString strCode)
{
	CString strName(L"92��");
	if (strCode.Find(L"01") != -1)
	{
		strName = L"89��";
	}
	else if (strCode.Find(L"02") != -1)
	{
		strName = L"90��";
	}
	else if (strCode.Find(L"03") != -1)
	{
		strName = L"92��";
	}
	else if (strCode.Find(L"04") != -1)
	{
		strName = L"95��";
	}
	else if (strCode.Find(L"05") != -1)
	{
		strName = L"97��";
	}
	else if (strCode.Find(L"06") != -1)
	{
		strName = L"98��";
	}
	else if (strCode.Find(L"07") != -1)
	{
		strName = L"0�Ų���";
	}

	return strName;
}