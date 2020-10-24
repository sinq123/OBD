



#ifndef BJCLXXBSERVICE_H
#define BJCLXXBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CBJCLXXBService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CBJCLXXBService(void);
	virtual ~CBJCLXXBService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CBJCLXXBService(const CBJCLXXBService &);
	CBJCLXXBService & operator =(const CBJCLXXBService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sBJCLXXBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetBJCLXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SBJCLXXB &sBJCLXXB);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsBJCLXXBData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetBJCLXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SBJCLXXB> &lsBJCLXXB);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sBJCLXXBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetBJCLXXBData(const _ConnectionPtr &pConnection, const SBJCLXXB &sBJCLXXB);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsBJCLXXBData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetBJCLXXBData(const _ConnectionPtr &pConnection, const std::list<SBJCLXXB> &lsBJCLXXB);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetBJCLXXBData_NonEmpty(const _ConnectionPtr &pConnection, const SBJCLXXB &sBJCLXXB);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sBJCLXXBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SBJCLXXB &sBJCLXXB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sBJCLXXBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SBJCLXXB &sBJCLXXB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SBJCLXXB &sBJCLXXB);
};

#endif