/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：ReportNumberGenerator.h
* 文件标识：
* 摘要：报告编号生成器
*       与SQLite数据库一起使用，该数据库目前命名为NHModeClientDB.db，位于程序主目录下的DB文件夹
*
* 当前版本：1.0
* 作者：Cui
* 完成日期：2010-01-09
*
* 历史信息：
*
*
* 当前版本：1.0.2
* 作者：Ming
* 日期：2010-07-19
* 描述：1)修改函数接口名GenerateReportNumber改为GenerateReportNumber_SQLite
*	    2)添加一接口GenerateReportNumber_SQLServer,从服务器数据库中读取检测次数,生成报告编号
*
* 当前版本：1.0.1
* 作者：Hyh
* 完成日期：2010年4月29日
* 摘要：1)修改GetFilePath函数从NHWin32Lib-CNHCommonAPI::GetFilePathExduqu读取
*
* 当前版本：1.0.0
* 完成日期：2010-01-09
* 作者：Cui
* 描述：正式发布使用
*/


#ifndef REPORT_NUMBER_GENERATOR_H
#define REPORT_NUMBER_GENERATOR_H

//#if defined(DLL_FILE)
//#define API_DECLARE(type) __declspec(dllexport) type __stdcall
//#else
//#define API_DECLARE(type) __declspec(dllimport) type __stdcall
//#endif

//extern "C"
//{
//	// 生成报告编号
//	// (out)pwchReportNumber,报告编号字符串指针
//	// (in)nLengthOfReportNumber,报告编号字符串长度
//	// (return)
//	API_DECLARE(DWORD) GenerateReportNumber_SQLite(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber=50);
//}

//extern "C"
//{
	// 从服务器数据库中读取检测次数,生成报告编号
	// (out)pwchReportNumber,报告编号字符串指针
	// (in)nLengthOfReportNumber,报告编号字符串长度
	// (return)
	DWORD GenerateReportNumber_SQLServer(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber=50);
//}

//extern "C"
//{
	// 从服务器数据库中读取检测次数,生成报告编号
	// (out)pwchReportNumber,报告编号字符串指针
	// (in)nLengthOfReportNumber,报告编号字符串长度
	// (return)
	DWORD GenerateReportNumber_StationTestAmount(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber=50);
//}

#endif