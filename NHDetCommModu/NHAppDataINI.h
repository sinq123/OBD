/*
* Copyright (c) 2009,��ɽ���ϻ��������޹�˾
* All rights reserved.
*
* �ļ����ƣ�NHAppDataINI.h
* �ļ���ʶ��
* ժ    Ҫ��App_Data���������ļ���������,���ݽṹ��ʽ����
*
* ��ǰ�汾��1.0
* ��    �ߣ�HB
* ������ڣ�2010-07-14
*
*
* ��ʷ��Ϣ��
*
* �汾��1.0.19
* ���ڣ�2013-02-28
* ���ߣ�Chc
* ����������ASM��VMAS��DIS��LUGDOWN����vector��ʽ��ȡ�������ݺ���GetIniRealTimeDataOfXXXXXXVector
*
* �汾��1.0.18
* ���ڣ�2012-08-13
* ���ߣ�Wu
* ������1)����SetIniUserInfo,���д���û����빦��
*		2)����GetIniUserInfo,��ӻ�ȡ�û����빦��
*
* �汾��1.0.17
* ���ڣ�2012-07-05
* ���ߣ�Raylee
* ����������GetIniLiteString()��ȡiniʱָ���Ļ���������Ϊwchar_t���ͳ���
*
* �汾��1.0.16
* ���ڣ�2011-09-28
* ���ߣ�Raylee
* ���������Ħ�г�˫������ؽṹ��ini��д����
*
* �汾��1.0.15
* ���ڣ�2011-08-24
* ���ߣ�hemincong
* ������ʹ��inilite��дini�ļ�
*
* �汾��1.0.14
* ���ڣ�2011-06-16
* ���ߣ�Sjming
* ����������CNHReadIni�಻�ȶ����ָֻ�ʹ��API������ȡIni�ļ�
*
* �汾��1.0.13
* ���ڣ�2011-06-01
* ���ߣ�hemincong
* �������Դ󲿷�GetIni��������CNHReadIni��ȡ
*
* �汾��1.0.12
* ���ڣ�2011-05-26
* ���ߣ�Raylee
* ������1�����FSUNHT��FSUYD���ini�Ա����ֶεĲ���
*		2��������ȡRemark�ֶλ���������Ϊ1024
*
* �汾��1.0.11
* ���ڣ�2011-05-18
* ���ߣ�Ming
* ����������GetIniRealTimeDataOfxxx����Ӳ���ʱ��SamplingTime�Ķ�ȡ����
*
* �汾��1.0.10
* ���ڣ�2011-04-15
* ���ߣ�Ming
* ������GetIniVehicleInfo/SetIniVehicleInfo������Ӷ�OwnerCharacter��OwnerDistrict��LicenseDepartment��LicenseIssueDate��LicenseValidDate�Ķ�д����
*		GetDboTestLog/SetDboTestLog������Ӷ�IsSentEP��OwnerCharacter��OwnerDistrict��LicenseDepartment��LicenseIssueDate��LicenseValidDate�Ķ�д����
*
* �汾��1.0.9
* ���ڣ�2011-04-02
* ���ߣ�Raylee
* ������GetIniSystemParameters/SetIniSystemParameters������Ӷ�дPeriodOfDynamometerForceCheck����
*
* �汾��1.0.8
* ���ߣ�Ming
* ���ڣ�2011-02-23
* ��������������GetIniResultOfFloFluxChk()����ResultOfFloFluxChk���Bug
*
* �汾��1.0.7
* ���ߣ�Raylee
* ���ڣ�2011-01-10
* �������������ݽṹ�ĸı䣬�޸ĺ���GetIniResultOfDynForceChk()
*
* �汾��1.0.6
* ���ߣ�Ming
* ���ڣ�2011-01-10
* ������1)����GetIniTestLog��SetIniTestLog,��ӳ�ԱEmissionStandard��EngineStroke��AnnualTestPeriod��HasOBD�Ļ�ȡ��д��
*
* �汾��1.0.5
* ���ߣ�Raylee
* ���ڣ�2011-01-05
* �������޸ĺ���GetIniResultOfDynForceChk()
*
* �汾��1.0.4
* ���ߣ�Ming
* ���ڣ�2010-12-11
* ������1)����GetIniBillInfo��GetIniTestLog,�ֱ���ӳ�ԱNumberOfTestPeriodAVFSU(��������ڵ�AVFSU������)��NumberOfTestPeriodMI(��������ڵ�MI������)�Ļ�ȡ
*		2)����SetIniBillInfo��SetIniTestLog,�ֱ���ӳ�ԱNumberOfTestPeriodAVFSU(��������ڵ�AVFSU������)��NumberOfTestPeriodMI(��������ڵ�MI������)��д��
*		3)��Ӻ���SetIniResultOfAVFSU��GetIniResultOfAVFSU ũ�ó����ɼ��ٽ�����ݵĻ�ȡ��д��
*		4)��Ӻ���GetIniRealTimeDataOfMI��SetIniResultOfMI��GetIniResultOfMI Ħ�г����ٽ�����ݵĻ�ȡ��д�룬ʵʱ���ݵĻ�ȡ
*
* �汾��1.0.4
* ���ߣ�Ming
* ���ڣ�2010-09-21
* ������1)����GetIniRealTimeDataOfASM��GetIniRealTimeDataOfLUGDOWN��GetIniRealTimeDataOfVMAS,�ֱ���ӳ�ԱIHP(ָʾ����)��PLHP(��������)�Ļ�ȡ
*
* �汾��1.0.3
* ���ߣ�Ming
* ���ڣ�2010-09-20
* ������1)����GetIniVehicleInfo��SetIniVehicleInfo,��ӳ�ԱEmissionStandard��EngineStroke��AnnualTestPeriod��HasOBD�Ļ�ȡ��д��
*
* �汾��1.0.2
* ���ߣ�Ming
* ���ڣ�2010-09-07
* ������1)����GetIniSystemParameters��SetIniSystemParameters,��ӳ�ԱPeriodOfEnvParamMeasurerCheck��TestPeriod�Ļ�ȡ��д��
*		2)��������SetIniSystemParameters�����ֶ���д�����bug
*
* �汾��1.0.1
* ���ߣ�Ming
* ���ڣ�2010-08-13
* ��������ӻ�ȡ��͸�����ɼ���ʵʱ���ݺ���GetIniRealTimeDataOfFSUNHT.
*/

