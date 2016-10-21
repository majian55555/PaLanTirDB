// DLGUpDatePassw.cpp : implementation file
//

#include "stdafx.h"
#include "WZGL.h"
#include "DLGUpDatePassw.h"
#include "RelManager.h"
#include "WZGLDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLGUpDatePassw dialog


DLGUpDatePassw::DLGUpDatePassw(CWnd* pParent /*=NULL*/)
	: CDialog(DLGUpDatePassw::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLGUpDatePassw)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DLGUpDatePassw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLGUpDatePassw)
	DDX_Control(pDX, IDC_EDIT_OLD, m_OldPassw);
	DDX_Control(pDX, IDC_EDIT_NEW_CONFIRM, m_NewConfirm);
	DDX_Control(pDX, IDC_EDIT_NEW, m_NewPassw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLGUpDatePassw, CDialog)
	//{{AFX_MSG_MAP(DLGUpDatePassw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLGUpDatePassw message handlers

void DLGUpDatePassw::OnOK() 
{
	// TODO: Add extra validation here
	CString oldPassw;
	this->m_OldPassw.GetWindowText(oldPassw);
	if(oldPassw.Compare(LPCTSTR(CWZGLDlg::m_Passw))!=0)
	{
		MessageBox("原始密码输入错误！！！");
		this->m_OldPassw.SetWindowText("");
		this->m_OldPassw.SetFocus();
		return;
	}
	CString newPassw;
	this->m_NewPassw.GetWindowText(newPassw);
	if(newPassw.GetLength()>20||newPassw.GetLength()<4)
	{
		MessageBox("新密码输入错误，请输入4-20个字符的密码！！！");
		this->m_NewPassw.SetWindowText("");
		this->m_NewPassw.SetFocus();
		return;
	}
	CString confirm;
	this->m_NewConfirm.GetWindowText(confirm);
	if(confirm.Compare(newPassw)!=0)
	{
		MessageBox("两次密码输入不一致！！！");
		this->m_NewConfirm.SetWindowText("");
		this->m_NewConfirm.SetFocus();
		return;
	}
	AttrInPage attrs[3];
	attrs[0].setNull(false);
	attrs[0].setLen(CWZGLDlg::m_UserName.GetLength());
	attrs[1].setNull(false);
	attrs[1].setLen(newPassw.GetLength());
	attrs[2].setNull(false);
	attrs[2].setLen(1);
	char tmpChar[50];
	strncpy(&tmpChar[0],LPCTSTR(CWZGLDlg::m_UserName),CWZGLDlg::m_UserName.GetLength());
	strncpy(&tmpChar[CWZGLDlg::m_UserName.GetLength()],LPCTSTR(newPassw),newPassw.GetLength());
	tmpChar[CWZGLDlg::m_UserName.GetLength()+newPassw.GetLength()]=CWZGLDlg::m_logSignal;
	Record tmpRec;
	ASSERT(tmpRec.setAttrsInPage(attrs,3)==OK);
	tmpRec.setRealData(tmpChar);
	Status status;
	ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_USER",status);
	ASSERT(status==OK);
	ASSERT(scanOp->startScan(0,CWZGLDlg::m_UserName.GetLength(),P_VCHAR,LPCTSTR(CWZGLDlg::m_UserName),EQ)==OK);
	RID tmpRid=NULLRID;
	ASSERT(scanOp->scanNext(tmpRid)==OK);
	ASSERT(scanOp->updateRecord(tmpRec)==OK);
	delete scanOp;
	CWZGLDlg::m_Passw=newPassw;
	MessageBox("密码修改成功。");
	this->m_OldPassw.SetWindowText("");
	this->m_NewPassw.SetWindowText("");
	this->m_NewConfirm.SetWindowText("");
	//CDialog::OnOK();
}
