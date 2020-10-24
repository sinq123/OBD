



#ifndef CLXHXXBSERVICE_H
#define CLXHXXBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CCLXHXXBService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CCLXHXXBService(void);
	virtual ~CCLXHXXBService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CCLXHXXBService(const CCLXHXXBService &);
	CCLXHXXBService & operator =(const CCLXHXXBService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sCLXHXXBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetCLXHXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SCLXHXXB &sCLXHXXB);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsCLXHXXBData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetCLXHXXBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SCLXHXXB> &lsCLXHXXB);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sCLXHXXBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetCLXHXXBData(const _ConnectionPtr &pConnection, const SCLXHXXB &sCLXHXXB);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsCLXHXXBData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetCLXHXXBData(const _ConnectionPtr &pConnection, const std::list<SCLXHXXB> &lsCLXHXXB);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetCLXHXXBData_NonEmpty(const _ConnectionPtr &pConnection, const SCLXHXXB &sCLXHXXB);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sCLXHXXBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SCLXHXXB &sCLXHXXB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sCLXHXXBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SCLXHXXB &sCLXHXXB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SCLXHXXB &sCLXHXXB);
};

#endif