#ifndef NHAPPDATAINI_H
#define NHAPPDATAINI_H

// ��ע��������ת����wchar_t���ձ�
// int wchar_t[11] �з���
// short wchar_t[6] �з���
// time wchar_t[19] ��ȷ���� 2008-08-08 23:59:59

#include "NHDef.h"
#include "NHStructDef.h"
#include <list>
#include <vector>
using std::list;
using std::vector;

// App_Data
// �û���Ϣ UserInfo.ini

void GetIniUserInfo(PUSERINFO const pIniUserInfo);
void SetIniUserInfo(const USERINFO& iniUserInfo);

// ������Ϣ<BillInfo>
//void GetIniBillInfo(PBILLINFO const pIniBillInfo);
//void SetIniBillInfo(const BILLINFO& iniBillInfo);

// ������Ϣ<VehicleInfo>
void GetIniVehicleInfo(PVEHICLEINFO const pIniVehicleInfo);
void SetIniVehicleInfo(const VEHICLEINFO& iniVehicleInfo);

// ������Ϣ<VehicleInfo>
void GetIniVehicleModel(PVEHICLEMODEL const pIniVehicleModel);
void SetIniVehicleModel(const VEHICLEMODEL& iniVehicleModel);

// ASM��ֵ��Ϣ<LimitOfASM>
void GetIniLimitOfASM(PLIMITOFASM const pIniLimitOfASM);
void SetIniLimitOfASM(const LIMITOFASM& iniLimitOfASM);

