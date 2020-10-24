/*
* Copyright (c) 2010,佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：NHSQLite3DBO.h
* 文件标识：
* 描述：基于CppSQLite3的SQLite3数据库操作
*
* 版本：1.0
* 作者：Cyx
* 日期：2010-09-05
*
* 历史信息：
*
*
* 版本：1.0.1
* 日期：2011-03-16
* 作者：Cyx
* 描述：1）取消对NHLog的引用
*
* 版本：1.0.0
* 日期：2010-09-05
* 作者：Cyx
* 描述：1）改进细节（引用外部类、输出错误信息到日志文件等等）
*       2)正式发布使用
*/

#ifndef NH_SQLITE3_DBO_H
#define NH_SQLITE3_DBO_H

#include "CppSQLite3.h"

//#ifdef DLL_FILE
//class _declspec(dllexport) CNHSQLite3DBO
//#else
//class _declspec(dllimport) CNHSQLite3DBO
//#endif
class CNHSQLite3DBO
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CNHSQLite3DBO(void);
	virtual ~CNHSQLite3DBO(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CNHSQLite3DBO(const CNHSQLite3DBO &);
	CNHSQLite3DBO & operator =(const CNHSQLite3DBO &);

public:
	// 打开数据库(默认路径)
	// (in&out)db:数据库对象
	// (return)错误代码:
	// 0x00:正常
	// 非0x00:异常
	static DWORD OpenDB(CppSQLite3DB &db);
	// 打开数据库(指定路径)
	// (in&out)db:数据库对象
	// (in)pwchDBPath:数据库路径
	// (return)错误代码:
	// 0x00:正常
	// 非0x00:异常
	static DWORD OpenDB(CppSQLite3DB &db, const wchar_t *const pwchDBPath);
	// 关闭数据库
	// (in&out)db:数据库对象
	// (return)错误代码:
	// 0x00:正常
	// 非0x00:异常
	static DWORD CloseDB(CppSQLite3DB &db);
	// 打开查询
	// (in&out)db:数据库对象
	// (in&out)query:查询对象
	// (in)pwchSQL:查询语句字符串指针
	// (return)错误代码:
	// 0x00:正常
	// 非0x00:异常
	static DWORD OpenQuery(CppSQLite3DB &db, CppSQLite3Query &query, const wchar_t *const pwchSQL);
	// 关闭查询
	// (in&out)query:查询对象
	// (return)错误代码:
	// 0x00:正常
	// 非0x00:异常
	static DWORD CloseQuery(CppSQLite3Query &query);
	// 执行DML
	// (in&out)db:数据库对象
	// (in)pwchSQL:DML语句字符串指针
	// (out)pnRowsInvolved：受影响的行的数目指针，可以为NULL
	// (return)错误代码:
	// 0x00:正常
	// 非0x00:异常
	static DWORD ExecuteDML(CppSQLite3DB &db, const wchar_t *const pwchSQL, int *const pnRowsInvolved=NULL);
	// 获取字段值
	// (in&out)query:查询对象
	// (out)pwchFieldValue:字段值字符串指针
	// (in)nFieldValueLength:字段值长度
	// (in)pwchFieldName:字段名字符串指针
	// (return)错误代码:
	// 0x00:正常
	// 非0x00:异常
	static DWORD GetFieldValue(CppSQLite3Query &query, wchar_t *const pwchFieldValue, const int nFieldValueLength, const wchar_t *const pwchFieldName);
	// 输出数据库错误信息（输出到输出窗口和日志文件）
	// (in&out)e:异常对象
	// (return)无
	static void OutputDBErrMsg(CppSQLite3Exception &e);
};

#endif