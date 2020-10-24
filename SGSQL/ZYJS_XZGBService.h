



#ifndef ZYJS_XZGBSERVICE_H
#define ZYJS_XZGBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CZYJS_XZGBService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CZYJS_XZGBService(void);
	virtual ~CZYJS_XZGBService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CZYJS_XZGBService(const CZYJS_XZGBService &);
	CZYJS_XZGBService & operator =(const CZYJS_XZGBService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sZYJS_XZGBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetZYJS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SZYJS_XZGB &sZYJS_XZGB);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsZYJS_XZGBData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetZYJS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SZYJS_XZGB> &lsZYJS_XZGB);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sZYJS_XZGBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetZYJS_XZGBData(const _ConnectionPtr &pConnection, const SZYJS_XZGB &sZYJS_XZGB);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsZYJS_XZGBData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetZYJS_XZGBData(const _ConnectionPtr &pConnection, const std::list<SZYJS_XZGB> &lsZYJS_XZGB);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetZYJS_XZGBData_NonEmpty(const _ConnectionPtr &pConnection, const SZYJS_XZGB &sZYJS_XZGB);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sZYJS_XZGBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SZYJS_XZGB &sZYJS_XZGB);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sZYJS_XZGBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SZYJS_XZGB &sZYJS_XZGB);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SZYJS_XZGB &sZYJS_XZGB);
};

#endif