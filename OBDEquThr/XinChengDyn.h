
#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\release\\NHWin32Lib.lib")
#endif

#pragma once

#ifndef XinChengDyn_H
#define XinChengDyn_H

#include "Dyn.h"
#include <stdint.h>

class CXinChengDyn: public CDyn
{
public:
	CXinChengDyn(void);
	~CXinChengDyn(void);


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
	DWORD GetStatus(std::bitset<16> *const pbs16Status = NULL){return DYN_NOT_SUPPORT;}

	// ������
	// (in)bVarNumber��Ҫ��ȡ�ı����ţ�����������Ӧ�ı�������ο�ͨѶ˵����
	// (out)pwVarValue����ȡ���ı���ֵ��������ο�ͨѶ˵���飬����Ϊ��ָ��
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ReadVar(const BYTE bVarNumber, WORD *const pwVarValue){return DYN_NOT_SUPPORT;}

	// д����
	// (in)bVarNumber��Ҫд��ı����ţ�����������Ӧ�ı�������ο�ͨѶ˵����
	// (in)pwVarValue��Ҫд��ı���ֵ�����ݺ�����ο�ͨѶ˵����
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD WriteVar(const BYTE bVarNumber, const WORD wVarValue){return DYN_NOT_SUPPORT;}

	// ���Ϳ�����
	// (in)bCtrlWord�������֣����������ݼ���������ο�ͨѶ˵����
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD SendCtrlWord(const BYTE bCtrlWord){return DYN_NOT_SUPPORT;}

	// ���������
	DWORD LiftBeamUp(void);

	// ���¾�����
	DWORD LiftBeamDown(void);

	// �������
	DWORD MotorOn(void);

	// ֹͣ���
	DWORD MotorOff(void);

	// �˳�����ģʽ
	DWORD ControlModeOff(void){return DYN_NOT_SUPPORT;}

	// ����ASM����ģʽ
	DWORD AsmRegulatorOn(const float fTargetPower, const float fAsmSpeed, const float fTireLosses){return DYN_NOT_SUPPORT;}

	// ���ú㹦��Ŀ��ֵ
	// (in)fPTarget���㹦��Ŀ��ֵ����λ-kW
	DWORD SetConstantPowerTarget(const float fPTarget);

	// �����㹦�ʿ���ģʽ
	DWORD ConstantPowerControlModeOn(void);

	// �������غɿ���ģʽ
	DWORD VarLoadControlModeOn(void){return DYN_NOT_SUPPORT;}

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
	DWORD SetUpperVelocityOfTimer1(const float fUpperVelocity){return DYN_NOT_SUPPORT;}

	// �������1�м��ٶ�
	// (in)fMiddleVelocity���м��ٶȣ���λ-km/h
	DWORD SetMiddleVelocityOfTimer1(const float fMiddleVelocity){return DYN_NOT_SUPPORT;}

	// �������1ֹͣ�ٶ�
	// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
	DWORD SetLowerVelocityOfTimer1(const float fLowerVelocity){return DYN_NOT_SUPPORT;}

	// ���ò⹦���������ٶ�
	// (in)fMaxVelocity; ����ٶȣ���λ-km/h
	DWORD SetMotorMaxVelocity(const float fMaxVelocity){return DYN_NOT_SUPPORT;}

	// ��ȡ�⹦���������ٶ�
	// (out)fMaxVelocity; ����ٶȣ���λ-km/h
    DWORD GetMotorMaxVelocity(float *const fMaxVelocity){return DYN_NOT_SUPPORT;}

	// �������1
	DWORD SetTimer1(void){return DYN_NOT_SUPPORT;}

	// �ر����1
	DWORD KillTimer1(void){return DYN_NOT_SUPPORT;}

	// ��ȡ���1ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	DWORD GetTimeOfTimer1(UINT *const punTime_MS){return DYN_NOT_SUPPORT;}

	// ��ȡ���1�м�ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	DWORD GetMiddleTimeOfTimer1(UINT *const punTime_MS){return DYN_NOT_SUPPORT;}

	// �������2��ʼ�ٶ�
	// (in)fUpperVelocity����ʼ�ٶȣ���λ-km/h
	DWORD SetUpperVelocityOfTimer2(const float fUpperVelocity){return DYN_NOT_SUPPORT;}

