// WZGL.h : main header file for the WZGL application
//

#if !defined(AFX_WZGL_H__8CCF8225_DAFC_4582_9984_6861B4BBB77C__INCLUDED_)
#define AFX_WZGL_H__8CCF8225_DAFC_4582_9984_6861B4BBB77C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWZGLApp:
// See WZGL.cpp for the implementation of this class
//

class CWZGLApp : public CWinApp
{
public:
	CWZGLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWZGLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWZGLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WZGL_H__8CCF8225_DAFC_4582_9984_6861B4BBB77C__INCLUDED_)
