/*
自由加速限值数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_ZYJS_XZGB_H)
#define SGSQLSERVERDBMODEL_ZYJS_XZGB_H

#include <string>

struct SZYJS_XZGB
{
	//[ID] [int] IDENTITY(1,1) NOT NULL,
	std::wstring strID;
	//[ZRDate20011001btgxz] [varchar](50) NULL,
	std::wstring strZRDate20011001btgxz;
	//[WLDate20011001btgxz] [varchar](50) NULL,
	std::wstring strWLDate20011001btgxz;
	//[ONLYTHIS] [varchar](50) NULL,
	std::wstring strONLYTHIS;
	//[XZTABLE] [varchar](50) NULL,
	std::wstring strXZTABLE;
	//[USESDJNXZ] [varchar](50) NULL,
	std::wstring strUSESDJNXZ;
	//[BTGXZBZ] [varchar](50) NULL,
	std::wstring strBTGXZBZ;
};

#endif