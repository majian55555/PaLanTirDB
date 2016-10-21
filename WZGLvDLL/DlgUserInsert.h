#if !defined(AFX_DLGUSERINSERT_H__FBC21713_A0D5_4D5B_817A_61B77B50864F__INCLUDED_)
#define AFX_DLGUSERINSERT_H__FBC21713_A0D5_4D5B_817A_61B77B50864F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUserInsert.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgUserInsert dialog

class DlgUserInsert : public CDialog
{
// Construction
public:
	DlgUserInsert(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgUserInsert)
	enum { IDD = IDD_DLG_USERIN };
	CEdit	m_UPassword;
	CEdit	m_UName;
	CComboBox	m_URight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgUserInsert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgUserInsert)
	virtual BOOL OnInitDialog();
	afx_msg void OnButCancel();
	afx_msg void OnButAdduser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSERINSERT_H__FBC21713_A0D5_4D5B_817A_61B77B50864F__INCLUDED_)
