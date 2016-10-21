// WZGLDlg.cpp : implementation file
//
#include "stdafx.h"
#include "WZGL.h"
#include "WZGLDlg.h"
#include "DlgUserInsert.h"
#include "Dlg_User_Find.h"
#include "DlgGoodsIn.h"
#include "DlgGoodsFind.h"
#include "DLGUpdatePassw.h"
#include "DlgLog.h"
#include "RelManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
DBManager dbManager;
BufMgr* bufMgr=NULL;
DB* theDB=NULL; 
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
char CWZGLDlg::m_logSignal='0';
CString CWZGLDlg::m_UserName="";
CString CWZGLDlg::m_Passw="";
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWZGLDlg dialog

CWZGLDlg::CWZGLDlg(CWnd* pParent /*=NULL*/):CDialog(CWZGLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWZGLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWZGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWZGLDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWZGLDlg, CDialog)
	//{{AFX_MSG_MAP(CWZGLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_USER_IN, OnMenuUserIn)
	ON_COMMAND(ID_MENU_SYS_INIT, OnMenuSysInit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_RELOGIN, OnMenuRelogin)
	ON_COMMAND(ID_MENU_USER_FIND, OnMenuUserFind)
	ON_COMMAND(ID_MENU_UPDATEPASSW, OnMenuUpdatepassw)
	ON_COMMAND(ID_MENU_GOODS_IN, OnMenuGoodsIn)
	ON_COMMAND(ID_MENU_GOODSFIND, OnMenuGoodsfind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWZGLDlg message handlers

BOOL CWZGLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	if(OK!=dbManager.openDataBase("WZGLDB.PLTDB",::theDB))
	{
		MessageBox("打开数据库失败，请与管理员联系!");
	}
	bufMgr=new BufMgr(1000);
	DlgLog logIn;
	int nResponse = logIn.DoModal();
	if (nResponse == IDCANCEL)
	{
		delete bufMgr;
		this->EndDialog(IDOK);
		return FALSE;
	}
	this->m_StatusBar.Create(this);
	UINT  Indicates[6]; 
	for (int i=0;i<6;i++)
		Indicates[i]=50+i;
	this->m_StatusBar.SetIndicators(Indicates,6);
	CRect rect;
	GetClientRect(rect);
	UINT PaneWidth = rect.Width()/6;
	//设置面板宽度
	for(int n=0;n<6;n++)
	{
		this->m_StatusBar.SetPaneInfo(n,50+n*10,SBPS_NORMAL,PaneWidth);
	}
	//设置状态栏面板文本
	this->m_StatusBar.SetPaneText(0,"用户名：");
	this->m_StatusBar.SetPaneText(1,CWZGLDlg::m_UserName);
	this->m_StatusBar.SetPaneText(2,"日期：");
	CString str=CTime::GetCurrentTime().Format("%Y-%m-%d");
	this->m_StatusBar.SetPaneText(3,str);
	this->m_StatusBar.SetPaneText(4,"用户类型：");
	if(CWZGLDlg::m_logSignal=='1')
		this->m_StatusBar.SetPaneText(5,"管理员");
	else
		this->m_StatusBar.SetPaneText(5,"普通用户");
	this->m_StatusBar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWZGLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWZGLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWZGLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWZGLDlg::OnMenuUserIn() 
{
	// TODO: Add your command handler code here
	if(CWZGLDlg::m_logSignal=='0')//普通用户
	{
		MessageBox("您无权进行此项操作，请与管理员联系。");
		return;
	}
	DlgUserInsert userInsert;
	userInsert.DoModal();
}

void CWZGLDlg::OnMenuSysInit() 
{
	// TODO: Add your command handler code here
	if(CWZGLDlg::m_logSignal=='0')//普通用户
	{
		MessageBox("您无权进行此项操作，请与管理员联系。");
		return;
	}
	if(MessageBox("初始化系统会删除原有数据，是否继续？！！！","警告",MB_ICONWARNING | MB_YESNO)==IDYES)
	{
		if(OK!=dbManager.closeDataBase("WZGLDB.PLTDB"))
		{
			MessageBox("关闭数据库失败，请与管理员联系!");
			this->DestroyWindow();
		}
		::dbManager.destroyDataBase("WZGLDB.PLTDB");
		if(::dbManager.createDataBase("WZGLDB.PLTDB")!=OK)
		{
			MessageBox("创建数据库失败，请与管理员联系!");
			this->DestroyWindow();
		}
		if(::createRel("WZGLDB.PLTDB","R_USER")!=OK)
		{
			MessageBox("创建用户表失败，请与管理员联系!");
			this->DestroyWindow();
		}
		if(::createRel("WZGLDB.PLTDB","R_MATERIEL")!=OK)
		{
			MessageBox("创建地图表失败，请与管理员联系!");
			this->DestroyWindow();
		}
		Record tmpRec;
		AttrInPage userAttrs[3];//用户表有3个字段，插入一个用户名为admin，密码为admin的用户，权限为1（管理员）
		userAttrs[0].setNull(false);
		userAttrs[0].setLen(5);//用户名
		userAttrs[1].setNull(false);
		userAttrs[1].setLen(5);//用户密码
		userAttrs[2].setNull(false);
		userAttrs[2].setLen(1);//用户权限
		char tmpData[]="adminadmin1";
		//MessageBox(tmpData);
		ASSERT(strlen(tmpData)==11);
		tmpRec.setAttrsInPage(userAttrs,3);
		tmpRec.setRealData(tmpData);
		Status status;
		InsertOperator* insertOp=new InsertOperator("WZGLDB.PLTDB","R_USER",status);
		ASSERT(status==OK);
		RID tmpRid=NULLRID;
		if(insertOp->insertRecord(tmpRec,tmpRid)!=OK)
		{
			MessageBox("用户记录插入失败，请与管理员联系!");
			this->DestroyWindow();
		}
		MessageBox("初始化系统成功，初始管理员名为admin，密码为admin。请及时修改密码。");
		delete insertOp;
		if(OK!=dbManager.openDataBase("WZGLDB.PLTDB",::theDB))
		{
			MessageBox("打开数据库失败，请与管理员联系!");
		}
	}
	return;
}

void CWZGLDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(MessageBox("是否退出！！！","提示",MB_ICONWARNING | MB_YESNO)==IDYES)
	{
		bufMgr->flushFile(::theDB);
		delete bufMgr;
		if(OK!=dbManager.closeDataBase("WZGLDB.PLTDB"))
		{
			MessageBox("关闭数据库失败，请与管理员联系!");
		}
		CDialog::OnClose();
	}
}

