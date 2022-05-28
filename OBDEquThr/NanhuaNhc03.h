/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NanhuaNhc03.h
* �ļ���ʶ��
* ժҪ���ϻ��⹦��NHC03ϵ��ͨѶ�࣬�̳���CDyn
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-12-09
*
* ��ʷ��Ϣ��
*
*
* ��ǰ�汾��1.0.6
* ���ڣ�2017-07-26
* ���ߣ�Qin
* ���������ݹ��ڲ⹦����������ʱ��Χ��˵����������
*
* ��ǰ�汾��1.0.5
* ���ڣ�2010-08-20
* ���ߣ�Qin
* ���������������ö�Ӧ�Ļ�ȡ�������ٶȵ��ú���
*
* ��ǰ�汾��1.0.4
* ���ڣ�2010-07-21
* ���ߣ�Qin
* �������������õ������ٶȵ��ú���
*
* �汾��1.0.3
* ���ڣ�2010-06-09
* ���ߣ�Cui
* ��������������Ap��Bp��Cp�Ķ�д������
* 	DWORD ReadApWithRemanence(float *const pfAp);
* 	DWORD ReadBpWithRemanence(float *const pfBp);
* 	DWORD ReadCpWithRemanence(float *const pfCp);
* 	DWORD WriteApWithRemanence(const float fAp);
* 	DWORD WriteBpWithRemanence(const float fBp);
* 	DWORD WriteCpWithRemanence(const float fCp);
* 	DWORD ReadApWithoutRemanence(float *const pfAp);
* 	DWORD ReadBpWithoutRemanence(float *const pfBp);
* 	DWORD ReadCpWithoutRemanence(float *const pfCp);
* 	DWORD WriteApWithoutRemanence(const float fAp);
* 	DWORD WriteBpWithoutRemanence(const float fBp);
* 	DWORD WriteCpWithoutRemanence(const float fCp);
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-06-04
* ���ߣ�Qin
* ������������غɿ���ģʽ���ú���
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-09
* ���ߣ�Cui
* ��������ʽ����ʹ��
*/

#pragma once

#ifndef NANHUA_NHC03_H
#define NANHUA_NHC03_H

#include "Dyn.h"

class CNanhuaNhc03 : public CDyn
{
public:
	CNanhuaNhc03(void);
	~CNanhuaNhc03(void);

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// (in)bPort�����ں�(1~255)
	// ���ش򿪽����
	// DYN_OPEN_SUCCEED
	// DYN_OPEN_FAIL
	DWORD Open(const BYTE bPort);

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// DYN_CLOSE_SUCCEED
	// DYN_CLOSE_FAIL_WIN32API
	// DYN_CLOSE_FAIL_NOOPENPORT
	DWORD Close(void);

	// �жϴ����Ƿ��
	bool IsOpen(void) const;

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
	DWORD GetRealTimeData(float *const pfP=NULL, float *const pfF=NULL, float *const pfV=NULL);

	// ȡ״̬
	// (out)pbs16Status��״̬λָ�룬��16λ������λ��Ӧ�ĺ�����ο�ͨѶ˵���飬����Ϊ��ָ�루����ͨѶ���ԣ�
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetStatus(std::bitset<16> *const pbs16Status = NULL);

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

	// ���ú���ز������ֱ���0.01km/h��
	DWORD SetConstantAccStep(const float fStep);

	// ��������ؿ���ģʽ
	DWORD ConstantAccControlModeOn(void);

	// ���ú���Ŀ��ֵ
	// (in)fFTarget������Ŀ��ֵ����λ-N
	DWORD SetConstantForceTarget(const float fFTarget);

	// ������������ģʽ
	DWORD ConstantForceControlModeOn(void);

	//���������غ�ģʽ
	DWORD VarLoadControlModeOn(void);

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
	// (out)fMaxVelocity;����ٶ� ��λ-km/h
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

	// ���û�������
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
	////////////////////////////////////// ���������й�end

