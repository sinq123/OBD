/*
双怠速过程数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_SDS_SECONDS_H)
#define SGSQLSERVERDBMODEL_SDS_SECONDS_H

#include <string>

struct SSDS_DataSeconds
{
	//[CLID] [varchar](50) NOT NULL,
	std::wstring strCLID;
	//[CLHP] [varchar](50) NULL,
	std::wstring strCLHP;
	//[JCSJ] [datetime] NULL,
	std::wstring strJCSJ;
	//[MMTIME] [varchar](8000) NULL,
	std::wstring strMMTIME;
	//[MMSX] [varchar](8000) NULL,
	std::wstring strMMSX;
	//[MMLB] [varchar](8000) NULL,
	std::wstring strMMLB;
	//[MMHC] [varchar](8000) NULL,
	std::wstring strMMHC;
	//[MMCO] [varchar](8000) NULL,
	std::wstring strMMCO;
	//[MMO2] [varchar](8000) NULL,
	std::wstring strMMO2;
	//[MMCO2] [varchar](8000) NULL,
	std::wstring strMMCO2;
	//[MMLAMDA] [varchar](8000) NULL,
	std::wstring strMMLAMDA;
	//[MMZS] [varchar](8000) NULL,
	std::wstring strMMZS;
	//[MMYW] [varchar](8000) NULL,
	std::wstring strMMYW;
	//[MMWD] [varchar](8000) NULL,
	std::wstring strMMWD;
	//[MMSD] [varchar](8000) NULL,
	std::wstring strMMSD;
	//[MMDQY] [varchar](8000) NULL,
	std::wstring strMMDQY;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[JYCS] [varchar](50) NULL,
	std::wstring strJYCS;
	//[CYDS] [varchar](50) NULL,
	std::wstring strCYDS;
	//[MMOBDJQMJDKD] [varchar](max) NULL,
	std::wstring strMMOBDJQMJDKD;
	//[MMOBDJSFHZ] [varchar](max) NULL,
	std::wstring strMMOBDJSFHZ;
	//[MMOBDQYCGQXH] [varchar](max) NULL,
	std::wstring strMMOBDQYCGQXH;
	//[MMOBDLAMBDA] [varchar](max) NULL,
	std::wstring strMMOBDLAMBDA;
	//[MMOBDSPEED] [varchar](max) NULL,
	std::wstring strMMOBDSPEED;
	//[MMOBDROTATESPEED] [varchar](max) NULL,
	std::wstring strMMOBDROTATESPEED;
	//[MMOBDJQL] [varchar](max) NULL,
	std::wstring strMMOBDJQL;
	//[MMOBDJQYL] [varchar](max) NULL,
	std::wstring strMMOBDJQYL;
	//[MMNO] [varchar](max) NULL,
	std::wstring strMMNO;
};

#endif