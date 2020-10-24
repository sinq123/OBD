#include "StdAfx.h"
#include "ManualDetDataHelper.h"


// ���ÿ�(MFC��չDLL)
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

	// ������־
	TESTLOG sTestLog;
	GetIniTestLog(&sTestLog);
	
	int nCode = 1;
	SManualItem sManualItem;
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"������е״���Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"������Ⱦ����װ���Ƿ���ȫ������";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"�����Ŀ";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����Ƿ�����ջ��ͻ�������ð��������";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"�����Ŀ";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"������ͨ��ϵͳ�Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"ȼ����������ϵͳ�Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"�����Ŀ";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����Ǳ����Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"���޿���Ӱ�찲ȫ���������ƫ��Ļ�е����";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"������������ϵͳ�Ƿ����κ�й©";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);		
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����ķ����������������ȴϵͳ���������Ե�Һ����©";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�Ƿ��OBDϵͳ";
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
	sManualItem.strItem = L"��̥��ѹ�Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"��̥�Ƿ������";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�Ƿ�رճ��Ͽյ���ů��ȸ����豸";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�Ƿ��Ѿ��жϳ����Ͽ���Ӱ������������еĹ��ܣ���ARS��ESP��EPCǣ�������ƻ��Զ��ƶ�ϵͳ��";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����������Ʒ�Ƿ��쳣";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;


	sManualItem.strItem = L"�Ƿ��ʺϹ��������";
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
	sManualItem.strItem = L"������е״���Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"��Ⱦ����װ���Ƿ���ȫ������";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"�����Ŀ";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"������ȼ��ϵͳ���õ�ر�";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����Ǳ����Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����Ƿ�����ջ��ͻ�������ð��������";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppRejItem;
	sManualItem.strRemark = L"�����Ŀ";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"���޿���Ӱ�찲ȫ���������ƫ��Ļ�е����";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"������������ϵͳ�Ƿ����κ�й©";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����ķ����������������ȴϵͳ���������Ե�Һ����©";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�Ƿ��OBDϵͳ";
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
	sManualItem.strItem = L"��̥��ѹ�Ƿ�����";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);		
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"��̥�Ƿ������";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�Ƿ�رճ��Ͽյ���ů��ȸ����豸";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�Ƿ��Ѿ��жϳ����Ͽ���Ӱ������������еĹ��ܣ���ARS��ESP��EPCǣ�������ƻ��Զ��ƶ�ϵͳ��";
	sManualItem.nJudge = 1;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);
	
	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�����������Ʒ�Ƿ��쳣";
	sManualItem.nJudge = 2;
	sManualItem.emManualItemType = MIT_AppMaiItem;
	sManualItem.strRemark = L"";
	m_vManualItem.push_back(sManualItem);

	sManualItem.nCode = nCode++;
	sManualItem.strItem = L"�Ƿ��ʺϹ��������";
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
//	// ������Ŀ��ţ���vector��������
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
//		// ����ַ�����֧������Ӣ�Ķ��ţ��ֺźͿո�ָ�
//		strTokenItem = strFailItem.Tokenize(L",�� ;��", curPos);		
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
//// ��ȡ���/��̬/�����ж����
//bool CManualDetDataHelper::GetItemJudge(const ManualItemType emType)
//{
//	for (UINT i=0; i<m_vManualDetData.size(); i++)
//	{
//		if (m_vManualDetData[i].emManualItemType == emType)
//		{
//			// �в��ϸ���Ŀ�����ز��ϸ�
//			return false;
//		}
//	}
//	// û�в��ϸ���ϸ�
//	return true;
//}
//
//// ��ȡ������Ŀ���ж����
//bool CManualDetDataHelper::GetItemJudge(const int nItem)
//{
//	for (UINT i=0; i<m_vManualDetData.size(); i++)
//	{
//		if (m_vManualDetData[i].nCode == nItem)
//		{
//			// �ҵ����ϸ���Ŀ�����ز��ϸ�
//			return false;
//		}
//	}
//	// û���ҵ����ϸ���ϸ�
//	return true;
//}
//
//// ��ȡ������Ŀ�Ĳ��ϸ�ԭ��
//LPCTSTR CManualDetDataHelper::GetItemReason(const int nItem)
//{
//	for (UINT i=0; i<m_vManualDetData.size(); i++)
//	{
//		if (m_vManualDetData[i].nCode == nItem)
//		{
//			// �ҵ����ϸ���Ŀ
//			return m_vManualDetData[i].strReason.c_str();
//		}
//	}
//	// û���ҵ����ϸ���
//	return L"";
//}
//
//// ��ȡ������Ŀ����������
//LPCTSTR CManualDetDataHelper::GetItemContent(const int nItem)
//{
//	if (nItem<1 || nItem>m_vManualItem.size())
//	{
//		return L"";
//	}
//	return m_vManualItem[nItem-1].strItem.c_str();
//}