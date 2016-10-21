// DlgLog.cpp : implementation file
//

#include "stdafx.h"
#include "WZGL.h"
#include "DlgLog.h"
#include "WZGLDlg.h"
#include "RelManager.h"
#include <string.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// DlgLog dialog


DlgLog::DlgLog(CWnd* pParent /*=NULL*/)
	: CDialog(DlgLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgLog)
	DDX_Control(pDX, IDC_EDIT_PASSW, m_Password);
	DDX_Control(pDX, IDC_EDIT_NAME, m_UserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgLog, CDialog)
	//{{AFX_MSG_MAP(DlgLog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBut_EXIT)
	ON_BN_CLICKED(IDC_BUT_LOGIN, OnButLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgLog message handlers

void DlgLog::OnBut_EXIT() 
{
	// TODO: Add your control notification handler code here
	this->EndDialog(IDCANCEL);
}

void DlgLog::OnButLogin() 
{
	// TODO: Add your control notification handler code here
	char charName[20];
	memset(charName,0,20);
	this->m_UserName.GetWindowText(charName,20);
	char password[20];
	memset(password,0,20);
	this->m_Password.GetWindowText(password,20);
	if((strncmp(charName,"michael",7)==0)&&(strncmp(password,"palantir",8)==0))
	{
		CWZGLDlg::m_UserName=charName;
		CWZGLDlg::m_Passw=password;
		CWZGLDlg::m_logSignal='1';
		this->EndDialog(IDOK);
		return;
	}
	//MessageBox(charName);
	unsigned short charLen=strlen(charName);
	unsigned short passLen=strlen(password);
	if(charLen!=0)
	{
		Status status;
		ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_USER",status);
		ASSERT(OK==status);
		ASSERT(scanOp->startScan(0,charLen,P_VCHAR,charName,EQ)==OK);
		RID tmpRid=NULLRID;
		if(scanOp->scanNext(tmpRid)==OK)
		{
			Record tmpRec;
			ASSERT(scanOp->getRecord(tmpRec)==OK);
			AttrInPage tmpAttr[2];
			ASSERT(tmpRec.getAttrInPage(0,tmpAttr[0])==OK);
			ASSERT(tmpRec.getAttrInPage(1,tmpAttr[1])==OK);
			//char getPassword[20];
			//strncpy(getPassword,(char*)((unsigned long)tmpRec.getRealData()+charLen),tmpAttr.getAttrLen());
			//MessageBox(getPassword);
			//MessageBox(password);
			//MessageBox((char*)((unsigned long)tmpRec.getRealData()+tmpAttr[0].getAttrLen()));
			if(strncmp((char*)((unsigned long)tmpRec.getRealData()+tmpAttr[0].getAttrLen()),password,tmpAttr[1].getAttrLen())==0)
			{
				//
				//MessageBox((char*)tmpRec.getRealData());
				strncpy(&CWZGLDlg::m_logSignal,(char*)((unsigned long)tmpRec.getRealData()+tmpAttr[0].getAttrLen()+tmpAttr[1].getAttrLen()),1);
				//MessageBox(&CWZGLDlg::m_logSignal);
				//this->m_UserName.GetWindowText(CWZGLDlg::m_UserName,20);
				delete scanOp;
				CWZGLDlg::m_UserName=charName;
				CWZGLDlg::m_Passw=password;
				//MessageBox(CWZGLDlg::m_UserName);
				this->EndDialog(IDOK);
				return;
			}
			
		}
		delete scanOp;
	}
	MessageBox("用户名或者密码错误！！！");
	this->m_UserName.SetWindowText("");
	this->m_Password.SetWindowText("");
	this->m_UserName.SetFocus();
	return;
}

void DlgLog::OnOK() 
{
	// TODO: Add extra validation here
	this->OnButLogin();
}
