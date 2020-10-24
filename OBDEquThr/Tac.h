/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�Tac.h
* �ļ���ʶ��
* ժҪ��ת�ټƳ�����
*		�Դ����⺯����ʽ����ת�ټƲ����ӿ�
*
* ��ǰ�汾��1.0
* ���ߣ�Cui,hyh
* ������ڣ�2009-12-08
*
* ��ʷ��Ϣ��
*
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/

#pragma once

#ifndef TAC_H
#define TAC_H

// ����ִ�н��
#define TAC_OPEN_SUCCEED								0x00  // �򿪴��ڳɹ�
#define TAC_OPEN_FAIL									0x01  // �򿪴���ʧ��
#define TAC_CLOSE_SUCCEED								0x00  // �رմ��ڳɹ�
#define TAC_CLOSE_FAIL_WIN32_API						0x01  // �رմ���ʧ�ܣ�ִ��win32 API(CloseHandle)ʧ��
#define TAC_CLOSE_FAIL_NO_OPEN_PORT						0x02  // �رմ���ʧ�ܣ�û�п��ԹرյĴ���
#define TAC_WR_SUCCEED									0x00  // д���ɹ�
#define TAC_WR_PORT_NOT_OPEN							0x01  // д��ʧ�ܣ�����δ��
#define TAC_WR_WRITE_SPECIFIED_DATA_FAIL				0x02  // д��ʧ�ܣ�дָ������ʧ��
#define TAC_WR_READ_SPECIFIED_DATA_FAIL					0x03  // д��ʧ�ܣ���ָ������ʧ��
#define TAC_WR_EQUIP_RETURE_0X15						0x04  // д��ʧ�ܣ��������ش�����루0x15��
#define TAC_WR_CHECK_SUM_FAIL							0x05  // д��ʧ�ܣ���������У���ʧ��
#define TAC_WR_UNKNOWN_ERROR							0xFF  // д��ʧ�ܣ�δ֪����

class CTac
{
public:
	CTac(void);
	virtual ~CTac(void);

public:
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// (in)bPort�����ں�(1~255)
	// (return)��
	// TAC_OPEN_SUCCEED
	// TAC_OPEN_FAIL
	virtual DWORD Open(BYTE bPort) = 0;

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// (return)��
	// TAC_CLOSE_SUCCEED
	// TAC_CLOSE_FAIL_WIN32API
	// TAC_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// �жϴ����ǻ��
	// (return)��
	// true:�����Ѵ�
	// false:����δ��
	virtual bool IsOpen(void) const = 0;

	// ��ȡת��
	// (out)pnRev��������ת��ָ�룬��λ��r/min
	// (return)��
	// TAC_WR_FUNC_SUCCEED
	// TAC_WR_FUNC_PORT_NOT_OPEN
	// TAC_WR_FUNC_WRITE_SPECIFIED_DATA_FAIL
	// TAC_WR_READ_SPECIFIED_DATA_FAIL
	// TAC_WR_EQUIP_RETURE_0X15
	// TAC_WR_CHECK_SUM_FAIL
	// TAC_WR_UNKNOWN_ERROR
	virtual DWORD GetRev(int *pnRev) = 0;
};

#endif