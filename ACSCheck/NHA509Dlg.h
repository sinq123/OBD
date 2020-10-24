#pragma once

//临时改变宏的含义“输出”为“输入”
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_IMPORT
#define AFX_EXT_API AFX_API_IMPORT
#define AFX_EXT_DATA AFX_DATA_IMPORT


//恢复宏的含义为输出
#undef AFX_EXT_CLASS
#undef AFX_EXT_API
#undef AFX_EXT_DATA
#define AFX_EXT_CLASS AFX_CLASS_EXPORT
#define AFX_EXT_API AFX_API_EXPORT
#define AFX_EXT_DATA AFX_DATA_EXPORT

// CNHA509Dlg 对话框

class CNHA509Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CNHA509Dlg)

public:
	CNHA509Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNHA509Dlg();

	// 对话框数据
	enum { IDD = IDD_NHA_509LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedBtnInsSam();
	afx_msg void OnBnClickedBtnInsZero();
	afx_msg void OnBnClickedBtnInsReset();
	afx_msg void OnBnClickedBtnInsBlo();
	afx_msg void OnBnClickedBtnLinearDetection();
	afx_msg void OnBnClickedBtnLinearCheckOut();
	afx_msg void OnBnClickedBtnCheckInFive();
	afx_msg void OnBnClickedBtnCheckOutFive();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	void LoadConfig(void);
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;

private:
	CString m_strConfigIsNH;
	CString m_strConfigPEF;
private:
	// 标题栏
	CShadeTitle m_lbHead;
	// 信息提示栏
	CLabel m_lbInfo;

	// 上下标文本控件
	CMathLabel m_mlb1;
	CMathLabel m_mlb2;
	CMathLabel m_mlb3;
	CMathLabel m_mlb4;
	CMathLabel m_mlb5;
	CMathLabel m_mlb6;

	// HC实时值
	CLabel m_lbHC;
	// CO实时值
	CLabel m_lbCO;
	// CO2实时值
	CLabel m_lbCO2;
	// NO实时值
	CLabel m_lbNO;
	// O2实时值
	CLabel m_lbO2;
	// PEF实时值
	CLabel m_lbPEF;
	// HC测量值显示框
	CLabel m_lbHCMeasuredValue;
	// CO测量值显示框
	CLabel m_lbCOMeasuredValue;
	// CO2测量值显示框
	CLabel m_lbCO2MeasuredValue;
	// O2测量值显示框
	CLabel m_lbO2MeasuredValue;
	// NO测量值显示框
	CLabel m_lbNOMeasuredValue;
	// HC绝对误差显示框
	CLabel m_lbHCAE;
	// CO绝对误差显示框
	CLabel m_lbCOAE;
	// CO2绝对误差显示框
	CLabel m_lbCO2AE;
	// O2绝对误差显示框
	CLabel m_lbO2AE;
	// NO绝对误差显示框
	CLabel m_lbNOAE;
	// HC相对误差显示框
	CLabel m_lbHCRE;
	// CO相对误差显示框
	CLabel m_lbCORE;
	// CO2相对误差显示框
	CLabel m_lbCO2RE;
	// O2相对误差显示框
	CLabel m_lbO2RE;
	// NO相对误差显示框
	CLabel m_lbNORE;
	// HC判定显示框
	CLabel m_lbHCJudgement;
	// CO判定显示框
	CLabel m_lbCOJudgement;
	// CO2判定显示框
	CLabel m_lbCO2Judgement;
	// O2判定显示框
	CLabel m_lbO2Judgement;
	// NO判定显示框
	CLabel m_lbNOJudgement;
	// HC绝对误差显示框
	CLabel m_lbHCAE2;
	// CO绝对误差显示框
	CLabel m_lbCOAE2;
	// CO2绝对误差显示框
	CLabel m_lbCO2AE2;
	// O2绝对误差显示框
	CLabel m_lbO2AE2;
	// NO绝对误差显示框
	CLabel m_lbNOAE2;
	// HC相对误差显示框
	CLabel m_lbHCRE2;
	// CO相对误差显示框
	CLabel m_lbCORE2;
	// CO2相对误差显示框
	CLabel m_lbCO2RE2;
	// O2相对误差显示框
	CLabel m_lbO2RE2;
	// NO相对误差显示框
	CLabel m_lbNORE2;
	// IDC_COM_LINEAR_DETECTION
	CComboBox m_cbLinearDet;
	// IDC_COM_CHECK_IN_FIVE
	CComboBox m_cbChechInFive;
	// IDC_ST_TYPR_OF_EQUIPMENT
	CLabel m_lbTyprOfEqu;
	// C3H8标称值编辑框
	CEdit m_edC3H8NominalValue;
	// CO标称值编辑框
	CNumericEdit m_edCONominalValue;
	// CO2标称值编辑框
	CNumericEdit m_edCO2NominalValue;
	// NO标称值编辑框
	CEdit m_edNONominalValue;
	// HC标称值编辑框
	CEdit m_edHCNominalValue;
	// O2标称值编辑框
	CNumericEdit m_edO2NominalValue;

	// HC
	CLabel m_lbSJHC1;
	CLabel m_lbSJHC2;
	CLabel m_lbSJHC3;
	CLabel m_lbSJHC4;
	CLabel m_lbSJHC5;
	// CO
	CLabel m_lbSJCO1;
	CLabel m_lbSJCO2;
	CLabel m_lbSJCO3;
	CLabel m_lbSJCO4;
	CLabel m_lbSJCO5;
	// CO2
	CLabel m_lbSJCO21;
	CLabel m_lbSJCO22;
	CLabel m_lbSJCO23;
	CLabel m_lbSJCO24;
	CLabel m_lbSJCO25;
	// NO
	CLabel m_lbSJNO1;
	CLabel m_lbSJNO2;
	CLabel m_lbSJNO3;
	CLabel m_lbSJNO4;
	CLabel m_lbSJNO5;
	// O2
	CLabel m_lbSJO21;
	CLabel m_lbSJO22;
	CLabel m_lbSJO23;
	CLabel m_lbSJO24;
	CLabel m_lbSJO25;

	// IDC_STATIC_STATION
	CLabel m_lbStation;
private:
	// 检测站信息
	CString m_strStationName;
private:
	// 打开串口并测试通讯
	void OpenSerialPortAndTestCommunication(void);

private:
	// 分析仪协议版本
	int m_nAnaProtocolVersion;
	// 分析仪线程
	CAnaThread *m_pAnaThread;
	// 获取检查气标称值输入
	// 返回0x00,标称值输入符合要求
	// 返回其它,标称值输入不符要求
	DWORD GetNominalValue(void);

private:
	// 标准气C3H8标称值,单位:ppm
	int m_nC3H8NominalValue;
	// 标准气CO标称值,单位:0.01%(放到100倍)
	int m_nCONominalValue;
	// 标准气CO2标称值,单位:0.01%(放到100倍)
	int m_nCO2NominalValue;
	// 标准气NO标称值,单位:0.01%(放到100倍)
	int m_nNONominalValue;
	// 标准气NO2标称值,单位:0.01%(放到100倍)
	int m_nNO2NominalValue;
	// 标准气HC标称值,单位:ppm,
	// 由C3H8标称值*PEF得到
	int m_nHCNominalValue;
	// 标准气O2标称值,单位:0.01%(放到100倍)
	int m_nO2NominalValue;

	short m_sRandomErrorHC;
	short m_sRandomErrorNO;
	short m_sRandomErrorNO2;
	short m_sRandomErrorNOX;
	float m_fRandomErrorCO;
	float m_fRandomErrorCO2;
	float m_fRandomErrorO2;
	float m_fRandomErrorPef;

	void RandomError(short& sHC, float& fPef, float& fCO, float& fCO2, float& fO2, short& sNO, short& sNO2, short& sNOx);

	// 计算结果
	bool CalculateResult(void);

	// PEF测量值,放大1000倍,四舍五入
	int m_nPEFMeasuredValue;
	// HC测量值,单位:ppm
	int m_nHCMeasuredValue;
	// CO测量值,单位:0.01%,放大100倍,四舍五入
	int m_nCOMeasuredValue;
	// CO2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nCO2MeasuredValue;
	// NO测量值,单位:0.01%,放大100倍,四舍五入
	int m_nNOMeasuredValue;
	// NO2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nNO2MeasuredValue;
	// O2测量值,单位:0.01%,放大100倍,四舍五入
	int m_nO2MeasuredValue;

	// HC绝对误差,单位:ppm
	int m_nHCAE;
	// CO绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nCOAE;
	// CO2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nCO2AE;
	// NO绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nNOAE;
	// NO2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nNO2AE;
	// O2绝对误差,单位:0.01%,放大100倍,四舍五入
	int m_nO2AE;

	// HC绝对误差是否通过
	bool m_bIsHCAEPass;
	// CO绝对误差是否通过
	bool m_bIsCOAEPass;
	// CO2绝对误差是否通过
	bool m_bIsCO2AEPass;
	// NO绝对误差是否通过
	bool m_bIsNOAEPass;
	// NO2绝对误差是否通过
	bool m_bIsNO2AEPass;
	// O2绝对误差是否通过
	bool m_bIsO2AEPass;

	// HC相对误差,单位:%
	int m_nHCRE;
	// CO相对误差,单位:%,放大100倍,四舍五入
	int m_nCORE;
	// CO2相对误差,单位:%,放大100倍,四舍五入
	int m_nCO2RE;
	// NO相对误差,单位:%,放大100倍,四舍五入
	int m_nNORE;
	// NO2相对误差,单位:%,放大100倍,四舍五入
	int m_nNO2RE;
	// O2相对误差,单位:%,放大100倍,四舍五入
	int m_nO2RE;

	// HC相对误差是否通过
	bool m_bIsHCREPass;
	// CO相对误差是否通过
	bool m_bIsCOREPass;
	// CO2相对误差是否通过
	bool m_bIsCO2REPass;
	// NO相对误差是否通过
	bool m_bIsNOREPass;
	// NO2相对误差是否通过
	bool m_bIsNO2REPass;
	// O2相对误差是否通过
	bool m_bIsO2REPass;

	// HC通道是否通过
	bool m_bIsHCPass;
	// CO通道是否通过
	bool m_bIsCOPass;
	// CO2通道是否通过
	bool m_bIsCO2Pass;
	// NO通道是否通过
	bool m_bIsNOPass;
	// NO2通道是否通过
	bool m_bIsNO2Pass;
	// O2通道是否通过
	bool m_bIsO2Pass;
	// 检查是否通过
	bool m_bIsCheckPass;

	COleDateTime m_odtStart;
	COleDateTime m_odtEnd;

private:
	bool UpIntwqfxybd(void);
};
