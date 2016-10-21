#if !defined(AFX_DLG_USER_FIND_H__C196C9A2_B954_445B_BF5F_ECBE31F107CA__INCLUDED_)
#define AFX_DLG_USER_FIND_H__C196C9A2_B954_445B_BF5F_ECBE31F107CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_User_Find.h : header file
//
#include "GirdList.h"
/////////////////////////////////////////////////////////////////////////////
// Dlg_User_Find dialog

class Dlg_User_Find : public CDialog
{
// Construction
public:
	Dlg_User_Find(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dlg_User_Find)
	enum { IDD = IDD_DLG_USERFIND };
	CButton	m_But_DeleteUser;
	CGirdList	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dlg_User_Find)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dlg_User_Find)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnClickListUserfind(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButDeleteuser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_USER_FIND_H__C196C9A2_B954_445B_BF5F_ECBE31F107CA__INCLUDED_)
