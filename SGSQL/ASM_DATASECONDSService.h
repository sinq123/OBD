



#ifndef ASM_DataSecondsSERVICE_H
#define ASM_DataSecondsSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CASM_DataSecondsService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CASM_DataSecondsService(void);
	virtual ~CASM_DataSecondsService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CASM_DataSecondsService(const CASM_DataSecondsService &);
	CASM_DataSecondsService & operator =(const CASM_DataSecondsService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sASM_DataSecondsData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_DataSecondsData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SASM_DataSeconds &sASM_DataSeconds);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsASM_DataSecondsData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetASM_DataSecondsData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SASM_DataSeconds> &lsASM_DataSeconds);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sASM_DataSecondsData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_DataSecondsData(const _ConnectionPtr &pConnection, const SASM_DataSeconds &sASM_DataSeconds);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsASM_DataSecondsData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_DataSecondsData(const _ConnectionPtr &pConnection, const std::list<SASM_DataSeconds> &lsASM_DataSeconds);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetASM_DataSecondsData_NonEmpty(const _ConnectionPtr &pConnection, const SASM_DataSeconds &sASM_DataSeconds);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_DataSecondsData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SASM_DataSeconds &sASM_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sASM_DataSecondsData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SASM_DataSeconds &sASM_DataSeconds);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SASM_DataSeconds &sASM_DataSeconds);
};

#endif