// VMAS��ֵ��Ϣ<LimitOfVMAS>
void GetIniLimitOfVMAS(PLIMITOFVMAS const pIniLimitOfVMAS);
void SetIniLimitOfVMAS(const LIMITOFVMAS& iniLimitOfVMAS);

// LimitOfLUGDOWN��ֵ��Ϣ<LimitOfLimitOfLUGDOWN>
void GetIniLimitOfLUGDOWN(PLIMITOFLUGDOWN const pIniLimitOfLUGDOWN);
void SetIniLimitOfLUGDOWN(const LIMITOFLUGDOWN& iniLimitOfLUGDOWN);

// DIS��ֵ��Ϣ<LimitOfDIS>
void GetIniLimitOfDIS(PLIMITOFDIS const pIniLimitOfDIS);
void SetIniLimitOfDIS(const LIMITOFDIS& iniLimitOfDIS);

// FSUNHT��ֵ��Ϣ<LimitOfFSUNHT>
void GetIniLimitOfFSUNHT(PLIMITOFFSUNHT const pIniLimitOfFSUNHT);
void SetIniLimitOfFSUNHT(const LIMITOFFSUNHT& iniLimitOfFSUNHT);

// FSUNHTOfHeavyVehicle��ֵ��Ϣ<LimitOfFSUNHTOfHeavyVehicle>
void GetIniLimitOfFSUNHTOfHeavyVehicle(PLIMITOFFSUNHTOFHEAVYVEHICLE const pIniLimitOfFSUNHTOfHeavyVehicle);
void SetIniLimitOfFSUNHTOfHeavyVehicle(const LIMITOFFSUNHTOFHEAVYVEHICLE& iniLimitOfFSUNHTOfHeavyVehicle);

// FSUNHTOfLightVehicle��ֵ��Ϣ<LimitOfFSUNHTOfLightVehicle>
void GetIniLimitOfFSUNHTOfLightVehicle(PLIMITOFFSUNHTOFLIGHTVEHICLE const pIniLimitOfFSUNHTOfLightVehicle);
void SetIniLimitOfFSUNHTOfLightVehicle(const LIMITOFFSUNHTOFLIGHTVEHICLE& iniLimitOfFSUNHTOfLightVehicle);

// FSUYD��ֵ��Ϣ<LimitOfFSUYD>
void GetIniLimitOfFSUYD(PLIMITOFFSUYD const pIniLimitOfFSUYD);
void SetIniLimitOfFSUYD(const LIMITOFFSUYD& iniLimitOfFSUYD);

// �������Ϣ<LineInfo>
void GetIniLineInfo(PLINEINFO const pIniLineInfo);
void SetIniLineInfo(const LINEINFO& iniLineInfo);

// ���վ��Ϣ<StationInfo>
void GetIniStationInfo(PSTATIONINFO const pIniStationInfo);
void SetIniStationInfo(const STATIONINFO& iniStationInfo);

// �ŷż����־��Ϣ<TestLog>
void GetIniTestLog(PTESTLOG const pIniTestLog);
void SetIniTestLog(const TESTLOG& iniTestLog);

// DIS�����<ResultOfDIS>
void GetIniResultOfDIS(PRESULTOFDIS const pIniResultOfDIS);
void GetIniResultOfDISEx(SResultOfDIS* const pResultOfDIS);
void SetIniResultOfDIS(const RESULTOFDIS& iniResultOfDIS);

