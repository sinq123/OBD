/*
稳态限值数据表模型

*/

#if !defined (SGSQLSERVERDBMODEL_ASM_GSXZ_H)
#define SGSQLSERVERDBMODEL_ASM_GSXZ_H

#include <string>

struct SASM_GSXZ
{
	//[ID] [int] IDENTITY(1,1) NOT NULL,
	std::wstring strID;
	//[AREA] [varchar](50) NULL,
	std::wstring strAREA;
	//[PFBZ] [varchar](50) NULL,
	std::wstring strPFBZ;
	//[MINRM] [int] NULL,
	std::wstring strMINRM;
	//[MAXRM] [int] NULL,
	std::wstring strMAXRM;
	//[CO25] [varchar](50) NULL,
	std::wstring strCO25;
	//[HC25] [varchar](50) NULL,
	std::wstring strHC25;
	//[NO25] [varchar](50) NULL,
	std::wstring strNO25;
	//[CO40] [varchar](50) NULL,
	std::wstring strCO40;
	//[HC40] [varchar](50) NULL,
	std::wstring strHC40;
	//[NO40] [varchar](50) NULL,
	std::wstring strNO40;
};

#endif