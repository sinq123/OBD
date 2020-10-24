/*
加载减速限值数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_JZJS_XZBZ_H)
#define SGSQLSERVERDBMODEL_JZJS_XZBZ_H

#include <string>

struct SJZJS_XZBZ
{
	//[ID] [varchar](50) NOT NULL,
	std::wstring strID;
	//[G1XZ] [varchar](50) NULL,
	std::wstring strG1XZ;
	//[G2XZ] [varchar](50) NULL,
	std::wstring strG2XZ;
	//[G3XZ] [varchar](50) NULL,
	std::wstring strG3XZ;
	//[G4XZ] [varchar](50) NULL,
	std::wstring strG4XZ;
};

#endif