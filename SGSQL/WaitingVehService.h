



#ifndef WaitingVehSERVICE_H
#define WaitingVehSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CWaitingVehService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CWaitingVehService(void);
	virtual ~CWaitingVehService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CWaitingVehService(const CWaitingVehService &);
	CWaitingVehService & operator =(const CWaitingVehService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sWaitingVehData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetWaitingVehData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SWaitingVeh &sWaitingVeh);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsWaitingVehData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetWaitingVehData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SWaitingVeh> &lsWaitingVeh);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sWaitingVehData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetWaitingVehData(const _ConnectionPtr &pConnection, const SWaitingVeh &sWaitingVeh);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsWaitingVehData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetWaitingVehData(const _ConnectionPtr &pConnection, const std::list<SWaitingVeh> &lsWaitingVeh);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetWaitingVehData_NonEmpty(const _ConnectionPtr &pConnection, const SWaitingVeh &sWaitingVeh);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sWaitingVehData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SWaitingVeh &sWaitingVeh);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sWaitingVehData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SWaitingVeh &sWaitingVeh);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SWaitingVeh &sWaitingVeh);
};

#endif