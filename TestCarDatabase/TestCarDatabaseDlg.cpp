
// TestCarDatabaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "TestCarDatabaseDlg.h"
#include "afxdialogex.h"
#include "DBConDlg.h"
#include "ComQuery.h"
#include "FaQuery.h"
#include "FuQuery.h"
#include "Track.h"
#include "InputOne.h"
#include "CvvImage.h"
#include "UpdataData.h"
#include "FaceDetect2.h"
#include<fstream>
#include<sstream>
#include<string>
#include "FaceRec.h"
#include "Step.h"
#include "Constant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestCarDatabaseDlg dialog




CTestCarDatabaseDlg::CTestCarDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestCarDatabaseDlg::IDD, pParent)
{
	nItem=-1;  //初始化列表框的行标号
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CTestCarDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, my_ctllist1);
}

BEGIN_MESSAGE_MAP(CTestCarDatabaseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_32771, &CTestCarDatabaseDlg::OnBnConDB)
	ON_COMMAND(ID_32789, &CTestCarDatabaseDlg::OnShowDatabase)
//	ON_BN_CLICKED(IDC_QUERY, &CTestCarDatabaseDlg::OnBnClickedQuery)
ON_COMMAND(ID_32783, &CTestCarDatabaseDlg::OnComQuery)
ON_COMMAND(ID_32787, &CTestCarDatabaseDlg::OnAcQuery)
ON_COMMAND(ID_32788, &CTestCarDatabaseDlg::OnFuQuery)
ON_COMMAND(ID_32790, &CTestCarDatabaseDlg::OnTrack)
ON_COMMAND(ID_32779, &CTestCarDatabaseDlg::OnInputOne)
ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTestCarDatabaseDlg::OnNMClickList1)
ON_BN_CLICKED(IDC_T_BUTTON1, &CTestCarDatabaseDlg::OnShowImg)
//ON_BN_CLICKED(IDC_T_BUTTON2, &CTestCarDatabaseDlg::OnDeleteData)
ON_COMMAND(ID_32791, &CTestCarDatabaseDlg::OnDeleteData)
ON_COMMAND(ID_32781, &CTestCarDatabaseDlg::OnUpdataData)
ON_COMMAND(ID_32792, &CTestCarDatabaseDlg::OnInputMulti)
ON_BN_CLICKED(IDC_T_BUTTON2, &CTestCarDatabaseDlg::OnHideImg)
ON_COMMAND(ID_32793, &CTestCarDatabaseDlg::OnStep)
ON_COMMAND(ID_32794, &CTestCarDatabaseDlg::OnConstant)
END_MESSAGE_MAP()


// CTestCarDatabaseDlg message handlers

