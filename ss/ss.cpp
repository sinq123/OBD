// ss.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<windows.h>
#include<iostream>
#include <vector>
#include <math.h>
#include<iomanip>
#include <fstream>
#include <vector>
#include <string>
#include<atlstr.h>
#include <stdint.h>

using namespace std;

BYTE ChkSum(
	const BYTE* const pbData, 
	const UINT uiBegin, 
		const UINT uiEnd, 
		BYTE* const pbCRC_HIGH, 
		BYTE* const pbCRC_LOW)
{
	//assert(NULL != pbData);
	//assert(0UL != uiEnd);
	//assert(uiEnd >= uiBegin);
	//assert(NULL != pbCRC_HIGH);
	//assert(NULL != pbCRC_LOW);

	BYTE bSum(0x00);

	for (UINT ui=uiBegin; ui<=uiEnd; ++ui)
	{
		bSum ^= pbData[ui];
	}

	char chCrc[3] = {'\0'};
	_snprintf_s(chCrc, sizeof(chCrc), _TRUNCATE, "%02X", bSum);

	(*pbCRC_HIGH) = (BYTE)chCrc[0];
	(*pbCRC_LOW) = (BYTE)chCrc[1];
	return bSum;
}


uint32_t data2hex(uint16_t data)
{
	uint32_t temp;
	uint8_t i = 0;

	temp = data;//转存需要计算的数值  
	while (temp)
	{
		temp >>= 1;
		i++;//计算当前值的尾数占有的位数  
	}
	i--;//计算下来，i会多加一次，这儿减掉  
	temp = data;  //再次赋值，准备合并
	temp = temp << (23 - i);//补足23位尾数  
	temp = (i + 127) << 23 | temp;//计算指数，并与尾数合并起来   
	temp = temp & ~(1 << 23);//确定正负  我这儿都是正数，就没管负数了  

	return temp;//这里就已经是以浮点数的存储方式表示的传进来的参数了
}

unsigned int flo_hex(float fvalue) 
{
	float temp = fvalue;
	unsigned int *hvalue;
	hvalue = (unsigned int *)&temp;
	//printf("%X\n", *hvalue);
	return *hvalue;
}

LONGLONG ReadCharToLL(unsigned char* str, int len)
{
	if (len <= 0)
	{
		return 0;
	}

	int i = 0;
	int value = 0;
	for (i = 0; i<len; i++)
	{
		value = value * 16 * 16 + (str[i] / 16) * 16 + str[i] % 16;
	}

	return value;
}

float Hex2Float(unsigned long long number)
{
	unsigned int sign = (number & 0x80000000) ? -1 : 1;//三目运算符
	unsigned int exponent = ((number >> 23) & 0xff) - 127;//先右移操作，再按位与计算，出来结果是30到23位对应的e
	float mantissa = 1 + ((float)(number & 0x7fffff) / 0x7fffff);//将22~0转化为10进制，得到对应的x
	return sign * mantissa * pow((double)2, (double)exponent);
}


int _tmain(int argc, _TCHAR* argv[])
{
	////02 2B 30 30 39 34 38 30 30 31 45 03 	
	////02 2B 30 30 39 34 30 30 30 31 36 03 
	////02 2B 30 30 30 30 30 30 30 31 42 03
	////02 2B 30 30 39 34 38 30 30 31 45 03
	//BYTE bWriteBuf[] = {0x02, 0x2b, 0x30, 0x30, 0x33, 0x36, 0x31, 0x30, 0x30, 0x00, 0x00, 0x03};

	//// 02 41 42 2B 30 30 30 30 30 30 33 31 42 03
	//// 02 41 42 2B 30 30 38 38 38 30 33 31 33 03
	////BYTE bWriteBuf[] = {0x02, 0x41, 0x42, 0x2b, 0x30, 0x30, 0x38, 0x38, 0x38, 0x30, 0x33, 0x00, 0x00, 0x03};
	//
	//BYTE bWriHigCrc(0x00), bWriLowCrc(0x00);

	//BYTE bSum = ChkSum(bWriteBuf, 1, sizeof(bWriteBuf)-4 , &bWriHigCrc, &bWriLowCrc);
	//bWriteBuf[sizeof(bWriteBuf)-3] = bWriHigCrc;
	//bWriteBuf[sizeof(bWriteBuf)-2] = bWriLowCrc;

	//for (int i=0; i< sizeof(bWriteBuf); i++)
	//{
	//	std::cout << std::setfill('0') << std::setw(2);
	//	std::cout << std::setiosflags(std::ios::uppercase) << std::hex << (unsigned int)(bWriteBuf[i]) <<std::endl;
	//}

	//std::cout << std::setiosflags(std::ios::uppercase) << std::hex << (unsigned int)(bSum) <<std::endl;
	////std::cout<< bWriteBuf[0] <<std::endl;

	float f = 33.5f;
	unsigned int v = 0;
	v = flo_hex(f);
	printf("%.2x\n", v);

	DWORD h = HIWORD(v);
	//printf("%.2x\n", h);
	DWORD l = LOWORD(v);
	//printf("%.2x\n", l);
	printf("%.2x\n", l & 255);
	printf("%.2x\n", l >> 8);
	printf("%.2x\n", h & 255);
	printf("%.2x\n", h >> 8);
	//buff[7] = (unsigned char)(l & 255);//得到低字节
	//buff[8] = (unsigned char)(l >> 8);//得到高字节
	//buff[9] = (unsigned char)(h & 255);//得到低字节
	//buff[10] = (unsigned char)(h >> 8);//得到高字节
	unsigned char szbuff[4] = { 0 };

	szbuff[0] = 0x02;
	szbuff[1] = 0x00;
	szbuff[2] = 0x00;
	szbuff[3] = 0x00;
	unsigned long ll = ReadCharToLL(szbuff, 4);
	printf("%d\n", ll);

	printf("%.2f\n", (float)Hex2Float(ll));

	system("pause");

	return 0;
}



