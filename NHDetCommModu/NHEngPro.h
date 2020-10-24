/*
* Copyright (c) 佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：NHEngPro.h.h
* 文件标识：
* 描述：工程项目的相关标识（用于加强软件可溯性）
*
* 版本：1.0
* 日期：2015-09-30
* 作者：Cyx
*/

#define ENG_PRO_STD // 标准版

//#define ENG_PRO_WKL// 经销商 微轲联版本 
//#define ENG_PRO_DL // 经销商 大雷版本


#ifndef NHENGPRO_H
#define NHENGPRO_H

#ifdef ENG_PRO_STD
const wchar_t * const ENG_PRO_NAME = L"STD";	// 工程项目名
const wchar_t * const ENG_PRO_NAME_EX = L"标准版";	// 工程项目名
const wchar_t * const ENG_PRO_VERSION = L"5.0.0";  // 程序版本
const wchar_t * const MAIN_MINOR_VERSION = L"5.0";  // 主次版本
#endif

#ifdef ENG_PRO_WKL
const wchar_t * const ENG_PRO_NAME = L"WKLSTD";	// 工程项目名
const wchar_t * const ENG_PRO_NAME_EX = L"微轲联标准版";	// 工程项目名
const wchar_t * const ENG_PRO_VERSION = L"5.0.0";  // 程序版本
const wchar_t * const MAIN_MINOR_VERSION = L"5.0";  // 主次版本
#endif

#ifdef ENG_PRO_DL
const wchar_t * const ENG_PRO_NAME = L"DLSTD";	// 工程项目名
const wchar_t * const ENG_PRO_NAME_EX = L"大雷标准版";	// 工程项目名
const wchar_t * const ENG_PRO_VERSION = L"5.0.0";  // 程序版本
const wchar_t * const MAIN_MINOR_VERSION = L"5.0";  // 主次版本
#endif

#endif