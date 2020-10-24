



#ifndef ZYJS_XZGBSERVICE_H
#define ZYJS_XZGBSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CZYJS_XZGBService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CZYJS_XZGBService(void);
	virtual ~CZYJS_XZGBService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CZYJS_XZGBService(const CZYJS_XZGBService &);
	CZYJS_XZGBService & operator =(const CZYJS_XZGBService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sZYJS_XZGBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetZYJS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SZYJS_XZGB &sZYJS_XZGB);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsZYJS_XZGBData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetZYJS_XZGBData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SZYJS_XZGB> &lsZYJS_XZGB);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sZYJS_XZGBData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_XZGBData(const _ConnectionPtr &pConnection, const SZYJS_XZGB &sZYJS_XZGB);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsZYJS_XZGBData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_XZGBData(const _ConnectionPtr &pConnection, const std::list<SZYJS_XZGB> &lsZYJS_XZGB);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_XZGBData_NonEmpty(const _ConnectionPtr &pConnection, const SZYJS_XZGB &sZYJS_XZGB);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sZYJS_XZGBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SZYJS_XZGB &sZYJS_XZGB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sZYJS_XZGBData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SZYJS_XZGB &sZYJS_XZGB);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SZYJS_XZGB &sZYJS_XZGB);
};

#endif