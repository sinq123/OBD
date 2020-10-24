/*
稳态过程数据表模型

*/

#if !defined (SGSQLSERVERDBMODEL_ASM_DATASECONDS_H)
#define SGSQLSERVERDBMODEL_ASM_DATASECONDS_H

#include <string>

struct SASM_DataSeconds
{
	//[CLID] [varchar](50) NOT NULL,
	std::wstring strCLID;
	//[JCSJ] [datetime] NULL,
	std::wstring strJCSJ;
	//[CLHP] [varchar](50) NULL,
	std::wstring strCLHP;
	//[MMTIME] [varchar](8000) NULL,
	std::wstring strMMTIME;
	//[MMLB] [varchar](8000) NULL,
	std::wstring strMMLB;
	//[MMSX] [varchar](8000) NULL,
	std::wstring strMMSX;
	//[MMCO] [varchar](8000) NULL,
	std::wstring strMMCO;
	//[MMHC] [varchar](8000) NULL,
	std::wstring strMMHC;
	//[MMNO] [varchar](8000) NULL,
	std::wstring strMMNO;
	//[MMCO2] [varchar](8000) NULL,
	std::wstring strMMCO2;
	//[MMO2] [varchar](8000) NULL,
	std::wstring strMMO2;
	//[MMWD] [varchar](8000) NULL,
	std::wstring strMMWD;
	//[MMSD] [varchar](8000) NULL,
	std::wstring strMMSD;
	//[MMDQY] [varchar](8000) NULL,
	std::wstring strMMDQY;
	//[MMCS] [varchar](8000) NULL,
	std::wstring strMMCS;
	//[MMGL] [varchar](8000) NULL,
	std::wstring strMMGL;
	//[MMZS] [varchar](8000) NULL,
	std::wstring strMMZS;
	//[MMXSXZ] [varchar](8000) NULL,
	std::wstring strMMXSXZ;
	//[MMSDXZ] [varchar](8000) NULL,
	std::wstring strMMSDXZ;
	//[MMGLYL] [varchar](8000) NULL,
	std::wstring strMMGLYL;
	//[MMYW] [varchar](8000) NULL,
	std::wstring strMMYW;
	//[MMJSGL] [varchar](8000) NULL,
	std::wstring strMMJSGL;
	//[MMZSGL] [varchar](max) NULL,
	std::wstring strMMZSGL;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[JYCS] [varchar](50) NULL,
	std::wstring strJYCS;
	//[CYDS] [varchar](50) NULL,
	std::wstring strCYDS;
	//[MMNL] [varchar](max) NULL,
	std::wstring strMMNL;
	//[MMZT] [varchar](max) NULL,
	std::wstring strMMZT;
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
	//[MMLAMBDA] [varchar](max) NULL,
	std::wstring strMMLAMBDA;
};

#endif