	// �������2ֹͣ�ٶ�
	// (in)fLowerVelocity��ֹͣ�ٶȣ���λ-km/h
	DWORD SetLowerVelocityOfTimer2(const float fLowerVelocity){return DYN_NOT_SUPPORT;}

	// �������2
	DWORD SetTimer2(void){return DYN_NOT_SUPPORT;}

	// �ر����2
	DWORD KillTimer2(void){return DYN_NOT_SUPPORT;}

	// ��ȡ���2ʱ��
	// (out)punTime_MS;ʱ��ָ�룬��λ-ms
	DWORD GetTimeOfTimer2(UINT *const punTime_MS){return DYN_NOT_SUPPORT;}

	// ��ȡ��������
	// (out)pusDIW����������ָ�룬��λ-kg
	DWORD GetDIW(USHORT *const pusDIW){return DYN_NOT_SUPPORT;}

	// д���������
	DWORD SetDIW(const USHORT usDIW){return DYN_NOT_SUPPORT;}

	//////////////////////////////////////// ������������ģʽ���begin
	// д��������ģ�����ģʽ�ĳ�����׼����[kg]
	// (in)������׼����,��λ-kg
	DWORD WriteVehicleRM(const USHORT usVehicleRM){return DYN_NOT_SUPPORT;}
	// д��������ģ�����ģʽ��Cv
	// �ڲ�����ת��Ϊ80km/h��Ӧ��ֵ
	// (in)Cvֵ��50km/h��Ӧ��ֵ����λ:kW
	DWORD WriteCv(const float fCv){return DYN_NOT_SUPPORT;}
	// д��������ģ�����ģʽ��Bv
	// (in)Bvֵ����λ:kW
	DWORD WriteBv(const float fBv){return DYN_NOT_SUPPORT;}
	// д��������ģ�����ģʽ��Av
	// (in)Avֵ����λ:kW
	DWORD WriteAv(const float fAv){return DYN_NOT_SUPPORT;}
	// д��������ģ�����ģʽ��At
	// (in)Atֵ����λ:kW
	DWORD WriteAt(const float fAt){return DYN_NOT_SUPPORT;}
	// д��������ģ�����ģʽ��Bt
	// (in)Btֵ����λ:kW
	DWORD WriteBt(const float fBt){return DYN_NOT_SUPPORT;}
	// д��������ģ�����ģʽ��Ct
	// (in)Ctֵ����λ:kW
	DWORD WriteCt(const float fCt){return DYN_NOT_SUPPORT;}
	// ������������ģ�����ģʽ
	DWORD DriveResistanceSimulationControlModeOn(void){return DYN_NOT_SUPPORT;}
	//////////////////////////////////////// ������������ģʽ���end

	// ������ʻ����
	DWORD ResetDistance(void){return DYN_NOT_SUPPORT;}
	// ��ȡ��ʻ����
	// (out)pusDistance:����ָ��,��λ:��(m)
	DWORD ReadDistance(USHORT *const pusDistance){return DYN_NOT_SUPPORT;}


