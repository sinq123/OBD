#ifndef EQU_OPE_H
#define EQU_OPE_H

extern "C"
{
	// 分析仪
	DWORD AFX_EXT_API ShowNHA503XGasCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	DWORD AFX_EXT_API ShowNHAMPGasCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	// 4气,无NO通道,用分析仪统一通讯协议及检查流程
	void AFX_EXT_API ShowNHA403XGasCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	// 5气,有NO通道,用503系列通讯协议及检查流程
	void AFX_EXT_API ShowNHA503XGasCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	// 5气,有NO通道,导入外部限值进行结果检查判定
	void AFX_EXT_API ShowNHA503XGasCheckDlg2(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	DWORD AFX_EXT_API LoadNHA503XEnvParamsCalibrationDlg(void);
	DWORD AFX_EXT_API LoadNHAGasResponseTimeDlg(void);

	// NO,NO2通道气体校准
	void AFX_EXT_API ShowNHNOxCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	// NO,NO2通道气体检查
	void AFX_EXT_API ShowNHNOxCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	
	// 烟度计
	DWORD AFX_EXT_API LoadYD1ChkDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);

	// 参数仪
	DWORD AFX_EXT_API LoadParameterEnvParamsCalibrationDlg(void);
	DWORD AFX_EXT_API LoadParameterEnvParamsCalibrationXDlg(void);
	DWORD AFX_EXT_API LoadParameterParamsCheckDlg(void);

	// 转速计
	DWORD AFX_EXT_API LoadTachometerCheckDlg(void);
	
	// OBD
	DWORD AFX_EXT_API LoadOBDOperationDlg(void);

	// 不透光
	void AFX_EXT_API ShowNHT1LCalChkDlg();
	DWORD AFX_EXT_API ShowNHT6CalChkDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);

	// 测功机
	void AFX_EXT_API ShowNHCCommonOperationDlg(void);
	void AFX_EXT_API ShowNHDCommonOperationDlg(void);
	DWORD AFX_EXT_API ShowNHCConstLoadDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	DWORD AFX_EXT_API ShowMHDConstLoadDlg(void);
	DWORD AFX_EXT_API ShowNHCControlModeDlg(void);
	DWORD AFX_EXT_API ShowNHDControlModeDlg(void);
	void AFX_EXT_API ShowNHCDIWDlg(void);
	void AFX_EXT_API ShowNHDDIWDlg(void);
	void AFX_EXT_API ShowNHCForceCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	void AFX_EXT_API ShowMHDForceCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	// 重型测功机力校准
	void AFX_EXT_API ShowNHCForceCalibrationHeavyDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	void AFX_EXT_API ShowNHCForceCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	void AFX_EXT_API ShowMHDForceCheckDlg(void);
	void AFX_EXT_API ShowNHCForceCheckHeavyDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	//行业标准滑行
	DWORD AFX_EXT_API ShowNHCPLHPDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
    //JJF1221-2009滑行标准
	DWORD AFX_EXT_API ShowNHCPLHP2Dlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	//GB2018滑行标准
	DWORD AFX_EXT_API ShowNHCPLHPGB2018Dlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	DWORD AFX_EXT_API ShowMHDPLHPDlg(void);
	void AFX_EXT_API ShowNHCReadWriteVarDlg(void);
	void AFX_EXT_API ShowNHDReadWriteVarDlg(void);
	DWORD AFX_EXT_API ShowNHCResponseTime(void);
	// 响应时间 jjf1221-2009
	DWORD AFX_EXT_API ShowNHCResponseTime2(void);
	DWORD AFX_EXT_API ShowDynamometerTemperatureCheckDlg(void);
	// JJF 1221-2009
	DWORD AFX_EXT_API ShowNHCVarLoadDlg();
	//变载荷滑行测试HJ/T290-2006
	DWORD AFX_EXT_API ShowNHCVarLoadDlg2();

	// 流量计
	DWORD AFX_EXT_API LoadFlowmeterDelayTimeConfigDlg(void);
	DWORD AFX_EXT_API LoadFlowmeterDilO2CalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	DWORD AFX_EXT_API LoadFlowmeterFlowCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	DWORD AFX_EXT_API LoadFlowmeterFreeFlowCheckDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen)= NULL);
	DWORD AFX_EXT_API LoadFlowmeterPressureCalibrationDlg(DWORD (CALLBACK * pfProcessCtrl)(const DWORD dwStatus, const void* pBuf, const LONG lBufLen) = NULL);
	DWORD AFX_EXT_API LoadFlowmeterRealTimeDateDlg(void);
	DWORD AFX_EXT_API LoadFlowmeterTPCalDlg(void);
	DWORD AFX_EXT_API LoadFlowmeterExternalCalDlg(void);
}

#endif