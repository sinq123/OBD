/*
* Copyright (c) ,佛山市南华仪器有限公司
* All rights reserved.
*
* 文件名称：NHJudge.h
* 文件标识：
* 摘要：判定模块
*
* 版本：1.0
* 作者：Raylee
* 日期：2018-04-16
*
*/


#ifndef NHJudge_H
#define NHJudge_H

enum NHJudgement
{
	Jud_Pass = 1,		// 合格
	Jud_Failed,			// 不合格
	Jud_Unfinished,		// 未完成
	Jud_Abandon,		// 放弃检测（例如有OBD的车辆，由于通讯原因而不检测，只能放弃）
	Jud_Unknow,			// 未知 
};

class CJudge
{
public:
	CJudge(void);
	virtual ~CJudge(void);

	// 禁用拷贝构造函数和拷贝赋值函数
private:
	CJudge(const CJudge &);
	CJudge & operator =(const CJudge &);

public:
	NHJudgement static GetJudgeApp(LPCTSTR szRunningNumber);
	NHJudgement static GetJudgeOBD(LPCTSTR szRunningNumber);

	NHJudgement static GetJudgeASM(LPCTSTR szRunningNumber);
	NHJudgement static GetJudgeVMAS(LPCTSTR szRunningNumber);
	NHJudgement static GetJudgeLugdown(LPCTSTR szRunningNumber);
	NHJudgement static GetJudgeDIS(LPCTSTR szRunningNumber);
	NHJudgement static GetJudgeFSUNHT(LPCTSTR szRunningNumber);

	NHJudgement static ReJudgeTestLog(LPCTSTR szRunningNumber);

};

#endif