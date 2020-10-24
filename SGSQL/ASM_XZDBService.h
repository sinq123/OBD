



#ifndef ASM_XZDBSERVICE_H
#define ASM_XZDBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CASM_XZDBService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CASM_XZDBService(void);
	virtual ~CASM_XZDBService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CASM_XZDBService(const CASM_XZDBService &);
	CASM_XZDBService & operator =(const CASM_XZDBService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sASM_XZDBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_XZDB &sASM_XZDB);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsASM_XZDBData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_XZDB> &lsASM_XZDB);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sASM_XZDBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_XZDBData(const _ConnectionPtr &pConnection, const SASM_XZDB &sASM_XZDB);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsASM_XZDBData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_XZDBData(const _ConnectionPtr &pConnection, const std::list<SASM_XZDB> &lsASM_XZDB);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_XZDBData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_XZDB &sASM_XZDB);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_XZDBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SASM_XZDB &sASM_XZDB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_XZDBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SASM_XZDB &sASM_XZDB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_XZDB &sASM_XZDB);
};

#endif