/*
* Copyright (c) 佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：NHCommFunc.h
* 文件标识：
* 描述：通用函数
*
* 版本：1.0
* 日期：2013-05-12
* 作者：
*
*
* 历史信息：
*
* 版本：
* 日期：20xx-06-xx
* 作者：Cyx
* 描述：正在开发
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
	// 是否混合动力车辆
	static bool IsHybridVehicle(const VehicleInfo& sVehInfo);
	// 是否OBD过渡阶段
	static bool IsOBDTemporary(void);
	// 是否NOx过渡阶段
	static bool IsNOxTemporary(void);
};