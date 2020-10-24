#ifndef LOG_IN_H
#define LOG_IN_H

extern "C"
{
	// µÇÂ¼
	DWORD AFX_EXT_API ShowLogIn(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);

}



#endif