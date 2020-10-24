/*
车辆型号数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_CLXHXXB_H)
#define SGSQLSERVERDBMODEL_CLXHXXB_H

#include <string>

struct SCLXHXXB
{
	//[CLXHBH] [int] IDENTITY(1,1) NOT NULL,
	std::wstring strCLXHBH;
	//[CLLX] [varchar](32) NULL,
	std::wstring strCLLX;
	//[CLXH] [varchar](32) NULL,
	std::wstring strCLXH;
	//[ZZCS] [varchar](64) NULL,
	std::wstring strZZCS;
	//[CLPP] [varchar](16) NULL,
	std::wstring strCLPP;
	//[JZZL] [int] NULL,
	std::wstring strJZZL;
	//[ZDZZL] [int] NULL,
	std::wstring strZDZZL;
	//[FDJPL] [float] NULL,
	std::wstring strFDJPL;
	//[RLLX] [varchar](32) NULL,
	std::wstring strRLLX;
	//[FDJEDZS] [float] NULL,
	std::wstring strFDJEDZS;
	//[QGS] [int] NULL,
	std::wstring strQGS;
	//[FDJEDGL] [float] NULL,
	std::wstring strFDJEDGL;
	//[GYFS] [varchar](32) NULL,
	std::wstring strGYFS;
	//[JQFS] [varchar](32) NULL,
	std::wstring strJQFS;
	//[BSXLX] [varchar](32) NULL,
	std::wstring strBSXLX;
	//[QDFS] [varchar](32) NULL,
	std::wstring strQDFS;
	//[JCFF] [varchar](32) NULL,
	std::wstring strJCFF;
	//[HDZK] [int] NULL,
	std::wstring strHDZK;
	//[JHZZ] [varchar](32) NULL,
	std::wstring strJHZZ;
};

#endif