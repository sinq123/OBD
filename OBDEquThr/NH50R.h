/*
* Copyright (c) 2009,��ɽ�л�������������޹�˾
* All rights reserved.
*
* �ļ�����:NH50R.h
* �ļ���ʶ:
* ժ    Ҫ:���ⶨ��LED��(�ͺ�:NH50R)��������
*          ͨ��TCP��̫��ͨѶЭ�����ͨѶ
*
* ��ǰ�汾:1.0.0
* ��    ��:Cui
* �������:2009-08-20
*
* ��ʷ��Ϣ:
* 1.��ǰ�汾:1.0.1
*   ��    ��:Cui
*   �������:2009-08-26
*   ժ    Ҫ:���Ӻ���SendToServer,���Ը���NH50RͨѶЭ�����Ϳ����ַ�
*			 ��̫��ͨѶЭ����UDP��ΪTCP,��ΪUDPͨѶӰ�����ݿ����

* 2.��ǰ�汾:1.0.2
*   ��    ��:Cui
*   �������:2009-09-02
*   ժ    Ҫ:������DisconnectServer������������һ��WSACleanup��

* 3.��ǰ�汾:1.0.3
*   ��    ��:Ming
*   �������:2013-05-14
*   ժ    Ҫ:���Ӵ������ӵ�����
*/


#pragma once

#ifndef NH50R_H
#define NH50R_H

#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "Panel.h"

class  CNH50R : public CPanel
{
public:
	CNH50R(void);
	~CNH50R(void);

public:
	// ͨ���������ӵ�����
	// (in)pwchIP:Socket������IP��ַ
	// (in)usPort:Socket�������˿�
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort);

	// �򿪴������ӵ�����
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// in nBaudRate�������� (110��300��600��1200��2400��4800��9600��14400��19200��38400��56000��57600��115200��128000��256000)
	DWORD ConnectServer(const BYTE bPort, const int nBaudRate = CBR_19200);
	
	// �Ͽ�����������
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD DisconnectServer(void);

	// �жϵ������Ƿ�����
	bool IsConnect(void) const;

	// ���ò����б�
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD SetList(void);

	// ɾ�������б�
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD DelList(void);

	// ��ʾ
	// (in)pwchDisplayTxt:��ʾ�ı�
	// (in)bTwinking:�Ƿ���˸
	// (in)chLineSpace:�м��
	// (in)chInDisplayMode:�뻨��
	// (in)chOutDisplayMode:������
	// (in)chFramePauseTime:֡ͣ��ʱ��
	// (in)chDisplaySpeed:��ʾ�ٶ�
	// (in)chFontAndSize:���弰��С
	// (in)chSetupType:����ģʽ
	// (in)chhAlign:ˮƽ����
	// (in)chvAlign:��ֱ����
	// (return): 0x00:����
	//           ��0x00:�쳣
	DWORD Display(const wchar_t *const pwchDisplayTxt,
		const bool bTwinking=false,
		const char chLineSpace='0',
		const char chInDisplayMode=0x30,
		const char chOutDisplayMode=0x30,
		const char chFramePauseTime='3',
		const char chDisplaySpeed='0',
		const char chFontAndSize='5',
		const char chSetupType='b',
		const char chhAlign='0',
		const char chvAlign='1'
		);

	// ��ʾ����
	// ������
	// nPanel������
	// nLine���к�
	// pwchLineTxt�����ı�
	// ���أ�0x00������
	//           ��0x00���쳣
	DWORD DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt);

	// ���͵�������
	// �������������õ�����
	// (in)pch:���ͻ���
	// (in)nLength:���ͻ��峤��
	// (return): 0x00:����
	//           ��0x00:�쳣
	inline DWORD SendToServer(const char *const pchBuf, const int nLen);

private:
	// �ͻ��������߳�
	SOCKET m_socket;
	// ���ھ��
	HANDLE m_hComm;
	// �����Ƿ�����
	bool m_bIsConnect_Serial;
	// �����Ƿ�����
	bool m_bIsConnect_Ethernet;

private:
	// ���ͻ���������:1024
	// LED��NH50R��������С������:1024�ֽ�
	const int m_nSendBufLength;

	// ���ͻ�����
	// 1024�ֽ�,������,�ڶ��н��з���,����ջ�з���
	char *m_pchSendBuf;
};

#endif