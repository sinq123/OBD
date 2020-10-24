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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������end 
};

#endif