// DIS real time data<RealTimeDataOfDIS>
void GetIniRealTimeDataOfDIS(list<REALTIMEDATAOFDIS>& listRealTimeDataOfDIS);
void GetIniRealTimeDataOfDISVector(vector<REALTIMEDATAOFDIS>& vRealTimeDataOfDIS);
void GetIniRealTimeDataOfDISEx(std::vector<SRealTimeDataOfDIS> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfDIS.ini");

// ASM test result<ResultOfASM>
void GetIniResultOfASM(PRESULTOFASM const pIniResultOfASM);
void GetIniResultOfASMEx(SResultOfASM* const pResultOfASM);
void SetIniResultOfASM(const RESULTOFASM& iniResultOfASM);

// ASM real time data<RealTimeDataOfASM>
void GetIniRealTimeDataOfASM(list<REALTIMEDATAOFASM>& listRealTimeDataOfASM);
void GetIniRealTimeDataOfASMVector(vector<REALTIMEDATAOFASM>& vRealTimeDataOfASM);
void GetIniRealTimeDataOfASMEx(std::vector<SRealTimeDataOfASM> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfASM.ini");

// VMAS test result<ResultOfVMAS>
void GetIniResultOfVMAS(PRESULTOFVMAS const pIniResultOfVMAS);
void GetIniResultOfVMASEx(SResultOfVMAS* const pResultOfVMAS);
void SetIniResultOfVMAS(const RESULTOFVMAS& iniResultOfVMAS);

// VMAS real time data<RealTimeDataOfVMAS>
void GetIniRealTimeDataOfVMAS(list<REALTIMEDATAOFVMAS>& listRealTimeDataOfVMAS);
void GetIniRealTimeDataOfVMASVector(vector<REALTIMEDATAOFVMAS>& vRealTimeDataOfVMAS);
void GetIniRealTimeDataOfVMASEx(std::vector<SRealTimeDataOfVMAS> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfVMAS.ini");

// LUGDOWN test result<ResultOfLUGDOWN>
void GetIniResultOfLUGDOWN(PRESULTOFLUGDOWN const pIniResultOfLUGDOWN);
void GetIniResultOfLUGDOWNEx(SResultOfLUGDOWN* const pResultOfLUGDOWN);
void SetIniResultOfLUGDOWN(const RESULTOFLUGDOWN& iniResultOfLUGDOWN);

// LUGDOWN real time data<RealTimeDataOfLUGDOWN>
void GetIniRealTimeDataOfLUGDOWN(list<REALTIMEDATAOFLUGDOWN>& listRealTimeDataOfLUGDOWN);
void GetIniRealTimeDataOfLUGDOWNVector(vector<REALTIMEDATAOFLUGDOWN>& vRealTimeDataOfLUGDOWN);
void GetIniRealTimeDataOfLUGDOWNEx(std::vector<SRealTimeDataOfLUGDOWN> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfLUGDOWN.ini");

// FSUNHT test result<ResultOfFSUNHT>
void GetIniResultOfFSUNHT(PRESULTOFFSUNHT const pIniResultOfFSUNHT);
void GetIniResultOfFSUNHTEx(SResultOfFSUNHT* const pResultOfFSUNHT);
void SetIniResultOfFSUNHT(const RESULTOFFSUNHT& iniResultOfFSUNHT);

// FSUNHT real time data<RealTimeDataOfFSUNHT>
void GetIniRealTimeDataOfFSUNHT(list<REALTIMEDATAOFFSUNHT>& listRealTimeDataOfFSUNHT);
void GetIniRealTimeDataOfFSUNHTVector(vector<REALTIMEDATAOFFSUNHT>& vRealTimeDataOfFSUNHT);
void GetIniRealTimeDataOfFSUNHTEx(std::vector<SRealTimeDataOfFSUNHT> &vtRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfFSUNHT.ini");

// FSUYD test result<ResultOfFSUYD>
void GetIniResultOfFSUYD(PRESULTOFFSUYD const pIniResultOfFSUYD);
void SetIniResultOfFSUYD(const RESULTOFFSUYD& iniResultOfFSUYD);

// AVFSU test result<ResultOfAVFSU>
void GetIniResultOfAVFSU(PRESULTOFAVFSU const pIniResultOfAVFSU);
void SetIniResultOfAVFSU(const RESULTOFAVFSU& iniResultOfAVFSU);

// MI test result<ResultOfMI>
void GetIniResultOfMI(PRESULTOFMI const pIniResultOfMI);
void SetIniResultOfMI(const RESULTOFMI& iniResultOfMI);

// MI real time data<RealTimeDataOfMI>
void GetIniRealTimeDataOfMI(list<REALTIMEDATAOFMI>& listRealTimeDataOfMI);

// MDIS test result<ResultOfMDIS>
void GetIniResultOfMDIS(PRESULTOFMDIS const pIniResultOfMDIS);
void SetIniResultOfMDIS(const RESULTOFMDIS& iniResultOfMDIS);

// MDIS real time data<RealTimeDataOfMDIS>
void GetIniRealTimeDataOfMDIS(list<REALTIMEDATAOFMDIS>& listRealTimeDataOfMDIS);

// �豸У׼�����Ϣ<EquCalChkInfo>
void GetIniEquCalChkInfo(PEQUCALCHKINFO const pIniEquCalChkInfo);
void SetIniEquCalChkInfo(const EQUCALCHKINFO& iniEquCalChkInfo);

// �豸ϵͳ��־<SystemLog>
void GetIniSystemLog(PSYSTEMLOG const pIniSystemLog);
void SetIniSystemLog(const SYSTEMLOG& iniSystemLog);

// �궨��־��Ϣ<DemarcationLog>
void GetIniDemarcationLog(PDEMARCATIONLOG const pIniDemarcationLog);
void SetIniDemarcationLog(const DEMARCATIONLOG& iniDemarcationLog);

// ����������У׼���<DemarcationResultOfAnaGasCal>
void GetIniResultOfAnaGasCal(PDEMARCATIONRESULTOFANAGASCAL const pIniResultOfAnaGasCal);

// ��������������<DemarcationResultOfAnaGasChk>
void GetIniResultOfAnaGasChk(PDEMARCATIONRESULTOFANAGASCHK const pIniResultOfAnaGasChk, wchar_t *pwchSaveFileName=L"ResultOfAnaGasChk.ini");
void GetIniRealTimeDataOfAnaGasChk(list<DEMARCATIONREALTIMEDATAOFANAGASCHK>& listRealTimeDataOfAnaGasChk);

// ��͸��ȼ�У׼�����<DemarcationResultOfOpaCalChk>
void GetIniResultOfOpaCalChk(PDEMARCATIONRESULTOFOPACALCHK const pIniResultOfOpaCalChk);

// ��ֽʽ�̶ȼƼ����<DemarcationResultOfSmoChk>
void GetIniResultOfSmoChk(PDEMARCATIONRESULTOFSMOCHK const pIniResultOfSmoChk);

// �⹦�����ػ��н��<DemarcationResultOfDynConstLoad>
void GetIniResultOfDynConstLoad(PDEMARCATIONRESULTOFDYNCONSTLOAD const pIniResultOfDynConstLoad);
void GetIniRealTimeDataOfDynConstLoad(list<DEMARCATIONREALTIMEDATAOFDYNCONSTLOAD>& listRealTimeDataOfDynConstLoad);

// �⹦����У׼���<DemarcationResultOfDynForceCal>
void GetIniResultOfDynForceCal(PDEMARCATIONRESULTOFDYNFORCECAL const pIniResultOfDynForceCal);

// �⹦���������<DemarcationResultOfDynForceChk>
void GetIniResultOfDynForceChk(PDEMARCATIONRESULTOFDYNFORCECHK const pIniResultOfDynForceChk);

// �⹦���������ʻ��н��<DemarcationResultOfDynPLHP>
void GetIniResultOfDynPLHP(PDEMARCATIONRESULTOFDYNPLHP const pIniResultOfDynPLHP);

// ����������У׼���<DemarcationResultOfFloFluxCal>
void GetIniResultOfFloFluxCal(PDEMARCATIONRESULTOFFLOFLUXCAL const pIniResultOfFloFluxCal);

// ���������������<DemarcationResultOfFloFluxChk>
void GetIniResultOfFloFluxChk(PDEMARCATIONRESULTOFFLOFLUXCHK const pIniResultOfFloFluxChk);

// ������ϡ����У׼���<DemarcationResultOfFloDilO2Cal>
void GetIniResultOfFloDilO2Cal(PDEMARCATIONRESULTOFFLODILO2CAL const pIniResultOfFloDilO2Cal);

// ������ѹ��У׼���<DemarcationResultOfFloPresCal>
void GetIniResultOfFloPresCal(PDEMARCATIONRESULTOFFLOPRESCAL const pIniResultOfFloPresCal);

// ϵͳ����<SystemParameters>
void GetIniSystemParameters(PSYSTEMPARAMETERS const pIniSystemParameters);
void SetIniSystemParameters(const SYSTEMPARAMETERS& iniSystemParameters);

// OBD�����<ResultOfOBD>
void  GetIniResultOfOBD(SResultOfOBD* const pResultOfOBD);
void  SetIniResultOfOBD(const SResultOfOBD& sResultOfOBD);

// VMAS idle data<RealTimeDataOfVMAS>
void GetIniIdleDataOfVMAS(list<REALTIMEDATAOFVMAS>& listIdleDataOfVMAS);
void SetIniIdleDataOfVMAS(REALTIMEDATAOFVMAS* pIdleDataOfVMAS);

// DIS idle data<RealTimeDataOfDIS>
void GetIniIdleDataOfDIS(list<REALTIMEDATAOFDIS>& listIdleDataOfDIS);
void SetIniIdleDataOfDIS(REALTIMEDATAOFDIS* pIdleDataOfDIS);

void SetIniRealTimeDataOfDISEx(std::list<SRealTimeDataOfDIS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfDIS.ini");//����ʵʱ����
void SetIniRealTimeDataOfVMASEx(std::list<SRealTimeDataOfVMAS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfVMAS.ini");//����ʵʱ����
void SetIniRealTimeDataOfFSUNHTEx(std::list<SRealTimeDataOfFSUNHT> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfFSUNHT.ini");//����ʵʱ����
void SetIniRealTimeDataOfLUGDOWNEx(std::list<SRealTimeDataOfLUGDOWN> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfLUGDOWN.ini");//����ʵʱ����
void SetIniRealTimeDataOfASMEx(std::list<SRealTimeDataOfASM> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfASM.ini");//����ʵʱ����


void GetIniRealTimeDataOfDISEx(std::list<SRealTimeDataOfDIS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfDIS.ini");//��ʵʱ����
void GetIniRealTimeDataOfVMASEx(std::list<SRealTimeDataOfVMAS> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfVMAS.ini");//��ʵʱ����
void GetIniRealTimeDataOfFSUNHTEx(std::list<SRealTimeDataOfFSUNHT> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfFSUNHT.ini");//��ʵʱ����
void GetIniRealTimeDataOfLUGDOWNEx(std::list<SRealTimeDataOfLUGDOWN> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfLUGDOWN.ini");//��ʵʱ����
void GetIniRealTimeDataOfASMEx(std::list<SRealTimeDataOfASM> &lsRTData, wchar_t *pwchSaveFileName=L"RealTimeDataOfASM.ini");//��ʵʱ����

// ��������������<ResultOfAnaGasChk>
void SetIniResultOfAnaGasChk(const DEMARCATIONRESULTOFANAGASCHK &IniResultOfAnaGasChk, wchar_t *pwchSaveFileName=L"ResultOfAnaGasChk.ini");
#endif