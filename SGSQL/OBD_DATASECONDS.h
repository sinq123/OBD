/*
OBD过程数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_OBD_DATASECONDS_H)
#define SGSQLSERVERDBMODEL_OBD_DATASECONDS_H

#include <string>

struct SOBD_DataSeconds
{
	//[CLID] [varchar](100) NOT NULL,
	std::wstring strCLID;
	//[VIN] [varchar](50) NULL,
	std::wstring strVIN;
	//[StartTime] [datetime] NULL,
	std::wstring strStartTime;
	//[Jcff] [varchar](10) NULL,
	std::wstring strJcff;
	//[Cyds] [varchar](20) NULL,
	std::wstring strCyds;
	//[DATA1] [varchar](max) NULL,
	std::wstring strDATA1;
	//[DATA2] [varchar](max) NULL,
	std::wstring strDATA2;
	//[DATA3] [varchar](max) NULL,
	std::wstring strDATA3;
	//[DATA4] [varchar](max) NULL,
	std::wstring strDATA4;
	//[DATA5] [varchar](max) NULL,
	std::wstring strDATA5;
	//[DATA6] [varchar](max) NULL,
	std::wstring strDATA6;
	//[DATA7] [varchar](max) NULL,
	std::wstring strDATA7;
	//[DATA8] [varchar](max) NULL,
	std::wstring strDATA8;
	//[DATA9] [varchar](max) NULL,
	std::wstring strDATA9;
	//[DATA10] [varchar](max) NULL,
	std::wstring strDATA10;
	//[DATA11] [varchar](max) NULL,
	std::wstring strDATA11;
	//[DATA12] [varchar](max) NULL,
	std::wstring strDATA12;
	//[DATA13] [varchar](max) NULL,
	std::wstring strDATA13;
	//[BY1] [varchar](max) NULL,
	std::wstring strBY1;
	//[BY2] [varchar](max) NULL,
	std::wstring strBY2;
	//[BY3] [varchar](max) NULL
	std::wstring strBY3;
};

#endif