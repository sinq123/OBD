



#ifndef JZJSSERVICE_H
#define JZJSSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CJZJSService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CJZJSService(void);
	virtual ~CJZJSService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CJZJSService(const CJZJSService &);
	CJZJSService & operator =(const CJZJSService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sJZJSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SJZJS &sJZJS);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsJZJSData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJSData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SJZJS> &lsJZJS);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sJZJSData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJSData(const _ConnectionPtr &pConnection, const SJZJS &sJZJS);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsJZJSData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJSData(const _ConnectionPtr &pConnection, const std::list<SJZJS> &lsJZJS);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJSData_NonEmpty(const _ConnectionPtr &pConnection, const SJZJS &sJZJS);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SJZJS &sJZJS);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJSData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SJZJS &sJZJS);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SJZJS &sJZJS);
};

#endif