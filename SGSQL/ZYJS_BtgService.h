



#ifndef ZYJS_BtgSERVICE_H
#define ZYJS_BtgSERVICE_H

#include <list>
#include "SGSQLServerDBModel.h"
#include "SGSQL.h"

class CZYJS_BtgService : public CSGSQLServerDBService
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CZYJS_BtgService(void);
	virtual ~CZYJS_BtgService(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CZYJS_BtgService(const CZYJS_BtgService &);
	CZYJS_BtgService & operator =(const CZYJS_BtgService &);

public:
	// ����1������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// sZYJS_BtgData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetZYJS_BtgData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, SZYJS_Btg &sZYJS_Btg);
	// ����n������
	// ������
	// pConnection������ָ������
	// pwchSql��Sql����ַ���ָ��
	// lsZYJS_BtgData�������б�Ӧ��
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD GetZYJS_BtgData(const _ConnectionPtr &pConnection, const wchar_t *pwchSql, std::list<SZYJS_Btg> &lsZYJS_Btg);

public:
	// д��1������
	// ������
	// pConnection������ָ������
	// sZYJS_BtgData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_BtgData(const _ConnectionPtr &pConnection, const SZYJS_Btg &sZYJS_Btg);
	// д��n������
	// ������
	// pConnection������ָ������
	// lsZYJS_BtgData�������б�����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_BtgData(const _ConnectionPtr &pConnection, const std::list<SZYJS_Btg> &lsZYJS_Btg);
	// д��1������
	// ������
	// pConnection������ָ������
	// sAxleLoadData�����ݽṹ����
	// ���أ���Ӱ��������
	// ��0xFFFFFFFF����������Ӱ������
	// 0xFFFFFFFF���쳣
	static DWORD SetZYJS_BtgData_NonEmpty(const _ConnectionPtr &pConnection, const SZYJS_Btg &sZYJS_Btg);

private:
	// ���ݿ��ֶ�(Field)������ṹ(Struct)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sZYJS_BtgData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD FieldToStruct(const _RecordsetPtr &pRecordset, SZYJS_Btg &sZYJS_Btg);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sZYJS_BtgData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField(const _RecordsetPtr &pRecordset, const SZYJS_Btg &sZYJS_Btg);
	// ����ṹ(Struct)�����ݿ��ֶ�(Field)��ֻд��ǿյ�����
	// ������
	// pRecordset����¼��ָ��Ӧ��
	// sAxleLoadData�����ݽṹ����
	// ���أ��������:
	// 0x00������
	// ��0x00���쳣
	static DWORD StructToField_NonEmpty(const _RecordsetPtr &pRecordset, const SZYJS_Btg &sZYJS_Btg);
};

#endif