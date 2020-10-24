/*
OBD结果数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_OBD_H)
#define SGSQLSERVERDBMODEL_OBD_H

#include <string>

struct SOBD
{
	//[CLID] [varchar](100) NOT NULL,
	std::wstring strCLID;
	//[StationID] [varchar](100) NULL,
	std::wstring strStationID;
	//[LineID] [varchar](100) NULL,
	std::wstring strLineID;
	//[DeviceID] [varchar](100) NULL,
	std::wstring strDeviceID;
	//[StartTime] [datetime] NULL,
	std::wstring strStartTime;
	//[EndTime] [datetime] NULL,
	std::wstring strEndTime;
	//[VIN] [varchar](50) NULL,
	std::wstring strVIN;
	//[XSJYYQ] [varchar](100) NULL,
	std::wstring strXSJYYQ;
	//[XSLC] [varchar](50) NULL,
	std::wstring strXSLC;
	//[KZDYMC] [varchar](200) NULL,
	std::wstring strKZDYMC;
	//[CALID] [varchar](200) NULL,
	std::wstring strCALID;
	//[CVN] [varchar](200) NULL,
	std::wstring strCVN;
	//[TX] [varchar](10) NULL,
	std::wstring strTX;
	//[TXSBYY] [varchar](20) NULL,
	std::wstring strTXSBYY;
	//[XSMILZT] [varchar](200) NULL,
	std::wstring strXSMILZT;
	//[SCMILZT] [varchar](200) NULL,
	std::wstring strSCMILZT;
	//[GZDM] [varchar](2000) NULL,
	std::wstring strGZDM;
	//[MILXSLC] [varchar](20) NULL,
	std::wstring strMILXSLC;
	//[GZZDQMS] [varchar](200) NULL,
	std::wstring strGZZDQMS;
	//[ZDJXZT] [varchar](200) NULL,
	std::wstring strZDJXZT;
	//[IUPR1] [varchar](100) NULL,
	std::wstring strIUPR1;
	//[IUPR2] [varchar](100) NULL,
	std::wstring strIUPR2;
	//[IUPR3] [varchar](100) NULL,
	std::wstring strIUPR3;
	//[IUPR4] [varchar](100) NULL,
	std::wstring strIUPR4;
	//[IUPR5] [varchar](100) NULL,
	std::wstring strIUPR5;
	//[IUPR6] [varchar](100) NULL,
	std::wstring strIUPR6;
	//[IUPR7] [varchar](100) NULL,
	std::wstring strIUPR7;
	//[IUPR8] [varchar](100) NULL,
	std::wstring strIUPR8;
	//[IUPR9] [varchar](100) NULL,
	std::wstring strIUPR9;
	//[IUPR10] [varchar](100) NULL,
	std::wstring strIUPR10;
	//[IUPR11] [varchar](100) NULL,
	std::wstring strIUPR11;
	//[YCPFCZZD] [varchar](50) NULL,
	std::wstring strYCPFCZZD;
	//[YCPFCZZDTX] [varchar](100) NULL,
	std::wstring strYCPFCZZDTX;
	//[FAILEDKIND] [varchar](100) NULL,
	std::wstring strFAILEDKIND;
	//[PDJG] [varchar](10) NULL,
	std::wstring strPDJG;
	//[Remark] [varchar](800) NULL,
	std::wstring strRemark;
	//[UploadStatus] [varchar](10) NULL,
	std::wstring strUploadStatus;
	//[BY1] [varchar](200) NULL,
	std::wstring strBY1;
	//[BY2] [varchar](200) NULL,
	std::wstring strBY2;
	//[BY3] [varchar](200) NULL,
	std::wstring strBY3;
	//[FREEZEDATA] [varchar](max) NULL,
	std::wstring strFREEZEDATA;
};

#endif