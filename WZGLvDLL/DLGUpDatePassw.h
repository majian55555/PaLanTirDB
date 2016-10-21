#if !defined(AFX_DLGUPDATEPASSW_H__22B7C83A_CF1C_4F9B_BE3D_C4C87AD6E41F__INCLUDED_)
#define AFX_DLGUPDATEPASSW_H__22B7C83A_CF1C_4F9B_BE3D_C4C87AD6E41F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGUpDatePassw.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DLGUpDatePassw dialog

class DLGUpDatePassw : public CDialog
{
// Construction
public:
	DLGUpDatePassw(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DLGUpDatePassw)
	enum { IDD = IDD_DLG_UPDATEPASSW };
	CEdit	m_OldPassw;
	CEdit	m_NewConfirm;
	CEdit	m_NewPassw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLGUpDatePassw)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLGUpDatePassw)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUPDATEPASSW_H__22B7C83A_CF1C_4F9B_BE3D_C4C87AD6E41F__INCLUDED_)
