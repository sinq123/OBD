#include "StdAfx.h"
#include "Ana.h"

#define ABSOLUTE_ZERO 273.15f // 绝对零度，单位：卡尔文
#define STD_ATM_PRE 101.33f // 标准大气压，单位：kPa

CAna::CAna(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_enMode = CAna::ToAnalyzer;
	::InitializeCriticalSection(&cInfoSection);	
}

CAna::~CAna(void)
{
	::DeleteCriticalSection(&cInfoSection);
}

float CAna::ActFlux2StdFlux0(const float fActFlux, const float fTemp, const float fPres)
{
	float f = fActFlux*ABSOLUTE_ZERO*fPres/((fTemp+ABSOLUTE_ZERO)*STD_ATM_PRE);
	// 格式化2位小数，并四舍五入
	f = CNHCommonAPI::round(f * 100.0f)/100.0f;
	return f;
}

float CAna::ActFlux2StdFlux20(const float fActFlux, const float fTemp, const float fPres)
{
	float f = fActFlux*(ABSOLUTE_ZERO+20.0f)*fPres/((fTemp+(ABSOLUTE_ZERO+20.0f))*STD_ATM_PRE);
	// 格式化2位小数，并四舍五入
	f = CNHCommonAPI::round(f * 100.0f)/100.0f;
	return f;
}

float CAna::StdFlux202ActFlux(const float fStdFlux20, const float fTemp, const float fPres)
{
	float f = fStdFlux20*STD_ATM_PRE/(ABSOLUTE_ZERO + 20.0f)*(fTemp+ABSOLUTE_ZERO)/fPres;
	// 格式化2位小数，并四舍五入
	f = CNHCommonAPI::round(f * 100.0f)/100.0f;
	return f;
}

float CAna::StdFlux202StdFlux0(const float fStdFlux20, const float fTemp, const float fPres)
{
	float fActFlux = StdFlux202ActFlux(fStdFlux20, fTemp, fPres);
	float fStdFlux0 = ActFlux2StdFlux0(fActFlux, fTemp, fPres);
	return fStdFlux0;
}

HANDLE CAna::GetComHandle(void) 
{
	return m_hComm;
}

void CAna::AttachComHandle(HANDLE hCom)
{
	m_hComm = hCom;
}

void CAna::DetachComHandle(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
}

CRITICAL_SECTION CAna::GetCriticalSection(void)
{
	return cInfoSection;
}

void CAna::AttachCriticalSection(CRITICAL_SECTION cs)  
{ 
	::DeleteCriticalSection(&cInfoSection);
	cInfoSection = cs; 
}

void CAna::DetachCriticalSection(void)  
{	
	::InitializeCriticalSection(&cInfoSection);	
}

bool CAna::WriteCommBuffer(BYTE* const pbWriteBuffer, DWORD const dwNumberOfBytesToWrite)
{
	if (!IsOpen())
	{
		return false;
	}

	DWORD dwBytesWritten(0x00);
	// 清除缓冲数据
	//PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	// 写Comm
	if (WriteFile(m_hComm, pbWriteBuffer, dwNumberOfBytesToWrite, &dwBytesWritten, NULL))
	{
		if (dwNumberOfBytesToWrite == dwBytesWritten)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CAna::ReadCommBuffer(BYTE* const pbReadBuffer, DWORD const dwNumberOfBytesToRead)
{
	if (!IsOpen())
	{
		return false;
	}

	DWORD dwBytesRead(0x00);
	// 读Comm
	BOOL bSucceed = ReadFile(m_hComm, pbReadBuffer, dwNumberOfBytesToRead, &dwBytesRead, NULL);
	if (bSucceed && dwNumberOfBytesToRead==dwBytesRead)
	{	
		return true;
	}
	else
	{
		return false;
	}
}