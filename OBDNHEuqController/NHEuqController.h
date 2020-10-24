#pragma once

#include <memory>
#include <string>

#include "..\OBDEquThr\EquThr.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\OBDEquThr_D.lib")
#else
#pragma comment(lib, "..\\release\\OBDEquThr.lib")
#endif

class AFX_CLASS_EXPORT CNHEuqController
{
public:
	static CNHEuqController& GetInstance(void);

private:
	CNHEuqController(void);
	virtual ~CNHEuqController(void);
	CNHEuqController(const CNHEuqController &);
	CNHEuqController & operator =(const CNHEuqController &);
	friend class std::auto_ptr<CNHEuqController>;

public:
	void Init(void);
	void UnInit(void);
		
public:
	// 废气分析仪线程
	CAnaThread *m_pAnaThread;
	bool m_bAnaThreadAttach;
	bool m_bConnectToFlowmeter;

	// 流量计
	CAnaThread *m_pFloThread;
	bool m_bFloThreadAttach;

	// 氮氧分析仪线程
	CAnaThread *m_pNOxThread;
	bool m_bNOxThreadAttach;

	// 不透光线程
	COpaThread *m_pOpaThread;

	// OBD适配器线程
	COBDAdapterThread *m_pOBDThread;

	// 参数采集仪线程
	CParaThread *m_pParaThread;

private:
	CString m_strObdPath;
};