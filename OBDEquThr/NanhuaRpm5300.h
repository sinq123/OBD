/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NanhuaRpm5300.h
* 文件标识：
* 摘要：转速计RPM5300通讯类，继承于抽象类CTac
*		依照转速适配器RPM5300通讯协议进行编写
*       具体请参考转速适配器RPM5300通讯协议说明书
*
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

#ifndef NANHUA_RPM5300_H
#define NANHUA_RPM5300_H

#include "Tac.h"

class CNanhuaRpm5300 : public CTac
{
public:
	CNanhuaRpm5300(void);
	~CNanhuaRpm5300(void);

public:
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// (return)：
	// TAC_OPEN_SUCCEED
	// TAC_OPEN_FAIL
	virtual DWORD Open(BYTE bPort);

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// (return)：
	// TAC_CLOSE_SUCCEED
	// TAC_CLOSE_FAIL_WIN32API
	// TAC_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void);

	// 判断串口是或打开
	// (return)：
	// true:串口已打开
	// false:串口未打开
	virtual bool IsOpen(void) const;

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
	virtual DWORD GetRev(int *pnRev);

private:
	// 串口句柄
	HANDLE m_hComm;
	// 串口操作临界区
	CRITICAL_SECTION m_csComm;
};

#endif