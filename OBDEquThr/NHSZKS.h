/*
* Copyright (c) 2009,佛山市华贝软件技术有限公司
* All rights reserved.
*
* 文件名称:CNHSZKS.h
* 文件标识:
* 摘    要:特殊定制LED屏(型号:CNHSZKS)基本操作
*          通过COM口通讯协议进行通讯
*
* 当前版本:1.0.0
* 作    者:Wuyq
* 完成日期:2013-09-10
*
* 历史信息:
*/
#pragma once

#ifndef NHSZKS_H
#define NHSZKS_H

#include "Panel.h"
#include <vector>

class CNHSZKS : public CPanel
{
public:
	CNHSZKS(void);
	~CNHSZKS(void);

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
	// 某行输出文本转换到buf里面
	// 参数：
	// nPanelIdx：屏号
	// nLineIdx：行号
	// strLineTxt：输出文本
	// chSendBuf[]：保存要发送的命令的buf
	// nIdx：buf开始写的偏移量
	// 返回：buf已写的最后一位的下一位
	int LineToBuf(const int nPanelIdx, const int nLineIdx, const std::wstring& strLineTxt, char chSendBuf[], const int nIdx = 0);

	// 字符串分行
	// 参数：
	// str：要分行的字符串
	// vResult：返回结果存放在此向量中
	// bKeepEnds：是否保存回车换行符
	// 返回：无返回值
	void SplitLines(const std::wstring& str, std::vector<std::wstring>& vResult, const bool bKeepEnds = false);

private:
	// 串口句柄
	HANDLE m_hComm;
	// 串口是否连接
	bool m_bIsConnect_Serial;
};

//深圳开仕智能显示系统设计中心计量、测量自动化设备led大屏幕显示通讯协议
//	
//一、通讯协议：
//数据流:
//<- :开始标记，屏号，行号，显示方式，...显示内容...,0DH;
//1, 格式:波特率=9600 BIT/S,1起始位,1停止位,8位数据;"9600,N,8,1"
//2, 发送的汉字西文数据流:
//	1) 00H；(通讯开始标记)
//	2) 31H；屏号 ；  
//		屏号>30H,30H=所有屏,
//		31H=1号屏 ；
//		32H=2号屏 ；
//		33H=3号屏 ；
//		34H....3AH,..3FH40H..；
//	3) 31H；行号；
//		行号>30H,30H=所有行,以广播的方式向所有行发送；
//		31H=第一行 ;在第一行显示16点阵字符；
//		32H＝第二行, 在第二行显示16点阵字符；
//		33H=第三行, 在第一行显示16点阵字符；
//		34H＝第四行，在第一行显示16点阵字符；
//	★  35H=第五行 在支持显示24点阵字符的显示屏,发送第五行就会显示24点阵字符；
//	★  36H=第六行 在支持显示24点阵字符的显示屏,发送第五行就会显示24点阵字符；
//	★  39H=第九行 在支持显示32点阵字符的显示屏,发送第九行就会显示32点阵字符；
//	4) 30H；左移方式;
//	   31H ；靠左显示
//	   32H ；居中显示
//	   33H ；靠右显示
//       
//说明： 
//	显示内容；若发送到每行显示内容超过屏物理长度时,自动左移显示信息，不删除任何信息；文字颜色:红；不设停留时间，显示完一幕完整信息再显示第二幕信息；
//
//结束位  0DH; (文字结束,通讯结束)
//
//
//例如:
//发送:00H, 31H,31h, 32H,"粤B 1234 检车速",0DH; 
//	此时结果是:1号显示屏第一行居中显示:"  粤B 1234 检车速  "
//发送:00H, 31H,32h,32H,"粤B 1234 检大灯",0DH; 
//	此时结果是:1号显示屏第二行居中显示:"  粤B 1234 检大灯  "
//发送:00H, 31H,33h,32H,"粤B 1234 检废气",0DH; 
//	此时结果是:1号显示屏第三行居中显示:"  粤B 1234 检废气  "
//发送:00H, 31H,31h, 31H,"粤B 1234 检车速",0DH; 
//	此时结果是:1号显示屏第一行靠左显示:"粤B 1234 检车速    "
//发送:00H, 31H,31h, 32H,"粤B 1234 时速40时踏行程开关",0DH; 
//	此时结果是:因为超过八个汉字,1号显示屏第一行自动循环向左走动显示:"粤B 1234时速40 
//时踏行程开关"
//															深圳市开仕智能显示系统设计中心
//															2005年03月21日

#endif