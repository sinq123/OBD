#include "StdAfx.h"
#include "Packet.h"
	
#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif


EPPacket::EPPacket(LPCTSTR szXML)
{
	strXML = szXML;

	wHeader = MAKEWORD('E', 'P');
	dwPacketLen = 0x00;
	bTail = '$';
}

std::string EPPacket::ToString()
{
	std::string strData = TCharToUTF8(strXML.c_str());
	dwPacketLen = sizeof(wHeader) + sizeof(dwPacketLen) + strData.size() + sizeof(bTail);

	std::string strRet;
	strRet.append((char *)&wHeader, sizeof(wHeader));
	strRet.append((char *)&dwPacketLen, sizeof(dwPacketLen));	
	strRet.append(strData);
	strRet.append((char *)&bTail, sizeof(bTail));

	return strRet;
}

bool EPPacket::Prase(std::string strRecv)
{
	int nMinPacketSize = sizeof(wHeader)+sizeof(dwPacketLen)+sizeof(bTail);
	if (strRecv.size() < nMinPacketSize)
	{
		return false;
	}

	int nOffset = 0;
	memcpy(&wHeader, strRecv.data()+nOffset, sizeof(wHeader));
	nOffset += sizeof(wHeader);
	// 校验包尾
	if (wHeader != MAKEWORD('E', 'P'))
	{
		return false;
	}
		
	memcpy(&dwPacketLen, strRecv.data()+nOffset, sizeof(dwPacketLen));
	nOffset += sizeof(dwPacketLen);	
	// 校验数据长度
	if (dwPacketLen != strRecv.size())
	{
		return false;
	}
		
	std::string strData;
	strData.assign(strRecv.data()+nOffset, dwPacketLen-nMinPacketSize);
	nOffset += dwPacketLen-nMinPacketSize;

	memcpy(&bTail, strRecv.data()+nOffset, sizeof(bTail));
	nOffset += sizeof(bTail);
		
	// 校验包尾
	if (bTail != '$')
	{
		return false;
	}

	strXML = UTF8ToTChar(strData.c_str());

	return true;
}

ZDPacket::ZDPacket(LPCTSTR szXML)
{
	strXML = szXML;

	wHeader = MAKEWORD('N', 'H');
	memset(szPacketLen, 0x00, sizeof(szPacketLen));
	bTail = '$';
}

std::string ZDPacket::ToString()
{
	std::string strData = TCharToUTF8(strXML.c_str());

	std::string strRet;
	strRet.append((char *)&wHeader, sizeof(wHeader));

	sprintf_s(szPacketLen, "%06d", min(strData.size()+9, 999999));
	strRet.append((char *)&szPacketLen, sizeof(szPacketLen)-1);	

	strRet.append(strData);
	strRet.append((char *)&bTail, sizeof(bTail));

	return strRet;
}

bool ZDPacket::Prase(std::string strRecv)
{
	if (strRecv.size() < 9)
	{
		return false;
	}

	int nOffset = 0;
	memcpy(&wHeader, strRecv.data()+nOffset, sizeof(wHeader));
	nOffset += sizeof(wHeader);
	// 校验包尾
	if (wHeader != MAKEWORD('N', 'H'))
	{
		return false;
	}

	memcpy(&szPacketLen, strRecv.data()+nOffset, sizeof(szPacketLen)-1);
	szPacketLen[6] = '\0';
	nOffset += sizeof(szPacketLen)-1;
	unsigned int nPacketLen = atoi(szPacketLen);	
	// 校验数据长度
	if (nPacketLen != strRecv.size())
	{
		return false;
	}

	std::string strData;
	strData.assign(strRecv.data()+nOffset, nPacketLen-9);
	nOffset += nPacketLen-9;

	memcpy(&bTail, strRecv.data()+nOffset, sizeof(bTail));
	nOffset += sizeof(bTail);

	// 校验包尾
	if (bTail != '$')
	{
		return false;
	}

	strXML = UTF8ToTChar(strData.c_str());

	return true;
}