void CWZGLDlg::OnMenuRelogin() 
{
	// TODO: Add your command handler code here
	DlgLog logIn;
	logIn.DoModal();
	this->m_StatusBar.SetPaneText(1,CWZGLDlg::m_UserName);
	CString str=CTime::GetCurrentTime().Format("%Y-%m-%d");
	this->m_StatusBar.SetPaneText(3,str);
	if(CWZGLDlg::m_logSignal=='1')
		this->m_StatusBar.SetPaneText(5,"管理员");
	else
		this->m_StatusBar.SetPaneText(5,"普通用户");
}

void CWZGLDlg::OnMenuUserFind() 
{
	// TODO: Add your command handler code here
	if(CWZGLDlg::m_logSignal=='0')//普通用户
	{
		MessageBox("您无权进行此项操作，请与管理员联系。");
		return;
	}
	Dlg_User_Find userFind;
	userFind.DoModal();
}

void CWZGLDlg::OnMenuUpdatepassw() 
{
	// TODO: Add your command handler code here
	DLGUpDatePassw changPassw;
	changPassw.DoModal();
}

void CWZGLDlg::OnMenuGoodsIn() 
{
	// TODO: Add your command handler code here
	DlgGoodsIn goodsIn;
	goodsIn.DoModal();
}

void CWZGLDlg::OnMenuGoodsfind() 
{
	// TODO: Add your command handler code here
	DlgGoodsFind goodsFind;
	goodsFind.DoModal();
}
