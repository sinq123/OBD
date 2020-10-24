



#ifndef JZJS_XZBZSERVICE_H
#define JZJS_XZBZSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CJZJS_XZBZService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CJZJS_XZBZService(void);
	virtual ~CJZJS_XZBZService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CJZJS_XZBZService(const CJZJS_XZBZService &);
	CJZJS_XZBZService & operator =(const CJZJS_XZBZService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sJZJS_XZBZData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJS_XZBZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SJZJS_XZBZ &sJZJS_XZBZ);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsJZJS_XZBZData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetJZJS_XZBZData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SJZJS_XZBZ> &lsJZJS_XZBZ);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sJZJS_XZBZData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_XZBZData(const _ConnectionPtr &pConnection, const SJZJS_XZBZ &sJZJS_XZBZ);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsJZJS_XZBZData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_XZBZData(const _ConnectionPtr &pConnection, const std::list<SJZJS_XZBZ> &lsJZJS_XZBZ);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetJZJS_XZBZData_NonEmpty(const _ConnectionPtr &pConnection, const SJZJS_XZBZ &sJZJS_XZBZ);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJS_XZBZData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SJZJS_XZBZ &sJZJS_XZBZ);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sJZJS_XZBZData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SJZJS_XZBZ &sJZJS_XZBZ);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SJZJS_XZBZ &sJZJS_XZBZ);
};

#endif