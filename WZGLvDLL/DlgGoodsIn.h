#if !defined(AFX_DLGGOODSIN_H__6F4AB3C0_9C90_4711_887D_A44A9B9BEB07__INCLUDED_)
#define AFX_DLGGOODSIN_H__6F4AB3C0_9C90_4711_887D_A44A9B9BEB07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGoodsIn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgGoodsIn dialog

class DlgGoodsIn : public CDialog
{
// Construction
public:
	DlgGoodsIn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgGoodsIn)
	enum { IDD = IDD_DLG_GOODSIN };
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
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGoodsIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGoodsIn)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_LastGoodsCode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGOODSIN_H__6F4AB3C0_9C90_4711_887D_A44A9B9BEB07__INCLUDED_)
