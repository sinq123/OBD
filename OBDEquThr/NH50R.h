/*
* Copyright (c) 2009,佛山市华贝软件技术有限公司
* All rights reserved.
*
* 文件名称:NH50R.h
* 文件标识:
* 摘    要:特殊定制LED屏(型号:NH50R)基本操作
*          通过TCP以太网通讯协议进行通讯
*
* 当前版本:1.0.0
* 作    者:Cui
* 完成日期:2009-08-20
*
* 历史信息:
* 1.当前版本:1.0.1
*   作    者:Cui
*   完成日期:2009-08-26
*   摘    要:增加函数SendToServer,可以根据NH50R通讯协议灵活发送控制字符
*			 以太网通讯协议由UDP改为TCP,因为UDP通讯影响数据库操作

* 2.当前版本:1.0.2
*   作    者:Cui
*   完成日期:2009-09-02
*   摘    要:更改了DisconnectServer函数里面多调用一次WSACleanup。

* 3.当前版本:1.0.3
*   作    者:Ming
*   完成日期:2013-05-14
*   摘    要:增加串口连接点阵屏
*/


#pragma once

#ifndef NH50R_H
#define NH50R_H

#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "Panel.h"

class  CNH50R : public CPanel
{
public:
	CNH50R(void);
	~CNH50R(void);

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
	DWORD ConnectServer(const BYTE bPort, const int nBaudRate = CBR_19200);
	
	// 断开点阵屏连接
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD DisconnectServer(void);

	// 判断点阵屏是否连接
	bool IsConnect(void) const;

	// 设置播放列表
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD SetList(void);

	// 删除播放列表
	// (return): 0x00:正常
	//           非0x00:异常
	DWORD DelList(void);

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
	// 参数：
	// nPanel：屏号
	// nLine：行号
	// pwchLineTxt：行文本
	// 返回：0x00：正常
	//           非0x00：异常
	DWORD DisLine(const int nPanel, const int nLine, const wchar_t* pwchLineTxt);

	// 发送到服务器
	// 可以灵活进行设置点阵屏
	// (in)pch:发送缓冲
	// (in)nLength:发送缓冲长度
	// (return): 0x00:正常
	//           非0x00:异常
	inline DWORD SendToServer(const char *const pchBuf, const int nLen);

private:
	// 客户端连接线程
	SOCKET m_socket;
	// 串口句柄
	HANDLE m_hComm;
	// 串口是否连接
	bool m_bIsConnect_Serial;
	// 网口是否连接
	bool m_bIsConnect_Ethernet;

private:
	// 发送缓冲区长度:1024
	// LED屏NH50R缓冲区大小来设置:1024字节
	const int m_nSendBufLength;

	// 发送缓冲区
	// 1024字节,容量大,在堆中进行分配,不在栈中分配
	char *m_pchSendBuf;
};

#endif