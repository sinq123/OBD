#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NHSZKS.h"
#include "NH50R.h"

// CPanelThread

class AFX_EXT_CLASS CPanelThread : public CWinThread
{
	DECLARE_DYNCREATE(CPanelThread)

protected:
	CPanelThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CPanelThread();

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
		// NH50R （青松广告屏）
		IM_NANHUA_NH50R,
		// NH1 （珠海新港）
		IM_NANHUA_NH1,
		// NH2 （昆明九站）
		IM_NANHUA_NH2,
		// NHSZKS （深圳开仕）
		IM_NANHUA_NHSZKS,
		// NHSHTHD （上海同含道）
		IM_NANHUA_NHSHTHD,
	};

public:
	// 通过网口连接点阵屏
	// (in)pwchIP:Socket服务器IP地址
	// (in)usPort:Socket服务器端口
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort);

	// 打开串口连接点阵屏
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// in nBaudRate：波特率 (110、300、600、1200、2400、4800、9600、14400、19200、38400、56000、57600、115200、128000、256000)
	DWORD ConnectServer(const BYTE bPort, const int nBaudRate = CBR_19200, const enum InstrumentModel im = IM_NANHUA_NH50R);

	// 断开点阵屏连接
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD DisconnectServer(void);

	// 判断点阵屏是否连接
	inline bool IsConnect(void) const
	{
		return m_pLEDPanel->IsConnect();
	};

	// 设置播放列表
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD SetList(void);

	// 删除播放列表
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD DelList(void);

	// 删除播放文件
	// (return): 0x00:正常
	//           非0x00:异常
	//DWORD DelFile(void);

	// 显示
	// (in)pwchDisplayTxt:显示文本
	// (in)bTwinking:是否闪烁
	// (in)chLineSpace:行间距
	// (in)chInDisplayMode:入花样
	// (in)chOutDisplayMode:出花样
	// (in)chFramePauseTime:帧停留时间
	// (in)chDisplaySpeed:显示速度
	// (in)chFontAndSize:字体及大小
	// (in)chSetupType:处理模式
	// (in)chhAlign:水平对齐
	// (in)chvAlign:垂直对齐
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD Display(const wchar_t *const pwchDisplayTxt,
		const bool bTwinking=false,
		const char chLineSpace='0',
		const char chInDisplayMode=0x30,
		const char chOutDisplayMode=0x30,
		const char chFramePauseTime='3',
		const char chDisplaySpeed='0',
		const char chFontAndSize='5',
		const char chSetupType='b',
		const char chhAlign='0',
		const char chvAlign='1'
		);

	// 显示单行
	// (in)nPanel：屏号
	// (in)nLine：行号
	// (in)pwchDisLine：单行显示内容
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt);

public:
	// 发送
	// 可以灵活进行设置点阵屏
	// (in)pch：发送缓冲
	// (in)nLength：发送缓冲长度
	// (return): 0x00：正常
	//           非0x00：异常
	DWORD SendToServer(const char *const pchBuf, const int nLen);

public:
	// 退出线程操作
	void QuitThread(void);

private:
	// 初始化设备
	void InitializeInstrument(const enum InstrumentModel im);
	// 卸载设备
	void UninitializeInstrument(void);

private:
	// 点阵屏操作类
	CPanel* m_pLEDPanel;
	// 线程退出标志
	bool m_bQuitThread;
};


