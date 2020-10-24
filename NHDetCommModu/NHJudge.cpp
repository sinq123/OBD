// ReportNumberGenerator.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "NHJudge.h"
#include <cassert>
#include <fstream>
#include <tchar.h>
#include <atlstr.h>

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT

#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

NHJudgement CJudge::GetJudgeApp(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
	
	CString strSQL;
	strSQL.Format(L"SELECT * FROM ResultOfAppearance WHERE RunningNumber='%s'", szRunningNumber);
	SResultOfAppearance sResultOfAppearance;
	DWORD dwRet = GetDboResultOfAppearance(strSQL, &sResultOfAppearance);

	if (dwRet == 0x00)
	{
		emJud = Jud_Unfinished;
	}
	else if (dwRet == 0x01)
	{
		if (sResultOfAppearance.strPass == L"1")
		{
			emJud = Jud_Pass;
		}
		else if (sResultOfAppearance.strPass == L"0")
		{
			emJud = Jud_Failed;
		}
	}

	return emJud;
}

NHJudgement CJudge::GetJudgeOBD(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
	
	CString strSQL;
	strSQL.Format(L"SELECT * FROM ResultOfOBD WHERE RunningNumber='%s'", szRunningNumber);
	SResultOfOBD sResultOfOBD;
	DWORD dwRet = GetDboResultOfOBD(strSQL, &sResultOfOBD);

	if (dwRet == 0x00)
	{
		emJud = Jud_Unfinished;
	}
	else if (dwRet == 0x01)
	{
		if (sResultOfOBD.strJudge == L"1")
		{
			emJud = Jud_Pass;
		}
		else if (sResultOfOBD.strJudge == L"2")
		{
			emJud = Jud_Failed;
		}
		else
		{
			if (sResultOfOBD.strComErrorJudge == L"2")
			{
				emJud = Jud_Abandon;
			}
			else
			{
				emJud = Jud_Unfinished;
			}
		}
	}

	return emJud;
}

NHJudgement CJudge::GetJudgeASM(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
	
	CString strSQL;
	strSQL.Format(L"SELECT * FROM ResultOfASM WHERE RunningNumber='%s'", szRunningNumber);
	RESULTOFASM sResultOfASM;
	DWORD dwRet = GetDboResultOfASM(strSQL, &sResultOfASM);

	if (dwRet == 0x00)
	{
		emJud = Jud_Unfinished;
	}
	else if (dwRet == 0x01)
	{
		if (sResultOfASM.wchPass[0] == L'1')
		{
			emJud = Jud_Pass;
		}
		else if (sResultOfASM.wchPass[0] == L'0')
		{
			emJud = Jud_Failed;
		}
	}

	return emJud;
}

NHJudgement CJudge::GetJudgeVMAS(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
	
	CString strSQL;
	strSQL.Format(L"SELECT * FROM ResultOfVMAS WHERE RunningNumber='%s'", szRunningNumber);
	RESULTOFVMAS sResultOfVMAS;
	DWORD dwRet = GetDboResultOfVMAS(strSQL, &sResultOfVMAS);

	if (dwRet == 0x00)
	{
		emJud = Jud_Unfinished;
	}
	else if (dwRet == 0x01)
	{
		if (sResultOfVMAS.wchPass[0] == L'1')
		{
			emJud = Jud_Pass;
		}
		else if (sResultOfVMAS.wchPass[0] == L'0')
		{
			emJud = Jud_Failed;
		}
	}

	return emJud;
}

NHJudgement CJudge::GetJudgeLugdown(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
		
	CString strSQL;
	strSQL.Format(L"SELECT * FROM ResultOfLUGDOWN WHERE RunningNumber='%s'", szRunningNumber);
	RESULTOFLUGDOWN sResultOfLUGDOWN;
	DWORD dwRet = GetDboResultOfLUGDOWN(strSQL, &sResultOfLUGDOWN);

	if (dwRet == 0x00)
	{
		emJud = Jud_Unfinished;
	}
	else if (dwRet == 0x01)
	{
		if (sResultOfLUGDOWN.wchPass[0] == L'1')
		{
			emJud = Jud_Pass;
		}
		else if (sResultOfLUGDOWN.wchPass[0] == L'0')
		{
			emJud = Jud_Failed;
		}
	}

	return emJud;
}

NHJudgement CJudge::GetJudgeDIS(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
	
	CString strSQL;
	strSQL.Format(L"SELECT * FROM ResultOfDIS WHERE RunningNumber='%s'", szRunningNumber);
	RESULTOFDIS sResultOfDIS;
	DWORD dwRet = GetDboResultOfDIS(strSQL, &sResultOfDIS);

	if (dwRet == 0x00)
	{
		emJud = Jud_Unfinished;
	}
	else if (dwRet == 0x01)
	{
		if (sResultOfDIS.wchPass[0] == L'1')
		{
			emJud = Jud_Pass;
		}
		else if (sResultOfDIS.wchPass[0] == L'0')
		{
			emJud = Jud_Failed;
		}
	}

	return emJud;
}

NHJudgement CJudge::GetJudgeFSUNHT(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
	
	CString strSQL;
	strSQL.Format(L"SELECT * FROM ResultOfFSUNHT WHERE RunningNumber='%s'", szRunningNumber);
	RESULTOFFSUNHT sResultOfFSUNHT;
	DWORD dwRet = GetDboResultOfFSUNHT(strSQL, &sResultOfFSUNHT);

	if (dwRet == 0x00)
	{
		emJud = Jud_Unfinished;
	}
	else if (dwRet == 0x01)
	{
		if (sResultOfFSUNHT.wchPass[0] == L'1')
		{
			emJud = Jud_Pass;
		}
		else if (sResultOfFSUNHT.wchPass[0] == L'0')
		{
			emJud = Jud_Failed;
		}
	}

	return emJud;
}

NHJudgement CJudge::ReJudgeTestLog(LPCTSTR szRunningNumber)
{
	NHJudgement emJud(Jud_Unknow);
	
	CString strSQL;
	strSQL.Format(L"SELECT * FROM TestLog WHERE RunningNumber='%s'", szRunningNumber);
	TESTLOG sTestLog;
	DWORD dwRet = GetDboTestLog(strSQL, &sTestLog);

	if (dwRet == 0x01)
	{
		// 外观是所有车都需要做的
		NHJudgement appJud = GetJudgeApp(szRunningNumber);	
		if (appJud == Jud_Failed)
		{
			emJud = Jud_Failed;
		}
		

	}

	return emJud;
}