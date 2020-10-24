/*
稳态结果数据表模型

*/

#if !defined (SGSQLSERVERDBMODEL_ASM_H)
#define SGSQLSERVERDBMODEL_ASM_H

#include <string>

struct SASM
{
	//[CLID] [varchar](50) NOT NULL,
	std::wstring strCLID;
	//[JCBGBH] [varchar](50) NULL,
	std::wstring strJCBGBH;
	//[CLPH] [varchar](50) NULL,
	std::wstring strCLPH;
	//[CO25CLZ] [varchar](50) NULL,
	std::wstring strCO25CLZ;
	//[CO25XZ] [varchar](50) NULL,
	std::wstring strCO25XZ;
	//[CO25PD] [varchar](50) NULL,
	std::wstring strCO25PD;
	//[NOX25CLZ] [varchar](50) NULL,
	std::wstring strNOX25CLZ;
	//[NOX25XZ] [varchar](50) NULL,
	std::wstring strNOX25XZ;
	//[NOX25PD] [varchar](50) NULL,
	std::wstring strNOX25PD;
	//[HC25CLZ] [varchar](50) NULL,
	std::wstring strHC25CLZ;
	//[HC25XZ] [varchar](50) NULL,
	std::wstring strHC25XZ;
	//[HC25PD] [varchar](50) NULL,
	std::wstring strHC25PD;
	//[CO40CLZ] [varchar](50) NULL,
	std::wstring strCO40CLZ;
	//[CO40XZ] [varchar](50) NULL,
	std::wstring strCO40XZ;
	//[CO40PD] [varchar](50) NULL,
	std::wstring strCO40PD;
	//[NOX40CLZ] [varchar](50) NULL,
	std::wstring strNOX40CLZ;
	//[NOX40XZ] [varchar](50) NULL,
	std::wstring strNOX40XZ;
	//[NOX40PD] [varchar](50) NULL,
	std::wstring strNOX40PD;
	//[HC40CLZ] [varchar](50) NULL,
	std::wstring strHC40CLZ;
	//[HC40XZ] [varchar](50) NULL,
	std::wstring strHC40XZ;
	//[HC40PD] [varchar](50) NULL,
	std::wstring strHC40PD;
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
	//[CGJXH] [varchar](50) NULL,
	std::wstring strCGJXH;
	//[CGJBH] [varchar](50) NULL,
	std::wstring strCGJBH;
	//[CGJZZC] [varchar](50) NULL,
	std::wstring strCGJZZC;
	//[FXYXH] [varchar](50) NULL,
	std::wstring strFXYXH;
	//[FXYBH] [varchar](50) NULL,
	std::wstring strFXYBH;
	//[FXYZZC] [varchar](50) NULL,
	std::wstring strFXYZZC;
	//[FDJZS5025] [varchar](50) NULL,
	std::wstring strFDJZS5025;
	//[FDJYW5025] [varchar](50) NULL,
	std::wstring strFDJYW5025;
	//[FDJZS2540] [varchar](50) NULL,
	std::wstring strFDJZS2540;
	//[FDJYW2540] [varchar](50) NULL,
	std::wstring strFDJYW2540;
	//[SHY] [varchar](50) NULL,
	std::wstring strSHY;
	//[SYNCHDATE] [varchar](50) NULL,
	std::wstring strSYNCHDATE;
	//[YW] [varchar](50) NULL,
	std::wstring strYW;
	//[JCKSSJ] [varchar](50) NULL,
	std::wstring strJCKSSJ;
	//[JCJSSJ] [varchar](50) NULL,
	std::wstring strJCJSSJ;
	//[CO25025] [varchar](50) NULL,
	std::wstring strCO25025;
	//[O25025] [varchar](50) NULL,
	std::wstring strO25025;
	//[CO22540] [varchar](50) NULL,
	std::wstring strCO22540;
	//[O22540] [varchar](50) NULL,
	std::wstring strO22540;
	//[JYLSH] [varchar](50) NULL,
	std::wstring strJYLSH;
	//[JYCS] [varchar](50) NULL,
	std::wstring strJYCS;
	//[SBRZBM] [varchar](50) NULL,
	std::wstring strSBRZBM;
	//[LAMBDA5025] [varchar](50) NULL,
	std::wstring strLAMBDA5025;
	//[LAMBDA2540] [varchar](50) NULL,
	std::wstring strLAMBDA2540;
	//[JZZGL5025] [varchar](50) NULL,
	std::wstring strJZZGL5025;
	//[JZZGL2540] [varchar](50) NULL,
	std::wstring strJZZGL2540;
	//[OBDBH] [varchar](50) NULL,
	std::wstring strOBDBH;
	//[OBDXH] [varchar](50) NULL,
	std::wstring strOBDXH;
	//[OBDZZC] [varchar](50) NULL,
	std::wstring strOBDZZC;
	//[IS5025END] [varchar](50) NULL,
	std::wstring strIS5025END;
	//[CS5025] [varchar](50) NULL,
	std::wstring strCS5025;
	//[CS2540] [varchar](50) NULL,
	std::wstring strCS2540;
};

#endif