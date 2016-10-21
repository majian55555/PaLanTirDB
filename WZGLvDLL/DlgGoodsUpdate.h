#if !defined(AFX_DLGGOODSUPDATE_H__29E2C389_3BA6_4267_843C_88427509831B__INCLUDED_)
#define AFX_DLGGOODSUPDATE_H__29E2C389_3BA6_4267_843C_88427509831B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGoodsUpdate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgGoodsUpdate dialog

class DlgGoodsUpdate : public CDialog
{
// Construction
public:
	DlgGoodsUpdate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgGoodsUpdate)
	enum { IDD = IDD_DLG_GOODSUPDATE };
	CEdit	m_GoodsSerial;
	CEdit	m_Location2;
	CEdit	m_Location1;
	CEdit	m_YearVer;
	CEdit	m_UseDep;
	CEdit	m_TotalNum;
	CEdit	m_Scale;
	CEdit	m_PicCode;
	CEdit	m_RemainNum;
	CEdit	m_OutTime;
	CEdit	m_OutNum;
	CEdit	m_Script;
	CEdit	m_InTime;
	CEdit	m_GoodsName;
	CEdit	m_GoodsCode;
	//
	CString	m_str_YearVer;
	CString	m_str_UseDep;
	CString	m_str_TotalNum;
	CString	m_str_Scale;
	CString	m_str_PicCode;
	CString	m_str_RemainNum;
	CString	m_str_OutTime;
	CString	m_str_OutNum;
	CString	m_str_InTime;
	CString	m_str_GoodsName;
	CString	m_str_GoodsCode;
	CString	m_str_Location2;
	CString	m_str_Location1;
	CString	m_str_Script;
	CString	m_str_GoodsSerial;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGoodsUpdate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGoodsUpdate)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGOODSUPDATE_H__29E2C389_3BA6_4267_843C_88427509831B__INCLUDED_)
