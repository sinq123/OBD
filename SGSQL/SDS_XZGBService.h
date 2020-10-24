



#ifndef SDS_XZGBSERVICE_H
#define SDS_XZGBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CSDS_XZGBService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CSDS_XZGBService(void);
	virtual ~CSDS_XZGBService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CSDS_XZGBService(const CSDS_XZGBService &);
	CSDS_XZGBService & operator =(const CSDS_XZGBService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sSDS_XZGBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetSDS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SSDS_XZGB &sSDS_XZGB);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsSDS_XZGBData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetSDS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SSDS_XZGB> &lsSDS_XZGB);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sSDS_XZGBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetSDS_XZGBData(const _ConnectionPtr &pConnection, const SSDS_XZGB &sSDS_XZGB);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsSDS_XZGBData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetSDS_XZGBData(const _ConnectionPtr &pConnection, const std::list<SSDS_XZGB> &lsSDS_XZGB);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetSDS_XZGBData_NonEmpty(const _ConnectionPtr &pConnection, const SSDS_XZGB &sSDS_XZGB);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sSDS_XZGBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SSDS_XZGB &sSDS_XZGB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sSDS_XZGBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SSDS_XZGB &sSDS_XZGB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SSDS_XZGB &sSDS_XZGB);
};

#endif