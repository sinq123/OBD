/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�DynThread.h
* �ļ���ʶ��
* ժҪ���⹦���߳�
*
* �汾��1.0
* ���ߣ�Cui
* ���ڣ�2009-12-10
*
* ��ʷ��Ϣ��
*
*
* �汾��1.0.6
* ���ڣ�2014-06-02
* ���ߣ�Qin
* ������������Ժ���ٿ���ģʽ
*
* �汾��1.0.5
* ���ڣ�2010-08-20
* ���ߣ�Qin
* ���������������ö�Ӧ�Ļ�ȡ�������ٶȵ��ú���
*
* �汾��1.0.4
* ���ڣ�2010-07-21
* ���ߣ�Qin
* �������������õ������ٶȵ��ú���
*
* �汾��1.0.3
* ���ڣ�2010-07-06
* ���ߣ�Cui
* ���������������⹦����֧��

* �汾��1.0.2
* ���ڣ�2010-06-04
* ���ߣ�Qin
* ������������غɿ���ģʽ���ú���
*
* �汾��1.0.1
* ���ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* �汾��1.0.0
* ���ڣ�2009-12-10
* ���ߣ�Cui
* ��������ʽ����ʹ��
*/

#pragma once

#include "..\NHDetCommModu\NHDetCommModu.h"

#include "NanhuaNhc03.h"
#include "MahaASMP.h"

// CDynThread

class AFX_EXT_CLASS CDynThread : public CWinThread
{
	DECLARE_DYNCREATE(CDynThread)

protected:
	CDynThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDynThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()

public:
	// �����ͺ�
	enum InstrumentModel
	{
		// �ϻ�NHC03
		IM_NANHUA_NHC03,
		// ���ASM-P
		IM_MAHA_ASMP,
	};

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// (in)bPort�����ں�(1~255)
	// ���ش򿪽����
	// DYN_OPEN_SUCCEED
	// DYN_OPEN_FAIL
	DWORD Open(const BYTE bPort, const enum InstrumentModel im);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// DYN_CLOSE_SUCCEED
	// DYN_CLOSE_FAIL_WIN32API
	// DYN_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����Ƿ��
	inline bool IsOpen(void) const
	{
		return m_pDyn->IsOpen();
	}
	// ��ȡ���ڲ���״̬
	inline DWORD GetOpeStatus(void) const
	{
		return m_dwStatus;
	}
public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������start 
	// ȡʵʱ����
	// (out)pfP������ָ�룬��λkW������Ϊ��ָ��
	// (out)pfF����ָ�룬��λN������Ϊ��ָ��
	// (out)pfV���ٶ�ָ�룬��λkm/h������Ϊ��ָ��
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetRealTimeData(float *const pfP, float *const pfF, float *const pfV);
	inline DWORD GetRealTimeData(void)
	{
		return m_pDyn->GetRealTimeData(&m_fP, &m_fF, &m_fV);
	}

	// ȡ״̬
	// (out)pbs16Status��״̬λָ�룬��16λ������λ��Ӧ�ĺ�����ο�ͨѶ˵���飬����Ϊ��ָ�루����ͨѶ���ԣ�
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetStatus(std::bitset<16> *const pbs16Status);
	inline DWORD GetStatus(void)
	{
		return m_pDyn->GetStatus(&m_btStatus);
	}

	// ������
	// (in)bVarNumber��Ҫ��ȡ�ı����ţ�����������Ӧ�ı�������ο�ͨѶ˵����
	// (out)pwVarValue����ȡ���ı���ֵ��������ο�ͨѶ˵���飬����Ϊ��ָ��
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ReadVar(const BYTE bVarNumber, WORD *const pwVarValue);
	inline DWORD ReadVar(const BYTE bVarNumber)
	{
		return m_pDyn->ReadVar(bVarNumber, &(m_wVarValues[bVarNumber]));
	}

