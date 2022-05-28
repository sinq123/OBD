/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�Dyn.h
* �ļ���ʶ��
* ժҪ���⹦��������
*		�Դ����⺯����ʽ����⹦�������ӿ�
*
* ��ǰ�汾��1.0
* ���ߣ�Cui
* ������ڣ�2009-12-09
*
* ��ʷ��Ϣ��
*
* �汾��1.0.5
* ���ڣ�2014-06-02
* ���ߣ�Qin
* ������������Ժ���ٿ���ģʽ

* ��ǰ�汾��1.0.4
* ���ڣ�2010-08-20
* ���ߣ�Qin
* ���������������ö�Ӧ�Ļ�ȡ�������ٶȵ��ú���
*
* ��ǰ�汾��1.0.3
* ���ڣ�2010-07-21
* ���ߣ�Qin
* �������������õ������ٶȵ��ýӿ�
*
* ��ǰ�汾��1.0.2
* ������ڣ�2010-06-04
* ���ߣ�Qin
* ������������غɿ���ģʽ���ýӿ�
*
* ��ǰ�汾��1.0.1
* ������ڣ�2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��

* ��ǰ�汾��1.0.0
* ������ڣ�2009-12-09
* ���ߣ�Cui
* ��������ʽ����ʹ��
*/

#pragma once


#ifndef DYN_H
#define DYN_H

// ����ִ�н��
#define DYN_OPEN_SUCCEED							0x00  // �򿪴��ڳɹ�
#define DYN_OPEN_FAIL								0x01  // �򿪴���ʧ��
#define DYN_CLOSE_SUCCEED							0x00  // �رմ��ڳɹ�
#define DYN_CLOSE_FAIL_WIN32API						0x01  // �رմ���ʧ�ܣ�ִ��win32 API(CloseHandle)ʧ��
#define DYN_CLOSE_FAIL_NOOPENPORT					0x02  // �رմ���ʧ�ܣ�û�п��ԹرյĴ���
#define DYN_WR_SUCCEED								0x00  // д���ɹ�
#define DYN_WR_PORT_NOT_OPEN						0x01  // д��ʧ�ܣ�����δ��
#define DYN_WR_WRITE_SPECIFIED_DATA_FAIL			0x02  // д��ʧ�ܣ�дָ������ʧ��
#define DYN_WR_READ_SPECIFIED_DATA_FAIL				0x03  // д��ʧ�ܣ���ָ������ʧ��
#define DYN_WR_EQUIP_RETURE_0X15					0x04  // д��ʧ�ܣ��������ش�����루0x15��
#define DYN_WR_CHECK_SUM_FAIL						0x05  // д��ʧ�ܣ���������У���ʧ��
#define DYN_NOT_SUPPORT								0x06  // ��֧�ֵĲ���
#define DYN_WR_UNKNOWN_ERROR						0xFF  // д��ʧ�ܣ�δ֪����

#include <bitset>

class CDyn
{
public:
	CDyn(void);
	virtual ~CDyn(void);

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ڲ���start
	// �򿪴���
	// �򿪴���ǰ���Զ�ʶ���Ƿ��Ѿ������Ѿ��򿪵Ĵ�����Դ������У��ڴ�֮ǰ���ȹر���ǰ�򿪵Ĵ�����Դ
	// (in)bPort�����ں�(1~255)
	// ���ش򿪽����
	// DYN_OPEN_SUCCEED
	// DYN_OPEN_FAIL
	virtual DWORD Open(const BYTE bPort) = 0;

	// �رմ���
	// �Զ�ʶ���Ƿ��п��Բ��رյĴ�����Դ
	// ���عرս����
	// DYN_CLOSE_SUCCEED
	// DYN_CLOSE_FAIL_WIN32API
	// DYN_CLOSE_FAIL_NOOPENPORT
	virtual DWORD Close(void) = 0;

	// �жϴ����Ƿ��
	virtual bool IsOpen(void) const = 0;

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
	virtual DWORD GetRealTimeData(float *const pfP=NULL, float *const pfF=NULL, float *const pfV=NULL) = 0;

	// ȡ״̬
	// (out)pbs16Status��״̬λָ�룬��16λ������λ��Ӧ�ĺ�����ο�ͨѶ˵���飬����Ϊ��ָ�루����ͨѶ���ԣ�
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD GetStatus(std::bitset<16> *const pbs16Status = NULL) = 0;

