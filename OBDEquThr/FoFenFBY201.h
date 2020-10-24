/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�FoFenFBY201.h
* �ļ���ʶ��
* ժҪ��
*		
*
* �汾��1.0.1
* ���ߣ�Raylee
* ���ڣ�2012-03-19
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.1
* ���ڣ�2012-06-25
* ���ߣ�Raylee
* ������GetTestState�������������汾��
*		1���ᵥ�ֽڵķ���0x82/0x41��
*		2������˵����Э�鷵��0x02 XX XX��
*
* �汾��1.0.0
* ���ڣ�2012-03-19
* ���ߣ�Raylee
* ��������ʽ����ʹ��
*/

#pragma once

#ifndef FOFEN_FBY201_H
#define FOFEN_FBY201_H

#include "Smo.h"

class CFoFenFBY201 : public CSmo
{
public:
	CFoFenFBY201(void);
	~CFoFenFBY201(void);

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