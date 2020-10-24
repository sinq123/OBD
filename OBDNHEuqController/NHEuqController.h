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
	// �����������߳�
	CAnaThread *m_pAnaThread;
	bool m_bAnaThreadAttach;
	bool m_bConnectToFlowmeter;

	// ������
	CAnaThread *m_pFloThread;
	bool m_bFloThreadAttach;

	// �����������߳�
	CAnaThread *m_pNOxThread;
	bool m_bNOxThreadAttach;

	// ��͸���߳�
	COpaThread *m_pOpaThread;

	// OBD�������߳�
	COBDAdapterThread *m_pOBDThread;

	// �����ɼ����߳�
	CParaThread *m_pParaThread;

private:
	CString m_strObdPath;
};