	// ������
	// (in)bVarNumber��Ҫ��ȡ�ı����ţ�����������Ӧ�ı�������ο�ͨѶ˵����
	// (out)pwVarValue����ȡ���ı���ֵ��������ο�ͨѶ˵���飬����Ϊ��ָ��
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD ReadVar(const BYTE bVarNumber, WORD *const pwVarValue) = 0;

	// д����
	// (in)bVarNumber��Ҫд��ı����ţ�����������Ӧ�ı�������ο�ͨѶ˵����
	// (in)pwVarValue��Ҫд��ı���ֵ�����ݺ�����ο�ͨѶ˵����
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD WriteVar(const BYTE bVarNumber, const WORD wVarValue) = 0;

	// ���Ϳ�����
	// (in)bCtrlWord�������֣����������ݼ���������ο�ͨѶ˵����
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD SendCtrlWord(const BYTE bCtrlWord) = 0;

	// ���������
	virtual DWORD LiftBeamUp(void) = 0;

	// ���¾�����
	virtual DWORD LiftBeamDown(void) = 0;

	// �������
	virtual DWORD MotorOn(void) = 0;

	// ֹͣ���
	virtual DWORD MotorOff(void) = 0;

	// �˳�����ģʽ
	virtual DWORD ControlModeOff(void) = 0;