	// д����
	// (in)bVarNumber��Ҫд��ı����ţ�����������Ӧ�ı�������ο�ͨѶ˵����
	// (in)pwVarValue��Ҫд��ı���ֵ�����ݺ�����ο�ͨѶ˵����
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteVar(const BYTE bVarNumber, const WORD wVarValue);

	// ���Ϳ�����
	// (in)bCtrlWord�������֣����������ݼ���������ο�ͨѶ˵����
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD SendCtrlWord(const BYTE bCtrlWord);

	// ���������
	DWORD LiftBeamUp(void);

	// ���¾�����
	DWORD LiftBeamDown(void);

	// �������
	DWORD MotorOn(void);

	// ֹͣ���
	DWORD MotorOff(void);

	// �˳�����ģʽ
	DWORD ControlModeOff(void);

	// ����ASM����ģʽ
	DWORD AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses);

	// ���ú㹦��Ŀ��ֵ
	// (in)fPTarget���㹦��Ŀ��ֵ����λ-kW
	DWORD SetConstantPowerTarget(const float fPTarget);

	// �����㹦�ʿ���ģʽ
	DWORD ConstantPowerControlModeOn(void);

	// �������غɿ���ģʽ
	DWORD VarLoadControlModeOn(void);

	// ���ú���Ŀ��ֵ
	// (in)fFTarget������Ŀ��ֵ����λ-N
	DWORD SetConstantForceTarget(const float fFTarget);

	// ������������ģʽ
	DWORD ConstantForceControlModeOn(void);

	// ���ú��ٶ�Ŀ��ֵ
	// (in)fVTarget�����ٶ�Ŀ��ֵ����λ-km/h
	DWORD SetConstantVelocityTarget(const float fVTarget);

	// �������ٶȿ���
	DWORD ConstantVelocityControlModeOn(void);

	// �������1��ʼ�ٶ�
	// (in)fUpperVelocity����ʼ�ٶȣ���λ-km/h
	DWORD SetUpperVelocityOfTimer1(const float fUpperVelocity);

	// �������1�м��ٶ�
	// (in)fMiddleVelocity���м��ٶȣ���λ-km/h
	DWORD SetMiddleVelocityOfTimer1(const float fMiddleVelocity);

	// �������1ֹͣ�ٶ�
	// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
	DWORD SetLowerVelocityOfTimer1(const float fLowerVelocity);

	// �������1
	DWORD SetTimer1(void);

	// �ر����1
	DWORD KillTimer1(void);

	// ��ȡ���1ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	DWORD GetTimeOfTimer1(UINT *const punTime_MS);

	// ��ȡ���1�м�ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	DWORD GetMiddleTimeOfTimer1(UINT *const punTime_MS);

	// �������2��ʼ�ٶ�
	// (in)fUpperVelocity����ʼ�ٶȣ���λ-km/h
	DWORD SetUpperVelocityOfTimer2(const float fUpperVelocity);

	// �������2ֹͣ�ٶ�
	// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
	DWORD SetLowerVelocityOfTimer2(const float fLowerVelocity);

	// ���õ��������ٶ�
	// (in)fMaxVelocity; ����ٶȣ���λ-km/h
	DWORD SetMotorMaxVelocity(const float fMaxVelocity);

	// ��ȡ���������ٶ�
	// (out)fMaxVelocity; ����ٶȣ���λ-km/h
	DWORD GetMotorMaxVelocity(float *const fMaxVelocity);

	// �������2
	DWORD SetTimer2(void);

	// �ر����2
	DWORD KillTimer2(void);

	// ��ȡ���2ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	DWORD GetTimeOfTimer2(UINT *const punTime_MS);

	// ��ȡ��������
	// (out)pusDIW����������ָ�룬��λ-kg
	DWORD GetDIW(USHORT *const pusDIW);

	// д���������
	DWORD SetDIW(const USHORT usDIW);

	//////////////////////////////////////// ������������ģʽ���begin
	// д��������ģ�����ģʽ�ĳ�����׼����[kg]
	// (in)������׼����,��λ-kg
	DWORD WriteVehicleRM(const USHORT usVehicleRM);
	// д��������ģ�����ģʽ��Cv
	// �ڲ�����ת��Ϊ80km/h��Ӧ��ֵ
	// (in)Cvֵ��50km/h��Ӧ��ֵ����λ:kW
	DWORD WriteCv(const float fCv);
	// д��������ģ�����ģʽ��Bv
	// (in)Bvֵ����λ:kW
	DWORD WriteBv(const float fBv);
	// д��������ģ�����ģʽ��Av
	// (in)Avֵ����λ:kW
	DWORD WriteAv(const float fAv);
	// д��������ģ�����ģʽ��At
	// (in)Atֵ����λ:kW
	DWORD WriteAt(const float fAt);
	// д��������ģ�����ģʽ��Bt
	// (in)Btֵ����λ:kW
	DWORD WriteBt(const float fBt);
	// д��������ģ�����ģʽ��Ct
	// (in)Ctֵ����λ:kW
	DWORD WriteCt(const float fCt);
	// ������������ģ�����ģʽ
	DWORD DriveResistanceSimulationControlModeOn(void);
	//////////////////////////////////////// ������������ģʽ���end

	// ������ʻ����
	DWORD ResetDistance(void);
	// ��ȡ��ʻ����
	// (out)pusDistance:����ָ��,��λ:��(m)
	DWORD ReadDistance(USHORT *const pusDistance);
	// (out)pfDynMaxPower:�⹦�������ع���[kw]
	DWORD ReadDynMaxPower(float *const pfDynMaxPower);

	////////////////////////////////////// ���������й�begin
	// ��ȡ��ʾAp(δȥ��ʣ����)
	// (out)fAp:����,��λ:kW
	DWORD ReadApWithRemanence(float *const pfAp);
	// ��ȡ��ʾBp(δȥ��ʣ����)
	// (out)fBp:����,��λ:kW
	DWORD ReadBpWithRemanence(float *const pfBp);
	// ��ȡ��ʾCp(δȥ��ʣ����)
	// (out)fCp:����,��λ:kW
	DWORD ReadCpWithRemanence(float *const pfCp);
	// д����ʾAp(δȥ��ʣ����)
	// (in)fAp:���ʣ���λ:kW
	DWORD WriteApWithRemanence(const float fAp);
	// д����ʾBp(δȥ��ʣ����)
	// (in)fBp:���ʣ���λ:kW
	 DWORD WriteBpWithRemanence(const float fBp);
	// д����ʾCp(δȥ��ʣ����)
	// (in)fCp:���ʣ���λ:kW
	DWORD WriteCpWithRemanence(const float fCp);
	// ��ȡ��ʵAp(ȥ��ʣ������)
	// (out)fAp:����,��λ:kW
	DWORD ReadApWithoutRemanence(float *const pfAp);
	// ��ȡ��ʵBp(ȥ��ʣ������)
	// (out)fBp:����,��λ:kW
	DWORD ReadBpWithoutRemanence(float *const pfBp);
	// ��ȡ��ʵCp(ȥ��ʣ������)
	// (out)fCp:����,��λ:kW
	DWORD ReadCpWithoutRemanence(float *const pfCp);
	// д����ʵAp(ȥ��ʣ������)
	// (in)fAp:���ʣ���λ:kW
	DWORD WriteApWithoutRemanence(const float fAp);
	// д����ʵBp(ȥ��ʣ������)
	// (in)fBp:���ʣ���λ:kW
	DWORD WriteBpWithoutRemanence(const float fBp);
	// д����ʵCp(ȥ��ʣ������)
	// (in)fCp:���ʣ���λ:kW
	DWORD WriteCpWithoutRemanence(const float fCp);
	// ���ú���ز������ֱ���0.01km/h��
	DWORD SetConstantAccStep(const float fStep);
	// ��������ؿ���ģʽ
	DWORD ConstantAccControlModeOn(void);
	////////////////////////////////////// ���������й�end

	////////////////////////////////////// ��Ӧʱ�����Begin
	// ��ʼ������Ӧʱ�����ģʽ
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ResponseTimeControlModeStart(void);
	// ֹͣ������Ӧʱ�����ģʽ
	DWORD ResponseTimeControlModeEnd(void);

	// ȡ��������
	// (out)pfF����ָ�룬��λN������Ϊ��ָ��
	// (out)pfV���ٶ�ָ�룬��λ[10m/h]������Ϊ��ָ��
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength=NULL);
	////////////////////////////////////// ��Ӧʱ�����End

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������end 


