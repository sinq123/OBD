/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FuLiVMT2000.h
* �ļ���ʶ��
* ժҪ�� ���ݸ���VMT-2000ת�ټ�ͨѶ�࣬�̳��ڳ�����CTac
*		����ת��������VMT-2000ͨѶЭ����б�д
*       ������ο�ת��������VMT-2000ͨѶЭ��˵����
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

#ifndef FuLi_VMT2000_H
#define FuLi_VMT2000_H

#include "Tac.h"


class CFuLiVMT2000 : public CTac
{
public:
	CFuLiVMT2000(void);
	~CFuLiVMT2000(void);

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
	
protected:
	// д����������
	// д����Ӧ�������ȡ��Ӧ�ķ�������
	// ��������Ҫ���ֱ��д��Ͷ�ȡ�����ݽ���У�����֤
	// (in)pbWriteBuffer��Ҫд����������ݣ�����Ϊ��ָ��
	// (in)dwWriteLength��Ҫд������ݳ���
	// (out)pbReadBuffer��Ҫ��ȡ���������ݣ�����Ϊ��ָ��
	// (in)dwReadLength��Ҫ��ȡ�����ݳ���
	// (in)bNeedWriteChkSum��true����ҪдУ�飬false������Ҫ
	// (in)bNeedReadChkSum��true����Ҫ��У�飬false������Ҫ
	// ���أ�����ִ�н����
	// NHC_WR_FUNC_SUCCEED
	// NHC_WR_FUNC_PORT_NOT_OPEN
	// NHC_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHC_WR_FUNC_EQUIP_RETURE_0X15
	// NHC_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD &dwReadLength, bool bNeedWriteChkSum=true, bool bNeedReadChkSum=true);

	// ����У���
	// ��ӣ�Ȼ��ȡ����һ
	// (in)p��Ҫ����У�����������
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	BYTE CheckSum(BYTE *p, UINT un);
private:
	// ���ھ��
	HANDLE m_hComm;
	// ���ڲ����ٽ���
	CRITICAL_SECTION m_csComm;
};

#endif