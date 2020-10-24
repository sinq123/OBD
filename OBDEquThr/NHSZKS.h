/*
* Copyright (c) 2009,��ɽ�л�������������޹�˾
* All rights reserved.
*
* �ļ�����:CNHSZKS.h
* �ļ���ʶ:
* ժ    Ҫ:���ⶨ��LED��(�ͺ�:CNHSZKS)��������
*          ͨ��COM��ͨѶЭ�����ͨѶ
*
* ��ǰ�汾:1.0.0
* ��    ��:Wuyq
* �������:2013-09-10
*
* ��ʷ��Ϣ:
*/
#pragma once

#ifndef NHSZKS_H
#define NHSZKS_H

#include "Panel.h"
#include <vector>

class CNHSZKS : public CPanel
{
public:
	CNHSZKS(void);
	~CNHSZKS(void);

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
	// ĳ������ı�ת����buf����
	// ������
	// nPanelIdx������
	// nLineIdx���к�
	// strLineTxt������ı�
	// chSendBuf[]������Ҫ���͵������buf
	// nIdx��buf��ʼд��ƫ����
	// ���أ�buf��д�����һλ����һλ
	int LineToBuf(const int nPanelIdx, const int nLineIdx, const std::wstring& strLineTxt, char chSendBuf[], const int nIdx = 0);

	// �ַ�������
	// ������
	// str��Ҫ���е��ַ���
	// vResult�����ؽ������ڴ�������
	// bKeepEnds���Ƿ񱣴�س����з�
	// ���أ��޷���ֵ
	void SplitLines(const std::wstring& str, std::vector<std::wstring>& vResult, const bool bKeepEnds = false);

private:
	// ���ھ��
	HANDLE m_hComm;
	// �����Ƿ�����
	bool m_bIsConnect_Serial;
};

//���ڿ���������ʾϵͳ������ļ����������Զ����豸led����Ļ��ʾͨѶЭ��
//	
//һ��ͨѶЭ�飺
//������:
//<- :��ʼ��ǣ����ţ��кţ���ʾ��ʽ��...��ʾ����...,0DH;
//1, ��ʽ:������=9600 BIT/S,1��ʼλ,1ֹͣλ,8λ����;"9600,N,8,1"
//2, ���͵ĺ�������������:
//	1) 00H��(ͨѶ��ʼ���)
//	2) 31H������ ��  
//		����>30H,30H=������,
//		31H=1���� ��
//		32H=2���� ��
//		33H=3���� ��
//		34H....3AH,..3FH40H..��
//	3) 31H���кţ�
//		�к�>30H,30H=������,�Թ㲥�ķ�ʽ�������з��ͣ�
//		31H=��һ�� ;�ڵ�һ����ʾ16�����ַ���
//		32H���ڶ���, �ڵڶ�����ʾ16�����ַ���
//		33H=������, �ڵ�һ����ʾ16�����ַ���
//		34H�������У��ڵ�һ����ʾ16�����ַ���
//	��  35H=������ ��֧����ʾ24�����ַ�����ʾ��,���͵����оͻ���ʾ24�����ַ���
//	��  36H=������ ��֧����ʾ24�����ַ�����ʾ��,���͵����оͻ���ʾ24�����ַ���
//	��  39H=�ھ��� ��֧����ʾ32�����ַ�����ʾ��,���͵ھ��оͻ���ʾ32�����ַ���
//	4) 30H�����Ʒ�ʽ;
//	   31H ��������ʾ
//	   32H ��������ʾ
//	   33H ��������ʾ
//       
//˵���� 
//	��ʾ���ݣ������͵�ÿ����ʾ���ݳ�����������ʱ,�Զ�������ʾ��Ϣ����ɾ���κ���Ϣ��������ɫ:�죻����ͣ��ʱ�䣬��ʾ��һĻ������Ϣ����ʾ�ڶ�Ļ��Ϣ��
//
//����λ  0DH; (���ֽ���,ͨѶ����)
//
//
//����:
//����:00H, 31H,31h, 32H,"��B 1234 �쳵��",0DH; 
//	��ʱ�����:1����ʾ����һ�о�����ʾ:"  ��B 1234 �쳵��  "
//����:00H, 31H,32h,32H,"��B 1234 ����",0DH; 
//	��ʱ�����:1����ʾ���ڶ��о�����ʾ:"  ��B 1234 ����  "
//����:00H, 31H,33h,32H,"��B 1234 �����",0DH; 
//	��ʱ�����:1����ʾ�������о�����ʾ:"  ��B 1234 �����  "
//����:00H, 31H,31h, 31H,"��B 1234 �쳵��",0DH; 
//	��ʱ�����:1����ʾ����һ�п�����ʾ:"��B 1234 �쳵��    "
//����:00H, 31H,31h, 32H,"��B 1234 ʱ��40ʱ̤�г̿���",0DH; 
//	��ʱ�����:��Ϊ�����˸�����,1����ʾ����һ���Զ�ѭ�������߶���ʾ:"��B 1234ʱ��40 
//ʱ̤�г̿���"
//															�����п���������ʾϵͳ�������
//															2005��03��21��

#endif