public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���������������첽��begin 
	// ��Ӧͬ��GetRealTimeData
	void GetRealTimeData_Asyn(HWND hCallBackWnd=NULL);
	// ��Ӧͬ��GetStatus
	void GetStatus_Asyn(HWND hCallBackWnd=NULL);
	// ��Ӧͬ��ReadVar
	void ReadVar_Asyn(const BYTE bVarNumber, HWND hCallBackWnd=NULL);
	// ��Ӧͬ��WriteVar
	void WriteVar_Asyn(const BYTE bVarNumber, const WORD wVarValue, HWND hCallBackWnd=NULL);
	// ��Ӧͬ��SendCtrlWord
	void SendCtrlWord_Asyn(const BYTE bCtrlWord, HWND hCallBackWnd=NULL);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���������������첽��end 

public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��������begin
	// ��ȡ����ʵʱֵ
	// return������ʵʱֵ����λ-kW
	inline float GetPValue(void) const
	{
		return m_fP;
	}
	// ��ȡ��ʵʱֵ
	// return����ʵʱֵ����λ-N
	inline float GetFValue(void) const
	{
		return m_fF;
	}
	// ��ȡ�ٶ�ʵʱֵ
	// return���ٶ�ʵʱֵ����λ-km/h
	inline float GetVValue(void) const
	{
		return m_fV;
	}
	// ��ȡ��д״̬
	inline DWORD GetWRSta(void) const
	{
		return m_dwWRSta;
	}
	// ָ��״̬λ�Ƿ������ź�״̬
	// (in)bStatusBitNumber��״̬λ�ţ�����ÿ��״̬λ�ĺ�����鿴ͨѶЭ�飬ȡֵ��Χֻ����0~15
	// return�������״̬Ϊ1������true�����򣬷���false
	inline bool IsTheStatusBitSignaled(const BYTE bStatusBitNumber) const
	{
		ASSERT(bStatusBitNumber < 16);

		return m_btStatus.test(bStatusBitNumber);
	}
	// ��ȡָ������ֵ
	// (in)bVarNumber��������
	// return������ֵ
	// �����źͱ���ֵ��Ӧ�ĺ�����鿴ͨѶЭ��
	inline WORD GetVarValue(const BYTE bVarNumber) const
	{
		return m_wVarValues[bVarNumber];
	}
	//inline float GetDynMaxPower() const
	//{
	//	return m_wVarValues[68]/100.0f;
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��������end


private:
	// ��Ӧ�첽��������
	afx_msg void OnGetRealTimeData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReadVar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWriteVar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSendCtrlWord(WPARAM wParam, LPARAM lParam);

public:
	// �˳��̲߳���
	void QuitThread(void);

private:
	// ��ʼ���豸
	void InitializeInstrument(const enum InstrumentModel im);
	// ж���豸
	void UninitializeInstrument(void);

protected:
	float m_fP;
	float m_fF;
	float m_fV;
	std::bitset<16> m_btStatus;
	WORD m_wVarValues[250];
	// д�����,�첽ʱ���м����
	WORD m_wVarValuesForWrite[250];
	DWORD m_dwWRSta; // ��д״̬

private:
	// �⹦��������
	CDyn *m_pDyn;
	// �߳��˳���־
	bool m_bQuitThread;
	// ���ڲ���״̬
	DWORD m_dwStatus;
};


