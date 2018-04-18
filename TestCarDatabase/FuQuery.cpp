// FuQuery.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "FuQuery.h"
#include "afxdialogex.h"


// CFuQuery dialog

IMPLEMENT_DYNAMIC(CFuQuery, CDialogEx)

CFuQuery::CFuQuery(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFuQuery::IDD, pParent)
{

}

CFuQuery::~CFuQuery()
{
}

void CFuQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFuQuery, CDialogEx)
	ON_BN_CLICKED(IDC_FU_BUTTON1, &CFuQuery::OnBnClickedFuButton1)
END_MESSAGE_MAP()


// CFuQuery message handlers


void CFuQuery::OnBnClickedFuButton1()
{
	GetDlgItemText(IDC_FU_EDIT1,car_plate);
	GetDlgItemText(IDC_FU_EDIT2,car_logo);
	GetDlgItemText(IDC_FU_EDIT3,car_type);
	GetDlgItemText(IDC_FU_EDIT4,car_colour);

	if (car_plate==""&&car_logo==""&&car_type==""&&car_colour=="")
	{
		MessageBox("请至少输入一个条件！");
	} 
	else
	{
		EndDialog(1);
	}

}