	// ����ASM����ģʽ
	virtual DWORD AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses) = 0;

	// ���ú㹦��Ŀ��ֵ
	// (in)fPTarget���㹦��Ŀ��ֵ����λ-kW
	virtual DWORD SetConstantPowerTarget(const float fPTarget) = 0;

	// �����㹦�ʿ���ģʽ
	virtual DWORD ConstantPowerControlModeOn(void) = 0;

	// �������غɿ���ģʽ
	virtual DWORD VarLoadControlModeOn(void) = 0;

	// ���ú���Ŀ��ֵ
	// (in)fFTarget������Ŀ��ֵ����λ-N
	virtual DWORD SetConstantForceTarget(const float fFTarget) = 0;

	// ������������ģʽ
	virtual DWORD ConstantForceControlModeOn(void) = 0;

	// ���ú��ٶ�Ŀ��ֵ
	// (in)fVTarget�����ٶ�Ŀ��ֵ����λ-km/h
	virtual DWORD SetConstantVelocityTarget(const float fVTarget) = 0;

	// �������ٶȿ���
	virtual DWORD ConstantVelocityControlModeOn(void) = 0;

	// �������1��ʼ�ٶ�
	// (in)fUpperVelocity����ʼ�ٶȣ���λ-km/h
	virtual DWORD SetUpperVelocityOfTimer1(const float fUpperVelocity) = 0;

	// �������1�м��ٶ�
	// (in)fMiddleVelocity���м��ٶȣ���λ-km/h
	virtual DWORD SetMiddleVelocityOfTimer1(const float fMiddleVelocity) = 0;

	// �������1ֹͣ�ٶ�
	// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
	virtual DWORD SetLowerVelocityOfTimer1(const float fLowerVelocity) = 0;

	// ���ò⹦���������ٶ�
	// (in)fMaxVelocity; ����ٶȣ���λ-km/h
	virtual DWORD SetMotorMaxVelocity(const float fMaxVelocity) = 0;

	// ��ȡ�⹦���������ٶ�
	// (out)fMaxVelocity; ����ٶȣ���λ-km/h
    virtual DWORD GetMotorMaxVelocity(float *const fMaxVelocity) = 0;

	// �������1
	virtual DWORD SetTimer1(void) = 0;

	// �ر����1
	virtual DWORD KillTimer1(void) = 0;

	// ��ȡ���1ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	virtual DWORD GetTimeOfTimer1(UINT *const punTime_MS) = 0;

	// ��ȡ���1�м�ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	virtual DWORD GetMiddleTimeOfTimer1(UINT *const punTime_MS) = 0;

	// �������2��ʼ�ٶ�
	// (in)fUpperVelocity����ʼ�ٶȣ���λ-km/h
	virtual DWORD SetUpperVelocityOfTimer2(const float fUpperVelocity) = 0;

	// �������2ֹͣ�ٶ�
	// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
	virtual DWORD SetLowerVelocityOfTimer2(const float fLowerVelocity) = 0;

	// �������2
	virtual DWORD SetTimer2(void) = 0;

	// �ر����2
	virtual DWORD KillTimer2(void) = 0;

	// ��ȡ���2ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	virtual DWORD GetTimeOfTimer2(UINT *const punTime_MS) = 0;

	// ��ȡ��������
	// (out)pusDIW����������ָ�룬��λ-kg
	virtual DWORD GetDIW(USHORT *const pusDIW) = 0;

	// д���������
	virtual DWORD SetDIW(const USHORT usDIW) = 0;

	//////////////////////////////////////// ������������ģʽ���begin
	// д��������ģ�����ģʽ�ĳ�����׼����[kg]
	// (in)������׼����,��λ-kg
	virtual DWORD WriteVehicleRM(const USHORT usVehicleRM) = 0;
	// д��������ģ�����ģʽ��Cv
	// �ڲ�����ת��Ϊ80km/h��Ӧ��ֵ
	// (in)Cvֵ��50km/h��Ӧ��ֵ����λ:kW
	virtual DWORD WriteCv(const float fCv) = 0;
	// д��������ģ�����ģʽ��Bv
	// (in)Bvֵ����λ:kW
	virtual DWORD WriteBv(const float fBv) = 0;
	// д��������ģ�����ģʽ��Av
	// (in)Avֵ����λ:kW
	virtual DWORD WriteAv(const float fAv) = 0;
	// д��������ģ�����ģʽ��At
	// (in)Atֵ����λ:kW
	virtual DWORD WriteAt(const float fAt) = 0;
	// д��������ģ�����ģʽ��Bt
	// (in)Btֵ����λ:kW
	virtual DWORD WriteBt(const float fBt) = 0;
	// д��������ģ�����ģʽ��Ct
	// (in)Ctֵ����λ:kW
	virtual DWORD WriteCt(const float fCt) = 0;
	// ������������ģ�����ģʽ
	virtual DWORD DriveResistanceSimulationControlModeOn(void) = 0;
	//////////////////////////////////////// ������������ģʽ���end

	// ������ʻ����
	virtual DWORD ResetDistance(void) = 0;
	// ��ȡ��ʻ����
	// (out)pusDistance:����ָ��,��λ:��(m)
	virtual DWORD ReadDistance(USHORT *const pusDistance) = 0;


	////////////////////////////////////// ���������й�begin
	// ��ȡ��ʾAp(δȥ��ʣ����)
	// (out)fAp:����,��λ:kW
	virtual DWORD ReadApWithRemanence(float *const pfAp) = 0;
	// ��ȡ��ʾBp(δȥ��ʣ����)
	// (out)fBp:����,��λ:kW
	virtual DWORD ReadBpWithRemanence(float *const pfBp) = 0;
	// ��ȡ��ʾCp(δȥ��ʣ����)
	// (out)fCp:����,��λ:kW
	virtual DWORD ReadCpWithRemanence(float *const pfCp) = 0;
	// д����ʾAp(δȥ��ʣ����)
	// (in)fAp:���ʣ���λ:kW
	virtual DWORD WriteApWithRemanence(const float fAp) = 0;
	// д����ʾBp(δȥ��ʣ����)
	// (in)fBp:���ʣ���λ:kW
	virtual DWORD WriteBpWithRemanence(const float fBp) = 0;
	// д����ʾCp(δȥ��ʣ����)
	// (in)fCp:���ʣ���λ:kW
	virtual DWORD WriteCpWithRemanence(const float fCp) = 0;
	// ��ȡ��ʵAp(ȥ��ʣ������)
	// (out)fAp:����,��λ:kW
	virtual DWORD ReadApWithoutRemanence(float *const pfAp) = 0;
	// ��ȡ��ʵBp(ȥ��ʣ������)
	// (out)fBp:����,��λ:kW
	virtual DWORD ReadBpWithoutRemanence(float *const pfBp) = 0;
	// ��ȡ��ʵCp(ȥ��ʣ������)
	// (out)fCp:����,��λ:kW
	virtual DWORD ReadCpWithoutRemanence(float *const pfCp) = 0;
	// д����ʵAp(ȥ��ʣ������)
	// (in)fAp:���ʣ���λ:kW
	virtual DWORD WriteApWithoutRemanence(const float fAp) = 0;
	// д����ʵBp(ȥ��ʣ������)
	// (in)fBp:���ʣ���λ:kW
	virtual DWORD WriteBpWithoutRemanence(const float fBp) = 0;
	// д����ʵCp(ȥ��ʣ������)
	// (in)fCp:���ʣ���λ:kW
	virtual DWORD WriteCpWithoutRemanence(const float fCp) = 0;
	// ���ú���ز������ֱ���0.01km/h��
	virtual DWORD SetConstantAccStep(const float fStep) = 0;
	// ��������ؿ���ģʽ
	virtual DWORD ConstantAccControlModeOn(void) = 0;
	////////////////////////////////////// ���������й�end

	////////////////////////////////////// ��Ӧʱ�����Begin
	// ��ʼ������Ӧʱ�����ģʽ
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD ResponseTimeControlModeStart(void)=0;
	// ֹͣ������Ӧʱ�����ģʽ
	virtual DWORD ResponseTimeControlModeEnd(void)=0;

	// ȡ��������
	// (out)pfF����ָ�룬��λN������Ϊ��ָ��
	// (out)pfV���ٶ�ָ�룬��λ[10m/h]������Ϊ��ָ��
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	virtual DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength=NULL)=0;
	////////////////////////////////////// ��Ӧʱ�����End

	///////////////////////////////////// �³ǵĿ�������Begin

	// ����IGBT��pwm��ռ�ձ�(%)
	// ����pwmռ�ձ�Ϊ50%��
	// XSI +0x00+0x20+0x7a+0x44+ ��У���룫C
	virtual DWORD XC_SetConstantIGBT(const float fpwm) = 0;

	// ����ѡ�õ���ͨ����ͨ��1��ͨ��2��ͨ��3��
	// Ĭ��ֵΪͨ��1��������Ϊͨ��1/2/3����������ֱ���õ�һ�������ֽ�����ʾѡ�õ�ͨ����
	// XSI +0+0+0+1/2/3+ ��У���룫C
	virtual DWORD XC_SetConstantForceChannel(const int nChannel) = 0;

	// ������
	//��Ŀǰ����ʹ�õ�ͨ���������㣺
	// XSZ+0+0+0+0+ ��У���룫C
	virtual DWORD XC_SetConstantForceClearing(void) = 0;

	// �������ֵ
	virtual DWORD XC_ClearKeyValue(void) = 0;

	// ���ù�Ͳֱ��(cm)��
	// ���Ͳֱ��Ϊ14.736cm��
	//XCS +0xa8+0xc6+0x6b+0x41+ ��У���룫C
	virtual DWORD XC_SetConstantRollerDiameter(const int nRollerDim) = 0;

	// ���ñ�����������(ÿȦ)
	// �������������Ϊ1000��
	//XCS +0x00+0x00+0x7a+0x44+ ��У���룫C
	virtual DWORD XC_SetConstantEncoderPulNum(const int nEnPulNum) = 0;

	// ����ͨ��1���궨ϵ��b[0]��force=b[0]*(ad_value-c[0])
	// ����ϵ��b[0]=1158.4��
	// XCA +0xcd+0xcc+0x90+0x44+ ��У���룫C
	virtual DWORD XC_SetConstantForceCailFac_B1(const float fb) = 0;

	//����ͨ��1���궨ϵ��c[0]��	
	// ����ϵ��c[0]=0.01��
	//XCB +0x0a+0xd7+0x23+0x3c+ ��У���룫C
	virtual DWORD XC_SetConstantForceCailFac_C1(const float fc) = 0;

	//����ͨ��2���궨ϵ��b[1]��	
	//����ϵ��b[1]=1158.4��
	//XCC +0xcd+0xcc+0x90+0x44+ ��У���룫C
	virtual DWORD XC_SetConstantForceCailFac_B2(const float fb) = 0;

	//����ͨ��2���궨ϵ��c[1]��	
	//����ϵ��c[1]=0.01��
	//XCD +0x0a+0xd7+0x23+0x3c + ��У���룫C
	virtual DWORD XC_SetConstantForceCailFac_C2(const float fc) = 0;

	//����ͨ��3���궨ϵ��b[2]��	
	//����ϵ��b[2]=1158.4��
	//XCE +0xcd+0xcc+0x90+0x44 + ��У���룫C
	virtual DWORD XC_SetConstantForceCailFac_B3(const float fb) = 0;

	//����ͨ��3���궨ϵ��c[2]��	
	//����ϵ��c[2]=0.01��
	//XCF +0x0a+0xd7+0x23+0x3c  +��У���룫C
	virtual DWORD XC_SetConstantForceCailFac_C3(const float fc) = 0;

	//���ú��ٿ���PID���Ʊ���ϵ��kp��	
	//����ϵ��c[2]=0.01��
	//XCH +0x0a+0xd7+0x23+0x3c  +��У���룫C
	virtual DWORD XC_SetConstantSpeedControl_P(const float fkp) = 0;

	//���ú��ٿ���PID���ƻ���ϵ��ki��	
	//����ϵ��c[2]=0.01��
	//XCI +0x0a+0xd7+0x23+0x3c +��У���룫C
	virtual DWORD XC_SetConstantSpeedControl_I(const float fki) = 0;

	//���ú��ٿ���PID����΢��ϵ��kd��	
	//����ϵ��c[2]=0.01��
	//XCJ+0x0a+0xd7+0x23+0x3c +��У���룫C
	virtual DWORD XC_SetConstantSpeedControl_D(const float fkd) = 0;

	//���ú�������PID���Ʊ���ϵ��kp_force��	
	//����ϵ��c[2]=0.01��
	//XCK +0x0a+0xd7+0x23+0x3c  +��У���룫C
	virtual DWORD XC_SetConstantForceControl_P(const float fkp) = 0;

	//���ú�������PID���Ʊ���ϵ�� ki_force��	
	//����ϵ��c[2]=0.01��
	//XCL +0x0a+0xd7+0x23+0x3c +��У���룫C
	virtual DWORD XC_SetConstantForceControl_I(const float fki) = 0;

	//���ú�������PID���Ʊ���ϵ�� kd_force��	
	//����ϵ��c[2]=0.01��
	//XCM +0x0a+0xd7+0x23+0x3c +��У���룫C
	virtual DWORD XC_SetConstantForceControl_D(const float fkd) = 0;

	//�������ٶȿ���	XBS + 0+0+0+0+ ��У���� �� C
	//������Ť�����	XBF + 0+0+0+0+ ��У���� �� C
	//�����㹦�ʿ���	XBP + 0+0+ 0+0+ ��У���� �� C

	//��������趨pwmֵ	XBI + 0+0+ 0+0+ ��У���� �� C
	virtual DWORD XC_StartSetPWN(const float fPWN) = 0;

	//ֹͣ����,pwm���0	XBS + 0+0+0+0+ ��У���� �� C
	virtual DWORD XC_StopPWN(void) = 0;

	//�𶯵��	XEB + 0+0+0+0+ ��У���� �� C
	//ֹͣ���	XES + 0+0+0+0+ ��У���� �� C
	//��ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH1: XEO + 0+0+0+1+ ��У���� �� C
	virtual DWORD XC_OpenRelay(const int nLH) = 0;

	//�ر�ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH3: XEF + 0+0+0+3+ ��У���� �� C
	virtual DWORD XC_CloseRelay(const int nLH) = 0;

	//����������	XJU+ 0+0+0+0+ ��У���� �� C
	//�������½�	XJD +0+0+0+0+ ��У���� �� C
	//����궨״̬	XDF +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_EnterCaliSta(void) = 0;

	//�˳��궨״̬	 XDS +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_ExitCaliSta(void) = 0;

	//��ȡ��������	 XRT +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_ReadEnvPar(float *const pfET=NULL, float *const pfAP=NULL, float *const pfRH=NULL) = 0;

	//��ȡ���궨ϵ��b,c	XRF +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_ReadForCalFac(float *const pfb=NULL, float *const pfc=NULL, float *const pfb1=NULL, float *const pfc1=NULL, float *const pfb2=NULL, float *const pfc2=NULL) = 0;

	//��ȡ��ͨ��	XRC +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_ReadForCha(int *const pnCha=NULL) = 0;

	//��ȡ����PIDϵ��	XRs +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_ReadConSpeed(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL) = 0;

	//��ȡ����PIDϵ��	XRf +0+0+0+0+ ��У���� �� C	
	virtual DWORD XC_ReadConForce(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL) = 0;

	//��ȡ��Ͳֱ���ͱ���������	 XRS +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_ReadRollerAndEncoder(int *const pnDia=NULL, int *const pnPul=NULL) = 0;

	//��3��ͨ���ı궨ϵ���̻���ROM��	 XMA +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_SolCalFac(void) = 0;

	//����Ͳֱ���ͱ���������̻���ROM��	XMS +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_SolRollerAndEncoder(void) = 0;

	//����ͨ����ѡ��̻���ROM��	XMC +0+0+0+0+ ��У���� �� C
	virtual DWORD XC_SolForCha(void) = 0;

	//�����ٿ��Ƶ�PIDϵ���̻���ROM��	XMD + 0+0+0+0+ ��У���� �� C
	virtual DWORD XC_SolConSheepPID(void) = 0;

	//���������Ƶ�PIDϵ���̻���ROM��	XME + 0+0+0+0+ ��У���� �� C
	virtual DWORD XC_SolConForcePID(void) = 0;

	///////////////////////////////////// �³ǵĿ�������End

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������end 
};

#endif
