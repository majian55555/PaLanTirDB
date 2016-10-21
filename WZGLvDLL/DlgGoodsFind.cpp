// DlgGoodsFind.cpp : implementation file
//

#include "stdafx.h"
#include "WZGL.h"
#include "DlgGoodsFind.h"
#include "RelManager.h"
#include "DlgGoodsUpdate.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgGoodsFind dialog


DlgGoodsFind::DlgGoodsFind(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGoodsFind::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGoodsFind)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgGoodsFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGoodsFind)
	DDX_Control(pDX, IDC_EDITRECCOUNT, m_RecCount);
	DDX_Control(pDX, ID_BUT_UPDATEGOODS, m_But_Update);
	DDX_Control(pDX, ID_BUT_DELETEGOODS, m_But_Delete);
	DDX_Control(pDX, IDC_EDIT_FILTER, m_Edit_Filter);
	DDX_Control(pDX, IDC_COMBO_OP, m_ComboOp);
	DDX_Control(pDX, IDC_COMBO_COL, m_ComboCol);
	DDX_Control(pDX, IDC_LIST_GOODS, m_GoodsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGoodsFind, CDialog)
	//{{AFX_MSG_MAP(DlgGoodsFind)
	ON_CBN_SELCHANGE(IDC_COMBO_COL, OnSelchangeComboCol)
	ON_BN_CLICKED(ID_BUT_DELETEGOODS, OnButDeletegoods)
	ON_NOTIFY(NM_CLICK, IDC_LIST_GOODS, OnClickListGoods)
	ON_BN_CLICKED(ID_BUT_UPDATEGOODS, OnButUpdategoods)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGoodsFind message handlers

void DlgGoodsFind::OnOK() 
{
	// TODO: Add extra validation here
	this->m_GoodsList.DeleteAllItems();
	unsigned short colIdx=0;
	CString strFilter,exFilter;
	this->m_Edit_Filter.GetWindowText(strFilter);
	Status status;
	ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_MATERIEL",status);
	ASSERT(status==OK);
	int curSel=this->m_ComboCol.GetCurSel();
	/*
	this->m_ComboCol.InsertString(0,"0:������");
	this->m_ComboCol.InsertString(1,"1:��ͼ���");
	this->m_ComboCol.InsertString(2,"2:��ͼ����");
	this->m_ComboCol.InsertString(3,"3:ͼ����");
	this->m_ComboCol.InsertString(4,"4:���");
	this->m_ComboCol.InsertString(5,"5:������");
	this->m_ComboCol.InsertString(6,"6:ʹ�õ�λ");
	this->m_ComboCol.InsertString(7,"7:����Ų�ѯ");
	this->m_ComboCol.InsertString(8,"8:�����+��Ų�ѯ");
	*/
	switch(curSel)
	{
	case 0://������
		break;
	case 1://���
		colIdx=1;
		break;
	case 2://����
		colIdx=2;
		break;
	case 3://ͼ��
		colIdx=6;
		break;
	case 4://���
		colIdx=7;
		break;
	case 5://����
		colIdx=8;
		break;
	case 6://��λ
		colIdx=11;
		break;
	case 7://��
		colIdx=12;
		break;
	case 8://��+��
		colIdx=12;
		if(strFilter.GetLength()!=8)
		{
			MessageBox("����ȷ������źͲ�ţ���8λ����");
			this->m_Edit_Filter.SetWindowText("");
			this->m_Edit_Filter.SetFocus();
			return;
		}
		exFilter=strFilter.Right(4);
		strFilter=strFilter.Left(4);
		break;
	default:
		MessageBox("���ִ����������Ա��ϵ��");
		break;
	}
	if(curSel==0)
		ASSERT(scanOp->startScan(colIdx,strFilter.GetLength(),P_VCHAR,NULL,EQ)==OK);
	else
	{
		int curSelOp=this->m_ComboOp.GetCurSel();
		switch(curSelOp)
		{
		case 0:
			ASSERT(scanOp->startScan(colIdx,strFilter.GetLength(),P_VCHAR,LPCTSTR(strFilter),NE)==OK);
			break;
		case 1:
			ASSERT(scanOp->startScan(colIdx,strFilter.GetLength(),P_VCHAR,LPCTSTR(strFilter),LT)==OK);
			break;
		case 2:
			ASSERT(scanOp->startScan(colIdx,strFilter.GetLength(),P_VCHAR,LPCTSTR(strFilter),LTE)==OK);
			break;
		case 3:
			ASSERT(scanOp->startScan(colIdx,strFilter.GetLength(),P_VCHAR,LPCTSTR(strFilter),EQ)==OK);
			break;
		case 4:
			ASSERT(scanOp->startScan(colIdx,strFilter.GetLength(),P_VCHAR,LPCTSTR(strFilter),GT)==OK);
			break;
		case 5:
			ASSERT(scanOp->startScan(colIdx,strFilter.GetLength(),P_VCHAR,LPCTSTR(strFilter),GTE)==OK);
			break;
		default:
			MessageBox("���ִ����������Ա��ϵ��");
			break;
		}
	}
	RID tmpRid=NULLRID;
	int count=0;
	while((status=scanOp->scanNext(tmpRid))==OK)
	{
		this->m_GoodsList.InsertItem(count,"");
		Record tmpRec;
		ASSERT(scanOp->getRecord(tmpRec)==OK);
		unsigned long offset=0;
		for(int i=0;i<15;i++)
		{
			AttrInPage tmpAttr;
			tmpRec.getAttrInPage(i,tmpAttr);
			char tmpChar[3300];
			strncpy(tmpChar,(char*)((unsigned long)tmpRec.getRealData()+offset),tmpAttr.getAttrLen());
			tmpChar[tmpAttr.getAttrLen()]='\0';
			if(i==8)
			{
				CString tmpStr="1:";
				tmpStr+=tmpChar;
				this->m_GoodsList.SetItemText(count,i,tmpStr);
			}
			else
				this->m_GoodsList.SetItemText(count,i,tmpChar);
			offset+=tmpAttr.getAttrLen();
		}
	}
	ASSERT(status==FILEEOF);
	delete scanOp;
	if(curSel==8)//��+��
	{
		int j=this->m_GoodsList.GetItemCount(),k=0;
		for(int i=0;i<j;i++)
		{
			char chrLocation2[5];
			this->m_GoodsList.GetItemText(i-k,13,chrLocation2,5);
			if(strcmp(LPCTSTR(exFilter),chrLocation2)!=0)
			{
				this->m_GoodsList.DeleteItem(i-k);
				k++;
			}
		}
	}
	char chrCount[4];
	itoa(this->m_GoodsList.GetItemCount(),chrCount,10);
	this->m_RecCount.SetWindowText(chrCount);
	//CDialog::OnOK();
}

