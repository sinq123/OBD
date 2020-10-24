#include "StdAfx.h"
#include "Opa.h"

COpa::COpa(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
	::InitializeCriticalSection(&cInfoSection);
}

COpa::~COpa(void)
{
	::DeleteCriticalSection(&cInfoSection);
}

HANDLE COpa::GetComHandle(void) 
{
	return m_hComm;
}

void COpa::AttachComHandle(HANDLE hCom)
{
	m_hComm = hCom;
}

void COpa::DetachComHandle(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
}

CRITICAL_SECTION COpa::GetCriticalSection(void)
{
	return cInfoSection;
}

void COpa::AttachCriticalSection(CRITICAL_SECTION cs)  
{ 
	::DeleteCriticalSection(&cInfoSection);
	cInfoSection = cs; 
}

void COpa::DetachCriticalSection(void)  
{
	cInfoSection = CRITICAL_SECTION();	
	::InitializeCriticalSection(&cInfoSection);	
}

float COpa::K2N(const float fK, const unsigned short usL)
{
	return 100 * (1 - exp(fK * (-usL/1000.0f)));
}

float COpa::N2K(const float fN, const unsigned short usL)
{
	return -1 / (usL/1000.0f) * log(1 - fN/100.0f);
}