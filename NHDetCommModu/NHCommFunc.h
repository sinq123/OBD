/*
* Copyright (c) ��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHCommFunc.h
* �ļ���ʶ��
* ������ͨ�ú���
*
* �汾��1.0
* ���ڣ�2013-05-12
* ���ߣ�
*
*
* ��ʷ��Ϣ��
*
* �汾��
* ���ڣ�20xx-06-xx
* ���ߣ�Cyx
* ���������ڿ���
*/

#pragma once
#include "NHDef.h"
#include "NHStructDef.h"

class CNHCommFunc
{
public:
	CNHCommFunc(void);
	virtual ~CNHCommFunc(void);

public:
	// �Ƿ��϶�������
	static bool IsHybridVehicle(const VehicleInfo& sVehInfo);
	// �Ƿ�OBD���ɽ׶�
	static bool IsOBDTemporary(void);
	// �Ƿ�NOx���ɽ׶�
	static bool IsNOxTemporary(void);
};