#include "ZipLib.h"

#include "../ZipLib/zLib/zlib.h"
#ifdef _DEBUG
#pragma comment(lib, "../ZipLib/zLib/zlibd.lib")
#else
#pragma comment(lib, "../ZipLib/zLib/zlib.lib")
#endif

CZipLib::CZipLib(void)
{
}


CZipLib::~CZipLib(void)
{
}


std::string CZipLib::ZipData(const std::string &strData)
{
	unsigned long ulSize = strData.length();

	unsigned char *szBuf = new unsigned char[ulSize]();
	int nRet = compress(szBuf, &ulSize, (unsigned char *)strData.c_str(), strData.length());

	std::string strZip((char *)szBuf, ulSize);
	delete szBuf;

	return strZip;
}

std::string CZipLib::UnZipData(const std::string &strZip)
{
	unsigned long ulSize = strZip.length()*1000;	// 按照0.1%的压缩率预设解压缓冲区

	unsigned char *szBuf = new unsigned char[ulSize]();
	int nRet = uncompress(szBuf, &ulSize, (unsigned char *)strZip.c_str(), strZip.length());

	std::string strData((char *)szBuf, ulSize);
	delete szBuf;

	return strData;
}