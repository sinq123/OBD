



#ifndef JZJS_DATASECONDSSERVICE_H
#define JZJS_DATASECONDSSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CJZJS_DATASECONDSService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CJZJS_DATASECONDSService(void);
	virtual ~CJZJS_DATASECONDSService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CJZJS_DATASECONDSService(const CJZJS_DATASECONDSService &);
	CJZJS_DATASECONDSService & operator =(const CJZJS_DATASECONDSService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sJZJS_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SJZJS_DataSeconds &sJZJS_DataSeconds);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsJZJS_DATASECONDSData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SJZJS_DataSeconds> &lsJZJS_DataSeconds);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sJZJS_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_DATASECONDSData(const _ConnectionPtr &pConnection, const SJZJS_DataSeconds &sJZJS_DataSeconds);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsJZJS_DATASECONDSData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_DATASECONDSData(const _ConnectionPtr &pConnection, const std::list<SJZJS_DataSeconds> &lsJZJS_DataSeconds);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_DATASECONDSData_NonEmpty(const _ConnectionPtr &pConnection, const SJZJS_DataSeconds &sJZJS_DataSeconds);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJS_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SJZJS_DataSeconds &sJZJS_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJS_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SJZJS_DataSeconds &sJZJS_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SJZJS_DataSeconds &sJZJS_DataSeconds);
};

#endif