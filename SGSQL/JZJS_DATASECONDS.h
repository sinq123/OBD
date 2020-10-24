/*
加载减速过程数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_JZJS_ADTASECONDS_H)
#define SGSQLSERVERDBMODEL_JZJS_ADTASECONDS_H

#include <string>

struct SJZJS_DataSeconds
{
	//[CLID] [varchar](50) NOT NULL,
	std::wstring strCLID;
	//[CLHP] [varchar](50) NULL,
	std::wstring strCLHP;
	//[JCSJ] [datetime] NULL,
	std::wstring strJCSJ;
	//[MMTIME] [varchar](max) NULL,
	std::wstring strMMTIME;
	//[MMSX] [varchar](max) NULL,
	std::wstring strMMSX;
	//[MMLB] [varchar](max) NULL,
	std::wstring strMMLB;
	//[MMCS] [varchar](max) NULL,
	std::wstring strMMCS;
	//[MMGL] [varchar](max) NULL,
	std::wstring strMMGL;
	//[MMK] [varchar](max) NULL,
	std::wstring strMMK;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[JYCS] [varchar](50) NULL,
	std::wstring strJYCS;
	//[CYDS] [varchar](50) NULL,
	std::wstring strCYDS;
	//[MMZS] [varchar](max) NULL,
	std::wstring strMMZS;
	//[MMZGL] [varchar](max) NULL,
	std::wstring strMMZGL;
	//[MMZSGL] [varchar](max) NULL,
	std::wstring strMMZSGL;
	//[MMGLXZXS] [varchar](max) NULL,
	std::wstring strMMGLXZXS;
	//[MMJSGL] [varchar](max) NULL,
	std::wstring strMMJSGL;
	//[MMBTGD] [varchar](max) NULL,
	std::wstring strMMBTGD;
	//[MMDQYL] [varchar](max) NULL,
	std::wstring strMMDQYL;
	//[MMXDSD] [varchar](max) NULL,
	std::wstring strMMXDSD;
	//[MMHJWD] [varchar](max) NULL,
	std::wstring strMMHJWD;
	//[MMNL] [varchar](max) NULL,
	std::wstring strMMNL;
	//[MMNO] [varchar](max) NULL,
	std::wstring strMMNO;
	//[MMYW] [varchar](max) NULL,
	std::wstring strMMYW;
	//[MMCO2] [varchar](max) NULL,
	std::wstring strMMCO2;
	//[MMPRESSURE] [varchar](max) NULL,
	std::wstring strMMPRESSURE;
	//[MMOBDYMKD] [varchar](max) NULL,
	std::wstring strMMOBDYMKD;
	//[MMOBDSPEED] [varchar](max) NULL,
	std::wstring strMMOBDSPEED;
	//[MMOBDPOWER] [varchar](max) NULL,
	std::wstring strMMOBDPOWER;
	//[MMOBDROTATESPEED] [varchar](max) NULL,
	std::wstring strMMOBDROTATESPEED;
	//[MMOBDJQL] [varchar](max) NULL,
	std::wstring strMMOBDJQL;
	//[MMOBDZYYL] [varchar](max) NULL,
	std::wstring strMMOBDZYYL;
	//[MMOBDHYL] [varchar](max) NULL,
	std::wstring strMMOBDHYL;
	//[MMOBDNOND] [varchar](max) NULL,
	std::wstring strMMOBDNOND;
	//[MMOBDNSPSL] [varchar](max) NULL,
	std::wstring strMMOBDNSPSL;
	//[MMOBDWD] [varchar](max) NULL,
	std::wstring strMMOBDWD;
	//[MMOBDKLBJQYC] [varchar](max) NULL,
	std::wstring strMMOBDKLBJQYC;
	//[MMOBDEGRKD] [varchar](max) NULL,
	std::wstring strMMOBDEGRKD;
	//[MMOBDRYPSYL] [varchar](max) NULL,
	std::wstring strMMOBDRYPSYL;
	//[MMOPNO] [varchar](max) NULL,
	std::wstring strMMOPNO;
	//[MMOPCODE] [varchar](max) NULL,
	std::wstring strMMOPCODE;
	//[MMNO2] [varchar](max) NULL,
	std::wstring strMMNO2;
	//[MMNOX] [varchar](max) NULL,
	std::wstring strMMNOX;
	//[MMNJ] [varchar](max) NULL,
	std::wstring strMMNJ;
	//[MMSDXZXS] [varchar](max) NULL,
	std::wstring strMMSDXZXS;
};

#endif