	////////////////////////////////////// ���������й�begin
	// ��ȡ��ʾAp(δȥ��ʣ����)
	// (out)fAp:����,��λ:kW
	DWORD ReadApWithRemanence(float *const pfAp){return DYN_NOT_SUPPORT;}
	// ��ȡ��ʾBp(δȥ��ʣ����)
	// (out)fBp:����,��λ:kW
	DWORD ReadBpWithRemanence(float *const pfBp){return DYN_NOT_SUPPORT;}
	// ��ȡ��ʾCp(δȥ��ʣ����)
	// (out)fCp:����,��λ:kW
	DWORD ReadCpWithRemanence(float *const pfCp){return DYN_NOT_SUPPORT;}
	// д����ʾAp(δȥ��ʣ����)
	// (in)fAp:���ʣ���λ:kW
	DWORD WriteApWithRemanence(const float fAp){return DYN_NOT_SUPPORT;}
	// д����ʾBp(δȥ��ʣ����)
	// (in)fBp:���ʣ���λ:kW
	DWORD WriteBpWithRemanence(const float fBp){return DYN_NOT_SUPPORT;}
	// д����ʾCp(δȥ��ʣ����)
	// (in)fCp:���ʣ���λ:kW
	DWORD WriteCpWithRemanence(const float fCp){return DYN_NOT_SUPPORT;}
	// ��ȡ��ʵAp(ȥ��ʣ������)
	// (out)fAp:����,��λ:kW
	DWORD ReadApWithoutRemanence(float *const pfAp){return DYN_NOT_SUPPORT;}
	// ��ȡ��ʵBp(ȥ��ʣ������)
	// (out)fBp:����,��λ:kW
	DWORD ReadBpWithoutRemanence(float *const pfBp){return DYN_NOT_SUPPORT;}
	// ��ȡ��ʵCp(ȥ��ʣ������)
	// (out)fCp:����,��λ:kW
	DWORD ReadCpWithoutRemanence(float *const pfCp){return DYN_NOT_SUPPORT;}
	// д����ʵAp(ȥ��ʣ������)
	// (in)fAp:���ʣ���λ:kW
	DWORD WriteApWithoutRemanence(const float fAp){return DYN_NOT_SUPPORT;}
	// д����ʵBp(ȥ��ʣ������)
	// (in)fBp:���ʣ���λ:kW
	DWORD WriteBpWithoutRemanence(const float fBp){return DYN_NOT_SUPPORT;}
	// д����ʵCp(ȥ��ʣ������)
	// (in)fCp:���ʣ���λ:kW
	DWORD WriteCpWithoutRemanence(const float fCp){return DYN_NOT_SUPPORT;}
	// ���ú���ز������ֱ���0.01km/h��
	DWORD SetConstantAccStep(const float fStep){return DYN_NOT_SUPPORT;}
	// ��������ؿ���ģʽ
	DWORD ConstantAccControlModeOn(void){return DYN_NOT_SUPPORT;}
	////////////////////////////////////// ���������й�end

	////////////////////////////////////// ��Ӧʱ�����Begin
	// ��ʼ������Ӧʱ�����ģʽ
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD ResponseTimeControlModeStart(void){return DYN_NOT_SUPPORT;}
	// ֹͣ������Ӧʱ�����ģʽ
	DWORD ResponseTimeControlModeEnd(void){return DYN_NOT_SUPPORT;}

	// ȡ��������
	// (out)pfF����ָ�룬��λN������Ϊ��ָ��
	// (out)pfV���ٶ�ָ�룬��λ[10m/h]������Ϊ��ָ��
	// ���غ���ִ�н����
	// DYN_WR_FUNC_SUCCEED
	// DYN_WR_FUNC_PORT_NOT_OPEN
	// DYN_WR_FUNC_READ_SPECIFIED_DATA_FAIL
	// DYN_WR_FUNC_EQUIP_RETURE_0X15
	// DYN_WR_FUNC_CHECK_SUM_FAIL
	DWORD GetResponseTimeData( float *const pfV=NULL, float *const pfF=NULL, const int nLength=NULL){return DYN_NOT_SUPPORT;}
	////////////////////////////////////// ��Ӧʱ�����End

	///////////////////////////////////// �³ǵĿ�������Begin

	//���ú��ٶ�ֵ(km/h) 	�����ٶ�Ϊ70.0km/h:
	//XSS+0x00+0x00+0x8c+0x42+ ��У���룫C

	//���ú�Ť��ֵ (N)	����Ť��Ϊ1000.5N:
	//XSF +0x00+0x20+0x7a+0x44+ ��У���룫C

	//���ú㹦��ֵ(kw) 	���ù���Ϊ10.1kw:
	//XSP +0x9a+0x99+0x21+0x41+ ��У���룫C


	// ����IGBT��pwm��ռ�ձ�(%)
	// ����pwmռ�ձ�Ϊ50%��
	// XSI +0x00+0x20+0x7a+0x44+ ��У���룫C
	DWORD XC_SetConstantIGBT(const float fpwm);

	// ����ѡ�õ���ͨ����ͨ��1��ͨ��2��ͨ��3��
	// Ĭ��ֵΪͨ��1��������Ϊͨ��1/2/3����������ֱ���õ�һ�������ֽ�����ʾѡ�õ�ͨ����
	// XSI +0+0+0+1/2/3+ ��У���룫C
	DWORD XC_SetConstantForceChannel(const int nChannel);

