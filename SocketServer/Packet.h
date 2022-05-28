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
	WORD wHeader;				// 包头2字节 EP	
	DWORD dwPacketLen;			// 包长度，4字节
	std::wstring strXML;		// UTF8格式的xml字符串
	BYTE bTail;					// 包尾1字节 $

	EPPacket(LPCTSTR szXML);

	std::string ToString();

	bool Prase(std::string strRecv);
};

struct ZDPacket
{
	WORD wHeader;				// 包头2字节 NH	
	char szPacketLen[7];		// 包长度6字节字符串表示
	std::wstring strXML;		// UTF8格式的xml字符串
	BYTE bTail;					// 包尾1字节 $

	ZDPacket(LPCTSTR szXML);

	std::string ToString();

	bool Prase(std::string strRecv);
};

#endif