



#ifndef OBDSERVICE_H
#define OBDSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class COBDService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	COBDService(void);
	virtual ~COBDService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	COBDService(const COBDService &);
	COBDService & operator =(const COBDService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sOBDData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetOBDData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SOBD &sOBD);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsOBDData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetOBDData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SOBD> &lsOBD);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sOBDData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetOBDData(const _ConnectionPtr &pConnection, const SOBD &sOBD);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsOBDData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetOBDData(const _ConnectionPtr &pConnection, const std::list<SOBD> &lsOBD);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetOBDData_NonEmpty(const _ConnectionPtr &pConnection, const SOBD &sOBD);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sOBDData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SOBD &sOBD);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sOBDData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SOBD &sOBD);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SOBD &sOBD);
};

#endif