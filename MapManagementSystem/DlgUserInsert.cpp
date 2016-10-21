// DlgUserInsert.cpp : implementation file
//

#include "stdafx.h"
#include "WZGL.h"
#include "DlgUserInsert.h"
#include "RelManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern DB* theDB;
/////////////////////////////////////////////////////////////////////////////
// DlgUserInsert dialog


DlgUserInsert::DlgUserInsert(CWnd* pParent /*=NULL*/)
	: CDialog(DlgUserInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgUserInsert)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgUserInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgUserInsert)
	DDX_Control(pDX, IDC_EDIT_UPASSW, m_UPassword);
	DDX_Control(pDX, IDC_EDIT_UNAME, m_UName);
	DDX_Control(pDX, IDC_COMBO_URIGHT, m_URight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgUserInsert, CDialog)
	//{{AFX_MSG_MAP(DlgUserInsert)
	ON_BN_CLICKED(IDC_BUT_CANCEL, OnButCancel)
	ON_BN_CLICKED(IDC_BUT_ADDUSER, OnButAdduser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgUserInsert message handlers

BOOL DlgUserInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_UName.SetWindowText("");
	this->m_UPassword.SetWindowText("");
	this->m_URight.AddString("��ͨ�û�");
	this->m_URight.AddString("����Ա");
	this->m_UName.SetFocus();
	this->m_URight.SetCurSel(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgUserInsert::OnButCancel() 
{
	// TODO: Add your control notification handler code here
	this->EndDialog(IDOK);
}

void DlgUserInsert::OnButAdduser() 
{
	// TODO: Add your control notification handler code here
	CString userName;
	this->m_UName.GetWindowText(userName);
	if(userName.GetLength()>20||userName.GetLength()<4)
	{
		MessageBox("�û����������������4-20���ַ����û���������");
		this->m_UName.SetWindowText("");
		this->m_UName.SetFocus();
		return;
	}
	Status status;
	RID tmpRid;
	ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_USER",status);
	ASSERT(status==OK);
	ASSERT(OK==scanOp->startScan(0,userName.GetLength(),P_VCHAR,LPCTSTR(userName),EQ));
	if(scanOp->scanNext(tmpRid)==OK)
	{
		MessageBox("�û����Ѵ��ڣ����������룡����");
		this->m_UName.SetWindowText("");
		this->m_UName.SetFocus();
		return;
	}
	delete scanOp;
	//
	CString password;
	this->m_UPassword.GetWindowText(password);
	if(password.GetLength()>20||password.GetLength()<4)
	{
		MessageBox("�����������������4-20���ַ������룡����");
		this->m_UPassword.SetWindowText("");
		this->m_UPassword.SetFocus();
		return;
	}
	//MessageBox((LPCTSTR)password);
	CString right;
	CString uright;
	this->m_URight.GetWindowText(right);
	if(right=="��ͨ�û�")
		uright='0';
	else if(right=="����Ա")
		uright='1';
	else
	{
		MessageBox("�û�Ȩ��ѡ�����������ô�����ģ�����");
		this->m_URight.SetCurSel(1);
		this->m_URight.SetFocus();
		return;
	}
	char userData[50];
	AttrInPage attrs[3];
	attrs[0].setNull(false);
	attrs[0].setLen(userName.GetLength());
	attrs[1].setNull(false);
	attrs[1].setLen(password.GetLength());
	attrs[2].setNull(false);
	attrs[2].setLen(uright.GetLength());
	unsigned int dataLen=userName.GetLength()+password.GetLength()+uright.GetLength();
	strncpy(userData,LPCTSTR(userName+password+uright),dataLen);
	userData[dataLen]='\0';
	ASSERT(strlen(userData)==dataLen);
	Record userRec;
	userRec.setAttrsInPage(attrs,3);
	userRec.setRealData(userData);
	InsertOperator* insertOp=new InsertOperator("WZGLDB.PLTDB","R_USER",status);
	ASSERT(status==OK);
	if(insertOp->insertRecord(userRec,tmpRid)==OK)
	{
		delete insertOp;
		MessageBox("������û��ɹ���");
		this->m_UName.SetWindowText("");
		this->m_UPassword.SetWindowText("");
		this->m_URight.SetCurSel(1);
		this->m_UName.SetFocus();
		bufMgr->flushFile(theDB);
		return;
	}
	else
	{
		delete insertOp;
		MessageBox("������û�ʧ�ܣ���������ϵ��");
		return;
	}
	return;
}
