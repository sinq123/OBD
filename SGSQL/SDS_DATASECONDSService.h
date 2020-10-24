



#ifndef SDS_DATASECONDSSERVICE_H
#define SDS_DATASECONDSSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CSDS_DATASECONDSService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	CSDS_DATASECONDSService(void);
	virtual ~CSDS_DATASECONDSService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CSDS_DATASECONDSService(const CSDS_DATASECONDSService &);
	CSDS_DATASECONDSService & operator =(const CSDS_DATASECONDSService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sSDS_DATASECONDSData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SSDS_DataSeconds &sSDS_DataSeconds);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsSDS_DATASECONDSData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SSDS_DataSeconds> &lsSDS_DataSeconds);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sSDS_DATASECONDSData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const SSDS_DataSeconds &sSDS_DataSeconds);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsSDS_DATASECONDSData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const std::list<SSDS_DataSeconds> &lsSDS_DataSeconds);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetSDS_DATASECONDSData_NonEmpty(const _ConnectionPtr &pConnection, const SSDS_DataSeconds &sSDS_DataSeconds);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sSDS_DATASECONDSData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SSDS_DataSeconds &sSDS_DataSeconds);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sSDS_DATASECONDSData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SSDS_DataSeconds &sSDS_DataSeconds);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SSDS_DataSeconds &sSDS_DataSeconds);
};

#endif