/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaYd1.h
* �ļ���ʶ��
* ժҪ���̶ȼ�YD1ͨѶ�࣬�̳��ڳ�����CSmo
*		�����̶ȼ�YD1ͨѶЭ����б�д
*       ������ο�ת�̶ȼ�YD1ͨѶЭ��˵����
*		
*
* ��ǰ�汾��1.0.1
* ���ߣ�hyh
* ������ڣ�2010-01-18
*
* ��ʷ��Ϣ��
*
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-01-18
* ���ߣ�hyh
* ������1)�޸�GetData1234()��GetData1()����
*       ���if(str.length() >= 14)��if(str.length() >= 4)��ֹ����
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-08
* ���ߣ�Cui,hyh
* ��������ʽ����ʹ��
*/

#pragma once

#ifndef NANHUA_YD1_H
#define NANHUA_YD1_H

#include "Smo.h"

class CNanhuaYd1 : public CSmo
{
public:
	CNanhuaYd1(void);
	~CNanhuaYd1(void);

public:
	// �򿪴���
	DWORD Open(BYTE bPort);
	// �رմ���
	DWORD Close(void);

	// �жϴ����ǻ��
	bool IsOpen(void) const;

	///@brief �̶��Զ�У׼      //append for qin      
	///��<9B> ���أ�<06>
	DWORD AutoAdj(BYTE *const pbResult=NULL);
	////@brief�˳����
	///��<9D> ����:�ɹ�<06>��ʧ��<15>
	DWORD QuitAutoTest(BYTE *const pbResult);
	///@brief �̶��Զ�����      //append for qin      
	///��<9C> ���أ�<06>��ʧ��<15>,������<94>
	DWORD AutoReadData(BYTE *const pbResult);
	///@brief �̶ȼƸ�λ            
	///��<9A> ���أ�<06>��ʧ��<15>
	DWORD Reset(BYTE *const pbResult=NULL);
	///@brief ��ʼ�Զ����          
	///��<92> ���أ���һ�� <93>���ڶ��� <95>�� ������ <95>�����Ĵ� <94>��ʧ��<15>
	DWORD StartAutoTest  (BYTE *const pbResult=NULL);    
	///@brief ȡ��һ������          
	/// ��<96> ���أ��ɹ�<����>��ʧ��<15>
	DWORD GetData1(BYTE *const pbResult, float *const pfData1);
	///@brief ȡ�ڶ��������Ĵ�����
	///��<97> ���أ��ɹ�<����>��ʧ��<15>
	DWORD GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4);
	///@brief ȡ���״̬
	///@param hCallBackWnd ������Ϣ���
	DWORD GetTestState(BYTE *const pbResult, BYTE *const pbState); 
	///@brief ����ʵʱ���ģʽ
	///@param hCallBackWnd ������Ϣ���
    DWORD StartRealTest  (BYTE *const pbResult);      
	////@brief ������Ӧ����ģʽ,������Ƚ�̤���ز���Ч(����ʵʱģʽ��)
	///@param hCallBackWnd ������Ϣ���
    DWORD AllowTest(BYTE *const pbResult);  
	///@brief ����һ�μ���Ƿ����,������̤�����Ƿ��ɿ�
	///@param hCallBackWnd ������Ϣ���
    DWORD GetSign(BYTE *const pbResult);
	///@brief �̶ȼ�Ӳ��λ            
	///��<9D> ���أ�<98>��ʧ��<15>
	DWORD HardReset(BYTE *const pbResult=NULL);

private:
	// ����ִ�з�������մ������ݣ�bStopOnError������ͨѶʧ��ʱֹͣ����ִ��
	DWORD WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const DWORD dwSleepBeforeRead=0);
	// ����ǰnλ��У���
	BYTE CheckSum(BYTE *const p, const UINT un);
private:
	// ���ھ��
	HANDLE m_hComm;
	//�ͻ���Ϣ�ٽ籣����
	CRITICAL_SECTION cInfoSection;
};
#endif