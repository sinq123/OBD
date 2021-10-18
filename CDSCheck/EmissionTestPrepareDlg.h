#pragma once

#include "DieselVehicleEquipmentPrepareDlg.h"
#include "GasolineVehicleEquipmentPrepareDlg.h"
#include "GasolineVehicleEquipmentPrepareNDlg.h"
#include "afxdtctl.h"

// CEmissionTestPrepareDlg 对话框

class CEmissionTestPrepareDlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CEmissionTestPrepareDlg)

public:
	CEmissionTestPrepareDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmissionTestPrepareDlg();

// 对话框数据
	enum { IDD = IDD_EMISSION_TEST_PREPARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnCbnSelchangeCbFilterTestType();
	afx_msg void OnEnChangeEdFilter();
	afx_msg void OnNMDblclkLstVehicle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnBnClickedButtonLiftBeamUp();
	afx_msg void OnBnClickedButtonLiftBeamDown();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnChangeVehicleInfo();
	afx_msg void OnBnClickedBtnLambdaLimit();
	
private:
	CListCtrl m_lstVehicle;
	CComboBox m_cbFilterTestType;
	CComboBox m_cbDriver;
	CEdit m_edFilter;
	CShadeTitle m_lbHead;
	CShadeTitle m_lbVehicleList;
	CShadeTitle m_lbVehicleInfo;
	CShadeTitle m_lbEParamInfo;
	CShadeTitle m_lbAcceptanceTime;

	//
	CDateTimeCtrl m_dtcAcceptanceStartTime;		// 开始日期变量
	CDateTimeCtrl  m_dtcAcceptanceEndTime;		// 结束日期变量

	// 测功机信息框
	CLabel m_lbDynInfo;
	CLabel m_lbEPPortInfo;

	// 车辆信息
	CComboBox m_cbTestType;
	CComboBox m_cbPlateType;
	CComboBox m_cbDriveType;
	CComboBox m_cbFuelType;
	CComboBox m_cbOilSupplyMode;
	CComboBox m_cbAirIntakeMode;
	CComboBox m_cbHasCatalyticConverter;
	CEdit m_edPlateNumber;
	CEdit m_edOwner;
	CEdit m_edVIN;
	CEdit m_edModel;
	CEdit m_edIsRetest;
	CEdit m_edUnladenMass;
	CDateTimeCtrl m_dtcProductionDate;
	CDateTimeCtrl m_dtcRegistrationDate;
	CNumericEdit m_edMaximumTotalMass;
	CNumericEdit m_edRatedPower;
	CNumericEdit m_edRatedRev;
	CEdit m_edItemApp;
	CEdit m_edItemOBD;
	CEdit m_edItemEmi;
	CNumericEdit m_edLambdaMin;
	CNumericEdit m_edLambdaMax;

	//环境参数
	CEdit m_edET;
	CEdit m_edRH;
	CEdit m_edAP;
	CEdit m_edOT;
	CEdit m_edRPM;
	
	// 引车员
	CString m_strDriver;
	// 数据库连接
	_ConnectionPtr m_pConnection;

private:
	// 连接数据库
	DWORD InitDatabase(void);
	// 关闭数据库连接
	DWORD UnInitDatabase(void);
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);
	// 初始化待检测列表
	void InitVehicleList(void);

	// 初始化车辆信息显示区域
	void InitVehicleInfoRegion(BOOL Enable=FALSE);
	// 初始化TestType Combobox
	void InitTestTypeCombobox(void);
	// 初始化引车员列表
	void InitDriverCombobox(void);
	// 初始化车牌类型
	void InitComboboxPlateType(void);
	// 初始化驱动方式
	void InitComboboxDriveType(void);
	// 初始化燃油种类
	void InitComboboxFuelType(void);
	// 初始化供油方式
	void InitComboboxOilSupplyMode(void);
	// 初始化进气方式
	void InitComboboxAirIntakeMode(void);
	// 初始化设备准备区域
	void InitEquipmentPrepareRegion(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	// 待检车ListCtrl字体
	CFont m_fontVehicleLCFont;

	// 系统类型
	NHCLIENT m_sIniNHClient;

	wchar_t m_wchPathOfNHClient[MAX_PATH];

private:
	// 汽油车设备准备
	CGasolineVehicleEquipmentPrepareNDlg m_cGasolineVehicleEquipmentPrepareNDlg;
	// 柴油车设备准备
	CDieselVehicleEquipmentPrepareDlg m_cDieselVehicleEquipmentPrepareDlg;
	// 设备准备区域列表
	short m_siPrepareRegionIndex;
	// 加载分析仪设备准备窗口
	void LoadGasolineVehicleEquipmentPrepare(void);
	// 加载不透光设备准备窗口
	void LoadDieselVehicleEquipmentPrepare(void);

private:
	// 当前选中车辆流水号
	CString m_strCurrentRunningNumber;
	// 当前选中车辆测试类型
	CString m_strCurrentTestType;
	
private:
	//获取环境参数
	void GetEPModeSimu(void);
	// 获取车辆信息
	void GetIntVehInfoList(void);
	// 获取车辆信息
	void GetIntVehInfo(void);

private:
	TESTLOG m_sTestLog;
	VEHICLEINFO m_sVehInfo;
};
