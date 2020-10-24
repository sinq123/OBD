/*
* 汽车安检人工检测项目处理类
* 传入104项
* 输出结果
*/

#pragma once

#include <string>
#include <vector>

enum ManualItemType
{
	MIT_NULL = 0,
	MIT_AppRejItem,		// 否决项
	MIT_AppMaiItem,		// 维护项
};

enum VehicleType
{
	VT_Gasoline,
	VT_Diesel,
};

struct SManualItem
{
	int nCode;						// 项目编码
	std::wstring strItem;			// 项目的内容描述
	int nJudge;						// 判定
	std::wstring strReason;			// 不合格原因
	ManualItemType emManualItemType;// 项目类型
	std::wstring strRemark;			// 备注
};

class CManualDetDataHelper
{
public:
	CManualDetDataHelper(void);
	virtual ~CManualDetDataHelper(void);
		
	void LoadGasolineDetItem(void);
	void LoadDieselDetItem(void);

public:
	// 定义项目
	std::vector<SManualItem> m_vManualItem;
	// 检测数据
	std::vector<SManualItem> m_vManualDetData;

};
