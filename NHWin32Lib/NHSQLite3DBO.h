/*
* Copyright (c) 2010,��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHSQLite3DBO.h
* �ļ���ʶ��
* ����������CppSQLite3��SQLite3���ݿ����
*
* �汾��1.0
* ���ߣ�Cyx
* ���ڣ�2010-09-05
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.1
* ���ڣ�2011-03-16
* ���ߣ�Cyx
* ������1��ȡ����NHLog������
*
* �汾��1.0.0
* ���ڣ�2010-09-05
* ���ߣ�Cyx
* ������1���Ľ�ϸ�ڣ������ⲿ�ࡢ���������Ϣ����־�ļ��ȵȣ�
*       2)��ʽ����ʹ��
*/

#ifndef NH_SQLITE3_DBO_H
#define NH_SQLITE3_DBO_H

#include "CppSQLite3.h"

//#ifdef DLL_FILE
//class _declspec(dllexport) CNHSQLite3DBO
//#else
//class _declspec(dllimport) CNHSQLite3DBO
//#endif
class CNHSQLite3DBO
{
	// ���ù��캯������������(��ֹ���ɶ���)
private:
	CNHSQLite3DBO(void);
	virtual ~CNHSQLite3DBO(void);

	// ���ÿ������캯���Ϳ�����ֵ����
private:
	CNHSQLite3DBO(const CNHSQLite3DBO &);
	CNHSQLite3DBO & operator =(const CNHSQLite3DBO &);

public:
	// �����ݿ�(Ĭ��·��)
	// (in&out)db:���ݿ����
	// (return)�������:
	// 0x00:����
	// ��0x00:�쳣
	static DWORD OpenDB(CppSQLite3DB &db);
	// �����ݿ�(ָ��·��)
	// (in&out)db:���ݿ����
	// (in)pwchDBPath:���ݿ�·��
	// (return)�������:
	// 0x00:����
	// ��0x00:�쳣
	static DWORD OpenDB(CppSQLite3DB &db, const wchar_t *const pwchDBPath);
	// �ر����ݿ�
	// (in&out)db:���ݿ����
	// (return)�������:
	// 0x00:����
	// ��0x00:�쳣
	static DWORD CloseDB(CppSQLite3DB &db);
	// �򿪲�ѯ
	// (in&out)db:���ݿ����
	// (in&out)query:��ѯ����
	// (in)pwchSQL:��ѯ����ַ���ָ��
	// (return)�������:
	// 0x00:����
	// ��0x00:�쳣
	static DWORD OpenQuery(CppSQLite3DB &db, CppSQLite3Query &query, const wchar_t *const pwchSQL);
	// �رղ�ѯ
	// (in&out)query:��ѯ����
	// (return)�������:
	// 0x00:����
	// ��0x00:�쳣
	static DWORD CloseQuery(CppSQLite3Query &query);
	// ִ��DML
	// (in&out)db:���ݿ����
	// (in)pwchSQL:DML����ַ���ָ��
	// (out)pnRowsInvolved����Ӱ����е���Ŀָ�룬����ΪNULL
	// (return)�������:
	// 0x00:����
	// ��0x00:�쳣
	static DWORD ExecuteDML(CppSQLite3DB &db, const wchar_t *const pwchSQL, int *const pnRowsInvolved=NULL);
	// ��ȡ�ֶ�ֵ
	// (in&out)query:��ѯ����
	// (out)pwchFieldValue:�ֶ�ֵ�ַ���ָ��
	// (in)nFieldValueLength:�ֶ�ֵ����
	// (in)pwchFieldName:�ֶ����ַ���ָ��
	// (return)�������:
	// 0x00:����
	// ��0x00:�쳣
	static DWORD GetFieldValue(CppSQLite3Query &query, wchar_t *const pwchFieldValue, const int nFieldValueLength, const wchar_t *const pwchFieldName);
	// ������ݿ������Ϣ�������������ں���־�ļ���
	// (in&out)e:�쳣����
	// (return)��
	static void OutputDBErrMsg(CppSQLite3Exception &e);
};

#endif