	////////////////////////////////////// ��Ӧʱ���й�begin
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
	DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength =10);
	////////////////////////////////////// ��Ӧʱ���й�end

		///////////////////////////////////// �³ǵĿ�������Begin

	// ����IGBT��pwm��ռ�ձ�(%)
	// ����pwmռ�ձ�Ϊ50%��
	// XSI +0x00+0x20+0x7a+0x44+ ��У���룫C
	DWORD XC_SetConstantIGBT(const float fpwm){return DYN_NOT_SUPPORT;}

	// ����ѡ�õ���ͨ����ͨ��1��ͨ��2��ͨ��3��
	// Ĭ��ֵΪͨ��1��������Ϊͨ��1/2/3����������ֱ���õ�һ�������ֽ�����ʾѡ�õ�ͨ����
	// XSI +0+0+0+1/2/3+ ��У���룫C
	DWORD XC_SetConstantForceChannel(const int nChannel){return DYN_NOT_SUPPORT;}

	// ������
	//��Ŀǰ����ʹ�õ�ͨ���������㣺
	// XSZ+0+0+0+0+ ��У���룫C
	DWORD XC_SetConstantForceClearing(void){return DYN_NOT_SUPPORT;}

	// �������ֵ
	DWORD XC_ClearKeyValue(void){return DYN_NOT_SUPPORT;}

	// ���ù�Ͳֱ��(cm)��
	// ���Ͳֱ��Ϊ14.736cm��
	//XCS +0xa8+0xc6+0x6b+0x41+ ��У���룫C
	DWORD XC_SetConstantRollerDiameter(const int nRollerDim){return DYN_NOT_SUPPORT;}

	// ���ñ�����������(ÿȦ)
	// �������������Ϊ1000��
	//XCS +0x00+0x00+0x7a+0x44+ ��У���룫C
	DWORD XC_SetConstantEncoderPulNum(const int nEnPulNum){return DYN_NOT_SUPPORT;}

	// ����ͨ��1���궨ϵ��b[0]��force=b[0]*(ad_value-c[0])
	// ����ϵ��b[0]=1158.4��
	// XCA +0xcd+0xcc+0x90+0x44+ ��У���룫C
	DWORD XC_SetConstantForceCailFac_B1(const float fb){return DYN_NOT_SUPPORT;}

	//����ͨ��1���궨ϵ��c[0]��	
	// ����ϵ��c[0]=0.01��
	//XCB +0x0a+0xd7+0x23+0x3c+ ��У���룫C
	DWORD XC_SetConstantForceCailFac_C1(const float fc){return DYN_NOT_SUPPORT;}

	//����ͨ��2���궨ϵ��b[1]��	
	//����ϵ��b[1]=1158.4��
	//XCC +0xcd+0xcc+0x90+0x44+ ��У���룫C
	DWORD XC_SetConstantForceCailFac_B2(const float fb){return DYN_NOT_SUPPORT;}

	//����ͨ��2���궨ϵ��c[1]��	
	//����ϵ��c[1]=0.01��
	//XCD +0x0a+0xd7+0x23+0x3c + ��У���룫C
	DWORD XC_SetConstantForceCailFac_C2(const float fc){return DYN_NOT_SUPPORT;}

	//����ͨ��3���궨ϵ��b[2]��	
	//����ϵ��b[2]=1158.4��
	//XCE +0xcd+0xcc+0x90+0x44 + ��У���룫C
	DWORD XC_SetConstantForceCailFac_B3(const float fb){return DYN_NOT_SUPPORT;}

	//����ͨ��3���궨ϵ��c[2]��	
	//����ϵ��c[2]=0.01��
	//XCF +0x0a+0xd7+0x23+0x3c  +��У���룫C
	DWORD XC_SetConstantForceCailFac_C3(const float fc){return DYN_NOT_SUPPORT;}

	//���ú��ٿ���PID���Ʊ���ϵ��kp��	
	//����ϵ��c[2]=0.01��
	//XCH +0x0a+0xd7+0x23+0x3c  +��У���룫C
	DWORD XC_SetConstantSpeedControl_P(const float fkp){return DYN_NOT_SUPPORT;}

	//���ú��ٿ���PID���ƻ���ϵ��ki��	
	//����ϵ��c[2]=0.01��
	//XCI +0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantSpeedControl_I(const float fki){return DYN_NOT_SUPPORT;}

	//���ú��ٿ���PID����΢��ϵ��kd��	
	//����ϵ��c[2]=0.01��
	//XCJ+0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantSpeedControl_D(const float fkd){return DYN_NOT_SUPPORT;}

	//���ú�������PID���Ʊ���ϵ��kp_force��	
	//����ϵ��c[2]=0.01��
	//XCK +0x0a+0xd7+0x23+0x3c  +��У���룫C
	DWORD XC_SetConstantForceControl_P(const float fkp){return DYN_NOT_SUPPORT;}

	//���ú�������PID���Ʊ���ϵ�� ki_force��	
	//����ϵ��c[2]=0.01��
	//XCL +0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantForceControl_I(const float fki){return DYN_NOT_SUPPORT;}

	//���ú�������PID���Ʊ���ϵ�� kd_force��	
	//����ϵ��c[2]=0.01��
	//XCM +0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantForceControl_D(const float fkd){return DYN_NOT_SUPPORT;}

	//�������ٶȿ���	XBS + 0+0+0+0+ ��У���� �� C
	//������Ť�����	XBF + 0+0+0+0+ ��У���� �� C
	//�����㹦�ʿ���	XBP + 0+0+ 0+0+ ��У���� �� C

	//��������趨pwmֵ	XBI + 0+0+ 0+0+ ��У���� �� C
	DWORD XC_StartSetPWN(const float fPWN){return DYN_NOT_SUPPORT;}

	//ֹͣ����,pwm���0	XBS + 0+0+0+0+ ��У���� �� C
	DWORD XC_StopPWN(void){return DYN_NOT_SUPPORT;}

	//�𶯵��	XEB + 0+0+0+0+ ��У���� �� C
	//ֹͣ���	XES + 0+0+0+0+ ��У���� �� C
	//��ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH1: XEO + 0+0+0+1+ ��У���� �� C
	DWORD XC_OpenRelay(const int nLH){return DYN_NOT_SUPPORT;}

	//�ر�ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH3: XEF + 0+0+0+3+ ��У���� �� C
	DWORD XC_CloseRelay(const int nLH){return DYN_NOT_SUPPORT;}

	//����������	XJU+ 0+0+0+0+ ��У���� �� C
	//�������½�	XJD +0+0+0+0+ ��У���� �� C
	//����궨״̬	XDF +0+0+0+0+ ��У���� �� C
	DWORD XC_EnterCaliSta(void){return DYN_NOT_SUPPORT;}

	//�˳��궨״̬	 XDS +0+0+0+0+ ��У���� �� C
	DWORD XC_ExitCaliSta(void){return DYN_NOT_SUPPORT;}

	//��ȡ��������	 XRT +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadEnvPar(float *const pfET=NULL, float *const pfAP=NULL, float *const pfRH=NULL){return DYN_NOT_SUPPORT;}

	//��ȡ���궨ϵ��b,c	XRF +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadForCalFac(float *const pfb=NULL, float *const pfc=NULL, float *const pfb1=NULL, float *const pfc1=NULL, float *const pfb2=NULL, float *const pfc2=NULL){return DYN_NOT_SUPPORT;}

	//��ȡ��ͨ��	XRC +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadForCha(int *const pnCha=NULL){return DYN_NOT_SUPPORT;}

	//��ȡ����PIDϵ��	XRs +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadConSpeed(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL){return DYN_NOT_SUPPORT;}

	//��ȡ����PIDϵ��	XRf +0+0+0+0+ ��У���� �� C	
	DWORD XC_ReadConForce(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL){return DYN_NOT_SUPPORT;}

	//��ȡ��Ͳֱ���ͱ���������	 XRS +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadRollerAndEncoder(int *const pnDia=NULL, int *const pnPul=NULL){return DYN_NOT_SUPPORT;}

	//��3��ͨ���ı궨ϵ���̻���ROM��	 XMA +0+0+0+0+ ��У���� �� C
	DWORD XC_SolCalFac(void){return DYN_NOT_SUPPORT;}

	//����Ͳֱ���ͱ���������̻���ROM��	XMS +0+0+0+0+ ��У���� �� C
	DWORD XC_SolRollerAndEncoder(void){return DYN_NOT_SUPPORT;}

	//����ͨ����ѡ��̻���ROM��	XMC +0+0+0+0+ ��У���� �� C
	DWORD XC_SolForCha(void){return DYN_NOT_SUPPORT;}

	//�����ٿ��Ƶ�PIDϵ���̻���ROM��	XMD + 0+0+0+0+ ��У���� �� C
	DWORD XC_SolConSheepPID(void){return DYN_NOT_SUPPORT;}

	//���������Ƶ�PIDϵ���̻���ROM��	XME + 0+0+0+0+ ��У���� �� C
	DWORD XC_SolConForcePID(void){return DYN_NOT_SUPPORT;}

	///////////////////////////////////// �³ǵĿ�������End

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������end 

