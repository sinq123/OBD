



#ifndef SDS_XZGBSERVICE_H
#define SDS_XZGBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CSDS_XZGBService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CSDS_XZGBService(void);
	virtual ~CSDS_XZGBService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CSDS_XZGBService(const CSDS_XZGBService &);
	CSDS_XZGBService & operator =(const CSDS_XZGBService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sSDS_XZGBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetSDS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SSDS_XZGB &sSDS_XZGB);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsSDS_XZGBData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetSDS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SSDS_XZGB> &lsSDS_XZGB);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sSDS_XZGBData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetSDS_XZGBData(const _ConnectionPtr &pConnection, const SSDS_XZGB &sSDS_XZGB);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsSDS_XZGBData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetSDS_XZGBData(const _ConnectionPtr &pConnection, const std::list<SSDS_XZGB> &lsSDS_XZGB);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetSDS_XZGBData_NonEmpty(const _ConnectionPtr &pConnection, const SSDS_XZGB &sSDS_XZGB);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sSDS_XZGBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SSDS_XZGB &sSDS_XZGB);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sSDS_XZGBData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SSDS_XZGB &sSDS_XZGB);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SSDS_XZGB &sSDS_XZGB);
};

#endif