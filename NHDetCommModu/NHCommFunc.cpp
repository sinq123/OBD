#include "StdAfx.h"
#include "NHCommFunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <ATLComTime.h>

CNHCommFunc::CNHCommFunc(void)
{
}


CNHCommFunc::~CNHCommFunc(void)
{
}


bool CNHCommFunc::IsHybridVehicle(const VehicleInfo& sVehInfo)
{
	// ����Դ���յ�
	std::wstring strPlateType = sVehInfo.wchPlateType;
	if (strPlateType.find(L"����Դ") != std::wstring::npos)
	{
		return true;
	}

	// ��¼����/�����Ϣ�ĵ������͵��ϳ�
	std::wstring strMotorModel = sVehInfo.wchMotorModel;
	std::wstring strEnergyStorageDeviceModel = sVehInfo.wchEnergyStorageDeviceModel;
	float fBatteryCapacity = (float)_wtof(sVehInfo.wchBatteryCapacity);
	if (strMotorModel != L"" 
		|| strEnergyStorageDeviceModel != L"" 
		|| fBatteryCapacity > 0 
		)
	{
		return true;
	}

	return false;
}

bool CNHCommFunc::IsOBDTemporary(void)
{
	COleDateTime odtNow = COleDateTime::GetCurrentTime();
	COleDateTime odtTime(2019, 11, 1, 0, 0, 0);			// ���ù�����2019-11-01 00:00:00
	if (odtNow < odtTime)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CNHCommFunc::IsNOxTemporary(void)
{
	COleDateTime odtNow = COleDateTime::GetCurrentTime();
	COleDateTime odtTime(2019, 11, 1, 0, 0, 0);			// ���ù�����2019-11-01 00:00:00
	if (odtNow < odtTime)
	{
		return true;
	}
	else
	{
		return false;
	}
}