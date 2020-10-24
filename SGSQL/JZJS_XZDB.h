/*
加载减速限值数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_JZJS_XZDB_H)
#define SGSQLSERVERDBMODEL_JZJS_XZDB_H

#include <string>

struct SJZJS_XZDB
{
	//[ID] [int] IDENTITY(1,1) NOT NULL,
	std::wstring strID;
	//[d1Date20000701b] [varchar](50) NULL,
	std::wstring strd1Date20000701b;
	//[d1Date2000070120050630] [varchar](50) NULL,
	std::wstring strd1Date2000070120050630;
	//[d1Date20050701] [varchar](50) NULL,
	std::wstring strd1Date20050701;
	//[d2Date20011001b] [varchar](50) NULL,
	std::wstring strd2Date20011001b;
	//[d2Date2001100120060630] [varchar](50) NULL,
	std::wstring strd2Date2001100120060630;
	//[d2Date20060701] [varchar](50) NULL,
	std::wstring strd2Date20060701;
	//[zxDate20010901b] [varchar](50) NULL,
	std::wstring strzxDate20010901b;
	//[zxDate2001090120040831] [varchar](50) NULL,
	std::wstring strzxDate2001090120040831;
	//[zxDate20040901] [varchar](50) NULL,
	std::wstring strzxDate20040901;
};

#endif