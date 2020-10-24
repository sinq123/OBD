#pragma once

// CHAT601Dlg 对话框

class CHAT601Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CHAT601Dlg)

public:
	CHAT601Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHAT601Dlg();

// 对话框数据
	enum { IDD = IDD_NHT_601OG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
