/*
双怠速结果数据表模型

*/
#if !defined (SGSQLSERVERDBMODEL_SDS_H)
#define SGSQLSERVERDBMODEL_SDS_H

#include <string>

struct SSDS
{
	//[CLID] [varchar](50) NOT NULL,
	std::wstring strCLID;
	//[JCBGBH] [varchar](50) NULL,
	std::wstring strJCBGBH;
	//[CLPH] [varchar](50) NULL,
	std::wstring strCLPH;
	//[COLOWCLZ] [varchar](50) NULL,
	std::wstring strCOLOWCLZ;
	//[COLOWXZ] [varchar](50) NULL,
	std::wstring strCOLOWXZ;
	//[COLOWPD] [varchar](50) NULL,
	std::wstring strCOLOWPD;
	//[HCLOWCLZ] [varchar](50) NULL,
	std::wstring strHCLOWCLZ;
	//[HCLOWXZ] [varchar](50) NULL,
	std::wstring strHCLOWXZ;
	//[HCLOWPD] [varchar](50) NULL,
	std::wstring strHCLOWPD;
	//[JYWDLOW] [varchar](50) NULL,
	std::wstring strJYWDLOW;
	//[ZSLOW] [varchar](50) NULL,
	std::wstring strZSLOW;
	//[COHIGHCLZ] [varchar](50) NULL,
	std::wstring strCOHIGHCLZ;
	//[COHIGHXZ] [varchar](50) NULL,
	std::wstring strCOHIGHXZ;
	//[COHIGHPD] [varchar](50) NULL,
	std::wstring strCOHIGHPD;
	//[HCHIGHCLZ] [varchar](50) NULL,
	std::wstring strHCHIGHCLZ;
	//[HCHIGHXZ] [varchar](50) NULL,
	std::wstring strHCHIGHXZ;
	//[HCHIGHPD] [varchar](50) NULL,
	std::wstring strHCHIGHPD;
	//[LAMDAHIGHCLZ] [varchar](50) NULL,
	std::wstring strLAMDAHIGHCLZ;
	//[LAMDAHIGHXZ] [varchar](50) NULL,
	std::wstring strLAMDAHIGHXZ;
	//[LAMDAHIGHPD] [varchar](50) NULL,
	std::wstring strLAMDAHIGHPD;
	//[JYWDHIGH] [varchar](50) NULL,
	std::wstring strJYWDHIGH;
	//[ZSHIGH] [varchar](50) NULL,
	std::wstring strZSHIGH;
	//[LOWPD] [varchar](50) NULL,
	std::wstring strLOWPD;
	//[HIGHPD] [varchar](50) NULL,
	std::wstring strHIGHPD;
	//[ZHPD] [varchar](50) NULL,
	std::wstring strZHPD;
	//[JCRQ] [datetime] NULL,
	std::wstring strJCRQ;
	//[WD] [varchar](50) NULL,
	std::wstring strWD;
	//[SD] [varchar](50) NULL,
	std::wstring strSD;
	//[DQY] [varchar](50) NULL,
	std::wstring strDQY;
	//[SBMC] [varchar](50) NULL,
	std::wstring strSBMC;
	//[SBXH] [varchar](50) NULL,
	std::wstring strSBXH;
	//[SBZZC] [varchar](50) NULL,
	std::wstring strSBZZC;
	//[ZSJXH] [varchar](50) NULL,
	std::wstring strZSJXH;
	//[ZSJBH] [varchar](50) NULL,
	std::wstring strZSJBH;
	//[ZSJZZC] [varchar](50) NULL,
	std::wstring strZSJZZC;
	//[FXYXH] [varchar](50) NULL,
	std::wstring strFXYXH;
	//[FXYBH] [varchar](50) NULL,
	std::wstring strFXYBH;
	//[FXYZZC] [varchar](50) NULL,
	std::wstring strFXYZZC;
	//[SHY] [varchar](50) NULL,
	std::wstring strSHY;
	//[SYNCHDATE] [varchar](50) NULL,
	std::wstring strSYNCHDATE;
	//[YW] [varchar](50) NULL,
	std::wstring strYW;
	//[GLKQXSSX] [varchar](50) NULL,
	std::wstring strGLKQXSSX;
	//[GLKQXSXX] [varchar](50) NULL,
	std::wstring strGLKQXSXX;
	//[JCKSSJ] [varchar](50) NULL,
	std::wstring strJCKSSJ;
	//[JCJSSJ] [varchar](50) NULL,
	std::wstring strJCJSSJ;
	//[CO2HIGH] [varchar](50) NULL,
	std::wstring strCO2HIGH;
	//[O2HIGH] [varchar](50) NULL,
	std::wstring strO2HIGH;
	//[CO2LOW] [varchar](50) NULL,
	std::wstring strCO2LOW;
	//[O2LOW] [varchar](50) NULL,
	std::wstring strO2LOW;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[JYCS] [varchar](50) NULL,
	std::wstring strJYCS;
	//[SBRZBM] [varchar](50) NULL,
	std::wstring strSBRZBM;
	//[COLOWXXZ] [varchar](50) NULL,
	std::wstring strCOLOWXXZ;
	//[COHIGHXXZ] [varchar](50) NULL,
	std::wstring strCOHIGHXXZ;
	//[COLOWXYZ] [varchar](50) NULL,
	std::wstring strCOLOWXYZ;
	//[COHIGHXYZ] [varchar](50) NULL,
	std::wstring strCOHIGHXYZ;
	//[CO2LOWXYZ] [varchar](50) NULL,
	std::wstring strCO2LOWXYZ;
	//[CO2HIGHXYZ] [varchar](50) NULL,
	std::wstring strCO2HIGHXYZ;
	//[HCLOWXYZ] [varchar](50) NULL,
	std::wstring strHCLOWXYZ;
	//[HCHIGHXYZ] [varchar](50) NULL,
	std::wstring strHCHIGHXYZ;
	//[OBDBH] [varchar](50) NULL,
	std::wstring strOBDBH;
	//[OBDXH] [varchar](50) NULL,
	std::wstring strOBDXH;
	//[OBDZZC] [varchar](50) NULL,
	std::wstring strOBDZZC;
};

#endif