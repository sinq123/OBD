/*
OutLookCheck数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_OUTLOOLCHECK_H)
#define SGSQLSERVERDBMODEL_OUTLOOLCHECK_H

#include <string>

struct SOutLookCheck
{
	//[ID] [bigint] IDENTITY(1,1) NOT NULL,
	std::wstring strID;
	//[CLHP] [varchar](50) NULL,
	std::wstring strCLHP;
	//[HPYS] [varchar](10) NULL,
	std::wstring strHPYS;
	//[VIN] [varchar](50) NULL,
	std::wstring strVIN;
	//[RLZL] [varchar](20) NULL,
	std::wstring strRLZL;
	//[JYLB] [varchar](50) NULL,
	std::wstring strJYLB;
	//[JCFF] [varchar](50) NULL,
	std::wstring strJCFF;
	//[JYR] [varchar](50) NULL,
	std::wstring strJYR;
	//[Data1] [varchar](50) NULL,
	std::wstring strData1;
	//[Data2] [varchar](50) NULL,
	std::wstring strData2;
	//[Data3] [varchar](500) NULL,
	std::wstring strData3;
	//[Data4] [varchar](50) NULL,
	std::wstring strData4;
	//[Data5] [varchar](500) NULL,
	std::wstring strData5;
	//[Data6] [varchar](50) NULL,
	std::wstring strData6;
	//[Data7] [varchar](50) NULL,
	std::wstring strData7;
	//[Data8] [varchar](500) NULL,
	std::wstring strData8;
	//[Data9] [varchar](50) NULL,
	std::wstring strData9;
	//[Data10] [varchar](50) NULL,
	std::wstring strData10;
	//[Data11] [varchar](50) NULL,
	std::wstring strData11;
	//[Data12] [varchar](50) NULL,
	std::wstring strData12;
	//[Data13] [varchar](50) NULL,
	std::wstring strData13;
	//[Data14] [varchar](50) NULL,
	std::wstring strData14;
	//[Data15] [varchar](50) NULL,
	std::wstring strData15;
	//[Data16] [varchar](50) NULL,
	std::wstring strData16;
	//[Data17] [varchar](50) NULL,
	std::wstring strData17;
	//[Data18] [varchar](50) NULL,
	std::wstring strData18;
	//[Data19] [varchar](50) NULL,
	std::wstring strData19;
	//[CheckResult] [varchar](50) NULL,
	std::wstring strCheckResult;
	//[CheckTime] [datetime] NULL,
	std::wstring strCheckTime;
	//[UploadStatus] [varchar](50) NULL,
	std::wstring strUploadStatus;
	//[BY1] [varchar](100) NULL,
	std::wstring strBY1;
	//[BY2] [varchar](100) NULL,
	std::wstring strBY2;
	//[BY3] [varchar](100) NULL,
	std::wstring strBY3;
};

#endif