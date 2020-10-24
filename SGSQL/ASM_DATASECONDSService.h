



#ifndef ASM_DataSecondsSERVICE_H
#define ASM_DataSecondsSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CASM_DataSecondsService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CASM_DataSecondsService(void);
	virtual ~CASM_DataSecondsService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CASM_DataSecondsService(const CASM_DataSecondsService &);
	CASM_DataSecondsService & operator =(const CASM_DataSecondsService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sASM_DataSecondsData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetASM_DataSecondsData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_DataSeconds &sASM_DataSeconds);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsASM_DataSecondsData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetASM_DataSecondsData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_DataSeconds> &lsASM_DataSeconds);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sASM_DataSecondsData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_DataSecondsData(const _ConnectionPtr &pConnection, const SASM_DataSeconds &sASM_DataSeconds);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsASM_DataSecondsData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_DataSecondsData(const _ConnectionPtr &pConnection, const std::list<SASM_DataSeconds> &lsASM_DataSeconds);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_DataSecondsData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_DataSeconds &sASM_DataSeconds);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sASM_DataSecondsData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SASM_DataSeconds &sASM_DataSeconds);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sASM_DataSecondsData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SASM_DataSeconds &sASM_DataSeconds);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_DataSeconds &sASM_DataSeconds);
};

#endif