BOOL CTestCarDatabaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//标题
	SetWindowText("车辆特征数据库管理系统");

	//添加菜单
	m_Menu.LoadMenu(IDR_MAIN_MENU);  //  IDR_MENU1为你加入的菜单的ID，在Resource视图的Menu文件夹下可以找到
	SetMenu(&m_Menu);

	CRect temprect(0,0,960,640); 
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	//得到原始客户区的坐标
	CRect rect;      
	GetClientRect(&rect);     //取客户区大小    
	old.x=rect.right-rect.left;  
	old.y=rect.bottom-rect.top;


	//初始化com库
	::CoInitialize(NULL);     

	//添加列表框的网格线！！！
	DWORD dwStyle = my_ctllist1.GetExtendedStyle();                   
	dwStyle |= LVS_EX_FULLROWSELECT;            
	dwStyle |= LVS_EX_GRIDLINES;                
	my_ctllist1.SetExtendedStyle(dwStyle);


	//添加列标题
	my_ctllist1.InsertColumn(0,"编号",LVCFMT_CENTER,100);
	my_ctllist1.InsertColumn(1,"车牌",LVCFMT_CENTER,150);	
	my_ctllist1.InsertColumn(2,"车标",LVCFMT_CENTER,150);
	my_ctllist1.InsertColumn(3,"车型",LVCFMT_CENTER,150);
	my_ctllist1.InsertColumn(4,"颜色",LVCFMT_CENTER,150);
	my_ctllist1.InsertColumn(5,"时间",LVCFMT_CENTER,200);
	my_ctllist1.InsertColumn(6,"位置",LVCFMT_CENTER,200);


	//设置静态文本框字体大小
	CFont * f; 
	f = new CFont; 
	f->CreateFont(20,            // nHeight 
		0,           // nWidth 
		0,           // nEscapement 
		0,           // nOrientation 
		FW_SEMIBOLD,     // nWeight 
		FALSE,        // bItalic 
		FALSE,       // bUnderline 
		0,           // cStrikeOut 
		ANSI_CHARSET,              // nCharSet 
		OUT_DEFAULT_PRECIS,        // nOutPrecision 
		CLIP_DEFAULT_PRECIS,       // nClipPrecision 
		DEFAULT_QUALITY,           // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体"));              // lpszFac

	GetDlgItem(IDC_T_STATIC1)->SetFont(f);
	GetDlgItem(IDC_T_STATIC2)->SetFont(f);
	GetDlgItem(IDC_T_STATIC3)->SetFont(f);
	GetDlgItem(IDC_T_STATIC4)->SetFont(f);
	GetDlgItem(IDC_T_STATIC5)->SetFont(f);
/*	GetDlgItem(IDC_T_STATIC6)->SetFont(f);
	GetDlgItem(IDC_T_STATIC7)->SetFont(f);*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestCarDatabaseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestCarDatabaseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		//对话框改变时，对picture控件上的图片进行重绘
		if(car_Img.data)
		{
			IplImage img=car_Img;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_T_CAR_IMG); 
		}
		if(face_Img.data)
		{
			IplImage img=face_Img;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_T_FACE_IMG); 
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestCarDatabaseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestCarDatabaseDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//控件随对话框大小而改变
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
	{  
		ReSize();  
	}  
}


//使控件随对话框大小的改变而改变
void CTestCarDatabaseDlg::ReSize()  
{  
	float fsp[2];  
	POINT Newp; //获取现在对话框的大小  
	CRect recta;      
	GetClientRect(&recta);     //取客户区大小    
	Newp.x=recta.right-recta.left;  
	Newp.y=recta.bottom-recta.top;  
	fsp[0]=(float)Newp.x/old.x;  
	fsp[1]=(float)Newp.y/old.y;  
	CRect Rect;  
	int woc;  
	CPoint OldTLPoint,TLPoint; //左上角  
	CPoint OldBRPoint,BRPoint; //右下角  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件    
	while(hwndChild)      
	{      
		woc=::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);    
		ScreenToClient(Rect);    
		OldTLPoint = Rect.TopLeft();    
		TLPoint.x = long(OldTLPoint.x*fsp[0]);    
		TLPoint.y = long(OldTLPoint.y*fsp[1]);    
		OldBRPoint = Rect.BottomRight();    
		BRPoint.x = long(OldBRPoint.x *fsp[0]);    
		BRPoint.y = long(OldBRPoint.y *fsp[1]);    
		Rect.SetRect(TLPoint,BRPoint);    
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}  
	old=Newp;  
}  


//连接数据库
void CTestCarDatabaseDlg::OnBnConDB()
{
	CDBConDlg a;
	int i=a.DoModal();

	if (i==1)
	{
		DataSourse=a.DataSourse;
		UseName=a.UseName;
		PassWord=a.PassWord;

		//创建实例
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

		//连接指定数据库
		if(!m_pConn->State)    
		{    
			CString sql;
			sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);
			//_bstr_t sql=(_bstr_t)sql1; 

			CString sInfo;  
			try    
			{    
				m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				sInfo.Format(_T("连接成功"));  
			}catch (_com_error e)    
			{  
				sInfo.Format(_T("连接失败"));  
			}    

			MessageBox(sInfo);  
		}
	}

	
}


//显示数据库中的记录集
void CTestCarDatabaseDlg::OnShowDatabase()
{
	//创建实例
	m_pConn.CreateInstance(__uuidof(Connection));    
	m_pRs.CreateInstance(__uuidof(Recordset));   

	//连接指定数据库
	if(!m_pConn->State)    
	{    
		if (DataSourse!=""&&UseName!=""&&PassWord!="")
		{
			CString sql;
			sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

			CString sInfo;  
			try    
			{    
				m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
			}catch (_com_error e)    
			{  
				sInfo.Format(_T("数据库连接失败"));  
				MessageBox(sInfo);  
			}    
		} 
		else
		{
			MessageBox("请检查数据库有无连接！");
			return;
		}
		

	}   

	//连接指定表
	CString biao;  
	biao.Format("select * from hr.carchrct order by car_ID");  
	m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

	my_ctllist1.DeleteAllItems();  //  清空列表框  
	int nItem;  
	_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

	// 判断记录集指针标志，是否读取完毕  
	while(!m_pRs->adoEOF)  
	{  
		// 获取记录集  
		vCarNum=m_pRs->GetCollect("car_ID");
		vCarPlate=m_pRs->GetCollect("car_plate");
		vCarLogo=m_pRs->GetCollect("car_logo");
		vCarSize=m_pRs->GetCollect("car_type");
		vCarColor=m_pRs->GetCollect("car_colour");
		vCarTime=m_pRs->GetCollect("car_time");
		vCarLocate=m_pRs->GetCollect("car_locate");

		nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
		my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
		// 移动记录集指针到下一条  
		m_pRs->MoveNext();  
	}  
}


//组合查询
void CTestCarDatabaseDlg::OnComQuery()
{
	CComQuery a;
	int i=a.DoModal();
	
	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;
		car_time=a.car_time;
		car_locate=a.car_locate;

		//创建实例
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

		//连接指定数据库
		if(!m_pConn->State)    
		{    
			if (DataSourse!=""&&UseName!=""&&PassWord!="")
			{
				CString sql;
				sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

				CString sInfo;  
				try    
				{    
					m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				}catch (_com_error e)    
				{  
					sInfo.Format(_T("数据库连接失败"));  
					MessageBox(sInfo);  
				}    
			} 
			else
			{
				MessageBox("请检查数据库有无连接！");
				return;
			}

		}   

		//连接指定表
		CString biao;  
		biao.Format("select * from hr.carchrct where car_plate like '%s%s%s' and car_logo like '%s%s%s' and car_type like '%s%s%s' and car_colour like '%s%s%s' and car_time like '%s%s%s'and car_locate like '%s%s%s' order by car_ID","%",car_plate,"%","%",car_logo,"%","%",car_type,"%","%",car_colour,"%","%",car_time,"%","%",car_locate,"%");  
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

		my_ctllist1.DeleteAllItems();  //  清空列表框  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

		// 判断记录集指针标志，是否读取完毕  
		while(!m_pRs->adoEOF)  
		{  
			// 获取记录集  
			vCarNum=m_pRs->GetCollect("car_ID");
			vCarPlate=m_pRs->GetCollect("car_plate");
			vCarLogo=m_pRs->GetCollect("car_logo");
			vCarSize=m_pRs->GetCollect("car_type");
			vCarColor=m_pRs->GetCollect("car_colour");
			vCarTime=m_pRs->GetCollect("car_time");
			vCarLocate=m_pRs->GetCollect("car_locate");

			nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
			my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
			// 移动记录集指针到下一条  
			m_pRs->MoveNext();  
		}  
	}
	
}


//精确套牌查询
void CTestCarDatabaseDlg::OnAcQuery()
{
	CFaQuery a;
	int i=a.DoModal();

	if (i==1)
	{
		car_plate=a.car_plate;

		//创建实例
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   
		m_pRs1.CreateInstance(__uuidof(Recordset));

		//连接指定数据库
		if(!m_pConn->State)    
		{    
			if (DataSourse!=""&&UseName!=""&&PassWord!="")
			{
				CString sql;
				sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

				CString sInfo;  
				try    
				{    
					m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				}catch (_com_error e)    
				{  
					sInfo.Format(_T("数据库连接失败"));  
					MessageBox(sInfo);  
				}    
			} 
			else
			{
				MessageBox("请检查数据库有无连接！");
				return;
			}

		} 

		//连接指定表
		CString biao;  
		biao.Format("select distinct car_plate,car_logo,car_type,car_colour from hr.carchrct where car_plate like '%s%s%s'","%",car_plate,"%");  //去重
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

		int nTotals=0;  //记录总数

		while(!m_pRs->adoEOF)  
		{  
			nTotals++;
			m_pRs->MoveNext();  
		}  

		//无套牌嫌疑
		if (nTotals<=1)
		{
			my_ctllist1.DeleteAllItems();  //  清空列表框  
			MessageBox("此车牌无套牌嫌疑！","查询结果");
		} 
		else  //有套牌嫌疑
		{
			biao.Format("select * from hr.carchrct where car_plate like '%s%s%s'","%",car_plate,"%");  
			m_pRs1->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

			my_ctllist1.DeleteAllItems();  //  清空列表框  
			int nItem;  
			_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

			// 判断记录集指针标志，是否读取完毕  
			while(!m_pRs1->adoEOF)  
			{  
				// 获取记录集  
				vCarNum=m_pRs1->GetCollect("car_ID");
				vCarPlate=m_pRs1->GetCollect("car_plate");
				vCarLogo=m_pRs1->GetCollect("car_logo");
				vCarSize=m_pRs1->GetCollect("car_type");
				vCarColor=m_pRs1->GetCollect("car_colour");
				vCarTime=m_pRs1->GetCollect("car_time");
				vCarLocate=m_pRs1->GetCollect("car_locate");

				nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
				my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				// 移动记录集指针到下一条  
				m_pRs1->MoveNext();  
			}  

			MessageBox("此车牌有套牌嫌疑，请查看具体记录！","查询结果");
		}
	}

	
}


//模糊套牌查询
void CTestCarDatabaseDlg::OnFuQuery()
{
	CFuQuery a;
	int i=a.DoModal();

	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;

		//创建实例
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

		//连接指定数据库
		if(!m_pConn->State)    
		{    
			if (DataSourse!=""&&UseName!=""&&PassWord!="")
			{
				CString sql;
				sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

				CString sInfo;  
				try    
				{    
					m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				}catch (_com_error e)    
				{  
					sInfo.Format(_T("数据库连接失败"));  
					MessageBox(sInfo);  
				}    
			} 
			else
			{
				MessageBox("请检查数据库有无连接！");
				return;
			}

		} 

		//连接指定表
		CString Query;

		Query.Format("CREATE TABLE hr.TEMP AS SELECT DISTINCT CAR_PLATE,CAR_LOGO,CAR_TYPE,CAR_COLOUR FROM hr.CARCHRCT WHERE car_plate like'%s%s%s' and car_logo like '%s%s%s' and car_type like '%s%s%s' and car_colour like '%s%s%s'","%",car_plate,"%","%",car_logo,"%","%",car_type,"%","%",car_colour,"%");
		_variant_t RecordsAffected;
		m_pConn->Execute((_bstr_t)Query,&RecordsAffected,adCmdText);
		CString Scrn;
		Scrn="SELECT * FROM hr.CARCHRCT WHERE CAR_PLATE IN(SELECT CAR_PLATE FROM hr.TEMP GROUP BY CAR_PLATE HAVING COUNT(*)>=2) ORDER BY CAR_PLATE";
		m_pRs->Open((_bstr_t)Scrn,(IDispatch*)m_pConn,adOpenStatic,adLockOptimistic,adCmdText);

		my_ctllist1.DeleteAllItems();  //  清空列表框  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

		if (!m_pRs->adoEOF)
		{
			// 判断记录集指针标志，是否读取完毕  
			while(!m_pRs->adoEOF)  
			{  
				// 获取记录集  
				vCarNum=m_pRs->GetCollect("car_ID");
				vCarPlate=m_pRs->GetCollect("car_plate");
				vCarLogo=m_pRs->GetCollect("car_logo");
				vCarSize=m_pRs->GetCollect("car_type");
				vCarColor=m_pRs->GetCollect("car_colour");
				vCarTime=m_pRs->GetCollect("car_time");
				vCarLocate=m_pRs->GetCollect("car_locate");

				nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
				my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				// 移动记录集指针到下一条  
				m_pRs->MoveNext();  
			}  

			MessageBox("有套牌车辆，详见查询记录!");
		} 
		else
		{
			MessageBox("无套牌车辆！");
		}
		CString Drop;
		Drop.Format("DROP TABLE hr.TEMP");
		m_pConn->Execute((_bstr_t)Drop,&RecordsAffected,adCmdText);
	}

	
}


//肇事逃逸追踪
void CTestCarDatabaseDlg::OnTrack()
{
	CTrack a;
	int i=a.DoModal();

	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;
		car_time=a.car_time;
		car_locate=a.car_locate;
		face_Img=a.face_Img;
		matchPar=a.matchPar;

		//创建实例
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

		//连接指定数据库
		if(!m_pConn->State)    
		{    
			if (DataSourse!=""&&UseName!=""&&PassWord!="")
			{
				CString sql;
				sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

				CString sInfo;  
				try    
				{    
					m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				}catch (_com_error e)    
				{  
					sInfo.Format(_T("数据库连接失败"));  
					MessageBox(sInfo);  
				}    
			} 
			else
			{
				MessageBox("请检查数据库有无连接！");
				return;
			}
		}   

		//连接指定表
		CString biao;  
		biao.Format("select * from hr.carchrct where  car_plate like '%s%s%s' and car_logo like '%s%s%s' and car_type like '%s%s%s' and car_colour like '%s%s%s' and car_locate like '%s%s%s' and car_time like '%s%s%s' order by car_plate,car_time","%",car_plate,"%","%",car_logo,"%","%",car_type,"%","%",car_colour,"%","%",car_locate,"%","%",car_time,"%"); 
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 


		my_ctllist1.DeleteAllItems();  //  清空列表框  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  


		if (face_Img.data)
		{
			cv::Mat_<double> train_Img,assist_Img,test_Img; //训练集、辅助集和测试集矩阵
			int face_ID;   //训练集图片编号
			std::string face_Path;   //人脸图片的路径
			cv::Mat_<double> img,imgData;
			int nTotals=0;  //记录总数

			while(!m_pRs->adoEOF)  //统计一共有多少记录
			{  
				nTotals++;
				m_pRs->MoveNext(); 
			}

			if (nTotals)
			{
				int *recordNum=new int[nTotals];   //记录集编号
				//int *recordState=new int[nTotals]();  //记录集状态，1表示识别成功，0表示未识别
				int k=0;

				m_pRs->MoveFirst();  //指针回归
				// 判断记录集指针标志，是否读取完毕  
				while(!m_pRs->adoEOF)  
				{  
					//获取记录集  
					face_ID=m_pRs->GetCollect("car_ID");
					recordNum[k]=face_ID;
					k++;

					std::ostringstream oss;
					oss<<face_ID;
					face_Path="FaceImage//"+oss.str()+".jpg";

					img=cv::imread(face_Path,0);
					if(img.data)
					{
						cv::resize(img,imgData,cv::Size(20,20));
						imgData=imgData.t();
						cv::Mat_<double> colImgs=imgData.reshape(imgData.rows*imgData.cols);
						train_Img.push_back(colImgs);
					}

					m_pRs->MoveNext();  
				}  

				train_Img=train_Img.t();  //转置后得到最终的训练集（图片以列存放）

				cv::FileStorage oos(".\\Assist.xml", cv::FileStorage::READ);  
				oos["Assist"] >>assist_Img;     //得到辅助集矩阵
				oos.release();


				cv::Mat test=face_Img.clone();  //复制一份人脸图像
				cv::Mat test_resize;
				cv::resize(test, test_resize,cv::Size(20,20));
				cv::Mat gray;
				// 灰度化
				if(test_resize.channels()==3)
					cv::cvtColor(test_resize,gray,CV_BGR2GRAY);
				else
					gray=test_resize;

				cv::Mat_<double> testData=gray;  //把图像数据转为double型，便于后面的人脸识别
				testData=testData.t();
				test_Img=testData.reshape((testData.rows)*(testData.cols));   //得到测试图片的矩阵
				test_Img = test_Img.t();

				int RecNum=(int)(nTotals*(1-(double)(matchPar)/100));
				if (RecNum==0)
				{
					RecNum++;
				}

				//调用人脸识别函数
				FaceRec a;
				int *result=a.recognition(train_Img,assist_Img,test_Img,RecNum,nTotals);

				my_ctllist1.DeleteAllItems();  //  清空列表框  

				for(int i=0;i<RecNum;i++)
				{
					m_pRs->MoveFirst();  //指针回归
					int j=result[i];
					while(j>0)//循环的作用是找到匹配到的记录
					{
						// 移动记录集指针到下一条  
						m_pRs->MoveNext();  
						j--;
					}

					// 获取记录集  
					vCarNum=m_pRs->GetCollect("car_ID");
					vCarPlate=m_pRs->GetCollect("car_plate");
					vCarLogo=m_pRs->GetCollect("car_logo");
					vCarSize=m_pRs->GetCollect("car_type");
					vCarColor=m_pRs->GetCollect("car_colour");
					vCarTime=m_pRs->GetCollect("car_time");
					vCarLocate=m_pRs->GetCollect("car_locate");

					nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
					my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
					my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
					my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
					my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
					my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
					my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				} 
			}
			else
			{
				my_ctllist1.DeleteAllItems();  //  清空列表框
			}
		} 
		else
		{
			// 判断记录集指针标志，是否读取完毕  
			while(!m_pRs->adoEOF)  
			{  
				// 获取记录集  
				vCarNum=m_pRs->GetCollect("car_ID");
				vCarPlate=m_pRs->GetCollect("car_plate");
				vCarLogo=m_pRs->GetCollect("car_logo");
				vCarSize=m_pRs->GetCollect("car_type");
				vCarColor=m_pRs->GetCollect("car_colour");
				vCarTime=m_pRs->GetCollect("car_time");
				vCarLocate=m_pRs->GetCollect("car_locate");

				nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
				my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				// 移动记录集指针到下一条  
				m_pRs->MoveNext();  
			}  
		}
	}
}



//单条录入
void CTestCarDatabaseDlg::OnInputOne()
{
	CInputOne a;

	a.DataSourse=DataSourse;
	a.UseName=UseName;
	a.PassWord=PassWord;

	a.DoModal();
	
}

//选定列表中的某一行
void CTestCarDatabaseDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(pNMItemActivate != NULL)
	{
		nItem = pNMItemActivate->iItem;
	}

	*pResult = 0;
}

//显示图片
void CTestCarDatabaseDlg::OnShowImg()
{
	if(nItem<0){
		AfxMessageBox("请先选定列表框中的一行！");
		return;
	}

	GetDlgItem(IDC_T_STATIC2)->ShowWindow(true);
	GetDlgItem(IDC_T_STATIC3)->ShowWindow(true);
	GetDlgItem(IDC_T_STATIC4)->ShowWindow(true);
	GetDlgItem(IDC_T_STATIC5)->ShowWindow(true);
	GetDlgItem(IDC_T_CAR_IMG)->ShowWindow(true);
	GetDlgItem(IDC_T_FACE_IMG)->ShowWindow(true);
	GetDlgItem(IDC_T_PLATE_IMG)->ShowWindow(true);
	GetDlgItem(IDC_T_LOGO_IMG)->ShowWindow(true);

	CString CarID=my_ctllist1.GetItemText(nItem,0);  //获取车编号
	CString car_Path="CarImage//"+CarID+".jpg";   //路径
	CString face_Path="FaceImage//"+CarID+".jpg";
	CString plate_Path="PlateImage//"+CarID+".jpg";
	CString logo_Path="LogoImage//"+CarID+".jpg";

	face_Img=cv::imread(face_Path.GetBuffer(0));
	car_Img=cv::imread(car_Path.GetBuffer(0));
	plate_Img=cv::imread(plate_Path.GetBuffer(0));
	logo_Img=cv::imread(logo_Path.GetBuffer(0));

	IplImage img1=car_Img;    //把mat格式转为IplImage，以便在控件中显示图片
	IplImage img2=face_Img;
	IplImage img3=plate_Img;
	IplImage img4=logo_Img;
	DrawPicToHDC(img1, IDC_T_CAR_IMG); 
	DrawPicToHDC(img2, IDC_T_FACE_IMG); 
	DrawPicToHDC(img3, IDC_T_PLATE_IMG); 
	DrawPicToHDC(img4, IDC_T_LOGO_IMG); 

	nItem=-1;
}

//隐藏图片
void CTestCarDatabaseDlg::OnHideImg()
{
	GetDlgItem(IDC_T_STATIC2)->ShowWindow(false);
	GetDlgItem(IDC_T_STATIC3)->ShowWindow(false);
	GetDlgItem(IDC_T_STATIC4)->ShowWindow(false);
	GetDlgItem(IDC_T_STATIC5)->ShowWindow(false);
	GetDlgItem(IDC_T_CAR_IMG)->ShowWindow(false);
	GetDlgItem(IDC_T_FACE_IMG)->ShowWindow(false);
	GetDlgItem(IDC_T_PLATE_IMG)->ShowWindow(false);
	GetDlgItem(IDC_T_LOGO_IMG)->ShowWindow(false);
}


//把图片显示在picture控件中
void CTestCarDatabaseDlg::DrawPicToHDC(IplImage img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf( &img ); // 复制图片
	cimg.DrawToHDC( hDC, &rect ); // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
}


//删除指定记录
void CTestCarDatabaseDlg::OnDeleteData()
{
	if(nItem<0){
		AfxMessageBox("请先选定列表框中的一行！");
		return;
	}
	CString CarID=my_ctllist1.GetItemText(nItem,0);  //获取车编号

	CString car_Path="CarImage//"+CarID+".jpg";   //路径
	CString face_Path="FaceImage//"+CarID+".jpg";
	int car__id=atoi(CarID);       //将CString转换成Int类型

	//创建实例
	m_pConn.CreateInstance(__uuidof(Connection));    
	m_pRs.CreateInstance(__uuidof(Recordset));   
	m_pRs1.CreateInstance(__uuidof(Recordset));

	//连接指定数据库
	if(!m_pConn->State)    
	{    
		if (DataSourse!=""&&UseName!=""&&PassWord!="")
		{
			CString sql;
			sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

			CString sInfo;  
			try    
			{    
				m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
			}catch (_com_error e)    
			{  
				sInfo.Format(_T("数据库连接失败"));  
				MessageBox(sInfo);  
			}    
		} 
		else
		{
			MessageBox("请检查数据库有无连接！");
			return;
		}
	}   

	//连接指定表
	CString biao;  
	biao.Format("DELETE FROM HR.CARCHRCT WHERE CAR_ID ='%d'",car__id);  
	m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);   //删除记录

	//删除图片
	DeleteFile(car_Path);
	DeleteFile(face_Path);

	CString biao2;  
	biao2.Format("select * from hr.carchrct order by car_ID");  
	m_pRs1->Open((_bstr_t)biao2,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
	my_ctllist1.DeleteAllItems();  //  清空列表框  
	int nItem;  
	_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

	// 判断记录集指针标志，是否读取完毕  
	while(!m_pRs1->adoEOF)  
	{  
		// 获取记录集  
		vCarNum=m_pRs1->GetCollect("car_ID");
		vCarPlate=m_pRs1->GetCollect("car_plate");
		vCarLogo=m_pRs1->GetCollect("car_logo");
		vCarSize=m_pRs1->GetCollect("car_type");
		vCarColor=m_pRs1->GetCollect("car_colour");
		vCarTime=m_pRs1->GetCollect("car_time");
		vCarLocate=m_pRs1->GetCollect("car_locate");

		nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
		my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
		// 移动记录集指针到下一条  
		m_pRs1->MoveNext();  
	}  
	MessageBox("删除成功！");
}


//更新数据
void CTestCarDatabaseDlg::OnUpdataData()
{
	if(nItem<0){
		AfxMessageBox("请先选定列表框中的一行！");
		return;
	}
	CString CarID=my_ctllist1.GetItemText(nItem,0);  //获取车编号
	int car__id=atoi(CarID);       //将CString转换成Int类型

	CUpdataData a;
	int i=a.DoModal();

	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;
		car_time=a.car_time;
		car_locate=a.car_locate;

		//创建实例
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

		//连接指定数据库
		if(!m_pConn->State)    
		{    
			if (DataSourse!=""&&UseName!=""&&PassWord!="")
			{
				CString sql;
				sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

				CString sInfo;  
				try    
				{    
					m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				}catch (_com_error e)    
				{  
					sInfo.Format(_T("数据库连接失败"));  
					MessageBox(sInfo);  
				}    
			} 
			else
			{
				MessageBox("请检查数据库有无连接！");
				return;
			}

		}   

		_RecordsetPtr m_pRs1;//记录集指针
		m_pRs1.CreateInstance(__uuidof(Recordset));

		if (car_plate!="")
		{
			//连接指定表
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_PLATE='%s' WHERE CAR_ID = '%d'",car_plate,car__id); 
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}
		
		if (car_logo!="")
		{
			//连接指定表
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_LOGO='%s' WHERE CAR_ID = '%d'",car_logo,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_type!="")
		{
			//连接指定表
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_TYPE='%s' WHERE CAR_ID = '%d'",car_type,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_colour!="")
		{
			//连接指定表
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_COLOUR='%s' WHERE CAR_ID = '%d'",car_colour,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_time!="")
		{
			//连接指定表
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_TIME='%s' WHERE CAR_ID = '%d'",car_time,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_locate!="")
		{
			//连接指定表
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_LOCATE='%s' WHERE CAR_ID = '%d'",car_locate,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}


		CString biao;  
		biao.Format("select * from hr.carchrct order by car_ID");  
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		my_ctllist1.DeleteAllItems();  //  清空列表框  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

		// 判断记录集指针标志，是否读取完毕  
		while(!m_pRs->adoEOF)  
		{  
			// 获取记录集  
			vCarNum=m_pRs->GetCollect("car_ID");
			vCarPlate=m_pRs->GetCollect("car_plate");
			vCarLogo=m_pRs->GetCollect("car_logo");
			vCarSize=m_pRs->GetCollect("car_type");
			vCarColor=m_pRs->GetCollect("car_colour");
			vCarTime=m_pRs->GetCollect("car_time");
			vCarLocate=m_pRs->GetCollect("car_locate");

			nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
			my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
			// 移动记录集指针到下一条  
			m_pRs->MoveNext();  
		}  

		MessageBox("更新成功！");
	}
}


//多条录入
void CTestCarDatabaseDlg::OnInputMulti()
{
	//创建实例
	m_pConn.CreateInstance(__uuidof(Connection));    
	m_pRs.CreateInstance(__uuidof(Recordset));   
	m_pRs1.CreateInstance(__uuidof(Recordset)); 

	//连接指定数据库
	if(!m_pConn->State)    
	{    
		if (DataSourse!=""&&UseName!=""&&PassWord!="")
		{
			CString sql;
			sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

			CString sInfo;  
			try    
			{    
				m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
			}catch (_com_error e)    
			{  
				sInfo.Format(_T("数据库连接失败"));  
				MessageBox(sInfo);  
			}    
		} 
		else
		{
			MessageBox("请检查数据库有无连接！");
			return;
		}

	}   


	//选择图片文件夹路径
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner =GetSafeHwnd();
	bInfo.lpszTitle ="请选择图片文件夹路径:";
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS; 

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo); //显示选择对话框
	if(lpDlist != NULL) //用户按了确定按钮
	{
		TCHAR chPath[MAX_PATH]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		picturePath = chPath; //将TCHAR类型的字符串转换为CString类型的字符串
	}


	// 选择txt文件（存储图片信息的文件）的路径
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files(*.txt)||*.txt"), NULL);                           
	dlg.m_ofn.lpstrTitle = _T("选择存储图片信息的txt文件");    // 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )                    // 判断是否获得文件
		return;

	txtPath= dlg.GetPathName();            // 获取文件路径


	//遍历图片文件夹、读取txt文件
	if ((txtPath!="")&&(picturePath!=""))
	{
		//遍历图片文件夹所需参数
		CFileFind ff;
		CString strUDDir = picturePath;
		CString sk=strUDDir+"\\*.*";
		BOOL res = ff.FindFile(sk);
		CString trainImgPath;
		cv::Mat carImage;   //逐张读取车辆图片

		//读取txt文件所需参数
		std::ifstream in(txtPath);  //打开指定的txt文件
		std::string line,word;   //txt中的每行，每行中的每个单词
		int nextID=0;

		//遍历图片文件夹
		while(res)
		{
			res = ff.FindNextFile();
			//不遍历子目录
			if(!ff.IsDirectory() && !ff.IsDots())
			{
				//从txt文件中逐行读取数据并存于line中，直至数据全部读取
				std::getline(in,line);
				std::istringstream ss(line);
				int i=1;
				while(ss>>word)
				{
					if(i==1)
						car_plate=word.c_str();
					if(i==2)
						car_logo=word.c_str();
					if(i==3)
						car_type=word.c_str();
					if(i==4)
						car_colour=word.c_str();
					if(i==5)
						car_time=word.c_str();
					if(i==6)
						car_locate=word.c_str();
					i++;    
				}

				if (car_plate!=""&&car_logo!=""&&car_type!=""&&car_colour!=""&&car_time!=""&&car_locate!="")
				{
					//获取图片数据
					trainImgPath=ff.GetFilePath();
					carImage=cv::imread(trainImgPath.GetBuffer(0),1);

					//获取下一个car_ID
					if (nextID==0)
					{
						CString sql_next="SELECT MAX(CAR_ID)  MAXID FROM hr.CARCHRCT";
						m_pRs1->Open((_bstr_t)sql_next,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);
						try
						{
							nextID = m_pRs1->GetCollect("MAXID");
							nextID++;
						}
						catch (_com_error e)
						{
							nextID = 1;
						}

					} 
					else
					{
						nextID++;
					}
					

					//特征图片（人脸、车牌、车标等）
					cv::Mat ImageData; 

					//提取特征
					//提取人脸特征
					FaceDetect2 fd;
					if(carImage.data)
					{
						fd.draw(carImage);
						cv::Mat faceImage = fd.face_Image; //调用FaceDetect类的人脸检测函数

						if(faceImage.data)
							cv::resize(faceImage,ImageData,cv::Size(150,150));
						else
						{
							MessageBox("没有提取到人脸特征","错误提示！");
							return;
						}
					}
					else
					{
						MessageBox("车辆图片有误！","错误提示！");
						return;
					}  

					//下面还需提取车牌、车标等特征



					// 保存车辆、人脸、车牌、车标图片到指定文件夹
					std::string car_path,face_path;  //路径
					std::ostringstream oss;
					oss<<nextID;
					car_path="CarImage//"+oss.str()+".jpg";
					face_path="FaceImage//"+oss.str()+".jpg";
					cv::imwrite(face_path,ImageData);
					cv::imwrite(car_path,carImage);


					//连接指定表
					CString sql="SELECT * FROM hr.CARCHRCT";

					if (m_pRs->State)
					{
						;
					//	m_pRs->Close();
					//	m_pRs->Release();
					} 
					else
					{
						m_pRs->Open((_bstr_t)sql,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);
					}
					
					try  
					{   
						CString sql1;
						sql1.Format("insert into hr.CARCHRCT (car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate) values ('%d','%s','%s','%s','%s','%s','%s')", nextID,car_plate,car_logo,car_type,car_colour,car_time,car_locate);
						m_pConn->Execute((_bstr_t)sql1,NULL,adExecuteNoRecords);  
						MessageBox("添加成功！","提示");  
					}  
					catch(_com_error e)  
					{  
						MessageBox("添加失败！","提示"); 
					}  
				}
				else
					MessageBox("TXT文件内容与数据库不匹配！");
						
			}
		}

		ff.Close(); // 不要忘记关闭	
		MessageBox("添加数据完成！");
	}
	else
	{
		MessageBox("请选择正确路径","错误提示！");
	}
}


//单步演示运行
void CTestCarDatabaseDlg::OnStep()
{
	CStep a;
	a.DoModal();
}

//连续演示运行
void CTestCarDatabaseDlg::OnConstant()
{
	CConstant a;
	a.DoModal();
}
