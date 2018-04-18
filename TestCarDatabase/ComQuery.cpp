// ComQuery.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "ComQuery.h"
#include "afxdialogex.h"


// CComQuery dialog

IMPLEMENT_DYNAMIC(CComQuery, CDialogEx)

CComQuery::CComQuery(CWnd* pParent /*=NULL*/)
	: CDialogEx(CComQuery::IDD, pParent)
{

}

CComQuery::~CComQuery()
{
}

void CComQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CComQuery, CDialogEx)
	ON_BN_CLICKED(IDC_C_BUTTON1, &CComQuery::OnBnClickedCButton1)
END_MESSAGE_MAP()


// CComQuery message handlers


void CComQuery::OnBnClickedCButton1()
{
	GetDlgItemText(IDC_C_EDIT1,car_plate);
	GetDlgItemText(IDC_C_EDIT2,car_logo);
	GetDlgItemText(IDC_C_EDIT3,car_type);
	GetDlgItemText(IDC_C_EDIT4,car_colour);
	GetDlgItemText(IDC_C_EDIT5,car_time);
	GetDlgItemText(IDC_C_EDIT6,car_locate);

	if (car_plate==""&&car_logo==""&&car_type==""&&car_colour==""&&car_time==""&&car_locate=="")
	{
		MessageBox("请至少输入一个条件！");
	} 
	else
	{
		EndDialog(1);
	}

}
