



#ifndef OBD_DATASECONDSSERVICE_H
#define OBD_DATASECONDSSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class COBD_DATASECONDSService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	COBD_DATASECONDSService(void);
	virtual ~COBD_DATASECONDSService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	COBD_DATASECONDSService(const COBD_DATASECONDSService &);
	COBD_DATASECONDSService & operator =(const COBD_DATASECONDSService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sOBD_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SOBD_DataSeconds &sOBD_DataSeconds);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsOBD_DATASECONDSData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SOBD_DataSeconds> &lsOBD_DataSeconds);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sOBD_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const SOBD_DataSeconds &sOBD_DataSeconds);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsOBD_DATASECONDSData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetOBD_DATASECONDSData(const _ConnectionPtr &pConnection, const std::list<SOBD_DataSeconds> &lsOBD_DataSeconds);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetOBD_DATASECONDSData_NonEmpty(const _ConnectionPtr &pConnection, const SOBD_DataSeconds &sOBD_DataSeconds);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sOBD_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SOBD_DataSeconds &sOBD_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sOBD_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SOBD_DataSeconds &sOBD_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SOBD_DataSeconds &sOBD_DataSeconds);
};

#endif