#pragma once


// CFuQuery dialog

class CFuQuery : public CDialogEx
{
	DECLARE_DYNAMIC(CFuQuery)

public:
	CFuQuery(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFuQuery();

// Dialog Data
	enum { IDD = IDD_FUQUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString car_ID,car_plate,car_logo,car_type,car_colour;

public:
	afx_msg void OnBnClickedFuButton1();
};
