/*
* Copyright (c) 2010,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHModeServerDB.h
* �ļ���ʶ��
* ������NHModeServerDB���ݿ��������,����ֵ�߼�����ע������ֵ�߼��ĺ����⣬����������ѭ���·��ع���
*		Getxxx��ִ�гɹ�����0x01���޶�Ӧ��¼����0x00���쳣����0xff��Setxxx���ɹ�����0x00���쳣����0xff
*
* �汾��1.0
* ���ߣ�HuaBei
* ���ڣ�2010-07-14
*
* ��ʷ��Ϣ��
*
* �汾��1.0.34
* ���ڣ�2014-06-20
* ���ߣ�Lihy
* ��������GetDboRealTimeDataOfLUGDOWN()���������Ӷ�ȡPowerCorrectionFactor�ֶ�
*
* �汾��1.0.33
* ���ڣ�2014-05-04
* ���ߣ�Lihy
* ��������GetDboTestLog()���������Ӷ�ȡSRM�ֶ�
*
* �汾��1.0.32
* ���ڣ�2013-08-19
* ���ߣ�Chc
* �������޸Ĺ������ݶ�ȡ��ȡReportNumber��RunningNumber��BUG
*
* �汾��1.0.31
* ���ڣ�2013-06-22
* ���ߣ�Chc
* �������޸Ĺ������ݶ�ȡ��д��ѹ�������ڴ�й¶��BUG
*
* �汾��1.0.30
* ���ڣ�2013-07-12
* ���ߣ�Duanjf
* ������SetDboSystemLogϵͳ��־SetDboLineLock�������ֹ�����
*
* �汾��1.0.29
* ���ڣ�2013-06-22
* ���ߣ�Chc
* ������������̬������˲̬�����ؼ��١�˫���١�Ħ�г����ٺ�Ħ�г�˫���ٵĹ������ݶ�ȡ��д��ѹ������
*
* �汾��1.0.28
* ���ڣ�2013-04-02
* ���ߣ�Chc
* ������1)�ڻ�ȡFSNHT��ֵ�����У������ȡ��ֵʧ�ܻ�գ���ֵ������Ϊ�������ֵ����ֵβ������Ӧ��1���Ա�������ֵ����Դ
		2)�޸�DIS��ȡĬ����ֵ�Ĵ���
		3)�����û�Ȩ�޵Ķ�ȡ��д�뺯��
		4)�޸�Ħ�г�˫���ٽ�����ݵĶ�ȡ��д�뺯��
*
* �汾��1.0.27
* ���ڣ�2013-01-28
* ���ߣ�Chc
* ������1)���Ӷ�ȡ����������GetDboWhiteList
*		2)���Ӷ�ȡ�ظ��Ժ���GetDboRepeat
*
* �汾��1.0.26
* ���ڣ�2013-01-05
* ���ߣ�Lihy
* ����������GetDboXXX����,ʹ������ڸ�ʽΪXXXX-XX-XX XX:XX:XX
*
* �汾��1.0.25
* ���ڣ�2012-10-08
* ���ߣ�Chc
* ����������GetDboTestLog����,ʹ������ڸ�ʽΪXXXX-XX-XX XX:XX:XX
*
* �汾��1.0.24
* ���ڣ�2012-10-08
* ���ߣ�wuyq
* ����������SetDboResultOfDIS����,ʹ���ݿ���O2������ȷ������(֮ǰ�����������CO2��)
*
*
*�汾��1.0.23
*���ڣ�2010-05-19
*���ߣ�Modb
*������1)��NHModeServerDB,����GetDboBillInfo��GetDboTestLog��SetDboBillInfo��SetTestLog��ӽӿ�wchNumberOfTestPeriodMDIS
	   2)��NHModeServerDB,����GetDboTestAmount��SetDboTestAmount��ӽӿ�wchMDISAmount
	   3)��NHModeServerDB,��ӻ�ȡĦ�г�˫���ٽ�����ݺ���GetDboResultOfMDIS
	   4)��NHModeServerDB,���д��Ħ�г�˫���ٽ�����ݺ���SetDboResultOfMDIS
	   5)��NHModeServerDB,���д��Ħ�г�˫����ʵʱ���ݺ���SetDborealTimeDataOfMDIS

* �汾��1.0.22
* ���ڣ�2011-10-10
* ���ߣ�hemincong
* ������.vt = VT_NULL;��Ϊ.ChangeType(VT_NULL);
*
* �汾��1.0.21
* ���ڣ�2011-05-19
* ���ߣ�Ming
* ������1)��Ӻ���SetDboTestLogIsSentEP()
*
* �汾��1.0.20
* ���ڣ�2011-05-17
* ���ߣ�Raylee
* ���������Sampling�ֶε���Ӧ����
*
* �汾��1.0.19
* ���ڣ�2011-04-28
* ���ߣ�Liangzs
* ������1��ͳһNHModeServerDB��NHModeClientDB������Ľӿڣ������������������б�����ֵ������ע������ֵ�߼��ĺ����⣬
*		����������ѭ���·��ع���Getxxx��ִ�гɹ�����0x01���޶�Ӧ��¼����0x00���쳣����0xff��Setxxx���ɹ�����0x00��
*		�쳣����0xff
*		2��ÿ����������ע�ͣ��ر��Ǽ��Ϸ���ֵ��ע��
*
* �汾��1.0.18
* ���ڣ�2011-04-25
* ���ߣ�Raylee
* ������1������SetTestLog��������RunningNumberΪ��������
*		2��ע�����ݿⳬʱ���ã�ʹ��ADOĬ������
*
* �汾��1.0.17
* ���ڣ�2011-04-15
* ���ߣ�Ming
* ����������GetDboVehicleInfo,SetDboVehicleInfo,��Ӷ�ȡ��д���ֶ�OwnerCharacter��OwnerDistrict��LicenseDepartment��LicenseIssueDate��LicenseValidDate
*		����GetDboTestLog,SetDboTestLog,��Ӷ�ȡ��д���ֶ�IsSentEP��OwnerCharacter��OwnerDistrict��LicenseDepartment��LicenseIssueDate��LicenseValidDate
*
* �汾��1.0.16
* ���ڣ�2011-04-07
* ���ߣ�Ming
* ��������Ӻ���GetDboStationTestAmount,SetDboStationTestAmount,ʵ�����ɼ��վ˳���
*
* �汾��1.0.15
* ���ڣ�2011-04-02
* ���ߣ�Raylee
* ������GetDboSystemParameters������Ӷ�ȡPeriodOfDynamometerForceCheck�ֶ�
*
* �汾��1.0.14
* ���ڣ�2011-03-11
* ���ߣ�Raylee
* ����������GetDboSystemParameters����û�ж�ȡPeriodOfEnvParamMeasurerCheck�ֶε�bug
*
* �汾��1.0.13
* ���ڣ�2011-01-20
* ���ߣ�Ming
* ������1)��ӻ�ȡʵʱ���ݺ���GetDboRealTimeDataOfASM()��GetDboRealTimeDataOfLUGDOWN()��GetDboRealTimeDataOfDIS()��GetDboRealTimeDataOfVMAS()
*		2)��Ӻ���SetDboTestLogIsSent()
*
* �汾��1.0.12
* ���ڣ�2011-01-17
* ���ߣ�Raylee
* ����������GetDboSystemParameters()����û�ж�ȡTestPeriod�ֶε�bug
*
* �汾��1.0.11
* ���ڣ�2011-01-15
* ���ߣ�Raylee
* ��������Ӻ���GetDboResultOfVMAS()
*
* �汾��1.0.10
* ���ڣ�2011-01-10
* ���ߣ�Ming
* ������1)�ڻ�ȡASM��LUGDOWN��VMAS��DIS��FSUYD��ֵ�����У������ȡ��ֵʧ�ܻ�գ���ֵ������Ϊ�������ֵ����ֵβ������Ӧ��1���Ա�������ֵ����Դ
*		2)����GetDboTestLog��SetDboTestLog��SetDboVehicleInfo����ĸ����ӿ�wchEngineStroke��wchEmissionStandard��wchAnnualTestPeriod��wchHasOBD
*
* �汾��1.0.9
* ���ڣ�2010-12-30
* ���ߣ�Ming
* ������������ü����״̬����SetDboLineInfoLineState
*
* �汾��1.0.8
* ���ڣ�2010-12-27
* ���ߣ�Raylee
* ����������GetDboVehicleModel����û�ж�ȡPassengerVehicle��bug
*
* �汾��1.0.7
* ���ڣ�2010-12-11
* ���ߣ�Ming
* ������1)����GetDboBillInfo��GetDboTestLog��SetDboBillInfo��SetTestLog��ӽӿ�wchNumberOfTestPeriodAVFSU��wchNumberOfTestPeriodMI	
*		2)����GetDboTestAmount��SetDboTestAmount��ӽӿ�wchMIAmount��wchAVFSUAmount
*		3)��ӻ�ȡũ�ó����ɼ��ٽ�����ݺ���GetDboResultOfAVFSU
*		4)���д��ũ�ó����ɼ��ٽ�����ݺ���SetDboResultOfAVFSU
*		5)��ӻ�ȡĦ�г����ٽ�����ݺ���GetDboResultOfMI
*		6)���д��Ħ�г����ٽ�����ݺ���SetDboResultOfMI
*		7)���д��Ħ�г�����ʵʱ���ݺ���SetDborealTimeDataOfMI
*
* �汾��1.0.6
* ���ڣ�2010-11-30
* ���ߣ�Raylee
* ���������UpdateTestPhoto()��д����ʱ��ӿ�
*
* �汾��1.0.5
* ���ڣ�2010-10-30
* ���ߣ�Raylee
* �������Ż����ʵʱ���ݵĲ��룬����ٶ�
*
* �汾��1.0.4
* ���ڣ�2010-10-08
* ���ߣ�Raylee
* ����������SetDboResultOfASM������Lambda5025��ת��ʱ��_wtoi��Ϊ_wtof
*
* �汾��1.0.3
* ���ڣ�2010-09-20
* ���ߣ�Ming
* ����������GetDboVehicleInfo����ĸ����ӿ�wchEngineStroke��wchEmissionStandard��wchAnnualTestPeriod��wchHasOBD
*
* �汾��1.0.2
* ���ڣ�2010-09-14
* ���ߣ�Qin
* ����������SetDboBillInfoTestState���������ӿ�wchLineNumber(����߱��)��StartTime(�ı���״̬ʱ��)
*
* �汾��1.0.1
* ���ڣ�2010-07-29
* ���ߣ�CYX
* ��������������GetDboResultOfASM��ȡLimitOfCO2540_EDP�ֶε�Bug
*
* �汾��1.0.0
* ���ڣ�2010-07-14
* ���ߣ�hyh, Ming
* ��������ʽ����ʹ��
*/