BOOL DlgGoodsFind::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->m_GoodsList.DeleteAllItems();
	// TODO: Add extra initialization here
	this->m_GoodsList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	this->m_GoodsList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_TWOCLICKACTIVATE |LVS_EX_GRIDLINES);
	this->m_GoodsList.InsertColumn(0,"��ͼ����",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(1,"��ͼ���",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(2,"��ͼ����",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(3,"��ͼ������",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(4,"��ͼ������",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(5,"��ͼ������",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(6,"��ͼͼ����",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(7,"���",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(8,"������",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(9,"��ͼ���ʱ��",LVCFMT_LEFT,120);
	this->m_GoodsList.InsertColumn(10,"��ͼ����ʱ��",LVCFMT_LEFT,120);
	this->m_GoodsList.InsertColumn(11,"ʹ�õ�λ",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(12,"���",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(13,"���",LVCFMT_LEFT,100);
	this->m_GoodsList.InsertColumn(14,"��ע",LVCFMT_LEFT,200);
//
	this->m_But_Delete.EnableWindow(FALSE);
	this->m_But_Update.EnableWindow(FALSE);
	this->m_ComboOp.EnableWindow(FALSE);
	this->m_Edit_Filter.EnableWindow(FALSE);
	//
	//this->m_ComboCol.InsertString(int nIndex,LPCTSTR lpszString);
	this->m_ComboCol.InsertString(0,"0:������");
	this->m_ComboCol.InsertString(1,"1:��ͼ���");
	this->m_ComboCol.InsertString(2,"2:��ͼ����");
	this->m_ComboCol.InsertString(3,"3:ͼ����");
	this->m_ComboCol.InsertString(4,"4:���");
	this->m_ComboCol.InsertString(5,"5:������");
	this->m_ComboCol.InsertString(6,"6:ʹ�õ�λ");
	this->m_ComboCol.InsertString(7,"7:����Ų�ѯ");
	this->m_ComboCol.InsertString(8,"8:�����+��Ų�ѯ");
	
	this->m_ComboCol.SetCurSel(0);
	this->m_ComboOp.AddString("=");
	this->m_ComboOp.AddString(">=");
	this->m_ComboOp.AddString(">");
	this->m_ComboOp.AddString("<=");
	this->m_ComboOp.AddString("<");
	this->m_ComboOp.AddString("!=");
	this->m_ComboOp.SetCurSel(3);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgGoodsFind::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DlgGoodsFind::OnSelchangeComboCol() 
{
	// TODO: Add your control notification handler code here
	int curSel=this->m_ComboCol.GetCurSel();
	if(curSel!=0)
	{
		this->m_ComboOp.EnableWindow();
		this->m_ComboOp.SetCurSel(3);
		this->m_Edit_Filter.EnableWindow();
		this->m_Edit_Filter.SetWindowText(NULL);
	}
	else
	{
		this->m_ComboOp.EnableWindow(FALSE);
		this->m_ComboOp.SetCurSel(3);
		this->m_Edit_Filter.EnableWindow(FALSE);
		this->m_Edit_Filter.SetWindowText(NULL);
	}
}

void DlgGoodsFind::OnButDeletegoods() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("�Ƿ�ɾ���ü�¼��","��ʾ",MB_ICONWARNING | MB_YESNO)==IDYES)
	{
		int curSel=this->m_GoodsList.GetSelectionMark();
		if(curSel==-1)
		{
			MessageBox("���󣡣���");
			this->m_But_Delete.EnableWindow(FALSE);
			this->m_But_Update.EnableWindow(FALSE);
			return;
		}
		CString strGoodsCode=this->m_GoodsList.GetItemText(curSel,0);
		Status status;
		RID tmpRid=NULLRID;
		ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_MATERIEL",status);
		ASSERT(status==OK);
		ASSERT(scanOp->startScan(0,strGoodsCode.GetLength(),P_CHAR,LPCTSTR(strGoodsCode),EQ)==OK);
		ASSERT(scanOp->scanNext(tmpRid)==OK);
		if(scanOp->deleteRecord()!=OK)
			MessageBox("ɾ����¼ʧ�ܣ�����");
		delete scanOp;
		MessageBox("ɾ����¼�ɹ���");
		this->m_ComboCol.SetCurSel(0);
		this->m_ComboOp.EnableWindow(FALSE);
		this->m_ComboOp.SetCurSel(3);
		this->m_Edit_Filter.EnableWindow(FALSE);
		this->m_Edit_Filter.SetWindowText(NULL);
		this->m_But_Delete.EnableWindow(FALSE);
		this->m_But_Update.EnableWindow(FALSE);
		this->OnOK();
	}
}

void DlgGoodsFind::OnClickListGoods(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int curSel=this->m_GoodsList.GetSelectionMark();
	if(curSel!=-1)
	{
		this->m_But_Delete.EnableWindow();
		this->m_But_Update.EnableWindow();
	}
	else
	{
		this->m_But_Delete.EnableWindow(FALSE);
		this->m_But_Update.EnableWindow(FALSE);
	}
	*pResult = 0;
}

void DlgGoodsFind::OnButUpdategoods() 
{
	// TODO: Add your control notification handler code here
	int curSel=this->m_GoodsList.GetSelectionMark();
	if(curSel==-1)
	{
		MessageBox("���󣡣���");
		this->m_But_Delete.EnableWindow(FALSE);
		this->m_But_Update.EnableWindow(FALSE);
		return;
	}
	DlgGoodsUpdate goodsUpdate;
	goodsUpdate.m_str_GoodsCode=this->m_GoodsList.GetItemText(curSel,0);
	goodsUpdate.m_str_GoodsSerial=this->m_GoodsList.GetItemText(curSel,1);
	goodsUpdate.m_str_GoodsName=this->m_GoodsList.GetItemText(curSel,2);
	goodsUpdate.m_str_TotalNum=this->m_GoodsList.GetItemText(curSel,3);
	goodsUpdate.m_str_RemainNum=this->m_GoodsList.GetItemText(curSel,4);
	goodsUpdate.m_str_OutNum=this->m_GoodsList.GetItemText(curSel,5);
	goodsUpdate.m_str_PicCode=this->m_GoodsList.GetItemText(curSel,6);
	goodsUpdate.m_str_YearVer=this->m_GoodsList.GetItemText(curSel,7);
	goodsUpdate.m_str_Scale=this->m_GoodsList.GetItemText(curSel,8);
	goodsUpdate.m_str_Scale.Delete(0,2);
	goodsUpdate.m_str_InTime=this->m_GoodsList.GetItemText(curSel,9);
	goodsUpdate.m_str_OutTime=this->m_GoodsList.GetItemText(curSel,10);
	goodsUpdate.m_str_UseDep=this->m_GoodsList.GetItemText(curSel,11);
	goodsUpdate.m_str_Location1=this->m_GoodsList.GetItemText(curSel,12);
	goodsUpdate.m_str_Location2=this->m_GoodsList.GetItemText(curSel,13);
	goodsUpdate.m_str_Script=this->m_GoodsList.GetItemText(curSel,14);
	//goodsUpdate.Create(IDD_DLG_GOODSUPDATE);
	//goodsUpdate.m_GoodsCode.SetWindowText(this->m_GoodsList.GetItemText(curSel,0));
	if(goodsUpdate.DoModal()==IDOK)
		this->OnOK();
}
