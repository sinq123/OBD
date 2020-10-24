



#ifndef ASM_GDXZSERVICE_H
#define ASM_GDXZSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CASM_GDXZService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CASM_GDXZService(void);
	virtual ~CASM_GDXZService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CASM_GDXZService(const CASM_GDXZService &);
	CASM_GDXZService & operator =(const CASM_GDXZService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sASM_GDXZData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_GDXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_GDXZ &sASM_GDXZ);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsASM_GDXZData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_GDXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_GDXZ> &lsASM_GDXZ);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sASM_GDXZData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_GDXZData(const _ConnectionPtr &pConnection, const SASM_GDXZ &sASM_GDXZ);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsASM_GDXZData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_GDXZData(const _ConnectionPtr &pConnection, const std::list<SASM_GDXZ> &lsASM_GDXZ);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_GDXZData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_GDXZ &sASM_GDXZ);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_GDXZData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SASM_GDXZ &sASM_GDXZ);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_GDXZData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SASM_GDXZ &sASM_GDXZ);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_GDXZ &sASM_GDXZ);
};

#endif