#if !defined(AFX_DLGGOODSFIND_H__E16A2362_BA3C_4D55_B4AE_505CC8DBDC25__INCLUDED_)
#define AFX_DLGGOODSFIND_H__E16A2362_BA3C_4D55_B4AE_505CC8DBDC25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGoodsFind.h : header file
//
#include "GirdList.h"
/////////////////////////////////////////////////////////////////////////////
// DlgGoodsFind dialog

class DlgGoodsFind : public CDialog
{
// Construction
public:
	DlgGoodsFind(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgGoodsFind)
	enum { IDD = IDD_DLG_GOODSFIND };
	CEdit	m_RecCount;
	CButton	m_But_Update;
	CButton	m_But_Delete;
	CEdit	m_Edit_Filter;
	CComboBox	m_ComboOp;
	CComboBox	m_ComboCol;
	CGirdList	m_GoodsList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGoodsFind)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGoodsFind)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboCol();
	afx_msg void OnButDeletegoods();
	afx_msg void OnClickListGoods(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButUpdategoods();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGOODSFIND_H__E16A2362_BA3C_4D55_B4AE_505CC8DBDC25__INCLUDED_)
