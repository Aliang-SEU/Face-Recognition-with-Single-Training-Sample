#pragma once


// CComQuery dialog

class CComQuery : public CDialogEx
{
	DECLARE_DYNAMIC(CComQuery)

public:
	CComQuery(CWnd* pParent = NULL);   // standard constructor
	virtual ~CComQuery();

// Dialog Data
	enum { IDD = IDD_COMQUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	 CString car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate;  //编号，车牌、车标、车型、颜色、时间、地点(在主对话框中用到，为public)
	 afx_msg void OnBnClickedCButton1();  
};
