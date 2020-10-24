/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaRpm5300.h
* �ļ���ʶ��
* ժҪ��ת�ټ�RPM5300ͨѶ�࣬�̳��ڳ�����CTac
*		����ת��������RPM5300ͨѶЭ����б�д
*       ������ο�ת��������RPM5300ͨѶЭ��˵����
*
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

#ifndef NANHUA_RPM5300_H
#define NANHUA_RPM5300_H

#include "Tac.h"

class CNanhuaRpm5300 : public CTac
{
public:
	CNanhuaRpm5300(void);
	~CNanhuaRpm5300(void);

public:
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// (in)bPort�����ں�(1~255)
	// (return)��
	// TAC_OPEN_SUCCEED
	// TAC_OPEN_FAIL
	virtual DWORD Open(BYTE bPort);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// (return)��
	// TAC_CLOSE_SUCCEED
	// TAC_CLOSE_FAIL_WIN32API
	// TAC_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void);

	// �жϴ����ǻ��
	// (return)��
	// true:�����Ѵ�
	// false:����δ��
	virtual bool IsOpen(void) const;

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
	virtual DWORD GetRev(int *pnRev);

private:
	// ���ھ��
	HANDLE m_hComm;
	// ���ڲ����ٽ���
	CRITICAL_SECTION m_csComm;
};

#endif