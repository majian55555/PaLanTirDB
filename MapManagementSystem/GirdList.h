#if !defined(AFX_GIRDLIST_H__15012E8F_1D7C_4E99_8E84_829A2518B4DB__INCLUDED_)
#define AFX_GIRDLIST_H__15012E8F_1D7C_4E99_8E84_829A2518B4DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GirdList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGirdList window

class CGirdList : public CListCtrl
{
// Construction
public:
	CGirdList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGirdList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGirdList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGirdList)
	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GIRDLIST_H__15012E8F_1D7C_4E99_8E84_829A2518B4DB__INCLUDED_)