	// ������
	//��Ŀǰ����ʹ�õ�ͨ���������㣺
	// XSZ+0+0+0+0+ ��У���룫C
	DWORD XC_SetConstantForceClearing(void);

	// �������ֵ
	DWORD XC_ClearKeyValue(void);

	// ���ù�Ͳֱ��(cm)��
	// ���Ͳֱ��Ϊ14.736cm��
	//XCS +0xa8+0xc6+0x6b+0x41+ ��У���룫C
	DWORD XC_SetConstantRollerDiameter(const int nRollerDim);

	// ���ñ�����������(ÿȦ)
	// �������������Ϊ1000��
	//XCS +0x00+0x00+0x7a+0x44+ ��У���룫C
	DWORD XC_SetConstantEncoderPulNum(const int nEnPulNum);

	// ����ͨ��1���궨ϵ��b[0]��force=b[0]*(ad_value-c[0])
	// ����ϵ��b[0]=1158.4��
	// XCA +0xcd+0xcc+0x90+0x44+ ��У���룫C
	DWORD XC_SetConstantForceCailFac_B1(const float fb);

	//����ͨ��1���궨ϵ��c[0]��	
	// ����ϵ��c[0]=0.01��
	//XCB +0x0a+0xd7+0x23+0x3c+ ��У���룫C
	DWORD XC_SetConstantForceCailFac_C1(const float fc);

	//����ͨ��2���궨ϵ��b[1]��	
	//����ϵ��b[1]=1158.4��
	//XCC +0xcd+0xcc+0x90+0x44+ ��У���룫C
	DWORD XC_SetConstantForceCailFac_B2(const float fb);

	//����ͨ��2���궨ϵ��c[1]��	
	//����ϵ��c[1]=0.01��
	//XCD +0x0a+0xd7+0x23+0x3c + ��У���룫C
	DWORD XC_SetConstantForceCailFac_C2(const float fc);

	//����ͨ��3���궨ϵ��b[2]��	
	//����ϵ��b[2]=1158.4��
	//XCE +0xcd+0xcc+0x90+0x44 + ��У���룫C
	DWORD XC_SetConstantForceCailFac_B3(const float fb);

	//����ͨ��3���궨ϵ��c[2]��	
	//����ϵ��c[2]=0.01��
	//XCF +0x0a+0xd7+0x23+0x3c  +��У���룫C
	DWORD XC_SetConstantForceCailFac_C3(const float fc);

	//���ú��ٿ���PID���Ʊ���ϵ��kp��	
	//����ϵ��c[2]=0.01��
	//XCH +0x0a+0xd7+0x23+0x3c  +��У���룫C
	DWORD XC_SetConstantSpeedControl_P(const float fkp);

	//���ú��ٿ���PID���ƻ���ϵ��ki��	
	//����ϵ��c[2]=0.01��
	//XCI +0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantSpeedControl_I(const float fki);

	//���ú��ٿ���PID����΢��ϵ��kd��	
	//����ϵ��c[2]=0.01��
	//XCJ+0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantSpeedControl_D(const float fkd);

	//���ú�������PID���Ʊ���ϵ��kp_force��	
	//����ϵ��c[2]=0.01��
	//XCK +0x0a+0xd7+0x23+0x3c  +��У���룫C
	DWORD XC_SetConstantForceControl_P(const float fkp);

	//���ú�������PID���Ʊ���ϵ�� ki_force��	
	//����ϵ��c[2]=0.01��
	//XCL +0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantForceControl_I(const float fki);

	//���ú�������PID���Ʊ���ϵ�� kd_force��	
	//����ϵ��c[2]=0.01��
	//XCM +0x0a+0xd7+0x23+0x3c +��У���룫C
	DWORD XC_SetConstantForceControl_D(const float fkd);

	//�������ٶȿ���	XBS + 0+0+0+0+ ��У���� �� C
	//������Ť�����	XBF + 0+0+0+0+ ��У���� �� C
	//�����㹦�ʿ���	XBP + 0+0+ 0+0+ ��У���� �� C

	//��������趨pwmֵ	XBI + 0+0+ 0+0+ ��У���� �� C
	DWORD XC_StartSetPWN(const float fPWN);

