/*
* Copyright (c) ,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHJudge.h
* �ļ���ʶ��
* ժҪ���ж�ģ��
*
* �汾��1.0
* ���ߣ�Raylee
* ���ڣ�2018-04-16
*
*/


#ifndef NHJudge_H
#define NHJudge_H

enum NHJudgement
{
	Jud_Pass = 1,		// �ϸ�
	Jud_Failed,			// ���ϸ�
	Jud_Unfinished,		// δ���
	Jud_Abandon,		// ������⣨������OBD�ĳ���������ͨѶԭ�������⣬ֻ�ܷ�����
	Jud_Unknow,			// δ֪ 
};

class CJudge
{
public:
	CJudge(void);
	virtual ~CJudge(void);

	// ���ÿ������캯���Ϳ�����ֵ����
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