



#ifndef BJCLXXBSERVICE_H
#define BJCLXXBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CBJCLXXBService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CBJCLXXBService(void);
	virtual ~CBJCLXXBService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CBJCLXXBService(const CBJCLXXBService &);
	CBJCLXXBService & operator =(const CBJCLXXBService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sBJCLXXBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetBJCLXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SBJCLXXB &sBJCLXXB);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsBJCLXXBData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetBJCLXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SBJCLXXB> &lsBJCLXXB);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sBJCLXXBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetBJCLXXBData(const _ConnectionPtr &pConnection, const SBJCLXXB &sBJCLXXB);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsBJCLXXBData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetBJCLXXBData(const _ConnectionPtr &pConnection, const std::list<SBJCLXXB> &lsBJCLXXB);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetBJCLXXBData_NonEmpty(const _ConnectionPtr &pConnection, const SBJCLXXB &sBJCLXXB);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sBJCLXXBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SBJCLXXB &sBJCLXXB);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sBJCLXXBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SBJCLXXB &sBJCLXXB);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SBJCLXXB &sBJCLXXB);
};

#endif