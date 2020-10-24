



#ifndef OBD_DATASECONDSSERVICE_H
#define OBD_DATASECONDSSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class COBD_DATASECONDSService : public CSGSQLServerDBService
{
	// 禁用构造函数和析构函数(禁止生成对象)
private:
	COBD_DATASECONDSService(void);
	virtual ~COBD_DATASECONDSService(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	COBD_DATASECONDSService(const COBD_DATASECONDSService &);
	COBD_DATASECONDSService & operator =(const COBD_DATASECONDSService &);

public:
	// 读出1条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// sOBD_DATASECONDSData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SOBD_DataSeconds &sOBD_DataSeconds);
	// 读出n条数据
	// 参数：
	// pConnection：连接指针引用
	// pwchSql：Sql语句字符串指针
	// lsOBD_DATASECONDSData：数据列表应用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD GetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SOBD_DataSeconds> &lsOBD_DataSeconds);

public:
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sOBD_DATASECONDSData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const SOBD_DataSeconds &sOBD_DataSeconds);
	// 写入n条数据
	// 参数：
	// pConnection：连接指针引用
	// lsOBD_DATASECONDSData：数据列表引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const std::list<SOBD_DataSeconds> &lsOBD_DataSeconds);
	// 写入1条数据
	// 参数：
	// pConnection：连接指针引用
	// sAxleLoadData：数据结构引用
	// 返回：受影响行数：
	// 非0xFFFFFFFF：正常，受影响行数
	// 0xFFFFFFFF：异常
	static DWORD SetOBD_DATASECONDSData_NonEmpty(const _ConnectionPtr &pConnection, const SOBD_DataSeconds &sOBD_DataSeconds);

private:
	// 数据库字段(Field)到程序结构(Struct)
	// 参数：
	// pRecordset：记录集指针应用
	// sOBD_DATASECONDSData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SOBD_DataSeconds &sOBD_DataSeconds);
	// 程序结构(Struct)到数据库字段(Field)
	// 参数：
	// pRecordset：记录集指针应用
	// sOBD_DATASECONDSData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SOBD_DataSeconds &sOBD_DataSeconds);
	// 程序结构(Struct)到数据库字段(Field)，只写入非空的数据
	// 参数：
	// pRecordset：记录集指针应用
	// sAxleLoadData：数据结构引用
	// 返回：错误代码:
	// 0x00：正常
	// 非0x00：异常
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SOBD_DataSeconds &sOBD_DataSeconds);
};

#endif