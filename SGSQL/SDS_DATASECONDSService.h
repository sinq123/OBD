



#ifndef SDS_DATASECONDSSERVICE_H
#define SDS_DATASECONDSSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CSDS_DATASECONDSService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CSDS_DATASECONDSService(void);
	virtual ~CSDS_DATASECONDSService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CSDS_DATASECONDSService(const CSDS_DATASECONDSService &);
	CSDS_DATASECONDSService & operator =(const CSDS_DATASECONDSService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sSDS_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SSDS_DataSeconds &sSDS_DataSeconds);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsSDS_DATASECONDSData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SSDS_DataSeconds> &lsSDS_DataSeconds);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sSDS_DATASECONDSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const SSDS_DataSeconds &sSDS_DataSeconds);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsSDS_DATASECONDSData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetSDS_DATASECONDSData(const _ConnectionPtr &pConnection, const std::list<SSDS_DataSeconds> &lsSDS_DataSeconds);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetSDS_DATASECONDSData_NonEmpty(const _ConnectionPtr &pConnection, const SSDS_DataSeconds &sSDS_DataSeconds);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sSDS_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SSDS_DataSeconds &sSDS_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sSDS_DATASECONDSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SSDS_DataSeconds &sSDS_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SSDS_DataSeconds &sSDS_DataSeconds);
};

#endif