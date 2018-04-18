#pragma once


// CDBConDlg dialog

class CDBConDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBConDlg)

public:
	CDBConDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDBConDlg();

// Dialog Data
	enum { IDD = IDD_DBCON };

public:
	CString DataSourse,UseName,PassWord;   //连接数据库的数据源、用户名、密码(在TestCar类中用到，所以为public)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCon();  //确认连接按钮
};
