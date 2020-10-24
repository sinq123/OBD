/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：Tac.h
* 文件标识：
* 摘要：转速计抽象类
*		以纯虚拟函数形式定义转速计操作接口
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

#ifndef TAC_H
#define TAC_H

// 函数执行结果
#define TAC_OPEN_SUCCEED								0x00  // 打开串口成功
#define TAC_OPEN_FAIL									0x01  // 打开串口失败
#define TAC_CLOSE_SUCCEED								0x00  // 关闭串口成功
#define TAC_CLOSE_FAIL_WIN32_API						0x01  // 关闭串口失败：执行win32 API(CloseHandle)失败
#define TAC_CLOSE_FAIL_NO_OPEN_PORT						0x02  // 关闭串口失败：没有可以关闭的串口
#define TAC_WR_SUCCEED									0x00  // 写读成功
#define TAC_WR_PORT_NOT_OPEN							0x01  // 写读失败：串口未打开
#define TAC_WR_WRITE_SPECIFIED_DATA_FAIL				0x02  // 写读失败：写指定数据失败
#define TAC_WR_READ_SPECIFIED_DATA_FAIL					0x03  // 写读失败：读指定数据失败
#define TAC_WR_EQUIP_RETURE_0X15						0x04  // 写读失败：仪器返回错误代码（0x15）
#define TAC_WR_CHECK_SUM_FAIL							0x05  // 写读失败：读出数据校验和失败
#define TAC_WR_UNKNOWN_ERROR							0xFF  // 写读失败：未知错误

class CTac
{
public:
	CTac(void);
	virtual ~CTac(void);

public:
	// 打开串口
	// 打开串口前，自动识别是否已经存在已经打开的串口资源，如果有，在打开之前，先关闭以前打开的串口资源
	// (in)bPort：串口号(1~255)
	// (return)：
	// TAC_OPEN_SUCCEED
	// TAC_OPEN_FAIL
	virtual DWORD Open(BYTE bPort) = 0;

	// 关闭串口
	// 自动识别是否有可以不关闭的串口资源
	// (return)：
	// TAC_CLOSE_SUCCEED
	// TAC_CLOSE_FAIL_WIN32API
	// TAC_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// 判断串口是或打开
	// (return)：
	// true:串口已打开
	// false:串口未打开
	virtual bool IsOpen(void) const = 0;

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
	virtual DWORD GetRev(int *pnRev) = 0;
};

#endif