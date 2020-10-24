



#ifndef ASM_GSXZSERVICE_H
#define ASM_GSXZSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CASM_GSXZService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CASM_GSXZService(void);
	virtual ~CASM_GSXZService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CASM_GSXZService(const CASM_GSXZService &);
	CASM_GSXZService & operator =(const CASM_GSXZService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sASM_GSXZData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetASM_GSXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_GSXZ &sASM_GSXZ);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsASM_GSXZData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetASM_GSXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_GSXZ> &lsASM_GSXZ);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sASM_GSXZData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_GSXZData(const _ConnectionPtr &pConnection, const SASM_GSXZ &sASM_GSXZ);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsASM_GSXZData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_GSXZData(const _ConnectionPtr &pConnection, const std::list<SASM_GSXZ> &lsASM_GSXZ);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetASM_GSXZData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_GSXZ &sASM_GSXZ);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sASM_GSXZData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SASM_GSXZ &sASM_GSXZ);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sASM_GSXZData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SASM_GSXZ &sASM_GSXZ);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_GSXZ &sASM_GSXZ);
};

#endif