#ifndef NHMODESERVERDB_H
#define NHMODESERVERDB_H

// �����ǵ����ļ�,����Ҫ���¶���
#ifndef SQLMAX
#define SQLMAX 1024 // sql �����������ַ���
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#include <list>
using std::list;

//extern "C"
//{
	// �û���Ϣ<UserInfo> UserInfo������в���

	// ��ȡ�û���Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboUserInfo(const wchar_t wchSql[SQLMAX], PUSERINFO const pDboUserInfo);
	// �����û���Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboUserInfo(const USERINFO &dboUserInfo);
	// ɾ���û���Ϣ
	// �ò���δʵ��
	DWORD DelDboUserInfo(const wchar_t wchSql[SQLMAX]);
	// ��ȡ�û�Ȩ��
	// ���أ�
	// 0x01����ȡ�ɹ�
	// 0x02����ȡʧ��
	// 0xff�����ݿ��쳣
	DWORD GetDboUserPermission(const wchar_t *const pwchName, std::string &strPermission);
	// дȨ�����������ݿ�
	// ���أ�
	// 0x00��д��ɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboUserPermission(const wchar_t *const pwchName, const std::string &strPermission);
	

	// ������Ϣ<BillInfo> BillInfo������в���
	// ��ȡ������Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	//DWORD GetDboBillInfo(const wchar_t wchSql[SQLMAX], PBILLINFO const pDboBillInfo);
	// ���ÿ�����Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	//DWORD SetDboBillInfo(const BILLINFO &dboBillInfo);
	// ���ÿ�����Ϣ�еļ��״̬����ʼ����ʱ�䣬����߱��
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	//DWORD SetDboBillInfoTestState(const wchar_t wchRunningNumber[50], const short siTestState,const wchar_t wchLineNumber[50] = NULL, const SYSTEMTIME *pstProcessingTime = NULL);
	// ���ÿ�����Ϣ�еļ��״̬�ͼ���߱��
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboBillInfoSelectState(const wchar_t wchRunningNumber[50], const wchar_t wchLineNumber[50]);
	// �����쳣�˳�ʱ�����ڼ쳵��״̬Ϊ����
	// ���أ�
	// 0x00��ִ�гɹ�
	// 0xff�����ݿ��쳣
	DWORD UpdateDboBillInfoPowerFailure(const wchar_t wchLineNumber[50]);
	// ɾ�������ɵĿ�����Ϣ
	// ���أ�
	// 0x00��ɾ���ɹ�
	// 0xff�����ݿ��쳣
	DWORD DelDboBillInfoTestFinished(const wchar_t wchRunningNumber[50]);
	// ɾ��������Ϣ
	// ���أ�
	// 0x00��ɾ���ɹ�
	// 0xff�����ݿ��쳣
	DWORD DelDboBillInfo(const wchar_t wchSql[SQLMAX]);
	// ������Ϣ<VehicleInfo> Vehicle������в���
	// ��ȡ������Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboVehicleInfo(const wchar_t wchSql[SQLMAX], PVEHICLEINFO const pDboVehicleInfo);
	// ���ó�����Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboVehicleInfo(const VEHICLEINFO &dboVehicleInfo);

	// ������Ϣ<VehicleModel> VehicleModel������в���
	// ��ȡ������Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboVehicleModel(const wchar_t wchSql[SQLMAX], PVEHICLEMODEL const pDboVehicleModel);
	// ���ó�����Ϣ
	// ���أ�
	// 0x00���޸ĳɹ�
	// 0x01: ��ӳɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboVehicleModel(const VEHICLEMODEL &dboVehicleModel);

	// ASM��ֵ��Ϣ<LimitOfASM> LimitOfASM������в���
	// ��ȡASM��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfASM(const wchar_t wchSql[SQLMAX], PLIMITOFASM const pDboLimitOfASM);

	// Vmas��ֵ��Ϣ<LimitOfVMAS> LimitOfVMAS������в���
	// ��ȡVMAS��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfVMAS(const wchar_t wchSql[SQLMAX], PLIMITOFVMAS const pDboLimitOfVMAS);

	// LimitOfLUGDOWN��ֵ��Ϣ<LimitOfLimitOfLUGDOWN> LimitOfLimitOfLUGDOWN������в���
	// ��ȡLUGDOWN��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfLUGDOWN(const wchar_t wchSql[SQLMAX], PLIMITOFLUGDOWN const pDboLimitOfLUGDOWN);

	// DIS��ֵ��Ϣ<LimitOfDIS> LimitOfDIS������в���
	// ��ȡDIS��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfDIS(const wchar_t wchSql[SQLMAX], PLIMITOFDIS const pDboLimitOfDIS);

	// FSUNHT��ֵ��Ϣ<LimitOfFSUNHT> LimitOfFSUNHT������в���
	// ��ȡFSNHT��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfFSUNHT(const wchar_t wchSql[SQLMAX], PLIMITOFFSUNHT const pDboLimitOfFSUNHT);

	// FSUNHTOfHeavyVehicle��ֵ��Ϣ<LimitOfFSUNHTOfHeavyVehicle> LimitOfFSUNHTOfHeavyVehicle������в���
	// ��ȡFSUNHTOfHeavyVehicle��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfFSUNHTOfHeavyVehicle(const wchar_t wchSql[SQLMAX], PLIMITOFFSUNHTOFHEAVYVEHICLE const pDboLimitOfFSUNHTOfHeavyVehicle);

	// FSUNHTOfLightVehicle��ֵ��Ϣ<LimitOfFSUNHTOfLightVehicle> LimitOfFSUNHTOfLightVehicle������в���
	// ��ȡFSUNHTOfLightVehicle��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfFSUNHTOfLightVehicle(const wchar_t wchSql[SQLMAX], PLIMITOFFSUNHTOFLIGHTVEHICLE const pDboLimitOfFSUNHTOfLightVehicle);

	// FSUYD��ֵ��Ϣ<LimitOfFSUYD> LimitOfFSUYD������в���
	// ��ȡFSUYD��ֵ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLimitOfFSUYD(const wchar_t wchSql[SQLMAX], PLIMITOFFSUYD const pDboLimitOfFSUYD);

	// �������Ϣ<LineInfo> LineInfo������в���
	// ��ȡ�������Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboLineInfo(const wchar_t wchSql[SQLMAX], PLINEINFO const pDboLineInfo);
	// ���ü������Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboLineInfo(const LINEINFO &dboLineInfo);
	// ���ü����״̬�ͼ����״̬���ʱ��
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboLineInfoLineState(const wchar_t wchLineNumber[50], const short siLineState, const SYSTEMTIME *pstChangeTime);

	// ���վ��Ϣ<StationInfo> StationInfo������в���
	// ��ȡ���վ��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboStationInfo(const wchar_t wchSql[SQLMAX], PSTATIONINFO const pDboStationInfo);
	// ���ü��վ��Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboStationInfo(const STATIONINFO& dboStationInfo);

	// �ŷż����־��Ϣ<TestLog> TestLog������в���
	// ��ȡ�����־��Ϣ
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestLog(const wchar_t wchSql[SQLMAX], PTESTLOG const pDboTestLog);
	// ���ü����־��Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestLog(const TESTLOG &dboTestLog);
	// ���ü����־��Ϣ�е��Ƿ����ֶ�
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestLogIsSent(const wchar_t wchRunningNumber[50], const bool bIsSent=false);
	// ���ü����־��Ϣ�е��Ƿ��ͻ��������ֶ�
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestLogIsSentEP(const wchar_t wchRunningNumber[50], const bool bIsSentEP=false);
	// ���ü����־�еļ��״̬����ʼ����ʱ�䣬����߱��
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestLogTestState(const wchar_t wchRunningNumber[50], const short siTestState,const wchar_t wchLineNumber[50] = NULL, const SYSTEMTIME *pstProcessingTime = NULL);

	// DIS�������<ResultOfDIS> ResultOfDIS������в���
	// ��ȡDIS�����
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfDIS(const wchar_t wchSql[SQLMAX], PRESULTOFDIS const pDboResultOfDIS);
	DWORD GetDboResultOfDIS_Zip(const wchar_t wchSQL[SQLMAX], SResultOfDIS* const pResultOfDIS);
	// ����DIS�����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfDIS(const RESULTOFDIS &dboResultOfDIS);
	DWORD SetDboResultOfDIS_Zip(const SResultOfDIS &sResultOfDIS);

	// DIS real time data<RealTimeDataOfDIS> RealTimeDataOfDIS������в���
	// ��ȡDISʵʱ����
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboRealTimeDataOfDIS(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	DWORD GetDboRealTimeDataOfDIS_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	// ����DISʵʱ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboRealTimeDataOfDIS(list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	DWORD SetDboRealTimeDataOfDIS_Zip(list<REALTIMEDATAOFDIS> &listRealTimeDataOfDIS);
	// ASM result data<ResultOfASM> ResultOfASM������в���
	// ��ȡASM�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfASM(const wchar_t wchSql[SQLMAX], PRESULTOFASM const pDboResultOfASM);
	DWORD GetDboResultOfASM_Zip(const wchar_t wchSQL[SQLMAX], SResultOfASM* const pResultOfASM);
	// ����ASM�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfASM(const RESULTOFASM &dboResultOfASM);
	DWORD SetDboResultOfASM_Zip(const SResultOfASM &sResultOfASM);

	// ASM real time data<RealTimeDataOfASM> RealTimeDataOfASM������в���
	// ��ȡASMʵʱ����
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboRealTimeDataOfASM(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);
	DWORD GetDboRealTimeDataOfASM_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);
	// ����ASMʵʱ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboRealTimeDataOfASM(list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);
	DWORD SetDboRealTimeDataOfASM_Zip(list<REALTIMEDATAOFASM> &listRealTimeDataOfASM);

	// VMAS result data<ResultOfVMAS> ResultOfVMAS������в���
	// ��ȡVMAS�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfVMAS(const wchar_t wchSql[SQLMAX], PRESULTOFVMAS const pDboResultOfVMAS);
	DWORD GetDboResultOfVMAS_Zip(const wchar_t wchSQL[SQLMAX], SResultOfVMAS* const pResultOfVMAS);
	// ����VMAS�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfVMAS(const RESULTOFVMAS &dboResultOfVMAS);
	DWORD SetDboResultOfVMAS_Zip(const SResultOfVMAS &sResultOfVMAS);

	// VMAS real time data<RealTimeDataOfVMAS> RealTimeDataOfVMAS������в���
	// ��ȡVMASʵʱ����
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboRealTimeDataOfVMAS(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	DWORD GetDboRealTimeDataOfVMAS_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	// ����VMASʵʱ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboRealTimeDataOfVMAS(list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	DWORD SetDboRealTimeDataOfVMAS_Zip(list<REALTIMEDATAOFVMAS> &listRealTimeDataOfVMAS);
	// LUGDOWN result data<ResultOfLUGDOWN> ResultOfLUGDOWN������в���
	// ��ȡLUGDOWN�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfLUGDOWN(const wchar_t wchSql[SQLMAX], PRESULTOFLUGDOWN const pDboResultOfLUGDOWN);
	DWORD GetDboResultOfLUGDOWN_Zip(const wchar_t wchSQL[SQLMAX], SResultOfLUGDOWN* const pResultOfLUGDOWN);
	// ����LUGDOWN�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfLUGDOWN(const RESULTOFLUGDOWN &dboResultOfLUGDOWN);
	DWORD SetDboResultOfLUGDOWN_Zip(const SResultOfLUGDOWN &sResultOfLUGDOWN);

	// LUGDOWN real time data<RealTimeDataOfLUGDOWN> RealTimeDataOfLUGDOWN������в���
	// ��ȡLUGDOWNʵʱ����
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboRealTimeDataOfLUGDOWN(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	DWORD GetDboRealTimeDataOfLUGDOWN_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	// ����LUGDOWNʵʱ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboRealTimeDataOfLUGDOWN(list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	DWORD SetDboRealTimeDataOfLUGDOWN_Zip(list<REALTIMEDATAOFLUGDOWN> &listRealTimeDataOfLUGDOWN);
	// FSUNHT�������<ResultOfFSUNHT> ResultOfFSUNHT������в���
	// ��ȡFSUNHT�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfFSUNHT(const wchar_t wchSql[SQLMAX], PRESULTOFFSUNHT const pDboResultOfFSUNHT);
	DWORD GetDboResultOfFSUNHT_Zip(const wchar_t wchSQL[SQLMAX], SResultOfFSUNHT* const pResultOfFSUNHT);
	// ����FSUNHT�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfFSUNHT(const RESULTOFFSUNHT &dboResultOfFSUNHT);
	DWORD SetDboResultOfFSUNHT_Zip(const SResultOfFSUNHT &sResultOfFSUNHT);


	// FSUNHT real time data<RealTimeDataOfFSUNHT> RealTimeDataOfFSUNHT������в���
	// ��ȡFSUNHTʵʱ����
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboRealTimeDataOfFSUNHT(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);
	DWORD GetDboRealTimeDataOfFSUNHT_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);
	// ����FSUNHTʵʱ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboRealTimeDataOfFSUNHT(list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);
	DWORD SetDboRealTimeDataOfFSUNHT_Zip(list<REALTIMEDATAOFFSUNHT> &listRealTimeDataOfFSUNHT);

	// FSUYD�������<ResultOfFSUYD> ResultOfFSUYD������в���
	// ��ȡFSUYD�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfFSUYD(const wchar_t wchSql[SQLMAX], PRESULTOFFSUYD const pDboResultOfFSUYD);
	// ����FSUYD�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfFSUYD(const RESULTOFFSUYD &dboResultOfFSUYD);

	// AVFSU�������<ResultOfAVFSU> ResultOfAVFSU������в���
	// ��ȡAVFSU�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfAVFSU(const wchar_t wchSql[SQLMAX], PRESULTOFAVFSU const pDboResultOfAVFSU);
	// ����AVFSU�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfAVFSU(const RESULTOFAVFSU &dboResultOfAVFSU);

	// MI�������<ResultOfMI> ResultOfMI������в���
	// ��ȡMI�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfMI(const wchar_t wchSql[SQLMAX], PRESULTOFMI const pDboResultOfMI);
	// ����MI�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfMI(const RESULTOFMI &dboResultOfMI);

	// MI real time data<RealTimeDataOfMI> RealTimeDataOfMI������в���
	// ����MIʵʱ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboRealTimeDataOfMI(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	DWORD GetDboRealTimeDataOfMI_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	DWORD SetDboRealTimeDataOfMI(list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	DWORD SetDboRealTimeDataOfMI_Zip(list<REALTIMEDATAOFMI> &listRealTimeDataOfMI);
	// MDIS�������<ResultOfMDIS> ResultOfMDIS������в���
	// ��ȡMDIS�������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboResultOfMDIS(const wchar_t wchSql[SQLMAX], PRESULTOFMDIS const pDboResultOfMDIS);
	// ����MDIS�������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboResultOfMDIS(const RESULTOFMDIS &dboResultOfMDIS);

	// MDIS real time data<RealTimeDataOfMI> RealTimeDataOfMDIS������в���
	// ����MDISʵʱ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboRealTimeDataOfMDIS(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	DWORD GetDboRealTimeDataOfMDIS_Zip(const wchar_t wchSql[SQLMAX], list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	DWORD SetDboRealTimeDataOfMDIS(list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	DWORD SetDboRealTimeDataOfMDIS_Zip(list<REALTIMEDATAOFMDIS> &listRealTimeDataOfMDIS);
	// �豸У׼�����Ϣ<EquCalChkInfo> EquCalChkInfo������в���
	DWORD SetDboEquCalChkInfo(const EQUCALCHKINFO &dboEquCalChkInfo);

	// �������֤��Ϣ<LineInfo>
	// return value
	// 0xff : Database exception
	// 0x00 : No record get
	// 0x01 : Get first record successful
	// 0x02 : Get CertificateAuthentication failed
	DWORD GetDboCertificateOfLineInfo(wchar_t const * pwchLineNumber, BYTE * const pbCertificateAuthentication, const UINT uiCALength, long& lDataSize);
	// return value
	// 0xff : Database exception
	// 0x00 : Update successful
	// 0x01 : LineNumber error
	DWORD SetDboCertificateOfLineInfo(wchar_t const * pwchLineNumber, BYTE * const pbCertificateAuthentication, const UINT uiCALength);

	// ϵͳ����<SystemParameters>
	// ��ȡϵͳ����
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboSystemParameters(const wchar_t wchSql[SQLMAX], PSYSTEMPARAMETERS const pDboSystemParameters);
	// ����ϵͳ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboSystemParameters(const SYSTEMPARAMETERS &dboSystemParameters);

	// ASM������<TestParametersOfASM>
	// ��ȡASM������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestParametersOfASM(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFASM const pDboTestParametersOfASM);

	// VMAS������<TestParametersOfVMAS>
	// ��ȡVMAS������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestParametersOfVMAS(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFVMAS const pDboTestParametersOfVMAS);

	// ����LUGDOWN������<TestParametersOfLUGDOWNHEAVY>
	// ��ȡLUGDOWNHEAVY������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestParametersOfLUGDOWNHEAVY(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFLUGDOWNHEAVY const pDboTestParametersOfLUGDOWNHEAVY);

	// ����LUGDOWN������<TestParametersOfLUGDOWNLIGHT>
	// ��ȡLUGDOWNLIGHT������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestParametersOfLUGDOWNLIGHT(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFLUGDOWNLIGHT const pDboTestParametersOfLUGDOWNLIGHT);

	// ˫���ټ�����<TestParametersOfDIS>
	// ��ȡDIS������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestParametersOfDIS(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFDIS const pDboTestParametersOfDIS);
	// ����DIS������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestParametersOfDIS(const TESTPARAMETERSOFDIS &dboTestParametersOfDIS);

	// ��͸�����ɼ��ټ�����<TestParametersOfFSUNHT>
	// ��ȡFSUNHT������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestParametersOfFSUNHT(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFFSUNHT const pDboTestParametersOfFSUNHT);
	// ����FSUNHT������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestParametersOfFSUNHT(const TESTPARAMETERSOFFSUNHT &dboTestParametersOfFSUNHT);

	// ��ֽʽ���ɼ��ټ�����<TestParametersOfFSUYD>
	// ��ȡFSUYD������
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestParametersOfFSUYD(const wchar_t wchSql[SQLMAX], PTESTPARAMETERSOFFSUYD const pDboTestParametersOfFSUYD);
	// ����FSUYD������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestParametersOfFSUYD(const TESTPARAMETERSOFFSUYD &dboTestParametersOfFSUYD);

	// �궨��־<DemarcationLog>
	// ���ñ궨��־��Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationLog(const DEMARCATIONLOG &dboDemarcationLog);

	// ����������У׼���<DemarcationResultOfAnaGasCal>
	// ���÷���������У׼�����Ϣ
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfAnaGasCal(const DEMARCATIONRESULTOFANAGASCAL &dboResultOfAnaGasCal);

	// ��������������<DemarcationResultOfAnaGasChk>
	// ���÷�������������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfAnaGasChk(const DEMARCATIONRESULTOFANAGASCHK &dboResultOfAnaGasChk);

	// ��͸��ȼ�У׼�����<DemarcationResultOfOpaCalChk>
	// ���ò�͸��ȼ�У׼�����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfOpaCalChk(const DEMARCATIONRESULTOFOPACALCHK &dboResultOfOpaCalChk);
	
	// ��ֽʽ�̶ȼƼ����<DemarcationResultOfSmoChk>
	// ������ֽʽ�̶ȼƼ����
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfSmoChk(const DEMARCATIONRESULTOFSMOCHK &dboResultOfSmoChk);
	
	// �⹦�����ػ��н��<DemarcationResultOfDynConstLoad>
	// ���ò⹦�����ػ��н��
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfDynConstLoad(const DEMARCATIONRESULTOFDYNCONSTLOAD &dboResultOfDynConstLoad);

	// �⹦����У׼���<DemarcationResultOfDynForceCal>
	// ���ò⹦����У׼���
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfDynForceCal(const DEMARCATIONRESULTOFDYNFORCECAL &dboResultOfDynForceCal);
	
	// �⹦���������<DemarcationResultOfDynForceChk>
	// ���ò⹦���������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfDynForceChk(const DEMARCATIONRESULTOFDYNFORCECHK &dboResultOfDynForceChk);

	// �⹦���������ʻ��н��<DemarcationResultOfDynPLHP>
	// ���ò⹦���������ʻ��н��
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfDynPLHP(const DEMARCATIONRESULTOFDYNPLHP &dboResultOfDynPLHP);
	
	// ����������У׼���<DemarcationResultOfFloFluxCal>
	// ��������������У׼���
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfFloFluxCal(const DemarcationResultOfFloFluxCal &dboResultOfFloFluxCal);

	// ���������������<DemarcationResultOfFloFluxChk>
	// �������������������
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfFloFluxChk(const DemarcationResultOfFloFluxChk &dboResultOfFloFluxChk);

	// ������ϡ����У׼���<DemarcationResultOfFloDilO2Cal>
	// ����������ϡ����У׼���
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfFloDilO2Cal(const DemarcationResultOfFloDilO2Cal &dboResultOfFloDilO2Cal);

	// ������ѹ��У׼���<DemarcationResultOfFloPresCal>
	// ����������ѹ��У׼���
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDemarcationResultOfFloPresCal(const DemarcationResultOfFloPresCal &dboResultOfFloPresCal);

	// ���ͼƬ<TestPhoto>
	// ��ȡ���ͼƬ
	// ���أ�
	// 0x01����ȡ�ɹ�
	// 0x02����ȡʧ��
	// 0xff�����ݿ��쳣
	DWORD GetDboTestPhoto(const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, UINT *const puiActualLengthInBytes, const BYTE bPhotoNumber = 5);
	DWORD GetDboTestPhoto(const wchar_t *const pwchRunningNumber, const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, UINT *const puiActualLengthInBytes, const BYTE bPhotoNumber = 5);
	// дͼƬ���������ݿ�
	// ���أ�
	// 0x00��д��ɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboTestPhoto(const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, const BYTE bPhotoNumber = 5);
	DWORD SetDboTestPhoto(const wchar_t *const pwchRunningNumber, const wchar_t *const pwchReportNumber, BYTE **const ppbPhoto, const UINT *puiPhotoLengthInBytes, const BYTE bPhotoNumber = 5);
	// ����ͼƬ
	// ���أ�
	// 0x00�����³ɹ�
	// 0xff�����ݿ��쳣
	DWORD UpdateTestPhoto(const wchar_t *const pwchReportNumber, const wchar_t *const pwchFieldName, const LPSYSTEMTIME lpst);
	DWORD UpdateTestPhoto(const wchar_t *const pwchRunningNumber, const wchar_t *const pwchReportNumber, const wchar_t *const pwchFieldName, const LPSYSTEMTIME lpst);

	// ���������<TestAmount>
	// ���û�м���߱�ź����ڶ�Ӧ�ļ�¼�������Զ�����һ����Ӧ��¼������������Ϊ0
	// (in)pwchLineNumber,����߱��
	// (in)st,����ʱ��
	// (in)nAmount,������������
	// (reutrn)
	// 0x00:����
	// ��0x00:�쳣
	DWORD GetDboTestAmount(const wchar_t * const pwchLineNumber, const SYSTEMTIME &st, PTESTAMOUNT const pDboTestAmount);
	
	// д�������<TestAmount>
	// (in)nAmount,��������
	// (reutrn)
	// 0x00:����
	// ��0x00:�쳣
	DWORD SetDboTestAmount(const wchar_t * const pwchLineNumber, const SYSTEMTIME &st, const TESTAMOUNT &dboTestAmount);

	// ���������<StationTestAmount>
	// ���أ�
	// 0x00:����
	// 0xff:�쳣
	DWORD GetDboStationTestAmount(const SYSTEMTIME &st, PTESTAMOUNT const pDboStationTestAmount);
	
	// д�������<StationTestAmount>
	// ���أ�
	// 0x00:����
	// ��0x00:�쳣
	DWORD SetDboStationTestAmount(const SYSTEMTIME &st, const TESTAMOUNT &dboStationTestAmount);

	// ɾ����ʷ����
	// (in)st��Ҫɾ������ʷ���ݵ�ʱ��㣬��������ʱ���
	// ���أ�
	// 0x00:����
	// ��0x00:�쳣
	DWORD DelDboHistoryData(const SYSTEMTIME &st);

	// ��������<T1>
	// (in)pwchPlateNumber,���ƺ���
	// (in)pwchPlateType,��������
	// (reutrn)
	// 0x00:���� �ڰ�������
	// ��0x00:�쳣 ���ڰ�������
	DWORD GetDboWhiteList(const wchar_t * const pwchPlateNumber, const wchar_t * const pwchPlateType);

	// ���ظ���<T2>
	// (in)pwchPlateNumber,���ƺ���
	// (in)pwchPlateType,��������
	// (in)pwchTestType,�������
	// (out)pwchReportNumber,������
	// (reutrn)
	// 0x00:���� ���ظ�����
	// ��0x00:�쳣 �����ظ�����
	DWORD GetDboRepeat(const wchar_t * const pwchPlateNumber, const wchar_t * const pwchPlateType, const wchar_t * const pwchTestType, wchar_t * const pwchReportNumber);
	DWORD GetDboRepeat(const wchar_t * const pwchPlateNumber, const wchar_t * const pwchPlateType, const wchar_t * const pwchTestType, wchar_t * const pwchRunningNumber, wchar_t * const pwchReportNumber);


	//д�ճ�������־
	//strMessage:��־ʱ������
	//Operator:�û�
	//OpTime:����ʱ��
	//Work:������Ŀ 1���ճ����в���,2������У׼,3�������Լ�
	//Pass:�Ƿ�ͨ�� 1��ͨ��,0�ǲ�ͨ��
	// ���أ�
	// 0x00:����
	// ��0x00:�쳣
	DWORD SetDboRunningLog(const wchar_t * strMessage,wchar_t * Operator,const wchar_t * OpTime,wchar_t * Work,wchar_t * Pass);

	// ϵͳ��־<SystemLog> SystemLog������в���
	//reason: 1:���м�¼ 2:У׼��¼ 3:���ϼ�¼ 4:ά�޼�¼
	DWORD SetDboSystemLog(const SYSTEMLOG &dboSystemLog);

	//��ֹ�����
	//bLock:trueʱ,��ֹ�����
	//bLock:falseʱ,���������
	 DWORD SetDboLineLock(const wchar_t wchLineNumber[50], bool bLock=false);
	 // ɾ��RealTimeData
	// ���أ�
	// 0x00��ɾ���ɹ�
	// 0xff�����ݿ��쳣
	DWORD DelDboRealTimeData(const wchar_t wchSql[SQLMAX]);

	// ����ReportNumber����TestLog�е�RunningNumber
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestLogRunningNumber(const wchar_t * const pwchReportNumber, wchar_t * const pwchRunningNumber);

	// ����RunningNumber����TestLog�е�ReportNumber
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboTestLogReportNumber(const wchar_t * const pwchRunningNumber, wchar_t * const pwchReportNumber);

	// ���ݿ�汾<DBVersion>
	// ��ȡ���ݿ�汾
	// ���أ�
	// 0x00���޶�Ӧ��¼
	// 0x01����ȡ�ɹ�
	// 0xff�����ݿ��쳣
	DWORD GetDboDBVersion(SDBVersion& sDBVersion);
	// �������ݿ�汾
	// ���أ�
	// 0x00�����óɹ�
	// 0xff�����ݿ��쳣
	DWORD SetDboDBVersion(const SDBVersion& sDBVersion);
	//���������¶ȼ�����
	DWORD SetDboRealTimeDataOfDIM(const REALTIMEDATAOFDIM &sRealTimeDataOfDIM);
	//���������¶ȼ���־
	DWORD SetDboLogOfDIM(const LOGOFDIM &sLogOfDIM);
	//��ȡ�����¶ȼ�����
	DWORD GetDboRealTimeDataOfDIM(const wchar_t wchSql[SQLMAX], PREALTIMEDATAOFDIM const pRealTimeDataOfDIM);
	//��ȡ�����¶ȼ���־
	DWORD GetDboLogOfDIM(const wchar_t wchSql[SQLMAX], PLOGOFDIM const pLogOfDIM);
	//ɾ�������¶ȼ�����
	DWORD DelDboRealTimeDataOfDIM(const wchar_t wchSql[SQLMAX]);
	//ɾ�������¶ȼ���־
	DWORD DelDboLogOfDIM(const wchar_t wchSql[SQLMAX]);
	//ͬ��������ʱ�䵽����
	DWORD SynDBTime(void);
	DWORD SynDBTimeWithMilliseconds(void);
	//��ȡ��������
	//���أ�0x00 �����ݼ�¼;0x01 �����ݼ�¼;0xff ����
	DWORD GetEPDataFromDBNew(wchar_t *const pwchEquNum=NULL, 
		float *const pfET=NULL, 
		float *const pfRH=NULL, 
		float *const pfAP=NULL, 
		float *const pfOilTemp=NULL, 
		USHORT *const pusRpm=NULL, 
		PREALTIMEDATAOFDIM const pRealTimeDataOfDIM=NULL
		);

	//��ȡOBD�������
	DWORD GetDboResultOfOBD(const wchar_t wchSql[SQLMAX], SResultOfOBD* const pResultOfOBD);
	//����OBD�������
	DWORD SetDboResultOfOBD(const SResultOfOBD &sResultOfOBD);
	
	DWORD GetDboResultOfAppearance(const wchar_t wchSql[SQLMAX], SResultOfAppearance* const pResultOfAppearance);
	DWORD SetDboResultOfAppearance(const SResultOfAppearance &sResultOfAppearance);

	//��ȡȼ������ϵͳ�������
	DWORD GetDboResultOfEECS(const wchar_t wchSql[SQLMAX], SResultOfEECS* const pResultOfEECS);
	//����ȼ������ϵͳ�������
	DWORD SetDboResultOfEECS(const SResultOfEECS &sResultOfEECS);

	//��ȡ�ָ����̶Ƚ������
	DWORD GetDboResultOfRingelmann(const wchar_t wchSql[SQLMAX], SResultOfRingelmann* const pResultOfRingelmann);
	//�����ָ����̶Ƚ������
	DWORD SetDboResultOfRingelmann(const SResultOfRingelmann &sResultOfRingelmann);

	// �������/¼������
	DWORD GetDboAppPhotoInfo(const wchar_t wchSql[SQLMAX], std::vector<SAppPhotoInfo> &vtAppPhotoInfo);
	DWORD SetDboAppPhotoInfo(const std::vector<SAppPhotoInfo> &vtAppPhotoInfo);

	// ����/¼�����ݱ����
	DWORD GetDboTestPhotoData(const wchar_t wchSql[SQLMAX], std::vector<STestPhotoData> &vtTestPhotoData);
	DWORD SetDboTestPhotoData(const std::vector<STestPhotoData> &vtTestPhotoData);

	// ��ȡ�������޸ĵĲ���
	DWORD GetDboWhiteModifiedParameters(const wchar_t wchSql[SQLMAX], SWhiteModifiedParameters* const pWhiteModifiedParameters);
	DWORD GetDboWhiteModifiedParameters(const wchar_t wchSql[SQLMAX], std::vector<SWhiteModifiedParameters> & vtWhiteModifiedParameters);
	// ���ð������޸ĵĲ���
	DWORD SetDboWhiteModifiedParameters(const SWhiteModifiedParameters &sWhiteModifiedParameters);

#endif