/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�MingQuanMQZ3.h
* �ļ���ʶ��
* ժҪ�� ��ȪMQZ-3ת�ټ�ͨѶ�࣬�̳��ڳ�����CTac
*		����ת��������MQZ-3ͨѶЭ����б�д
*       ������ο�ת��������MQZ-3ͨѶЭ��˵����
*
*
* �汾��1.0
* ���ߣ�Raylee
* ���ڣ�2017-05-24
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.0
* ���ڣ�2017-05-24
* ���ߣ�Raylee
* ��������ʽ����
*/

#pragma once

#ifndef MingQuan_MQZ3_H
#define MingQuan_MQZ3_H

#include "Tac.h"

class CMingQuanMQZ3 : public CTac
{
public:
	CMingQuanMQZ3(void);
	~CMingQuanMQZ3(void);

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