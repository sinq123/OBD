#pragma once

// CHAT601Dlg �Ի���

class CHAT601Dlg : public CDialogZoom
{
	DECLARE_DYNAMIC(CHAT601Dlg)

public:
	CHAT601Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHAT601Dlg();

// �Ի�������
	enum { IDD = IDD_NHT_601OG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
