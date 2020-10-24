/*
* Copyright (c) ��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHEngPro.h.h
* �ļ���ʶ��
* ������������Ŀ����ر�ʶ�����ڼ�ǿ��������ԣ�
*
* �汾��1.0
* ���ڣ�2015-09-30
* ���ߣ�Cyx
*/

#define ENG_PRO_STD // ��׼��

//#define ENG_PRO_WKL// ������ ΢�����汾 
//#define ENG_PRO_DL // ������ ���װ汾


#ifndef NHENGPRO_H
#define NHENGPRO_H

#ifdef ENG_PRO_STD
const wchar_t * const ENG_PRO_NAME = L"STD";	// ������Ŀ��
const wchar_t * const ENG_PRO_NAME_EX = L"��׼��";	// ������Ŀ��
const wchar_t * const ENG_PRO_VERSION = L"5.0.0";  // ����汾
const wchar_t * const MAIN_MINOR_VERSION = L"5.0";  // ���ΰ汾
#endif

#ifdef ENG_PRO_WKL
const wchar_t * const ENG_PRO_NAME = L"WKLSTD";	// ������Ŀ��
const wchar_t * const ENG_PRO_NAME_EX = L"΢������׼��";	// ������Ŀ��
const wchar_t * const ENG_PRO_VERSION = L"5.0.0";  // ����汾
const wchar_t * const MAIN_MINOR_VERSION = L"5.0";  // ���ΰ汾
#endif

#ifdef ENG_PRO_DL
const wchar_t * const ENG_PRO_NAME = L"DLSTD";	// ������Ŀ��
const wchar_t * const ENG_PRO_NAME_EX = L"���ױ�׼��";	// ������Ŀ��
const wchar_t * const ENG_PRO_VERSION = L"5.0.0";  // ����汾
const wchar_t * const MAIN_MINOR_VERSION = L"5.0";  // ���ΰ汾
#endif

#endif