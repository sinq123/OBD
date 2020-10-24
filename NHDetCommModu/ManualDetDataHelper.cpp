#include "StdAfx.h"
#include "ManualDetDataHelper.h"


// 自用库(MFC扩展DLL)
#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include <algorithm>

CManualDetDataHelper::CManualDetDataHelper(void)
{
}

CManualDetDataHelper::~CManualDetDataHelper(void)
{
}

void CManualDetDataHelper::LoadGasolineDetItem(void)
{
	m_vManualItem.clear();

	// 保存日志
	TESTLOG sTestLog;
	GetIniTestLog(&sTestLog);
	
	int nCode = 1;
	SManualItem sManualItem;
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆机械状况是否良好";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"排气污染控制装置是否齐全，正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"否决项目";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆是否存在烧机油或者严重冒黑烟现象";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"否决项目";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"曲轴箱通风系统是否正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"燃油蒸发控制系统是否正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"否决项目";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车上仪表工作是否正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"有无可能影响安全或引起测试偏差的机械故障";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆进、排气系统是否有任何泄漏";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);		
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆的发动机、变速箱和冷却系统等有无明显的液体渗漏";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"是否带OBD系统";
	if (wcscmp(L"1",sTestLog.wchHasOBD)==0)
	{
		sManualItem.nJudge = 1;
	}
	else
	{
		sManualItem.nJudge = 2;
	}
	
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"轮胎气压是否正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"轮胎是否干燥、清洁";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"是否关闭车上空调、暖风等附属设备";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"是否已经中断车辆上可能影响测试正常进行的功能，如ARS、ESP、EPC牵引力控制或自动制动系统等";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆油箱和油品是否异常";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;


	sManualItem.strItem = L"是否适合工况法检测";
	if (wcscmp(L"1",sTestLog.wchTestType)==0 || wcscmp(L"2",sTestLog.wchTestType)==0||wcscmp(L"3",sTestLog.wchTestType)==0 )
	{
		sManualItem.nJudge = 1;
	}
	else
	{
		sManualItem.nJudge = 2;
	}
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
}

void CManualDetDataHelper::LoadDieselDetItem(void)
{
	m_vManualItem.clear();

	TESTLOG sTestLog;
	GetIniTestLog(&sTestLog);

	int nCode = 1;
	SManualItem sManualItem;

	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆机械状况是否良好";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"污染控制装置是否齐全，正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"否决项目";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"发动机燃油系统采用电控泵";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车上仪表工作是否正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆是否存在烧机油或者严重冒黑烟现象";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"否决项目";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"有无可能影响安全或引起测试偏差的机械故障";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆进、排气系统是否有任何泄漏";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆的发动机、变速箱和冷却系统等有无明显的液体渗漏";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"是否带OBD系统";
	if (wcscmp(L"1",sTestLog.wchHasOBD)==0)
	{
		sManualItem.nJudge = 1;
	}
	else
	{
		sManualItem.nJudge = 2;
	}
	
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"轮胎气压是否正常";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);		
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"轮胎是否干燥、清洁";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"是否关闭车上空调、暖风等附属设备";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"是否已经中断车辆上可能影响测试正常进行的功能，如ARS、ESP、EPC牵引力控制或自动制动系统等";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"车辆油箱和油品是否异常";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);

	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"是否适合工况法检测";
	if (wcscmp(L"1",sTestLog.wchTestType)==0 || wcscmp(L"2",sTestLog.wchTestType)==0||wcscmp(L"3",sTestLog.wchTestType)==0 )
	{
		sManualItem.nJudge = 1;
	}
	else
	{
		sManualItem.nJudge = 2;
	}
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
}

//int CManualDetDataHelper::Item_cmp_acs(SManualItem p1, SManualItem p2)
//{
//	return p1.nCode < p2.nCode;
//}
//
//int CManualDetDataHelper::Item_cmp_desc(SManualItem p1, SManualItem p2)
//{
//	return p1.nCode > p2.nCode;
//}
//
//void CManualDetDataHelper::AppendManualDetData(const int nItem)
//{
//	AppendManualDetData(nItem, L"");
//}
//
//void CManualDetDataHelper::AppendManualDetData(const int nItem, LPCTSTR strReason)
//{
//	if (nItem<1 || nItem>m_vManualItem.size())
//	{
//		return;
//	}
//
//	if (GetItemJudge(nItem))
//	{
//		SManualItem sManualItem = m_vManualItem[nItem-1];
//		sManualItem.strReason = strReason;
//		m_vManualDetData.push_back(sManualItem);
//	}
//	else
//	{
//		for (UINT i=0; i<m_vManualDetData.size(); i++)
//		{
//			if (m_vManualDetData[i].nCode == nItem)
//			{
//				m_vManualDetData[i].strReason = strReason;
//				break;
//			}
//		}
//	}
//
//	// 按照项目序号，对vector升序排列
//	sort(m_vManualDetData.begin(), m_vManualDetData.end(), Item_cmp_acs);
//}
//
//void CManualDetDataHelper::AppendManualDetData(LPCTSTR strItem)
//{
//	CString strFailItem(strItem);
//	strFailItem.Trim();
//
//	CString strTokenItem;
//	int curPos= 0;
//	do
//	{
//		// 拆分字符串，支持以中英文逗号，分号和空格分隔
//		strTokenItem = strFailItem.Tokenize(L",， ;；", curPos);		
//		AppendManualDetData(_wtoi(strTokenItem));
//	}
//	while (strTokenItem != L"");
//}
//
//void CManualDetDataHelper::ClearManualDetData(void)
//{
//	m_vManualDetData.clear();
//}
//
//// 获取外观/动态/底盘判定结果
//bool CManualDetDataHelper::GetItemJudge(const ManualItemType emType)
//{
//	for (UINT i=0; i<m_vManualDetData.size(); i++)
//	{
//		if (m_vManualDetData[i].emManualItemType == emType)
//		{
//			// 有不合格项目，返回不合格
//			return false;
//		}
//	}
//	// 没有不合格项，合格
//	return true;
//}
//
//// 获取具体项目的判定结果
//bool CManualDetDataHelper::GetItemJudge(const int nItem)
//{
//	for (UINT i=0; i<m_vManualDetData.size(); i++)
//	{
//		if (m_vManualDetData[i].nCode == nItem)
//		{
//			// 找到不合格项目，返回不合格
//			return false;
//		}
//	}
//	// 没有找到不合格项，合格
//	return true;
//}
//
//// 获取具体项目的不合格原因
//LPCTSTR CManualDetDataHelper::GetItemReason(const int nItem)
//{
//	for (UINT i=0; i<m_vManualDetData.size(); i++)
//	{
//		if (m_vManualDetData[i].nCode == nItem)
//		{
//			// 找到不合格项目
//			return m_vManualDetData[i].strReason.c_str();
//		}
//	}
//	// 没有找到不合格项
//	return L"";
//}
//
//// 获取具体项目的内容描述
//LPCTSTR CManualDetDataHelper::GetItemContent(const int nItem)
//{
//	if (nItem<1 || nItem>m_vManualItem.size())
//	{
//		return L"";
//	}
//	return m_vManualItem[nItem-1].strItem.c_str();
//}