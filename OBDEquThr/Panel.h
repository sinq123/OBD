/*
* Copyright (c) 2010,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：CLEDPanel.h
* 文件标识：
* 摘要：LED抽象类
*		以纯虚拟函数形式定义LED操作接口
*
* 当前版本：1.0.0
* 作者：Wuyq
* 完成日期：2013-09-10
*
* 历史信息：
*
* 当前版本：1.0.0
* 完成日期：2013-09-10
* 作者：Wuyq
* 描述：正式发布使用
*/

#pragma once

class CPanel
{
public:
	CPanel(void);
	virtual ~CPanel(void);

public:
	// 通过网口连接点阵屏
	// (in)pwchIP:Socket服务器IP地址
	// (in)usPort:Socket服务器端口
	// (return): 0x00:正常
	//           非0x00:异常
	virtual DWORD ConnectServer(const wchar_t *const pwchIP, const unsigned short usPort) = 0;

	// 打开串口连接点阵屏
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// in bPort：串口号(1~255)
	// in nBaudRate：波特率 (110、300、600、1200、2400、4800、9600、14400、19200、38400、56000、57600、115200、128000、256000)
	virtual DWORD ConnectServer(const BYTE bPort, const int nBaudRate = CBR_19200) = 0;
	
	// 断开点阵屏连接
	// (return): 0x00:正常
	//           非0x00:异常
	virtual DWORD DisconnectServer(void) = 0;

	// 判断点阵屏是否连接
	virtual bool IsConnect(void) const = 0;

	// 设置播放列表
	// (return): 0x00:正常
	//           非0x00:异常
	virtual DWORD SetList(void) = 0;

	// 删除播放列表
	// (return): 0x00:正常
	//           非0x00:异常
	virtual DWORD DelList(void) = 0;

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
	virtual DWORD Display(const wchar_t *const pwchDisplayTxt,
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
		) = 0;

	// 显示单行
	// 参数：
	// nPanel：屏号
	// nLine：行号
	// pwchLineTxt：行文本
	// 返回：0x00：正常
	//           非0x00：异常
	virtual DWORD DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt) = 0;

	// 发送到服务器
	// 可以灵活进行设置点阵屏
	// (in)pch:发送缓冲
	// (in)nLength:发送缓冲长度
	// (return): 0x00:正常
	//           非0x00:异常
	virtual DWORD SendToServer(const char *const pchBuf, const int nLen) = 0;
};
