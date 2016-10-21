// RelManager.cpp: implementation of the RelManager class.
//
//////////////////////////////////////////////////////////////////////
#include "RelManager.h"
#include <iostream.h>
#include <cmath>

/////////////////////Implementation of Class DataPageInfo.////////////////////////////////////////////////////////////////
const unsigned long DataPageInfo::getDataPageNo() const
{
	return this->dataPageNo;
}
const unsigned short DataPageInfo::getFreeSpace() const
{
	return this->freeSpace;
}
void DataPageInfo::setDataPageNo(const unsigned long dpNo)
{
	this->dataPageNo=dpNo;
}
void DataPageInfo::setFreeSpace(const unsigned short freespace)
{
	this->freeSpace=freespace;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Function CreateRelation.///////////////////////////////////////////////////////////
const Status createRel(const std::string dbname,const std::string relationname)
{
	//assert fileName is not null and not more than MAXNAMESIZE
	if((0==relationname.length())&&(MAXNAMESIZE<relationname.length()))
		return BADRELNAME;
	DB* db=NULL;
	Status 			status;
	RelHdrPage*		hdrPage=NULL;
	unsigned long	hdrPageNo=0;
	unsigned long	newPageNo=0;
	Page*			newPage=NULL;
	//create new relation
	if((status=dbManager.openDataBase(dbname,db))!=OK)
		return status;
	if((status=db->createRel(relationname))!=OK)
		return status;
	if((status=db->getRelFirstPageNo(relationname,hdrPageNo))!=OK)
		return status;
	if((status=bufMgr->readPage(db,hdrPageNo,newPage))!=OK)
		return status;
	hdrPage=reinterpret_cast<RelHdrPage*>(newPage);
	hdrPage->init();
	ASSERT(hdrPage!=NULL);
	//alloc No.1 data page
	if((status=bufMgr->allocPage(db,newPageNo,newPage))!=OK)
		return status;
	//initial first data page
	newPage->init(newPageNo);
	//set hdrPage's pages and pageCnt
	hdrPage->pages[0].setDataPageNo(newPageNo);
	hdrPage->pages[0].setFreeSpace(newPage->getFreeSpace());
	hdrPage->dataPageCnt++;
	//unpin data page and write hdrPage to file
	if((status=bufMgr->unPinPage(db,newPageNo,true))!=OK)
		return status;
	if((status=bufMgr->unPinPage(db,hdrPageNo,true))!=OK)
		return status;
	//bufMgr->flushFile(db));
	if((status=dbManager.closeDataBase(dbname))!=OK)
		return status;
	return OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Function DestroyRelation.//////////////////////////////////////////////////////////
#define RDP(p) (*(reinterpret_cast<RelHdrPage*>(p)))
const Status destroyRel(const std::string dbname,const std::string relationname)
{
	//assert fileName is not null and not more than MAXNAMESIZE
	if((0!=relationname.length())&&(MAXNAMESIZE<relationname.length()))
		return BADRELNAME;
	DB* db=NULL;
	Status status;
	Page* relHdrPage=new Page;
	unsigned long relFirstPageNo=0;
	if((status=dbManager.openDataBase(dbname,db))!=OK)
		return status;
	if((status=db->getRelFirstPageNo(relationname,relFirstPageNo))!=OK)
		return status;
	if((status=bufMgr->readPage(db,relFirstPageNo,relHdrPage))!=OK)
		return status;
	unsigned long nextRHPNo=RDP(relHdrPage).nextRelHdrPageNo;
	for(unsigned long i=0;i<DPICNT;i++)
	{
		if(RDP(relHdrPage).pages[i].getDataPageNo()==0)
			continue;
		if((status=bufMgr->disposePage(db,RDP(relHdrPage).pages[i].getDataPageNo()))!=OK)
			return status;
	}
	while(nextRHPNo!=0)//there are more than one relation Head Page
	{
		if((status=bufMgr->readPage(db,nextRHPNo,relHdrPage))!=OK)
			return status;
		for(unsigned long i=0;i<DPICNT;i++)
		{
			if(RDP(relHdrPage).pages[i].getDataPageNo()==0)
				continue;
			if((status=bufMgr->disposePage(db,RDP(relHdrPage).pages[i].getDataPageNo()))!=OK)
				return status;
		}
		unsigned long thisRHPNo=nextRHPNo;
		nextRHPNo=RDP(relHdrPage).nextRelHdrPageNo;
		if((status=bufMgr->unPinPage(db,thisRHPNo,false))!=OK)
			return status;
		if((status=bufMgr->disposePage(db,thisRHPNo))!=OK)
			return status;
	}
	if((status=bufMgr->unPinPage(db,relFirstPageNo,true))!=OK)
		return status;
	if((status=bufMgr->disposePage(db,relFirstPageNo))!=OK)
		return status;
	if((status=db->destroyRel(relationname))!=OK)
		return status;
	return dbManager.closeDataBase(dbname);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of class RelOperator./////////////////////////////////////////////////////////////////
RelOperator::RelOperator(const std::string &dbname,const std::string &relname, Status &returnStatus):firstRHPN(0)
{
	if((dbname.length()==0)&&(dbname.length()>MAXNAMESIZE))
	{
		returnStatus=BADDBNAME;
		return;
	}
	if((relname.length()==0)&&(relname.length()>MAXNAMESIZE))
	{
		returnStatus=BADRELNAME;
		return;
	}
	this->relName=relname;
	returnStatus=dbManager.openDataBase(dbname,this->db);
	if(returnStatus!=OK) return;
	returnStatus=db->getRelFirstPageNo(relname,this->curRHPN);
	if(returnStatus!=OK) return;
	unsigned long *tmpPtr=const_cast<unsigned long*>(&(this->firstRHPN));
	*tmpPtr=this->curRHPN;
	Page* pagePtr;
	returnStatus=bufMgr->readPage(db,this->curRHPN,pagePtr);
	if(returnStatus!=OK) return;
	this->curRHP=reinterpret_cast<RelHdrPage*>(pagePtr);
	this->dirtyCRHP=false;
	//set current data page of RelOperator
	this->curDataPageNo=this->curRHP->pages[0].getDataPageNo();
	returnStatus=bufMgr->readPage(db,this->curDataPageNo,this->curDataPage);
	if(returnStatus!=OK) return;
	this->dirtyCDP=false;
	this->curRec=NULLRID;
}
RelOperator::~RelOperator()
{
	Status status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP);
	if (status!=OK) 
		cout << "Unpin relation head page error when running RelOperator destructor" << endl;
	if (this->curDataPage!=NULL)
	{
		status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP);
		this->curDataPage=NULL;
		this->curRec=NULLRID;
		this->curDataPageNo=0;
		if (status!=OK)
			cout << "Unpin current data page error when running RelOperator destructor" << endl;
	}
	status=dbManager.closeDataBase(this->db->getDBName());
	if (status!=OK)
		cout << "close file error when running RelOperator destructor" << endl;
}
const unsigned long RelOperator::getDataPageCnt() const
{
	if(this->curRHP->lastRelHdrPageNo==0) //curRHP is the first RHP in this relation
		return this->curRHP->dataPageCnt;
	else//curRHP is not the first RHP in this relation
	{
		Status status;
		Page* pagePtr=NULL;
		if((status=bufMgr->readPage(this->db,this->firstRHPN,pagePtr))!=OK)
			return status;
		unsigned long DPCnt=RDP(pagePtr).dataPageCnt;
		if((status=bufMgr->unPinPage(this->db,this->firstRHPN,false))!=OK)
			return status;
		return DPCnt;
	}
}
const unsigned long RelOperator::getRecCnt() const
{
	if(this->curRHP->lastRelHdrPageNo==0) //curRHP is the first RHP in this relation
		return this->curRHP->recCnt;
	else//curRHP is not the first RHP in this relation
	{
		Status status;
		Page* pagePtr=NULL;
		if((status=bufMgr->readPage(this->db,this->firstRHPN,pagePtr))!=OK)
			return status;
		unsigned long recCnt=RDP(pagePtr).recCnt;
		if((status=bufMgr->unPinPage(this->db,this->firstRHPN,false))!=OK)
			return status;
		return recCnt;
	}
}
const Status RelOperator::setRecCnt(unsigned long newRecCnt)
{
	if(this->curRHP->lastRelHdrPageNo==0) //curRHP is the first RHP in this relation
		this->curRHP->recCnt=newRecCnt;
	else//curRHP is not the first RHP in this relation
	{
		Status status;
		Page* pagePtr=NULL;
		if((status=bufMgr->readPage(this->db,this->firstRHPN,pagePtr))!=OK)
			return status;
		this->curRHP->recCnt=newRecCnt;
		if((status=bufMgr->unPinPage(this->db,this->firstRHPN,true))!=OK)
			return status;
	}
	return OK;
}
const Status RelOperator::setDataPageCnt(unsigned long newDPCnt)
{
	if(this->curRHP->lastRelHdrPageNo==0) //curRHP is the first RHP in this relation
		this->curRHP->dataPageCnt=newDPCnt;
	else//curRHP is not the first RHP in this relation
	{
		Status status;
		Page* pagePtr=NULL;
		if((status=bufMgr->readPage(this->db,this->firstRHPN,pagePtr))!=OK)
			return status;
		this->curRHP->dataPageCnt=newDPCnt;
		if((status=bufMgr->unPinPage(this->db,this->firstRHPN,true))!=OK)
			return status;
	}
	return OK;
}
const std::string RelOperator::getRelName() const
{
	return this->relName;
}
const Status RelOperator::getRecord(const RID &rid, Record & rec)
{
	Status status;
	if(rid.getPageNo()==this->curDataPageNo)
	{
		if((status=this->curDataPage->getRecord(rid,rec))!=OK)
			return status;
	}
	else
	{
		if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
			return status;
		this->curDataPageNo=rid.getPageNo();
		if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
			return status;
		this->dirtyCDP=false;
		if((status=this->curDataPage->getRecord(rid,rec))!=OK)
			return status;
	}
	if(rec.isLargeRec()==true)//
	{
		char charTmp[PAGESIZE-DPFIXED];
		memcpy(charTmp,rec.realData,PAGESIZE-DPFIXED-rec.getAttrsLen());
		Page* pagePtr=NULL;
		unsigned long nextPageNo=this->curDataPage->getNextPageNo();
		rec.realData=new char[rec.getRealLength()];
		memcpy(rec.realData,charTmp,PAGESIZE-DPFIXED-rec.getAttrsLen());
		unsigned long offset=PAGESIZE-DPFIXED-rec.getAttrsLen();
		while(nextPageNo!=0)
		{
			if((status=bufMgr->readPage(this->db,nextPageNo,pagePtr))!=OK)
				return status;
			pagePtr->getPureData((void*)((unsigned long)rec.realData+offset));
			offset+=pagePtr->getFreePtr();
			nextPageNo=pagePtr->getNextPageNo();
			if((status=bufMgr->unPinPage(this->db,nextPageNo,false))!=OK)
				return status;
		}
	}
	this->curRec=rid;
	return OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of class InsertOperator.//////////////////////////////////////////////////////////////
const Status InsertOperator::insertLRecord(const Record & rec, RID& outRid)
{
	Status status;
	unsigned long newPageNo=0,tmpPageNo=0;
	Page *newPage=NULL,*tmpPage=NULL;
	int blankIndex=-1;
	if((blankIndex=this->curRHP->hasSuitablePages(PAGESIZE-DPFIXED))!=-1)//has a suitable pages
	{
		newPageNo=this->curRHP->pages[blankIndex].getDataPageNo();
		if((status=bufMgr->readPage(this->db,newPageNo,newPage))!=OK)
			return status;
	}
	if((blankIndex=this->curRHP->hasBlankPages())==-1)//doesn't have a blank pages
	{
		unsigned long nextRHPNo=this->firstRHPN;
		while(nextRHPNo!=0)
		{
			if((status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP))!=OK)
				return status;
			if((status=bufMgr->readPage(this->db,nextRHPNo,newPage))!=OK)
				return status;
			this->curRHP=reinterpret_cast<RelHdrPage*>(newPage);
			this->curRHPN=nextRHPNo;
			if((blankIndex=this->curRHP->hasBlankPages())!=-1)
				break;
			nextRHPNo=this->curRHP->nextRelHdrPageNo;
			this->dirtyCRHP=false;
		}
		if(blankIndex==-1)//we need a new RHP
		{
			Page* newRHP=NULL;
			unsigned long newRHPNo=0;
			if((status=bufMgr->allocPage(this->db,newRHPNo,newRHP))!=OK)
				return status;
			RDP(newRHP).init();
			RDP(newRHP).lastRelHdrPageNo=this->curRHPN;
			this->curRHP->nextRelHdrPageNo=newRHPNo;
			if((status=bufMgr->unPinPage(this->db,this->curRHPN,true))!=OK)
				return status;
			this->curRHP=(RelHdrPage*)newRHP;
			this->curRHPN=newRHPNo;
			this->dirtyCRHP=true;
			blankIndex=0;
		}
	if((status=bufMgr->allocPage(this->db,newPageNo,newPage))!=OK)
		return status;
	newPage->init(newPageNo);
	}
	//insert a large record
	if((status=newPage->insertLRec(rec,outRid))!=OK)
		return status;
	unsigned short remainLen=rec.getRealLength()-(PAGESIZE-DPFIXED-rec.getAttrsLen());
	unsigned long offset=rec.getRealLength()-remainLen;
	Page* lastPage=newPage;
	do
	{
		if((status=bufMgr->allocPage(this->db,tmpPageNo,tmpPage))!=OK)
			return status;
		lastPage->setNextPage(tmpPageNo);
		tmpPage->init(tmpPageNo);
		if(remainLen>=PAGESIZE-DPFIXED)
		{
			if((status=tmpPage->insertPureData((void*)((unsigned long)rec.getRealData()+offset),(unsigned short)(PAGESIZE-DPFIXED)))!=OK)
				return status;
			remainLen-=(unsigned short)(PAGESIZE-DPFIXED);
			offset=rec.getRealLength()-remainLen;
		}
		else
		{
			if((status=tmpPage->insertPureData((void*)((unsigned long)rec.getRealData()+offset),remainLen))!=OK)
				return status;
			remainLen=0;
		}
		lastPage=tmpPage;
		if((status=bufMgr->unPinPage(this->db,tmpPageNo,true))!=OK)
			return status;
	}while(remainLen>0);
	this->curRHP->pages[blankIndex].setDataPageNo(newPageNo);
	this->curRHP->pages[blankIndex].setFreeSpace(0);
	this->dirtyCDP=true;
	this->dirtyCRHP=true;
	return OK;		
}
const Status InsertOperator::insertRecord(const Record & rec, RID& outRid)
{
	Status status;
	unsigned long firstRHPNo=0;
	Page *newPage=NULL;
	unsigned short addDPCnt=0;
	if(rec.isLargeRec()==true)//insert a large record
	{
		if((status=this->insertLRecord(rec,outRid))!=OK)
			return status;
		addDPCnt=1;
	}
	else //insert a usual record
	{
		firstRHPNo=this->firstRHPN;
		if(firstRHPNo!=this->curRHPN)
		{//read first RHP into this->curRHP
			if((status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP))!=OK)
				return status;
			if((status=bufMgr->readPage(this->db,firstRHPNo,newPage))!=OK)
				return status;
			this->curRHP=reinterpret_cast<RelHdrPage*>(newPage);
			this->curRHPN=firstRHPNo;
			this->dirtyCRHP=false;
		}
		int suitableIndex=this->curRHP->hasSuitablePages(rec.getWholeLength());
		unsigned long nextRHPNo=this->curRHP->nextRelHdrPageNo;
		while(suitableIndex==-1)//no suitable pages
		{
			if((suitableIndex=this->curRHP->hasBlankPages())!=-1)//has blank pages
				break;
			if(nextRHPNo==0)//we need a new RHP
			{
				Page* newRHP=NULL;
				unsigned long newRHPNo=0;
				if((status=bufMgr->allocPage(this->db,newRHPNo,newRHP))!=OK)
					return status;
				RDP(newRHP).init();
				RDP(newRHP).lastRelHdrPageNo=this->curRHPN;
				this->curRHP->nextRelHdrPageNo=newRHPNo;
				if((status=bufMgr->unPinPage(this->db,this->curRHPN,true))!=OK)
					return status;
				this->curRHP=(RelHdrPage*)newRHP;
				this->curRHPN=newRHPNo;
				this->dirtyCRHP=true;
				suitableIndex=0;
				break;
			}
			if((status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP))!=OK)
				return status;
			if((status=bufMgr->readPage(this->db,nextRHPNo,newPage))!=OK)
				return status;
			this->curRHP=reinterpret_cast<RelHdrPage*>(newPage);
			this->curRHPN=nextRHPNo;
			if((suitableIndex=this->curRHP->hasSuitablePages(rec.getWholeLength()))!=-1)
				break;
			nextRHPNo=this->curRHP->nextRelHdrPageNo;
			this->dirtyCRHP=false;
		}
		if(this->curRHP->pages[suitableIndex].getDataPageNo()==0)
		{
			if(this->curDataPageNo!=0&&this->curDataPage!=NULL)
			{
				if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
					return status;
			}
			if((status=bufMgr->allocPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
				return status;
			this->curDataPage->init(this->curDataPageNo);
			this->curRHP->pages[suitableIndex].setFreeSpace(this->curDataPage->getFreeSpace());
			this->curRHP->pages[suitableIndex].setDataPageNo(this->curDataPageNo);
			addDPCnt++;
		}
		if(this->curDataPageNo==0)
		{
			this->curDataPageNo=this->curRHP->pages[suitableIndex].getDataPageNo();
			if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
				return status;
		}
		if(this->curRHP->pages[suitableIndex].getDataPageNo()!=this->curDataPageNo)
		{
			if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
					return status;
			this->curDataPageNo=this->curRHP->pages[suitableIndex].getDataPageNo();
			if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
				return status;
		}
		if((status=this->curDataPage->insertRecord(rec,outRid))!=OK)
			return status;
		this->curRHP->pages[suitableIndex].setFreeSpace(this->curDataPage->getFreeSpace());
	}
	if(firstRHPNo!=this->curRHPN)
	{//read first RHP into this->curRHP
		if((status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP))!=OK)
			return status;
		if((status=bufMgr->readPage(this->db,firstRHPNo,newPage))!=OK)
			return status;
		this->curRHP=reinterpret_cast<RelHdrPage*>(newPage);
		this->curRHPN=firstRHPNo;
	}
	this->setRecCnt(this->getRecCnt()+1);
	this->setDataPageCnt(this->getDataPageCnt()+addDPCnt);
	this->dirtyCRHP=true;
	this->dirtyCDP=true;
	this->curRec=outRid;
	return OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class ScanOperator.////////////////////////////////////////////////////////////////
const Status ScanOperator::startScan(const unsigned short attrIdx_,
                                     const unsigned short attrLen_,
                                     const Datatype type_,
                                     const char* filter_,
                                     const Operator op_)
{
	Status status;
	// start by making sure the first page of the file is in the buffer pool
	// if not, unpin the currently pinned page and then read the 
	// first page of the file into the buffer pool
	if (this->firstRHPN!=this->curRHPN)
	{
		if((status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP))!=OK)
			return status;
        this->curRHPN=this->firstRHPN;
		Page* tmpPagePtr=NULL;
		if((status=bufMgr->readPage(this->db,this->curRHPN,tmpPagePtr))!=OK)
			return status;
		this->curRHP=reinterpret_cast<RelHdrPage*>(tmpPagePtr);
        this->dirtyCRHP=false;
    }
	if (this->curDataPageNo!=this->curRHP->pages[0].getDataPageNo())
	{
		if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
			return status;
        this->curDataPageNo=this->curRHP->pages[0].getDataPageNo();
		if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
			return status;
        this->dirtyCDP=false;
        this->curRec=NULLRID;
    }
	/*// then check the rest of the paramters
	if (!filter_)
	{                        // no filtering requested
        this->filter=NULL;
        return OK;
    }*/
	if((type_ !=P_CHAR&&type_!=P_VCHAR&&type_!=P_INTEGER&&type_!=P_FLOAT&&type_!=P_DATE&&type_!=P_DATETIME&&type_!=P_BOOLEAN)||
      (type_==P_INTEGER&&attrLen_!=SIZEOFINT||type_==P_FLOAT&&attrLen_!=SIZEOFFLOAT||type_==P_DATE&&attrLen_!=SIZEOFDATE||type_==P_DATETIME&&attrLen_!=SIZEOFDATETIME||type_==P_BOOLEAN&&attrLen_!=SIZEOFBOOL)||
      (op_ !=LT&&op_!=LTE&&op_!=EQ&&op_!=GTE&&op_!=GT&&op_!=NE&&op_!=ISNULL&&op_!=ISNOTNULL)||
	  (type_==P_BLOB&&type_==P_CLOB)||(type_==P_CHAR&&attrLen_>MAXOFCHAR||type_==P_VCHAR&&attrLen_>MAXOFCHAR))
    {
        return BADSCANPARM;
    }
	this->attrIdx=attrIdx_;
	this->attrLen=attrLen_;
	this->type=type_;
	this->filter=filter_;
	this->op=op_;
	return OK;
}
const Status ScanOperator::endScan()
{
	// nothing really to do here as we want to leave the last page pinned
	return OK;
}
const Status ScanOperator::markScan()
{
	// make a snapshot of the state of the scan
	this->markedPageNo=this->curDataPageNo;
	this->markedRec=this->curRec;
	return OK;
}
const Status ScanOperator::resetScan()
{
	Status status;
	if (this->markedPageNo==this->curDataPageNo)
		return OK;
	if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
		return status;
	if((status=bufMgr->readPage(this->db,this->markedPageNo,this->curDataPage))!=OK)
		return status;
	this->curDataPageNo=this->markedPageNo;
	this->dirtyCDP=false;
	this->curRec=this->markedRec;
    return OK;
}
const Status ScanOperator::getRecord(Record & rec)
{
	if(this->curRec.getPageNo()==0)
		return BADRID;
	return RelOperator::getRecord(this->curRec,rec);
}
void ScanOperator::markDirty()
{
    this->dirtyCDP=true;
}
// update current record
const Status ScanOperator::updateRecord(const Record & rec)
{
	if(this->curRec.getPageNo()==0)
		return BADRID;
	Status status;
	if (this->curRec.getPageNo()!=this->curDataPageNo)
		return BADPAGENO;
	status=this->curDataPage->updateRecord(rec,this->curRec);
	if(status==NOSPACE)//we want to update a large record
	{
		if((status=this->curDataPage->deleteRecord(this->curRec))!=OK)
			return status;
		int curIdx=this->curRHP->getCurIdx(this->curDataPageNo);
		ASSERT(curIdx!=-1);
		this->curRHP->pages[curIdx].setFreeSpace(this->curDataPage->getFreeSpace());
		this->dirtyCDP=true;
		InsertOperator* insertOp=new InsertOperator(this->db->getDBName(),this->relName,status);
		ASSERT(status==OK);
		if((status=insertOp->insertRecord(rec,this->curRec))!=OK)
			return status;
		delete insertOp;
		return OK;
	}
	ASSERT(status==OK);
	int curIdx=this->curRHP->getCurIdx(this->curDataPageNo);
	ASSERT(curIdx!=-1);
	this->curRHP->pages[curIdx].setFreeSpace(this->curDataPage->getFreeSpace());
	if(this->curDataPage->getNextPageNo()!=0)//this is a large record
	{
		ASSERT(this->curDataPage->getSlotCnt()==-1);
		unsigned long nextPageNo=this->curDataPage->getNextPageNo(),lastPageNo=0;
		Page* tmpPage;
		while(nextPageNo!=0)
		{
			if((status=bufMgr->readPage(this->db,nextPageNo,tmpPage))!=OK)
				return status;
			lastPageNo=nextPageNo;
			nextPageNo=tmpPage->getNextPageNo();
			if((status=bufMgr->unPinPage(this->db,lastPageNo,false))!=OK)
				return status;
			if((status=bufMgr->disposePage(this->db,lastPageNo))!=OK)
				return status;
		}
	}
	this->dirtyCDP=true;
	return OK;
}
const Status ScanOperator::deleteRecord()
{
	if(this->curRec.getPageNo()==0)
		return BADRID;
	Status status;
	if (this->curRec.getPageNo()!=this->curDataPageNo)
		return BADPAGENO;
	if((status=this->curDataPage->deleteRecord(this->curRec))!=OK)
		return status;
	int curIdx=this->curRHP->getCurIdx(this->curDataPageNo);
	ASSERT(curIdx!=-1);
	this->curRHP->pages[curIdx].setFreeSpace(this->curDataPage->getFreeSpace());
	if(this->curDataPage->getNextPageNo()!=0)//if this is a large record
	{
		ASSERT(this->curDataPage->getSlotCnt()==-1);
		unsigned long nextPageNo=this->curDataPage->getNextPageNo(),lastPageNo=0;
		Page* tmpPage;
		while(nextPageNo!=0)
		{
			if((status=bufMgr->readPage(this->db,nextPageNo,tmpPage))!=OK)
				return status;
			lastPageNo=nextPageNo;
			nextPageNo=tmpPage->getNextPageNo();
			if((status=bufMgr->unPinPage(this->db,lastPageNo,false))!=OK)
				return status;
			if((status=bufMgr->disposePage(this->db,lastPageNo))!=OK)
				return status;
		}
	}
	this->markDirty();
	this->setRecCnt(this->getRecCnt()-1);
	this->dirtyCRHP=true;
	return OK;
}