private:
	// д����������
	// д����Ӧ�������ȡ��Ӧ�ķ�������
	// ��������Ҫ���ֱ��д��Ͷ�ȡ�����ݽ���У�����֤
	// (in)pbWriteBuffer��Ҫд�����������ָ�룬����Ϊ��ָ��
	// (in)dwWriteLength��Ҫд������ݳ���
	// (out)pbReadBuffer��Ҫ��ȡ����������ָ�룬����Ϊ��ָ��
	// (in)dwReadLength��Ҫ��ȡ�����ݳ���
	// (in)bNeedWriteChkSum��true����ҪдУ�飬false������Ҫ
	// (in)bNeedReadChkSum��true����Ҫ��У�飬false������Ҫ
	// ���أ�����ִ�н����
	// NHC_WR_FUNC_SUCCEED
	// NHC_WR_FUNC_PORT_NOT_OPEN
	// NHC_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// NHC_WR_FUNC_EQUIP_RETURE_0X15
	// NHC_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteAndReadData(BYTE *const pbWriteBuffer, const DWORD dwWriteLength, BYTE *const pbReadBuffer, const DWORD dwReadLength, const bool bNeedWriteChkSum=true, const bool bNeedReadChkSum=true);

	// ����У���
	// ��ӣ�Ȼ��ȡ����һ
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	BYTE CheckSum(const BYTE *const p, const UINT un);

private:
	// ת��float��word������Ap��Bp��Cp��д��
	static void ConvertFloatToWord(const float fSource, WORD *const pwDestination);
	static void ConvertWordToFloat(const WORD wSource, float *const pfDestination);

private:
	// ���ھ��
	HANDLE m_hComm;
	//�ͻ���Ϣ�ٽ籣����
	CRITICAL_SECTION cInfoSection;
};

#endif