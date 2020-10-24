/*******************************************
* URLCode.h
******************************************/
#pragma once
#include <string>
#include "kutfconv.h"
using std::string;

class CURLCode 
{
public:
	CURLCode(){};
	virtual ~CURLCode(){};
	
	static string Encode(const char* lpszData)
	{
		string strResult = "";
		unsigned char* pInTmp = (unsigned char*)lpszData;
		// do encoding
		while (*pInTmp)
		{
			if (isspace(*pInTmp))
			{
				strResult += '+';
			}
			else if ((*pInTmp>='0' && *pInTmp<='9')
				|| (*pInTmp>='a' && *pInTmp<='z')
				|| (*pInTmp>='A' && *pInTmp<='Z')
				|| *pInTmp == '.'
				|| *pInTmp == '-'
				|| *pInTmp == '*'
				|| *pInTmp == '_'
				)
			{
				strResult += *pInTmp;
			}
			else
			{
				strResult += '%';
				strResult += toHex(*pInTmp>>4);
				strResult += toHex(*pInTmp%16);
			}
			pInTmp++;
		}

		return strResult;
	}

	static string Decode(string inTmp)
	{
		string strResult = "";
		size_t inlen = inTmp.length();
		for(size_t i=0;i<inlen;i++)
		{
			if(inTmp.at(i)=='%')
			{
				i++;
				char c = fromHex(inTmp.at(i++));
				c = c << 4;
				c += fromHex(inTmp.at(i));
				strResult += c;
			}
			else if(inTmp.at(i)=='+')
			{
				strResult += ' ';
			}
			else
			{
				strResult += inTmp.at(i);
			}
		}
		return strResult;
	}

	static std::wstring EncodeW(const wchar_t* lpszData)
	{
		std::wstring strResult = L"";
		//strResult = KANSIToUTF16(Encode(KUTF16ToANSI(lpszData)).c_str());
		strResult = KANSIToUTF16(Encode(KUTF16ToUTF8(lpszData)).c_str());
		return strResult;
	}

	static std::wstring DecodeW(std::wstring inTmp)
	{
		std::wstring strResult = L"";
		std::string strCliper = KUTF16ToANSI(inTmp.c_str());
		strResult = KANSIToUTF16(Decode(strCliper).c_str());
		return strResult;
	}

private:
	inline static unsigned char toHex(const unsigned char &x){ return x > 9 ? x + 55: x + 48; }
	inline static unsigned char fromHex(const unsigned char &x){ return x > 64 ? x - 55 : x - 48; }
};