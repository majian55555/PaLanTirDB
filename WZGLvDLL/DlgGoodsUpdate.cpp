// DlgGoodsUpdate.cpp : implementation file
//

#include "stdafx.h"
#include "WZGL.h"
#include "WZGLDlg.h"
#include "DlgGoodsUpdate.h"
#include "RelManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern DB* theDB;
/////////////////////////////////////////////////////////////////////////////
// DlgGoodsUpdate dialog


DlgGoodsUpdate::DlgGoodsUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGoodsUpdate::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGoodsUpdate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgGoodsUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GOODSSERIAL, m_GoodsSerial);
	DDX_Control(pDX, IDC_EDIT_LOCATION2, m_Location2);
	DDX_Control(pDX, IDC_EDIT_LOCATION1, m_Location1);
	DDX_Control(pDX, IDC_EDIT_YEARVER, m_YearVer);
	DDX_Control(pDX, IDC_EDIT_USEDEP, m_UseDep);
	DDX_Control(pDX, IDC_EDIT_TOTALNUM, m_TotalNum);
	DDX_Control(pDX, IDC_EDIT_SCALE, m_Scale);
	DDX_Control(pDX, IDC_EDIT_PICCODE, m_PicCode);
	DDX_Control(pDX, IDC_EDIT_REMAINNUM, m_RemainNum);
	DDX_Control(pDX, IDC_EDIT_OUTTIME, m_OutTime);
	DDX_Control(pDX, IDC_EDIT_OUTNUM, m_OutNum);
	DDX_Control(pDX, IDC_EDIT_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_EDIT_INTIME, m_InTime);
	DDX_Control(pDX, IDC_EDIT_GOODSNAME, m_GoodsName);
	DDX_Control(pDX, IDC_EDIT_GOODSCODE, m_GoodsCode);
	//{{AFX_DATA_MAP(DlgGoodsUpdate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGoodsUpdate, CDialog)
	//{{AFX_MSG_MAP(DlgGoodsUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGoodsUpdate message handlers

