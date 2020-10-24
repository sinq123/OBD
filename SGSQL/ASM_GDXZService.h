



#ifndef ASM_GDXZSERVICE_H
#define ASM_GDXZSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CASM_GDXZService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CASM_GDXZService(void);
	virtual ~CASM_GDXZService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CASM_GDXZService(const CASM_GDXZService &);
	CASM_GDXZService & operator =(const CASM_GDXZService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sASM_GDXZData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetASM_GDXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_GDXZ &sASM_GDXZ);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsASM_GDXZData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetASM_GDXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_GDXZ> &lsASM_GDXZ);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sASM_GDXZData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_GDXZData(const _ConnectionPtr &pConnection, const SASM_GDXZ &sASM_GDXZ);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsASM_GDXZData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_GDXZData(const _ConnectionPtr &pConnection, const std::list<SASM_GDXZ> &lsASM_GDXZ);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_GDXZData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_GDXZ &sASM_GDXZ);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sASM_GDXZData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SASM_GDXZ &sASM_GDXZ);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sASM_GDXZData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SASM_GDXZ &sASM_GDXZ);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_GDXZ &sASM_GDXZ);
};

#endif