



#ifndef JZJS_XZDBSERVICE_H
#define JZJS_XZDBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CJZJS_XZDBService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CJZJS_XZDBService(void);
	virtual ~CJZJS_XZDBService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CJZJS_XZDBService(const CJZJS_XZDBService &);
	CJZJS_XZDBService & operator =(const CJZJS_XZDBService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sJZJS_XZDBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJS_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SJZJS_XZDB &sJZJS_XZDB);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsJZJS_XZDBData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJS_XZDBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SJZJS_XZDB> &lsJZJS_XZDB);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sJZJS_XZDBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_XZDBData(const _ConnectionPtr &pConnection, const SJZJS_XZDB &sJZJS_XZDB);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsJZJS_XZDBData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_XZDBData(const _ConnectionPtr &pConnection, const std::list<SJZJS_XZDB> &lsJZJS_XZDB);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_XZDBData_NonEmpty(const _ConnectionPtr &pConnection, const SJZJS_XZDB &sJZJS_XZDB);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJS_XZDBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SJZJS_XZDB &sJZJS_XZDB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJS_XZDBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SJZJS_XZDB &sJZJS_XZDB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SJZJS_XZDB &sJZJS_XZDB);
};

#endif