



#ifndef JZJS_XZDBSERVICE_H
#define JZJS_XZDBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CJZJS_XZDBService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CJZJS_XZDBService(void);
	virtual ~CJZJS_XZDBService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CJZJS_XZDBService(const CJZJS_XZDBService &);
	CJZJS_XZDBService & operator =(const CJZJS_XZDBService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sJZJS_XZDBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetJZJS_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SJZJS_XZDB &sJZJS_XZDB);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsJZJS_XZDBData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetJZJS_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SJZJS_XZDB> &lsJZJS_XZDB);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sJZJS_XZDBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetJZJS_XZDBData(const _ConnectionPtr &pConnection, const SJZJS_XZDB &sJZJS_XZDB);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsJZJS_XZDBData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetJZJS_XZDBData(const _ConnectionPtr &pConnection, const std::list<SJZJS_XZDB> &lsJZJS_XZDB);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetJZJS_XZDBData_NonEmpty(const _ConnectionPtr &pConnection, const SJZJS_XZDB &sJZJS_XZDB);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sJZJS_XZDBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SJZJS_XZDB &sJZJS_XZDB);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sJZJS_XZDBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SJZJS_XZDB &sJZJS_XZDB);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SJZJS_XZDB &sJZJS_XZDB);
};

#endif