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