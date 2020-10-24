/*
自由加速结果数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_ZYJS_BTG_H)
#define SGSQLSERVERDBMODEL_ZYJS_BTG_H

#include <string>

struct SZYJS_Btg
{
	//[CLID] [varchar](50) NOT NULL,
	std::wstring strCLID;
	//[JCBGBH] [varchar](50) NULL,
	std::wstring strJCBGBH;
	//[CLPH] [varchar](50) NULL,
	std::wstring strCLPH;
	//[JCRQ] [datetime] NULL,
	std::wstring strJCRQ;
	//[FIRSTDATA] [varchar](50) NULL,
	std::wstring strFIRSTDATA;
	//[SECONDDATA] [varchar](50) NULL,
	std::wstring strSECONDDATA;
	//[THIRDDATA] [varchar](50) NULL,
	std::wstring strTHIRDDATA;
	//[AVERAGEDATA] [varchar](50) NULL,
	std::wstring strAVERAGEDATA;
	//[YDXZ] [varchar](50) NULL,
	std::wstring strYDXZ;
	//[YDPD] [varchar](50) NULL,
	std::wstring strYDPD;
	//[DSZS] [varchar](50) NULL,
	std::wstring strDSZS;
	//[ZHPD] [varchar](50) NULL,
	std::wstring strZHPD;
	//[WD] [varchar](50) NULL,
	std::wstring strWD;
	//[SD] [varchar](50) NULL,
	std::wstring strSD;
	//[DQY] [varchar](50) NULL,
	std::wstring strDQY;
	//[SBMC] [varchar](50) NULL,
	std::wstring strSBMC;
	//[SBXH] [varchar](50) NULL,
	std::wstring strSBXH;
	//[SBZZC] [varchar](50) NULL,
	std::wstring strSBZZC;
	//[YDJXH] [varchar](50) NULL,
	std::wstring strYDJXH;
	//[YDJBH] [varchar](50) NULL,
	std::wstring strYDJBH;
	//[YDJZZC] [varchar](50) NULL,
	std::wstring strYDJZZC;
	//[ZSJXH] [varchar](50) NULL,
	std::wstring strZSJXH;
	//[ZSJBH] [varchar](50) NULL,
	std::wstring strZSJBH;
	//[ZSJZZC] [varchar](50) NULL,
	std::wstring strZSJZZC;
	//[SHY] [varchar](50) NULL,
	std::wstring strSHY;
	//[SYNCHDATE] [varchar](50) NULL,
	std::wstring strSYNCHDATE;
	//[YW] [varchar](50) NULL,
	std::wstring strYW;
	//[JCKSSJ] [varchar](50) NULL,
	std::wstring strJCKSSJ;
	//[JCJSSJ] [varchar](50) NULL,
	std::wstring strJCJSSJ;
	//[JCFF] [varchar](50) NULL,
	std::wstring strJCFF;
	//[FOURTHDATA] [varchar](50) NULL,
	std::wstring strFOURTHDATA;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[JYCS] [varchar](50) NULL,
	std::wstring strJYCS;
	//[SBRZBM] [varchar](50) NULL,
	std::wstring strSBRZBM;
	//[OBDBH] [varchar](50) NULL,
	std::wstring strOBDBH;
	//[OBDXH] [varchar](50) NULL,
	std::wstring strOBDXH;
	//[OBDZZC] [varchar](50) NULL,
	std::wstring strOBDZZC;
	//[SCZS] [varchar](50) NULL,
	std::wstring strSCZS;
	//[FIRSTSCZS] [varchar](50) NULL,
	std::wstring strFIRSTSCZS;
	//[SECONDSCZS] [varchar](50) NULL,
	std::wstring strSECONDSCZS;
	//[THIRDSCZS] [varchar](50) NULL,
	std::wstring strTHIRDSCZS;
};

#endif