



#ifndef OutLookCheckSERVICE_H
#define OutLookCheckSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class COutLookCheckService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	COutLookCheckService(void);
	virtual ~COutLookCheckService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	COutLookCheckService(const COutLookCheckService &);
	COutLookCheckService & operator =(const COutLookCheckService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sOutLookCheckData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetOutLookCheckData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SOutLookCheck &sOutLookCheck);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsOutLookCheckData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetOutLookCheckData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SOutLookCheck> &lsOutLookCheck);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sOutLookCheckData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetOutLookCheckData(const _ConnectionPtr &pConnection, const SOutLookCheck &sOutLookCheck);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsOutLookCheckData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetOutLookCheckData(const _ConnectionPtr &pConnection, const std::list<SOutLookCheck> &lsOutLookCheck);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetOutLookCheckData_NonEmpty(const _ConnectionPtr &pConnection, const SOutLookCheck &sOutLookCheck);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sOutLookCheckData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SOutLookCheck &sOutLookCheck);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sOutLookCheckData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SOutLookCheck &sOutLookCheck);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SOutLookCheck &sOutLookCheck);
};

#endif