/*
自由加速过程数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_ZYJS_DATASECONDS_H)
#define SGSQLSERVERDBMODEL_ZYJS_DATASECONDS_H

#include <string>

struct SZYJS_DataSeconds
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
	//[MMK] [varchar](8000) NULL,
	std::wstring strMMK;
	//[MMZS] [varchar](8000) NULL,
	std::wstring strMMZS;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[JYCS] [varchar](50) NULL,
	std::wstring strJYCS;
	//[CYDS] [varchar](50) NULL,
	std::wstring strCYDS;
	//[MMYW] [varchar](max) NULL,
	std::wstring strMMYW;
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
	//[MMWD] [varchar](max) NULL,
	std::wstring strMMWD;
	//[MMSD] [varchar](max) NULL,
	std::wstring strMMSD;
	//[MMDQY] [varchar](max) NULL,
	std::wstring strMMDQY;
	//[MMNS] [varchar](max) NULL,
	std::wstring strMMNS;
};

#endif