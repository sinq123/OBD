/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：TacThread.h
* 文件标识：
* 摘要：转速计线程
*
* 当前版本：1.0
* 作者：Cui,hyh
* 完成日期：2009-12-08
*
* 历史信息：
*
*
* 当前版本：1.0.0
* 完成日期：2009-12-08
* 作者：Cui,hyh
* 描述：正式发布使用
*/


#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaRpm5300.h"
#include "FuLiVMT2000.h"
#include "HuaGongVTET1000.h"
#include "MingQuanMQZ3.h"

// CTacThread

class AFX_EXT_CLASS CTacThread : public CWinThread
{
	DECLARE_DYNCREATE(CTacThread)

protected:
	CTacThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTacThread();

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
		// RPM5300
		IM_NANHUA_RPM5300,
		// RPM5500(仲未出世)
		//IM_NANHUA_RPM5500,
		// 福立
		IM_FULI_VMT2000,
		// 华工
		IM_HUAGONG_VTET1000,
		// 鸣泉
		IM_MINGQUAN_MQZ3,
	};

public:
	inline InstrumentModel GetInstrumentModel(void) const
	{
		return m_emInstrumentModel;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作begin
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// (return)：
	// TAC_OPEN_SUCCEED
	// TAC_OPEN_FAIL
	DWORD Open(const BYTE bPort, const wchar_t *szProtocol);
		
	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// (return)：
	// TAC_CLOSE_SUCCEED
	// TAC_CLOSE_FAIL_WIN32API
	// TAC_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// 判断串口是或打开
	// (return)：
	// true:串口已打开
	// false:串口未打开
	inline bool IsOpen(void) const
	{
		return m_pTac->IsOpen();
	}
	// 获取串口操作状态
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 串口操作end

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器同步操作begin
	// 获取转速
	// (out)pnRev：发动机转速指针，单位，r/min
	// (return)：
	// TAC_WR_FUNC_SUCCEED
	// TAC_WR_FUNC_PORT_NOT_OPEN
	// TAC_WR_FUNC_WRITE_SPECIFIED_DATA_FAIL
	// TAC_WR_READ_SPECIFIED_DATA_FAIL
	// TAC_WR_EQUIP_RETURE_0X15
	// TAC_WR_CHECK_SUM_FAIL
	// TAC_WR_UNKNOWN_ERROR
	DWORD GetRev(int *pnRev);
	inline DWORD GetRev(void)
	{
		return GetRev(&m_nRev);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器同步操作end

public:
	// 异步获取
	void GetRev_Asyn(HWND hCallBackWnd=NULL);

public:
	// 退出线程操作
	void QuitThread(void);

public:
	// 获取转速值
	inline int GetRevValue(void) const
	{
		return m_nRev;
	}

private:
	// 响应异步操作函数
	afx_msg void OnGetRev(WPARAM wParam, LPARAM lParam);

private:
	// 初始化设备
	void InitializeInstrument(const wchar_t *szProtocol);
	// 卸载设备
	void UninitializeInstrument(void);

private:
	// RPM5300操作类
	CTac* m_pTac;
	// 转速,单位:r/min
	int m_nRev;
	// 线程退出标志
	bool m_bQuitThread;
	// 设备型号
	InstrumentModel m_emInstrumentModel;
	// 串口操作状态
	DWORD m_dwStatus;
};


