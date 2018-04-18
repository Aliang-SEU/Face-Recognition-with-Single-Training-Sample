// DBConDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "DBConDlg.h"
#include "afxdialogex.h"


// CDBConDlg dialog

IMPLEMENT_DYNAMIC(CDBConDlg, CDialogEx)

CDBConDlg::CDBConDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBConDlg::IDD, pParent)
{

}

CDBConDlg::~CDBConDlg()
{
}

void CDBConDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDBConDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CON, &CDBConDlg::OnBnClickedCon)
END_MESSAGE_MAP()


// CDBConDlg message handlers

//确认连接按钮
void CDBConDlg::OnBnClickedCon()
{
	GetDlgItemText(IDC_DB_EDIT1,DataSourse);
	GetDlgItemText(IDC_DB_EDIT2,UseName);
	GetDlgItemText(IDC_DB_EDIT3,PassWord);
	if (DataSourse!=""&&UseName!=""&&PassWord!="")
	{
		EndDialog(1);
	} 
	else
	{
		MessageBox("输入信息不能为空！");
	}
}
