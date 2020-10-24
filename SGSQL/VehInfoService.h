



#ifndef VehInfoSERVICE_H
#define VehInfoSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CVehInfoService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CVehInfoService(void);
	virtual ~CVehInfoService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CVehInfoService(const CVehInfoService &);
	CVehInfoService & operator =(const CVehInfoService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sVehInfoData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetVehInfoData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SVehInfo &sVehInfo);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsVehInfoData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetVehInfoData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SVehInfo> &lsVehInfo);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sVehInfoData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetVehInfoData(const _ConnectionPtr &pConnection, const SVehInfo &sVehInfo);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsVehInfoData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetVehInfoData(const _ConnectionPtr &pConnection, const std::list<SVehInfo> &lsVehInfo);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetVehInfoData_NonEmpty(const _ConnectionPtr &pConnection, const SVehInfo &sVehInfo);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sVehInfoData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SVehInfo &sVehInfo);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sVehInfoData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SVehInfo &sVehInfo);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SVehInfo &sVehInfo);
};

#endif