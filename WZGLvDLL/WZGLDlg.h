// WZGLDlg.h : header file
//

#if !defined(AFX_WZGLDLG_H__84BD1C44_7A1E_457C_90BD_B33CEDCCB0DA__INCLUDED_)
#define AFX_WZGLDLG_H__84BD1C44_7A1E_457C_90BD_B33CEDCCB0DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWZGLDlg dialog

class CWZGLDlg : public CDialog
{
// Construction
public:
	CWZGLDlg(CWnd* pParent = NULL);	// standard constructor
	static char m_logSignal;
	static CString m_UserName;
	static CString m_Passw;
// Dialog Data
	//{{AFX_DATA(CWZGLDlg)
	enum { IDD = IDD_WZGL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWZGLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CStatusBar m_StatusBar;
	// Generated message map functions
	//{{AFX_MSG(CWZGLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMenuUserIn();
	afx_msg void OnMenuSysInit();
	afx_msg void OnClose();
	afx_msg void OnMenuRelogin();
	afx_msg void OnMenuUserFind();
	afx_msg void OnMenuUpdatepassw();
	afx_msg void OnMenuGoodsIn();
	afx_msg void OnMenuGoodsfind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WZGLDLG_H__84BD1C44_7A1E_457C_90BD_B33CEDCCB0DA__INCLUDED_)
