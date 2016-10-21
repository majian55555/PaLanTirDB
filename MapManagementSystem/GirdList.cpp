// GirdList.cpp : implementation file
//

#include "stdafx.h"
#include "WZGL.h"
#include "GirdList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGirdList

CGirdList::CGirdList()
{
}

CGirdList::~CGirdList()
{
}


BEGIN_MESSAGE_MAP(CGirdList, CListCtrl)
	//{{AFX_MSG_MAP(CGirdList)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICK, OnItemclick)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGirdList message handlers

void CGirdList::OnItemclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CGirdList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int curSel=this->GetSelectionMark();
	if(curSel!=-1)
	{
		CRect rect;
		this->GetItemRect(curSel,rect,LVIR_LABEL);
		if(point.x>rect.right||point.x<rect.left||point.y>rect.bottom||point.y<rect.top)
		{
			this->SetSelectionMark(-1);
		}
	}
	CListCtrl::OnLButtonDown(nFlags, point);
}