	//ֹͣ����,pwm���0	XBS + 0+0+0+0+ ��У���� �� C
	DWORD XC_StopPWN(void);

	//�𶯵��	XEB + 0+0+0+0+ ��У���� �� C
	//ֹͣ���	XES + 0+0+0+0+ ��У���� �� C

	//��ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH1: XEO + 0+0+0+1+ ��У���� �� C
	DWORD XC_OpenRelay(const int nLH);

	//�ر�ָ���̵�����LH1/LH2/LH3��	��LH1/LH2/LH3���ֱ��Ӧ����1/2/3����򿪼̵���LH3: XEF + 0+0+0+3+ ��У���� �� C
	DWORD XC_CloseRelay(const int nLH);

	//����������	XJU+ 0+0+0+0+ ��У���� �� C
	//�������½�	XJD +0+0+0+0+ ��У���� �� C
	//����궨״̬	XDF +0+0+0+0+ ��У���� �� C
	DWORD XC_EnterCaliSta(void);

	//�˳��궨״̬	 XDS +0+0+0+0+ ��У���� �� C
	DWORD XC_ExitCaliSta(void);

	//��ȡ��������	 XRT +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadEnvPar(float *const pfET=NULL, float *const pfAP=NULL, float *const pfRH=NULL);

	//��ȡ���궨ϵ��b,c	XRF +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadForCalFac(float *const pfb=NULL, float *const pfc=NULL, float *const pfb1=NULL, float *const pfc1=NULL, float *const pfb2=NULL, float *const pfc2=NULL);

	//��ȡ��ͨ��	XRC +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadForCha(int *const pnCha=NULL);

	//��ȡ����PIDϵ��	XRs +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadConSpeed(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL);

	//��ȡ����PIDϵ��	XRf +0+0+0+0+ ��У���� �� C	
	DWORD XC_ReadConForce(float *const pfP=NULL, float *const pfI=NULL, float *const pfD=NULL);

	//��ȡ��Ͳֱ���ͱ���������	 XRS +0+0+0+0+ ��У���� �� C
	DWORD XC_ReadRollerAndEncoder(int *const pnDia=NULL, int *const pnPul=NULL);

	//��3��ͨ���ı궨ϵ���̻���ROM��	 XMA +0+0+0+0+ ��У���� �� C
	DWORD XC_SolCalFac(void);

	//����Ͳֱ���ͱ���������̻���ROM��	XMS +0+0+0+0+ ��У���� �� C
	DWORD XC_SolRollerAndEncoder(void);

	//����ͨ����ѡ��̻���ROM��	XMC +0+0+0+0+ ��У���� �� C
	DWORD XC_SolForCha(void);

	//�����ٿ��Ƶ�PIDϵ���̻���ROM��	XMD + 0+0+0+0+ ��У���� �� C
	DWORD XC_SolConSheepPID(void);

	//���������Ƶ�PIDϵ���̻���ROM��	XME + 0+0+0+0+ ��У���� �� C
	DWORD XC_SolConForcePID(void);

	///////////////////////////////////// �³ǵĿ�������End

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������end 

private:
	// ����У���
	// ���
	// (in)p��Ҫ����У�����������ָ��
	// (in)un��Ҫ����У������ݳ���
	// ����У���
	BYTE CheckSum(const BYTE *const p, const UINT un);

	// floatת16����
	// �򵥵�д����printf("%x\n",*(unsigned int *)&fvalue);
	unsigned int flo_hex(float fvalue);
	//ʮ������ת��ʮ������   IEEE754������
	uint32_t data2hex(uint16_t data);

	// ʮ��������ת������ͨ�÷���
	/**
	* ��unsigned char����ת����long long��ֵ
	* {0x00 0x00 0x20 0x00}ת��֮��õ�8192
	*
	* @param str ����
	* @param len str���鳤��
	* @returns 0 on error
	*/
	LONGLONG ReadCharToLL(unsigned char* str, int len);
	float Hex2Float(unsigned long long number);
	
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
private:
	// ���ھ��
	HANDLE m_hComm;
	//�ͻ���Ϣ�ٽ籣����
	CRITICAL_SECTION cInfoSection;

};

#endif