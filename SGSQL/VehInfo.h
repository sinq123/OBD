/*
车辆信息数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_VEHINFO_H)
#define SGSQLSERVERDBMODEL_VEHINFO_H

#include <string>

struct SVehInfo
{
	//[CLHP] [varchar](50) NOT NULL,
	std::wstring strCLHP;
	//[CPYS] [varchar](50) NOT NULL,
	std::wstring strCPYS;
	//[CLLX] [varchar](50) NULL,
	std::wstring strCLLX;
	//[CLZL] [varchar](50) NULL,
	std::wstring strCLZL;
	//[SSXQ] [varchar](50) NULL,
	std::wstring strSSXQ;
	//[SFWDZR] [varchar](50) NULL,
	std::wstring strSFWDZR;
	//[SFYQBF] [varchar](50) NULL,
	std::wstring strSFYQBF;
	//[CZ] [varchar](100) NULL,
	std::wstring strCZ;
	//[CZDZ] [varchar](100) NULL,
	std::wstring strCZDZ;
	//[SYXZ] [varchar](50) NULL,
	std::wstring strSYXZ;
	//[PP] [varchar](50) NULL,
	std::wstring strPP;
	//[XH] [varchar](50) NULL,
	std::wstring strXH;
	//[CLSBM] [varchar](50) NULL,
	std::wstring strCLSBM;
	//[FDJHM] [varchar](50) NULL,
	std::wstring strFDJHM;
	//[FDJXH] [varchar](50) NULL,
	std::wstring strFDJXH;
	//[FDJSCQY] [varchar](100) NULL,
	std::wstring strFDJSCQY;
	//[QDLTQY] [varchar](50) NULL,
	std::wstring strQDLTQY;
	//[RYPH] [varchar](50) NULL,
	std::wstring strRYPH;
	//[SCQY] [varchar](100) NULL,
	std::wstring strSCQY;
	//[HDZK] [varchar](50) NULL,
	std::wstring strHDZK;
	//[JSSZK] [varchar](50) NULL,
	std::wstring strJSSZK;
	//[ZZL] [varchar](50) NULL,
	std::wstring strZZL;
	//[HDZZL] [varchar](50) NULL,
	std::wstring strHDZZL;
	//[ZBZL] [varchar](50) NULL,
	std::wstring strZBZL;
	//[JZZL] [varchar](50) NULL,
	std::wstring strJZZL;
	//[ZCRQ] [datetime] NULL,
	std::wstring strZCRQ;
	//[SCRQ] [datetime] NULL,
	std::wstring strSCRQ;
	//[FDJPL] [varchar](50) NULL,
	std::wstring strFDJPL;
	//[RLZL] [varchar](50) NULL,
	std::wstring strRLZL;
	//[EDGL] [varchar](50) NULL,
	std::wstring strEDGL;
	//[EDZS] [varchar](50) NULL,
	std::wstring strEDZS;
	//[BSQXS] [varchar](50) NULL,
	std::wstring strBSQXS;
	//[DWS] [varchar](50) NULL,
	std::wstring strDWS;
	//[GYFS] [varchar](50) NULL,
	std::wstring strGYFS;
	//[DPFS] [varchar](50) NULL,
	std::wstring strDPFS;
	//[JQFS] [varchar](50) NULL,
	std::wstring strJQFS;
	//[QGS] [varchar](50) NULL,
	std::wstring strQGS;
	//[QDXS] [varchar](50) NULL,
	std::wstring strQDXS;
	//[CHZZ] [varchar](50) NULL,
	std::wstring strCHZZ;
	//[DLSP] [varchar](50) NULL,
	std::wstring strDLSP;
	//[SFSRL] [varchar](50) NULL,
	std::wstring strSFSRL;
	//[JHZZ] [varchar](50) NULL,
	std::wstring strJHZZ;
	//[OBD] [varchar](50) NULL,
	std::wstring strOBD;
	//[DKGYYB] [varchar](50) NULL,
	std::wstring strDKGYYB;
	//[LXDH] [varchar](50) NULL,
	std::wstring strLXDH;
	//[JCFS] [varchar](50) NULL,
	std::wstring strJCFS;
	//[JCLB] [varchar](50) NULL,
	std::wstring strJCLB;
	//[HPZL] [varchar](50) NULL,
	std::wstring strHPZL;
	//[CSYS] [varchar](50) NULL,
	std::wstring strCSYS;
	//[ZXBZ] [varchar](50) NULL,
	std::wstring strZXBZ;
	//[CCS] [varchar](50) NULL,
	std::wstring strCCS;
	//[ZDJGL] [varchar](50) NULL,
	std::wstring strZDJGL;
	//[CYXZ] [varchar](50) NULL,
	std::wstring strCYXZ;
	//[SCR] [varchar](50) NULL,
	std::wstring strSCR;
	//[SCRXH] [varchar](50) NULL,
	std::wstring strSCRXH;
	//[DPF] [varchar](50) NULL,
	std::wstring strDPF;
	//[DPFXH] [varchar](50) NULL,
	std::wstring strDPFXH;
	//[DDJXH] [varchar](50) NULL,
	std::wstring strDDJXH;
	//[CNZZXH] [varchar](50) NULL,
	std::wstring strCNZZXH;
	//[DCRL] [varchar](50) NULL,
	std::wstring strDCRL;
	//[BY1] [varchar](50) NULL,
	std::wstring strBY1;
	//[BY2] [varchar](50) NULL,
	std::wstring strBY2;
	//[BY3] [varchar](50) NULL,
	std::wstring strBY3;
	//[BY4] [varchar](50) NULL,
	std::wstring strBY4;
	//[ID] [int] IDENTITY(1,1) NOT NULL
	std::wstring strID;
};

#endif