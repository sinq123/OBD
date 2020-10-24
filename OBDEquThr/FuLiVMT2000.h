/*
* Copyright (c) 2009,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：FuLiVMT2000.h
* 文件标识：
* 摘要： 广州福立VMT-2000转速计通讯类，继承于抽象类CTac
*		依照转速适配器VMT-2000通讯协议进行编写
*       具体请参考转速适配器VMT-2000通讯协议说明书
*
*
* 版本：1.0
* 作者：Raylee
* 日期：2017-05-24
*
* 历史信息：
*
*
* 版本：1.0.0
* 日期：2017-05-24
* 作者：Raylee
* 描述：正式发布
*/

#pragma once

#ifndef FuLi_VMT2000_H
#define FuLi_VMT2000_H

#include "Tac.h"


class CFuLiVMT2000 : public CTac
{
public:
	CFuLiVMT2000(void);
	~CFuLiVMT2000(void);

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
	
protected:
	// 写读串口数据
	// 写入相应的命令，读取对应的返回数据
	// 并根据需要，分别对写入和读取的数据进行校验和验证
	// (in)pbWriteBuffer：要写入的数据内容，不能为空指针
	// (in)dwWriteLength：要写入的数据长度
	// (out)pbReadBuffer：要读取的数据内容，不能为空指针
	// (in)dwReadLength：要读取的数据长度
	// (in)bNeedWriteChkSum：true，需要写校验，false：不需要
	// (in)bNeedReadChkSum：true，需要读校验，false：不需要
	// 返回，函数执行结果：
	// NHC_WR_FUNC_SUCCEED
	// NHC_WR_FUNC_PORT_NOT_OPEN
	// NHC_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHC_WR_FUNC_EQUIP_RETURE_0X15
	// NHC_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *pbWriteBuffer, DWORD dwWriteLength, BYTE *pbReadBuffer, DWORD &dwReadLength, bool bNeedWriteChkSum=true, bool bNeedReadChkSum=true);

	// 计算校验和
	// 相加，然后取反加一
	// (in)p：要进行校验的数据内容
	// (in)un：要进行校验的数据长度
	// 返回校验和
	BYTE CheckSum(BYTE *p, UINT un);
private:
	// 串口句柄
	HANDLE m_hComm;
	// 串口操作临界区
	CRITICAL_SECTION m_csComm;
};

#endif