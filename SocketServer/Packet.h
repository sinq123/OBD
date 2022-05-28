#ifndef EPANDOZE_H
#define EPANDOZE_H

#include <string>
#include <windows.h>

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

struct EPPacket
{
	WORD wHeader;				// ��ͷ2�ֽ� EP	
	DWORD dwPacketLen;			// �����ȣ�4�ֽ�
	std::wstring strXML;		// UTF8��ʽ��xml�ַ���
	BYTE bTail;					// ��β1�ֽ� $

	EPPacket(LPCTSTR szXML);

	std::string ToString();

	bool Prase(std::string strRecv);
};

struct ZDPacket
{
	WORD wHeader;				// ��ͷ2�ֽ� NH	
	char szPacketLen[7];		// ������6�ֽ��ַ�����ʾ
	std::wstring strXML;		// UTF8��ʽ��xml�ַ���
	BYTE bTail;					// ��β1�ֽ� $

	ZDPacket(LPCTSTR szXML);

	std::string ToString();

	bool Prase(std::string strRecv);
};

#endif