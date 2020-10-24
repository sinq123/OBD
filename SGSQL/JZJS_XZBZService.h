



#ifndef JZJS_XZBZSERVICE_H
#define JZJS_XZBZSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CJZJS_XZBZService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CJZJS_XZBZService(void);
	virtual ~CJZJS_XZBZService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CJZJS_XZBZService(const CJZJS_XZBZService &);
	CJZJS_XZBZService & operator =(const CJZJS_XZBZService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sJZJS_XZBZData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetJZJS_XZBZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SJZJS_XZBZ &sJZJS_XZBZ);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsJZJS_XZBZData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetJZJS_XZBZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SJZJS_XZBZ> &lsJZJS_XZBZ);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sJZJS_XZBZData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetJZJS_XZBZData(const _ConnectionPtr &pConnection, const SJZJS_XZBZ &sJZJS_XZBZ);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsJZJS_XZBZData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetJZJS_XZBZData(const _ConnectionPtr &pConnection, const std::list<SJZJS_XZBZ> &lsJZJS_XZBZ);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetJZJS_XZBZData_NonEmpty(const _ConnectionPtr &pConnection, const SJZJS_XZBZ &sJZJS_XZBZ);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sJZJS_XZBZData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SJZJS_XZBZ &sJZJS_XZBZ);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sJZJS_XZBZData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SJZJS_XZBZ &sJZJS_XZBZ);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SJZJS_XZBZ &sJZJS_XZBZ);
};

#endif