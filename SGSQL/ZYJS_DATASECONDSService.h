



#ifndef ZYJS_DATASECONDSSERVICE_H
#define ZYJS_DATASECONDSSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CZYJS_DATASECONDSService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CZYJS_DATASECONDSService(void);
	virtual ~CZYJS_DATASECONDSService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CZYJS_DATASECONDSService(const CZYJS_DATASECONDSService &);
	CZYJS_DATASECONDSService & operator =(const CZYJS_DATASECONDSService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sZYJS_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetZYJS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SZYJS_DataSeconds &sZYJS_DataSeconds);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsZYJS_DATASECONDSData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetZYJS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SZYJS_DataSeconds> &lsZYJS_DataSeconds);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sZYJS_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_DATASECONDSData(const _ConnectionPtr &pConnection, const SZYJS_DataSeconds &sZYJS_DataSeconds);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsZYJS_DATASECONDSData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_DATASECONDSData(const _ConnectionPtr &pConnection, const std::list<SZYJS_DataSeconds> &lsZYJS_DataSeconds);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_DATASECONDSData_NonEmpty(const _ConnectionPtr &pConnection, const SZYJS_DataSeconds &sZYJS_DataSeconds);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sZYJS_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SZYJS_DataSeconds &sZYJS_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sZYJS_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SZYJS_DataSeconds &sZYJS_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SZYJS_DataSeconds &sZYJS_DataSeconds);
};

#endif