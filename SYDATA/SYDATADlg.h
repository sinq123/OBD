
// SYDATADlg.h : ͷ�ļ�
//

#pragma once

#include "..\NHLib\NHLib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHLib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHLib.lib")
#endif

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#include "..\NHDetCommModu\NHDetCommModu.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHDetCommModu_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHDetCommModu.lib")
#endif

#include "..\NHModeServerDB\NHModeServerDB.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHModeServerDB_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHModeServerDB.lib")
#endif

struct SCarInfo
{
	//1	����������	TsNo
	CString strTsNo;
	//2	����߱��	TestLineNo
	CString strTestLineNo;
	//3	����ID	Clid
	CString strClid;
	//4	����	Hp
	CString strHp;
	//5	������ɫ	Cpys
	CString strCpys;
	//6	��������ʽ	Bxqxs
	CString strBxqxs;
	//7	��׼����	Jzzl
	CString strJzzl;
	//8	ȼ������	Rlzl
	CString strRlzl;
	//9	�����	Edgl
	CString strEdgl;
	//10	�ת��	Edzs
	CString strEdzs;
	//11	���	Cc
	CString strCc;
	//12	C0+Co2��ֵ	CoCo2
	CString strCoCo2;
	//13	��������	Lxcz
	CString strLxcz;
	//14	�ۼƳ���	Ljcz
	CString strLjcz;
	//15	������ʽ	Jqfs
	CString strJqfs;
	//16	�Ƿ�OBD���	OBD
	CString strOBD;

};

// CSYDATADlg �Ի���
class CSYDATADlg : public CDialogEx
{
// ����
public:
	CSYDATADlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSYDATADlg();
// �Ի�������
	enum { IDD = IDD_SYDATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedButtonUpdateVehicleList();
	afx_msg void OnBnClickedBtnJumptestobd();
	afx_msg void OnBnClickedBtnUp();
private:
	// ������Ϣ��ַ
	CEdit m_edVehInfoPath;

	CEdit m_edMsg;
	// ����������
	CLabel m_lbTsNO;
	// ����
	CLabel m_lbHP;
	// ������ɫ
	CLabel m_lbCpys;
	// ȼ������
	CLabel m_lbRlzl;
	// ������ʽ
	CLabel m_lbJqfs;
	// �Ƿ�OBD���
	CLabel m_lbTestOBD;

	// ����Դ
	CEdit m_edCatalog;
	// ���ݿ�����
	CEdit m_edSource;
	// ��¼��
	CEdit m_edUserID;
	// ����
	CEdit m_edPassWord;
	// OBD���Ա�б�
	CComboBox m_comOBDJCY;

	SCarInfo m_sCarInfo;
private:
	_ConnectionPtr m_pConnection;

	bool OpenSQL(void);
	bool Close(void);
	bool m_bIsOKOpenDB;
private:
	// ��ȡ������Ϣ
	void SetConfig(void);
	// OBD���Ա
	void GetOBDJCY(void);

	// ��ȡ������Ϣ
	void GetVehXml(void);

	// ��ȡ�ļ���Ϣ
	wstring FilePathToContent(const CString strPath);

	// �趨������Ϣ
	void SetVehWin(const SCarInfo& sCarInfo);

	// ��ȡ�ļ�·��
	bool GetFilePath(CString& strMsg, CString& strFilePath);

	// ��������OBD���
	void JumpTestOBD(void);

	// д��OBD�������TestLog�޸Ĳ���
	void WriterResultOfOBDAndTestLog(void);

	// OBD���ݲ���
	bool SetOBDLOG(SResultOfOBD& sResultOfOBD, CString& strMsg);
	CString GetOBDType(const int& nType, const CString& strFulType);
	void GetEngineCALID(const CString& strOBDType, const CString& strFulType, CString& strEngineCALID, CString& strEngineCVN);

};
