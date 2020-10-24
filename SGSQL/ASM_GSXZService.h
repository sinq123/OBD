



#ifndef ASM_GSXZSERVICE_H
#define ASM_GSXZSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CASM_GSXZService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CASM_GSXZService(void);
	virtual ~CASM_GSXZService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CASM_GSXZService(const CASM_GSXZService &);
	CASM_GSXZService & operator =(const CASM_GSXZService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sASM_GSXZData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_GSXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_GSXZ &sASM_GSXZ);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsASM_GSXZData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_GSXZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_GSXZ> &lsASM_GSXZ);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sASM_GSXZData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_GSXZData(const _ConnectionPtr &pConnection, const SASM_GSXZ &sASM_GSXZ);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsASM_GSXZData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_GSXZData(const _ConnectionPtr &pConnection, const std::list<SASM_GSXZ> &lsASM_GSXZ);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_GSXZData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_GSXZ &sASM_GSXZ);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_GSXZData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SASM_GSXZ &sASM_GSXZ);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_GSXZData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SASM_GSXZ &sASM_GSXZ);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_GSXZ &sASM_GSXZ);
};

#endif