const bool ScanOperator::matchRec(const Record & rec) const
{
	
	// see if length is beyond end of record
	// maybe this should be an error
	if(this->attrLen>=rec.getWholeLength())
		return false;
	Status status=OK;
	AttrInPage attr;
	if((status=rec.getAttrInPage(this->attrIdx,attr))!=OK)
	{
		Error error;
		error.print(status);
		return false;
	}
	//we don't want to do matching between LOB
	ASSERT(attr.isLOB()==false);
	if(this->op==ISNULL)
	{
		if(attr.isNull()==true)
			return true;
		else
			return false;
	}
	if(this->op==ISNOTNULL)
	{
		if(attr.isNull()==false)
			return true;
		else
			return false;
	}
	// no filtering requested
	if(this->filter==NULL)
		return true;
	//if the length of attr are not equal,we will return false
	if(attr.getAttrLen()!=this->attrLen)
		return false;
	//get the offset in real data
	unsigned long offset=0;
	AttrInPage tmpAttr;
	for(unsigned short i=0;;i++)
	{
		if((this->attrIdx-i)==0)
			break;
		if((status=rec.getAttrInPage(i,tmpAttr))!=OK)
		{
			Error error;
			error.print(status);
			return false;
		}
		offset+=tmpAttr.getAttrLen();
	}
	float diff=0;// < 0 if attr < fltr
	switch(this->type)
	{
		case P_INTEGER:
			int iattr, ifltr;// word-alignment problem possible
			memcpy(&iattr,(char*)rec.getRealData()+offset,this->attrLen);
			memcpy(&ifltr,this->filter,this->attrLen);
			diff=iattr-ifltr;
			break;
		case P_FLOAT:
			float fattr, ffltr;// word-alignment problem possible
			memcpy(&fattr,(char*)rec.getRealData()+offset,this->attrLen);
			memcpy(&ffltr,this->filter,this->attrLen);
			//cout << "fattr:" << fattr << " ffltr" << ffltr << endl;
			diff=fattr-ffltr;
			break;
		case P_CHAR:
			diff=strncmp((char*)rec.getRealData()+offset,this->filter,this->attrLen);
			break;
		case P_VCHAR:
			if(this->attrLen==0&&attr.getAttrLen()==0)
				return true;
			diff=strncmp((char*)rec.getRealData()+offset,this->filter,this->attrLen);
			break;
		case P_DATE:
			break;
		case P_DATETIME:
			break;
		case P_BOOLEAN:
			break;
		case P_BLOB:
			break;
		case P_CLOB:
			break;
	}
	switch(this->op)
	{
		case LT:  if (diff < 0.0) return true;break;
		case LTE: if (diff <= 0.0) return true;break;
		case EQ:  if (diff == 0.0) return true;break;
		case GTE: if (diff >= 0.0) return true;break;
		case GT:  if (diff > 0.0) return true;break;
		case NE:  if (diff != 0.0) return true;break;
		default: break;
	}
	return false;
}
const Status ScanOperator::scanNext(RID& outRid)
{
	Status 	status=OK;
	RID		nextRid=NULLRID,tmpRid=NULLRID;
	Record  rec;
	int curIdx=this->curRHP->getCurIdx(this->curDataPageNo);
	ASSERT(curIdx!=-1);
	while(true)
	{
		curIdx=this->curRHP->getCurIdx(this->curDataPageNo);
		if(this->curRec.getPageNo()==0)//this->curRec is NULLRID
		{
			status=this->curDataPage->firstRecord(tmpRid);
			if((status==NORECORDS)&&((unsigned long)(curIdx+1)==DPICNT)&&(this->curRHP->nextRelHdrPageNo==0))
				return FILEEOF;
			if((status==NORECORDS)&&((unsigned long)(curIdx+1)==DPICNT)&&(this->curRHP->nextRelHdrPageNo!=0))
			{
				int nextRHPNo=this->curRHP->nextRelHdrPageNo;
				if((status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP))!=OK)
					return status;
				this->curRHPN=nextRHPNo;
				Page* tmpPage=NULL;
				if((status=bufMgr->readPage(this->db,this->curRHPN,tmpPage))!=OK)
					return status;
				this->curRHP=(RelHdrPage*)tmpPage;
				if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
					return status;
				this->curDataPageNo=this->curRHP->pages[0].getDataPageNo();
				ASSERT(this->curDataPageNo!=0);
				if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
					return status;
				this->curRec=NULLRID;
				continue;
			}
			if((status==NORECORDS)&&(this->curRHP->pages[curIdx+1].getDataPageNo()==0))
				return FILEEOF;
			if((status==NORECORDS)&&(this->curRHP->pages[curIdx+1].getDataPageNo()!=0))
			{
				if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
					return status;
				this->curDataPageNo=this->curRHP->pages[curIdx+1].getDataPageNo();
				if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
					return status;
				this->curRec=NULLRID;
				continue;
			}
			if ((status!=OK)&&(status!=NORECORDS)) return status;
			this->curRec=tmpRid;
		}
		else//this->curRec is valid RID
		{
			status=this->curDataPage->nextRecord(this->curRec,nextRid);
			if(status==OK)
			{
				tmpRid=nextRid;
				this->curRec=tmpRid;
			}
			if((status==ENDOFPAGE)&&((unsigned long)(curIdx+1)==DPICNT)&&(this->curRHP->nextRelHdrPageNo==0))
				return FILEEOF;
			if((status==ENDOFPAGE)&&((unsigned long)(curIdx+1)==DPICNT)&&(this->curRHP->nextRelHdrPageNo!=0))
			{
				int nextRHPNo=this->curRHP->nextRelHdrPageNo;
				if((status=bufMgr->unPinPage(this->db,this->curRHPN,this->dirtyCRHP))!=OK)
					return status;
				this->curRHPN=nextRHPNo;
				Page* tmpPage=NULL;
				if((status=bufMgr->readPage(this->db,this->curRHPN,tmpPage))!=OK)
					return status;
				this->curRHP=(RelHdrPage*)tmpPage;
				if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
					return status;
				this->curDataPageNo=this->curRHP->pages[0].getDataPageNo();
				ASSERT(this->curDataPageNo!=0);
				if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
					return status;
				this->curRec=NULLRID;
				continue;
			}
			if((status==ENDOFPAGE)&&(this->curRHP->pages[curIdx+1].getDataPageNo()==0))
				return FILEEOF;
			if((status==ENDOFPAGE)&&(this->curRHP->pages[curIdx+1].getDataPageNo()!=0))
			{
				if((status=bufMgr->unPinPage(this->db,this->curDataPageNo,this->dirtyCDP))!=OK)
					return status;
				this->curDataPageNo=this->curRHP->pages[curIdx+1].getDataPageNo();
				if((status=bufMgr->readPage(this->db,this->curDataPageNo,this->curDataPage))!=OK)
					return status;
				this->curRec=NULLRID;
				continue;
			}
			if((status!=OK)&&(status!=ENDOFPAGE))
				return status;
		}
		if((status=this->getRecord(rec))!=OK)
			return status;
		if(this->matchRec(rec)==true)
		{
			outRid=this->curRec;
			break;
		}
	}
	return OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////