/*
待检列表数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_WAITINGVEH_H)
#define SGSQLSERVERDBMODEL_WAITINGVEH_H

#include <string>

struct SWaitingVeh
{
	//[CLID] [varchar](50) NOT NULL,
	std::wstring strCLID;
	//[DLSJ] [datetime] NULL,
	std::wstring strDLSJ;
	//[CLHP] [varchar](50) NULL,
	std::wstring strCLHP;
	//[CPYS] [varchar](50) NULL,
	std::wstring strCPYS;
	//[JCFF] [varchar](50) NULL,
	std::wstring strJCFF;
	//[XSLC] [varchar](50) NULL,
	std::wstring strXSLC;
	//[JCCS] [varchar](50) NULL,
	std::wstring strJCCS;
	//[CZY] [varchar](50) NULL,
	std::wstring strCZY;
	//[JSY] [varchar](50) NULL,
	std::wstring strJSY;
	//[DLY] [varchar](50) NULL,
	std::wstring strDLY;
	//[JCFY] [varchar](50) NULL,
	std::wstring strJCFY;
	//[TEST] [varchar](50) NULL,
	std::wstring strTEST;
	//[JCBGBH] [varchar](50) NULL,
	std::wstring strJCBGBH;
	//[JCGWH] [varchar](50) NULL,
	std::wstring strJCGWH;
	//[JCZBH] [varchar](50) NULL,
	std::wstring strJCZBH;
	//[JCRQ] [datetime] NULL,
	std::wstring strJCRQ;
	//[BGJCFFYY] [varchar](100) NULL,
	std::wstring strBGJCFFYY;
	//[SFCS] [varchar](50) NULL,
	std::wstring strSFCS;
	//[ECRYPT] [varchar](100) NULL,
	std::wstring strECRYPT;
	//[SFCJ] [varchar](50) NULL,
	std::wstring strSFCJ;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[HPZL] [varchar](50) NULL,
	std::wstring strHPZL;
	//[ZT] [varchar](50) NULL,
	std::wstring strZT;
	//[WXBJ] [varchar](50) NULL,
	std::wstring strWXBJ;
	//[WXCD] [varchar](50) NULL,
	std::wstring strWXCD;
	//[WXSJ] [varchar](50) NULL,
	std::wstring strWXSJ;
	//[WXFY] [varchar](50) NULL,
	std::wstring strWXFY;
	//[GYXTXS] [varchar](50) NULL,
	std::wstring strGYXTXS;
	//[SFLJ] [varchar](50) NULL,
	std::wstring strSFLJ;
	//[FWLX] [varchar](50) NULL,
	std::wstring strFWLX;
	//[ICCARDNO] [varchar](50) NULL,
	std::wstring strICCARDNO;
	//[DPFS] [varchar](50) NULL,
	std::wstring strDPFS;
	//[SOURCE] [varchar](50) NULL,
	std::wstring strSOURCE;
	//[VIN] [varchar](50) NULL,
	std::wstring strVIN;
	//[lambdaup] [varchar](50) NULL,
	std::wstring strlambdaup;
	//[lambdadown] [varchar](50) NULL,
	std::wstring strlambdadown;
	//[uselambda] [varchar](50) NULL,
	std::wstring struselambda;
	//[IsOutlookChecked] [varchar](10) NULL,
	std::wstring strIsOutlookChecked;
	//[OutLookCheckID] [varchar](50) NULL,
	std::wstring strOutLookCheckID;
	//[OBDPFONLY] [varchar](10) NULL,
	std::wstring strOBDPFONLY;
	//[HaveCheckOBD] [varchar](100) NULL,
	std::wstring strHaveCheckOBD;
};

#endif