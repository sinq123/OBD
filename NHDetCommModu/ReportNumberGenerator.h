/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�ReportNumberGenerator.h
* �ļ���ʶ��
* ժҪ��������������
*       ��SQLite���ݿ�һ��ʹ�ã������ݿ�Ŀǰ����ΪNHModeClientDB.db��λ�ڳ�����Ŀ¼�µ�DB�ļ���
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2010-01-09
*
* ��ʷ��Ϣ��
*
*
* ��ǰ�汾��1.0.2
* ���ߣ�Ming
* ���ڣ�2010-07-19
* ������1)�޸ĺ����ӿ���GenerateReportNumber��ΪGenerateReportNumber_SQLite
*	    2)���һ�ӿ�GenerateReportNumber_SQLServer,�ӷ��������ݿ��ж�ȡ������,���ɱ�����
*
* ��ǰ�汾��1.0.1
* ���ߣ�Hyh
* ������ڣ�2010��4��29��
* ժҪ��1)�޸�GetFilePath������NHWin32Lib-CNHCommonAPI::GetFilePathExduqu��ȡ
*
* ��ǰ�汾��1.0.0
* ������ڣ�2010-01-09
* ���ߣ�Cui
* ��������ʽ����ʹ��
*/


#ifndef REPORT_NUMBER_GENERATOR_H
#define REPORT_NUMBER_GENERATOR_H

//#if defined(DLL_FILE)
//#define API_DECLARE(type) __declspec(dllexport) type __stdcall
//#else
//#define API_DECLARE(type) __declspec(dllimport) type __stdcall
//#endif

//extern "C"
//{
//	// ���ɱ�����
//	// (out)pwchReportNumber,�������ַ���ָ��
//	// (in)nLengthOfReportNumber,�������ַ�������
//	// (return)
//	API_DECLARE(DWORD) GenerateReportNumber_SQLite(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber=50);
//}

//extern "C"
//{
	// �ӷ��������ݿ��ж�ȡ������,���ɱ�����
	// (out)pwchReportNumber,�������ַ���ָ��
	// (in)nLengthOfReportNumber,�������ַ�������
	// (return)
	DWORD GenerateReportNumber_SQLServer(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber=50);
//}

//extern "C"
//{
	// �ӷ��������ݿ��ж�ȡ������,���ɱ�����
	// (out)pwchReportNumber,�������ַ���ָ��
	// (in)nLengthOfReportNumber,�������ַ�������
	// (return)
	DWORD GenerateReportNumber_StationTestAmount(wchar_t *const pwchReportNumber, const int nLengthOfReportNumber=50);
//}

#endif