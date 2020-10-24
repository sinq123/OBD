/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称: NHConfigINI.h
* 文件标识: 
* 摘    要: Config数据配置文件基本操作,数据结构格式定义
*
* 当前版本: 1.0
* 作    者: HB
* 完成日期: 未完成
*/

#ifndef NHCONFIGINI_H
#define NHCONFIGINI_H

#include "NHStructDef.h"

// Config
void GetIniNHClient(PNHCLIENT const pIniNHClient);
void SetIniNHClient(const NHCLIENT& iniNHClient);

#endif