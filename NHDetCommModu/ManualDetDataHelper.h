/*
* ���������˹������Ŀ������
* ����104��
* ������
*/

#pragma once

#include <string>
#include <vector>

enum ManualItemType
{
	MIT_NULL = 0,
	MIT_AppRejItem,		// �����
	MIT_AppMaiItem,		// ά����
};

enum VehicleType
{
	VT_Gasoline,
	VT_Diesel,
};

struct SManualItem
{
	int nCode;						// ��Ŀ����
	std::wstring strItem;			// ��Ŀ����������
	int nJudge;						// �ж�
	std::wstring strReason;			// ���ϸ�ԭ��
	ManualItemType emManualItemType;// ��Ŀ����
	std::wstring strRemark;			// ��ע
};

class CManualDetDataHelper
{
public:
	CManualDetDataHelper(void);
	virtual ~CManualDetDataHelper(void);
		
	void LoadGasolineDetItem(void);
	void LoadDieselDetItem(void);

public:
	// ������Ŀ
	std::vector<SManualItem> m_vManualItem;
	// �������
	std::vector<SManualItem> m_vManualDetData;

};
