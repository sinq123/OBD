/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�Smo.h
* �ļ���ʶ��
* ժҪ���̶ȼƳ�����
*		�Դ����⺯����ʽ�����̶ȼƲ����ӿ�
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

#ifndef SMO_H
#define SMO_H

// ����ִ�н��
#define SMO_OPEN_SUCCEED						0x00  // �򿪴��ڳɹ�
#define SMO_OPEN_FAIL							0x01  // �򿪴���ʧ��
#define SMO_CLOSE_SUCCEED						0x00  // �رմ��ڳɹ�
#define SMO_CLOSE_FAIL_WIN32_API				0x01  // �رմ���ʧ�ܣ�ִ��win32 API(CloseHandle)ʧ��
#define SMO_CLOSE_FAIL_NO_OPEN_PORT				0x02  // �رմ���ʧ�ܣ�û�п��ԹرյĴ���
#define SMO_WR_SUCCEED							0x00  // д���ɹ�
#define SMO_WR_PORT_NOT_OPEN					0x01  // д��ʧ�ܣ�����δ��
#define SMO_WR_READ_SPECIFIED_DATA_FAIL			0x02  // д��ʧ�ܣ���ָ������ʧ��
#define SMO_WR_WRITE_SPECIFIED_DATA_FAIL		0x03  // д��ʧ�ܣ�дָ������ʧ��
#define SMO_WR_EQUIP_RETURE_0X15				0x04  // д��ʧ�ܣ��������ش�����루0x15��
#define SMO_WR_CHECK_SUM_FAIL					0x05  // д��ʧ�ܣ���������У���ʧ��
#define SMO_WR_UNKNOWN_ERROR					0xFF  // д��ʧ�ܣ�δ֪����

class CSmo
{
public:
	CSmo(void);
	virtual ~CSmo(void);

public:
	// �򿪴���
	virtual DWORD Open(BYTE bPort) = 0;
	// �رմ���
	virtual DWORD Close(void) = 0;

	// �жϴ����ǻ��
	virtual bool IsOpen(void) const = 0;

	///@brief �̶��Զ�У׼ 
	///��<9B> ���أ�<06>
	virtual DWORD AutoAdj(BYTE *const pbResult=NULL) = 0;
	////@brief�˳����
	///��<9D> ����:�ɹ�<06>��ʧ��<15>
	virtual DWORD QuitAutoTest(BYTE *const pbResult) = 0;
	///@brief �̶��Զ�����      //append for qin      
	///��<9C> ���أ�<06>��ʧ��<15>,������<94>
	virtual DWORD AutoReadData(BYTE *const pbResult) = 0;
	///@brief �̶ȼƸ�λ            
	///��<9A> ���أ�<06>��ʧ��<15>
	virtual DWORD Reset(BYTE *const pbResult=NULL) = 0;
	///@brief ��ʼ�Զ����          
	///��<92> ���أ���һ�� <93>���ڶ��� <95>�� ������ <95>�����Ĵ� <94>��ʧ��<15>
	virtual DWORD StartAutoTest(BYTE *const pbResult=NULL) = 0;    
	///@brief ȡ��һ������          
	/// ��<96> ���أ��ɹ�<����>��ʧ��<15>
	virtual DWORD GetData1(BYTE *const pbResult, float *const pfData1) = 0;
	///@brief ȡ�ڶ��������Ĵ�����
	///��<97> ���أ��ɹ�<����>��ʧ��<15>
	virtual DWORD GetData1234(BYTE *const pbResult, float *const pfData1, float *const pfData2, float *const pfData3, float *const pfData4) = 0;
	///@brief ȡ���״̬
	///@param hCallBackWnd ������Ϣ���
	virtual DWORD GetTestState(BYTE *const pbResult, BYTE *const pbState) = 0; 
	///@brief ����ʵʱ���ģʽ
	///@param hCallBackWnd ������Ϣ���
    virtual DWORD StartRealTest  (BYTE *const pbResult) = 0;      
	////@brief ������Ӧ����ģʽ,������Ƚ�̤���ز���Ч(����ʵʱģʽ��)
	///@param hCallBackWnd ������Ϣ���
    virtual DWORD AllowTest(BYTE *const pbResult) = 0;  
	///@brief ����һ�μ���Ƿ����,������̤�����Ƿ��ɿ�
	///@param hCallBackWnd ������Ϣ���
    virtual DWORD GetSign(BYTE *const pbResult) = 0;
	///@brief �̶ȼ�Ӳ��λ            
	///��<9D> ���أ�<98>��ʧ��<15>
	virtual DWORD HardReset(BYTE *const pbResult=NULL) = 0;
};
#endif