// UpdataData.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "UpdataData.h"
#include "afxdialogex.h"


// CUpdataData dialog

IMPLEMENT_DYNAMIC(CUpdataData, CDialogEx)

CUpdataData::CUpdataData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdataData::IDD, pParent)
{

}

CUpdataData::~CUpdataData()
{
}

void CUpdataData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdataData, CDialogEx)
	ON_BN_CLICKED(IDC_U_BUTTON1, &CUpdataData::OnBnClickedUButton1)
END_MESSAGE_MAP()


// CUpdataData message handlers


void CUpdataData::OnBnClickedUButton1()
{
	GetDlgItemText(IDC_U_EDIT1,car_plate);
	GetDlgItemText(IDC_U_EDIT2,car_logo);
	GetDlgItemText(IDC_U_EDIT3,car_type);
	GetDlgItemText(IDC_U_EDIT4,car_colour);
	GetDlgItemText(IDC_U_EDIT5,car_time);
	GetDlgItemText(IDC_U_EDIT6,car_locate);

	if (car_plate==""&&car_logo==""&&car_type==""&&car_colour==""&&car_time==""&&car_locate=="")
	{
		MessageBox("请至少输入一个条件！");
	} 
	else
	{
		EndDialog(1);
	}
}
