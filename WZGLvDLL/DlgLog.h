#if !defined(AFX_DLGLOG_H__A8716BC8_923A_460E_AA12_953ED97B8EBB__INCLUDED_)
#define AFX_DLGLOG_H__A8716BC8_923A_460E_AA12_953ED97B8EBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgLog dialog

class DlgLog : public CDialog
{
// Construction
public:
	DlgLog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgLog)
	enum { IDD = IDD_DLG_LOGON };
	CEdit	m_Password;
	CEdit	m_UserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgLog)
	afx_msg void OnBut_EXIT();
	afx_msg void OnButLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOG_H__A8716BC8_923A_460E_AA12_953ED97B8EBB__INCLUDED_)
