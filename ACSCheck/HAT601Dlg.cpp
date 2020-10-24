// HAT601Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ACSCheck.h"
#include "HAT601Dlg.h"
#include "afxdialogex.h"


// CHAT601Dlg 对话框

IMPLEMENT_DYNAMIC(CHAT601Dlg, CDialogZoom)

CHAT601Dlg::CHAT601Dlg(CWnd* pParent /*=NULL*/)
	: CDialogZoom(CHAT601Dlg::IDD, pParent)
{

}

CHAT601Dlg::~CHAT601Dlg()
{
}

void CHAT601Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogZoom::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHAT601Dlg, CDialogZoom)
END_MESSAGE_MAP()


// CHAT601Dlg 消息处理程序
