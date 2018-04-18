// FaQuery.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "FaQuery.h"
#include "afxdialogex.h"


// CFaQuery dialog

IMPLEMENT_DYNAMIC(CFaQuery, CDialogEx)

CFaQuery::CFaQuery(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFaQuery::IDD, pParent)
{

}

CFaQuery::~CFaQuery()
{
}

void CFaQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFaQuery, CDialogEx)
	ON_BN_CLICKED(IDC_FA_BUTTON1, &CFaQuery::OnBnClickedFaButton1)
END_MESSAGE_MAP()


// CFaQuery message handlers


void CFaQuery::OnBnClickedFaButton1()
{
	GetDlgItemText(IDC_FA_EDIT1,car_plate);
	if (car_plate=="")
	{
		MessageBox("输入车牌不能为空！");
	} 
	else
	{
		EndDialog(1);
	}
	
}
