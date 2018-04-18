#pragma once


// CFaQuery dialog

class CFaQuery : public CDialogEx
{
	DECLARE_DYNAMIC(CFaQuery)

public:
	CFaQuery(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFaQuery();

// Dialog Data
	enum { IDD = IDD_FAQUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString car_plate;
	afx_msg void OnBnClickedFaButton1();
};
