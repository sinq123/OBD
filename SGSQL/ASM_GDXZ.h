/*
稳态限值数据表模型

*/

#if !defined (SGSQLSERVERDBMODEL_ASM_GDXZ_H)
#define SGSQLSERVERDBMODEL_ASM_GDXZ_H

#include <string>

struct SASM_GDXZ
{
	//[ID] [varchar](50) NOT NULL,
	std::wstring strID;
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