void DlgGoodsUpdate::OnOK() 
{
	// TODO: Add extra validation here
	CString strings[15];
	//1
	this->m_GoodsCode.GetWindowText(strings[0]);
	ASSERT(strings[0].GetLength()==10);
	//2
	this->m_GoodsSerial.GetWindowText(strings[1]);
	if(strings[1].GetLength()<1||strings[1].GetLength()>30)
	{
		MessageBox("请输入1-30个字符的地图编号.");
		this->m_GoodsName.SetFocus();
		return;
	}
	//3
	this->m_GoodsName.GetWindowText(strings[2]);
	if(strings[2].GetLength()<1||strings[2].GetLength()>30)
	{
		MessageBox("请输入1-30个字符的地图名称.");
		this->m_GoodsName.SetFocus();
		return;
	}
	//4
	this->m_TotalNum.GetWindowText(strings[3]);
	if(strings[3].IsEmpty())
	{
		MessageBox("请输入地图编制数.");
		this->m_TotalNum.SetFocus();
		return;
	}
	//5
	this->m_RemainNum.GetWindowText(strings[4]);
	if(strings[4].IsEmpty())
	{
		MessageBox("请输入地图现有数.");
		this->m_RemainNum.SetFocus();
		return;
	}
	//6
	this->m_OutNum.GetWindowText(strings[5]);
	if(strings[5].IsEmpty())
	{
		MessageBox("请输入地图出库数.");
		this->m_OutNum.SetFocus();
		return;
	}
	//7
	this->m_PicCode.GetWindowText(strings[6]);
	if(strings[6].GetLength()<1||strings[6].GetLength()>30)
	{
		MessageBox("请输入1-30个字符的图幅号.");
		this->m_PicCode.SetFocus();
		return;
	}
	//8
	this->m_YearVer.GetWindowText(strings[7]);
	if(strings[7].GetLength()!=4)
	{
		MessageBox("请输入4位年版数.");
		this->m_YearVer.SetFocus();
		return;
	}
	//9
	this->m_Scale.GetWindowText(strings[8]);
	if(strings[8].IsEmpty())
	{
		MessageBox("请输入比例尺.");
		this->m_Scale.SetFocus();
		return;
	}
	//10
	this->m_InTime.GetWindowText(strings[9]);
	if(strings[9].GetLength()!=8)
	{
		MessageBox("请正确输入地图入库年份（如：19990605）.");
		this->m_InTime.SetFocus();
		return;
	}
	//11
	this->m_OutTime.GetWindowText(strings[10]);
	if(strings[10].GetLength()!=8)
	{
		MessageBox("请正确输入地图出库年份（如：19990605）.");
		this->m_OutTime.SetFocus();
		return;
	}
	//12
	this->m_UseDep.GetWindowText(strings[11]);
	if(strings[11].GetLength()<1||strings[11].GetLength()>30)
	{
		MessageBox("请输入1-30个字符的使用单位名称.");
		this->m_UseDep.SetFocus();
		return;
	}
	//13
	this->m_Location1.GetWindowText(strings[12]);
	if(strings[12].GetLength()!=4)
	{
		MessageBox("请输入4位数字的箱号.");
		this->m_Location1.SetFocus();
		return;
	}
	//14
	this->m_Location2.GetWindowText(strings[13]);
	if(strings[13].GetLength()!=4)
	{
		MessageBox("请输入4位数字的层号.");
		this->m_Location2.SetFocus();
		return;
	}
	//15
	this->m_Script.GetWindowText(strings[14]);
	if(strings[14].GetLength()>3000)
	{
		MessageBox("请输入0-3000个字符的备注.");
		this->m_UseDep.SetFocus();
		return;
	}
	//
	AttrInPage attrs[15];
	char chrData[3300];
	int offset=0;
	for(int i=0;i<15;i++)
	{
		attrs[i].setNull(false);
		attrs[i].setLen(strings[i].GetLength());
		strncpy((char*)((unsigned long)chrData+offset),(LPCTSTR)strings[i],strings[i].GetLength());
		offset+=strings[i].GetLength();
	}
	chrData[offset]='\0';
	//MessageBox(chrData);
	Status status;
	Record tmpRec;
	ASSERT(tmpRec.setAttrsInPage(attrs,15)==OK);
	tmpRec.setRealData(chrData);
	RID tmpRid=NULLRID;
	ScanOperator* scanOp=new ScanOperator("WZGLDB.PLTDB","R_MATERIEL",status);
	ASSERT(status==OK);
	ASSERT(scanOp->startScan(0,strings[0].GetLength(),P_CHAR,LPCTSTR(strings[0]),EQ)==OK);
	ASSERT(scanOp->scanNext(tmpRid)==OK);
	if(scanOp->updateRecord(tmpRec)==OK)
	{
		delete scanOp;
		bufMgr->flushFile(::theDB);
		MessageBox("修改成功。");
		this->EndDialog(IDOK);
		return;
	}
	/*if(scanOp->deleteRecord()==OK)
	{
		delete scanOp;
		InsertOperator* insertOp=new InsertOperator("WZGLDB.PLTDB","R_MATERIEL",status);
		ASSERT(status==OK);
		if(insertOp->insertRecord(tmpRec,tmpRid)==OK)
		{
			delete insertOp;
			bufMgr->flushFile(::theDB);
			MessageBox("修改成功。");
			this->EndDialog(IDOK);
			return;
		}
		delete insertOp;
	}*/
	delete scanOp;
	MessageBox("修改失败，请与管理员联系。");
	return;
	//CDialog::OnOK();
}

void DlgGoodsUpdate::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL DlgGoodsUpdate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	//
	this->m_GoodsCode.SetWindowText(this->m_str_GoodsCode);
	this->m_GoodsName.SetWindowText(this->m_str_GoodsName);
	this->m_TotalNum.SetWindowText(this->m_str_TotalNum);
	this->m_RemainNum.SetWindowText(this->m_str_RemainNum);
	this->m_OutNum.SetWindowText(this->m_str_OutNum);
	this->m_PicCode.SetWindowText(this->m_str_PicCode);
	this->m_YearVer.SetWindowText(this->m_str_YearVer);
	this->m_Scale.SetWindowText(this->m_str_Scale);
	this->m_InTime.SetWindowText(this->m_str_InTime);
	this->m_OutTime.SetWindowText(this->m_str_OutTime);
	this->m_UseDep.SetWindowText(this->m_str_UseDep);
	this->m_Location1.SetWindowText(this->m_str_Location1);
	this->m_Location2.SetWindowText(this->m_str_Location2);
	this->m_Script.SetWindowText(this->m_str_Script);
	this->m_GoodsSerial.SetWindowText(this->m_str_GoodsSerial);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
