/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�CLEDPanel.h
* �ļ���ʶ��
* ժҪ��LED������
*		�Դ����⺯����ʽ����LED�����ӿ�
*
* ��ǰ�汾��1.0.0
* ���ߣ�Wuyq
* ������ڣ�2013-09-10
*
* ��ʷ��Ϣ��
*
* ��ǰ�汾��1.0.0
* ������ڣ�2013-09-10
* ���ߣ�Wuyq
* ��������ʽ����ʹ��
*/

#pragma once

class CPanel
{
public:
	CPanel(void);
	virtual ~CPanel(void);

public:
	// ͨ���������ӵ�����
	// (in)pwchIP:Socket������IP��ַ
	// (in)usPort:Socket�������˿�
	// (return): 0x00:����
	//           ��0x00:�쳣
	virtual DWORD ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort) = 0;

	// �򿪴������ӵ�����
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// in bPort�����ں�(1~255)
	// in nBaudRate�������� (110��300��600��1200��2400��4800��9600��14400��19200��38400��56000��57600��115200��128000��256000)
	virtual DWORD ConnectServer(const BYTE bPort, const int nBaudRate = CBR_19200) = 0;
	
	// �Ͽ�����������
	// (return): 0x00:����
	//           ��0x00:�쳣
	virtual DWORD DisconnectServer(void) = 0;

	// �жϵ������Ƿ�����
	virtual bool IsConnect(void) const = 0;

	// ���ò����б�
	// (return): 0x00:����
	//           ��0x00:�쳣
	virtual DWORD SetList(void) = 0;

	// ɾ�������б�
	// (return): 0x00:����
	//           ��0x00:�쳣
	virtual DWORD DelList(void) = 0;

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
	virtual DWORD Display(const wchar_t *const pwchDisplayTxt,
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
		) = 0;

	// ��ʾ����
	// ������
	// nPanel������
	// nLine���к�
	// pwchLineTxt�����ı�
	// ���أ�0x00������
	//           ��0x00���쳣
	virtual DWORD DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt) = 0;

	// ���͵�������
	// �������������õ�����
	// (in)pch:���ͻ���
	// (in)nLength:���ͻ��峤��
	// (return): 0x00:����
	//           ��0x00:�쳣
	virtual DWORD SendToServer(const char *const pchBuf, const int nLen) = 0;
};
