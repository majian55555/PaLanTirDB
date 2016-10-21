// Dlg_User_Find.cpp : implementation file
//

#include "stdafx.h"
#include "WZGL.h"
#include "Dlg_User_Find.h"
#include "RelManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dlg_User_Find dialog


Dlg_User_Find::Dlg_User_Find(CWnd* pParent /*=NULL*/)
	: CDialog(Dlg_User_Find::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dlg_User_Find)
	//}}AFX_DATA_INIT
}


void Dlg_User_Find::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dlg_User_Find)
	DDX_Control(pDX, IDC_BUT_DELETEUSER, m_But_DeleteUser);
	DDX_Control(pDX, IDC_LIST_USERFIND, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dlg_User_Find, CDialog)
	//{{AFX_MSG_MAP(Dlg_User_Find)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USERFIND, OnClickListUserfind)
	ON_BN_CLICKED(IDC_BUT_DELETEUSER, OnButDeleteuser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dlg_User_Find message handlers

void Dlg_User_Find::OnOK() 
{
	// TODO: Add extra validation here
	/*int curSel=this->m_List.GetSelectionMark();
	char tmpChar[10];
	itoa(curSel,tmpChar,10);
	MessageBox(tmpChar);*/
	this->m_List.DeleteAllItems();
	Status status;
	ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_USER",status);
	ASSERT(status==OK);
	ASSERT(OK==scanOp->startScan(0,0,P_CHAR,NULL,EQ));
	RID tmpRid=NULLRID;
	for(int j=0;(status=scanOp->scanNext(tmpRid))==OK;j++)
	{
		this->m_List.InsertItem(j,"");
		Record tmpRec;
		ASSERT(scanOp->getRecord(tmpRec)==OK);
		unsigned long offset=0;
		for(int i=0;i<2;i++)
		{
			AttrInPage tmpAttr;
			tmpRec.getAttrInPage(i,tmpAttr);
			char tmpChar[20];
			strncpy(tmpChar,(char*)((unsigned long)tmpRec.getRealData()+offset),tmpAttr.getAttrLen());
			tmpChar[tmpAttr.getAttrLen()]='\0';
			this->m_List.SetItemText(j,i,tmpChar);
			offset+=tmpAttr.getAttrLen();
		}
		char chrRight='1';
		if(strncmp(&chrRight,(char*)((unsigned long)tmpRec.getRealData()+offset),1)==0)
			this->m_List.SetItemText(j,i,"管理员");
		else
			this->m_List.SetItemText(j,i,"普通用户");
	}
	ASSERT(status==FILEEOF);
	delete scanOp;
	//CDialog::OnOK();
}

BOOL Dlg_User_Find::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	this->m_List.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	this->m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_TWOCLICKACTIVATE |LVS_EX_GRIDLINES);
	this->m_List.InsertColumn(0,"用户名",LVCFMT_LEFT,120);
	this->m_List.InsertColumn(1,"用户密码",LVCFMT_LEFT,120);
	this->m_List.InsertColumn(2,"用户权限",LVCFMT_LEFT,80);
	this->m_But_DeleteUser.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void Dlg_User_Find::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void Dlg_User_Find::OnClickListUserfind(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//this->m_But_DeleteUser.EnableWindow(FALSE);
	int curSel=this->m_List.GetSelectionMark();
	if(curSel!=-1)
		this->m_But_DeleteUser.EnableWindow();
	else
		this->m_But_DeleteUser.EnableWindow(FALSE);
	//char tmp[10];
	//itoa(curSel,tmp,10);
	//MessageBox(tmp);
	*pResult = 0;
}


void Dlg_User_Find::OnButDeleteuser()
{
	// TODO: Add your control notification handler code here
	int curSel=this->m_List.GetSelectionMark();
	CString chrUName=this->m_List.GetItemText(curSel,0);
	if(MessageBox("是否删除用户名为“"+chrUName+"”的用户？","提示",MB_ICONWARNING | MB_YESNO)==IDYES)
	{
		Status status;
		ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_USER",status);
		ASSERT(OK==scanOp->startScan(0,chrUName.GetLength(),P_VCHAR,LPCTSTR(chrUName),EQ));
		RID tmpRid=NULLRID;
		ASSERT(OK==scanOp->scanNext(tmpRid));
		ASSERT(OK==scanOp->deleteRecord());
		delete scanOp;
		this->m_List.DeleteItem(curSel);
	}
}
