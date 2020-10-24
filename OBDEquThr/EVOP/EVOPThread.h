#pragma once

#include "EVOP.h"
#include "NHEVOP.h"


class AFX_EXT_CLASS CEVOPThread : public CWinThread
{
	DECLARE_DYNCREATE(CEVOPThread)
	
protected:
	CEVOPThread(void);
	virtual ~CEVOPThread(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()
	
public:
	// 仪器型号
	enum InstrumentModel
	{
		IM_NHEVOP,
	};

public:
	// 打开串口
	DWORD Open(const BYTE bPort, const int nBaudRate, const wchar_t *szProtocol);

	// 关闭串口
	DWORD Close(void);

	// 判断串口是或打开
	bool IsOpen(void) const;

	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emInstrumentModel;
	}
	

	// 开始检测
	DWORD StartTest(const int nItem, const int nType);
	// 取实时数据
	DWORD GetRTData(float &fP1, float &fP2, float &fP1_T, float &fP2_T);
	// 取状态
	DWORD GetState(int &nState);
	// 取反馈信息
	DWORD GetTestInfo(int &nTestState, int &nTimeCount);
	// 结束检测
	DWORD EndTest(int &nItem, int &nJudge);

private:
	// 初始化设备
	// 参数：
	// szProtocol：设备协议
	void InitializeInstrument(const wchar_t *szProtocol);
	// 卸载设备
	void UninitializeInstrument(void);

public:
	// 退出线程操作
	void QuitThread(void);


private:
	// 对象
	CEVOP* m_pAdapter;
	InstrumentModel m_emInstrumentModel;
	

private:
	// 线程退出标志
	bool m_bQuitThread;

	// 串口号
	BYTE m_bComPort;
	// 串口波特率
	int m_nComBaudRate;
};