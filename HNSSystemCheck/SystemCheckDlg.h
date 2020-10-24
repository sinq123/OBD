
// SystemCheckDlg.h : 头文件
//

#pragma once


// CSystemCheckDlg 对话框
class CSystemCheckDlg : public CDialogZoom
{
// 构造
public:
	CSystemCheckDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSystemCheckDlg();

// 对话框数据
	enum { IDD = IDD_SYSTEMCHECK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedBtnDailyDemarcation();
	afx_msg void OnBnClickedBtnEmissionTest();
	afx_msg void OnBnClickedBtnDataManagement();
	afx_msg void OnBnClickedBtnSystemManagement();
	afx_msg void OnBnClickedBtnSystemCheck();
	afx_msg void OnBnClickedBtnTrainingPresentation();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CInfoButton m_ibEmissionTest;
	CInfoButton m_ibDailyDemarcation;
	CInfoButton m_ibDataMangement;
	CInfoButton m_ibSystemConfig;
	CInfoButton m_ibTrainingPresentation;
	CInfoButton m_ibSystemCheck;
	CButtonST m_btnExit;
	// 名称及版本标签
	CShadeStatic m_ssVersion;
	// 标题栏
	CShadeTitle m_stTitle;
	// 南华Logo
	CImageWnd m_iwNHLogo;
	// 环保Logo
	CImageWnd m_iwEPLogo;
	// 检测线编号+设备认证编号显示框
	CLabel m_lbLineNumberAndEquipmentLicense;
	// time
	CLabel m_lbTime;
	// 定制版本标志
	CLabel m_lbSign;

private:
	// 初始化控件
	void InitCtrls(void);
	// 设置对话框字体
	void SetDlgFont(void);

private:
	// 通用对话框字体
	CFont m_fontDlgFont;
	// 通用对话框字体大小
	int m_nDlgFontSize;
	// 系统类型
	NHCLIENT m_sIniNHClient;
	// 当前系统时间
	SYSTEMTIME m_sCurrencyTime;

};
