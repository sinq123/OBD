// EquOpe.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "EquOpe.h"

#include "NHA503XGasCalibrationDlg.h"
#include "NHAMPGasCalibrationDlg.h"
#include "NHA403XGasCheckDlg.h"
#include "NHA503XGasCheckDlg.h"
#include "NHA503XGasCheckDlg2.h"
#include "NHA503XEnvParamsCalibrationDlg.h"
#include "NHAGasResponseTimeDlg.h"

#include "NHNOxCalibrationDlg.h"
#include "NHNOxCheckDlg.h"

#include "YD1CheckDlg.h"

#include "ParameterEnvParamsCalibrationDlg.h"
#include "ParameterEnvParamsCalibrationXDlg.h"
#include "ParameterParamsCheckDlg.h"

#include "TachometerCheckDlg.h"
#include "OBDOperationDlg.h"

#include "NHT1LCalibrationCheckDlg.h"
#include "NHT6CalChkDlg.h"

#include "NHCCommonOperationDlg.h"
#include "NHCConstLoadDlg.h"
#include "NHCControlModeDlg.h"
#include "NHCDIWDlg.h"
#include "NHCForceCalibrationDlg.h"
#include "MHDForceCalibrationDlg.h"
#include "NHCForceCalibrationHeavyDlg.h"
#include "NHCForceCheckDlg.h"
#include "NHCForceCheckHeavyDlg.h"
#include "NHCPLHPDlg.h"
#include "NHCPLHP2Dlg.h"
#include "NHCPLHPGB2018Dlg.h"
#include "NHCReadWriteVarDlg.h"
#include "NHCResponseTimeDlg.h"
#include "NHCResponseTime2Dlg.h"
#include "DynamometerTemperatureCheckDlg.h"
#include "NHCVarLoadDlg.h"
#include "NHCVarLoadDlg2.h"

#include "FlowmeterDelayTimeConfigDlg.h"
#include "FlowmeterDilO2CalibrationDlg.h"
#include "FlowmeterFlowCalibrationDlg.h"
#include "FlowmeterFreeFlowCheckDlg.h"
#include "FlowmeterPressureCalibrationDlg.h"
#include "FlowmeterRealTimeDataDlg.h"
#include "FlowmeterTPCalDlg.h"
#include "FlowmeterExternalAgreementCalDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern AFX_EXTENSION_MODULE EquOpeDLL;

DWORD ShowNHA503XGasCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHA503XGasCalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD ShowNHAMPGasCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHAMPGasCalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

void ShowNHA403XGasCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHA403XGasCheckDlg dlg;
	//dlg.SetWriteAnaGasChkDataCallBack(pfWriteAnaGasChkData);
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowNHA503XGasCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHA503XGasCheckDlg dlg;
	//dlg.SetWriteAnaGasChkDataCallBack(pfWriteAnaGasChkData);
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowNHA503XGasCheckDlg2(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHA503XGasCheckDlg2 dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

DWORD LoadNHA503XEnvParamsCalibrationDlg(void)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHA503XEnvParamsCalibrationDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD LoadNHAGasResponseTimeDlg(void)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHAGasResponseTimeDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

void ShowNHNOxCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHNOxCalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowNHNOxCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHNOxCheckDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

DWORD LoadYD1ChkDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CYD1CheckDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD LoadParameterEnvParamsCalibrationDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CParameterEnvParamsCalibrationDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD LoadParameterEnvParamsCalibrationXDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CParameterEnvParamsCalibrationXDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD LoadParameterParamsCheckDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CParameterParamsCheckDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD LoadTachometerCheckDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CTachometerCheckDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD LoadOBDOperationDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	COBDOperationDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

void ShowNHT1LCalChkDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHT1LCalibrationCheckDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

DWORD ShowNHT6CalChkDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHT6CalChkDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

void ShowNHCCommonOperationDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCCommonOperationDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowNHDCommonOperationDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	// TODO:待完成

	AfxSetResourceHandle(hOld);
}

DWORD ShowNHCConstLoadDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCConstLoadDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD ShowMHDConstLoadDlg(void)
{
	return 0;
}

DWORD ShowNHCControlModeDlg(void)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCControlModeDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD ShowNHDControlModeDlg(void)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	// TODO:待完成

	AfxSetResourceHandle(hOld);

	return 0;
}

void ShowNHCDIWDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCDIWDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowNHDDIWDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	// TODO:待完成

	AfxSetResourceHandle(hOld);
}

void ShowNHCForceCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCForceCalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowMHDForceCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CMHDForceCalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowNHCForceCalibrationHeavyDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCForceCalibrationHeavyDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowNHCForceCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCForceCheckDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowMHDForceCheckDlg(void)
{}

void ShowNHCForceCheckHeavyDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCForceCheckHeavyDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);	
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

DWORD ShowNHCPLHPDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCPLHPDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD ShowNHCPLHP2Dlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCPLHP2Dlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD ShowNHCPLHPGB2018Dlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCPLHPGB2018Dlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD ShowMHDPLHPDlg(void)
{
	return 0;
}

void ShowNHCReadWriteVarDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCReadWriteVarDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);
}

void ShowMHDReadWriteVarDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	// TODO:待完成

	AfxSetResourceHandle(hOld);
}

DWORD ShowNHCResponseTime()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCResponseTimeDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD ShowDynamometerTemperatureCheckDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CDynamometerTemperatureCheckDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD ShowNHCVarLoadDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCVarLoadDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD ShowNHCVarLoadDlg2()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCVarLoadDlg2 dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD LoadFlowmeterDelayTimeConfigDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterDelayTimeConfigDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD LoadFlowmeterDilO2CalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterDilO2CalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD LoadFlowmeterFlowCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterFlowCalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD LoadFlowmeterFreeFlowCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterFreeFlowCheckDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD LoadFlowmeterPressureCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)/*=NULL*/)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterPressureCalibrationDlg dlg;
	dlg.SetProcessCtrlCallBack(pfProcessCtrl);
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD LoadFlowmeterRealTimeDateDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterRealTimeDataDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD ShowNHCResponseTime2()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CNHCResponseTime2Dlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0x00;
}

DWORD LoadFlowmeterTPCalDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterTPCalDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}

DWORD LoadFlowmeterExternalCalDlg()
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(EquOpeDLL.hResource);

	CFlowmeterExternalCalDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(hOld);

	return 0;
}