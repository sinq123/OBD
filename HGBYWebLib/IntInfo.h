#ifndef INTINFO_H
#define INTINFO_H

#include <string>

// 返回定义
struct SHGBYMSG
{
	// code
	std::wstring strcode;
	// message
	std::wstring strmessage;
	// body
	std::wstring strbody;
};

struct SGetJobList
{
	//AUT_PLATE	String	号牌号码
	std::wstring strAUT_PLATE;
	//AUT_PCLASS	String	号牌种类
	std::wstring strAUT_PCLASS;
	//DETECT_ID	Long	检测流水号
	std::wstring strDETECT_ID;
	//DET_STATUS	Int	检测状态
	std::wstring strDET_STATUS;
	//DET_DATE	String	检测时间
	std::wstring strDET_DATE;
	//INSPECT_ID	String	检测项目ID//ASM  稳态工况法；//LUGD 加载减速法；//TSIC 双怠速法；//FRAO自由加速-不透光烟度计法；//FRAF 自由加速-滤纸烟度法；
	std::wstring strINSPECT_ID;
	//UNDO_ITEMS	String	未完成项目，以”,”分开,如“V,O,G”//V 外观检测//O OBD检测//G 尾气检测
	std::wstring strUNDO_ITEMS;
	//TESTCATEGORY	String	检验类型,1- 定期检验、2-注册登记检验、3-实验比对 4- 监督性抽检、5- 变更登记和转移登记检验
	std::wstring strTESTCATEGORY;
};

#endif