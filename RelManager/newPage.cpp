#include <iostream>
#include <string>
#include <cmath>
#include <memory>
using namespace std;
#include "newPage.h"
/////////////////////Implementation of Class AttrInPage.//////////////////////////////////////////////////////////////////
const bool AttrInPage::isNull() const
{
	return this->attrisNull;
}
const unsigned short AttrInPage::getAttrLen() const
{
	return this->attrLen;
}
void AttrInPage::setNull(const bool isnull)
{
	this->attrisNull=isnull;
}
void AttrInPage::setLen(const unsigned short length)
{
	this->attrLen=length;
}
const bool AttrInPage::isLOB() const
{
	return this->attrisLOB;
}
void AttrInPage::setLOB(const bool islob)
{
	this->attrisLOB=islob;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class Record./////////////////////////////////////////////////////////////////////
const Status Record::initRec(void *thisRec) //initial record's attributes
{
	if(thisRec==NULL)
		return BADRECPTR;
	this->attrCnt=*((unsigned short*)thisRec);
	if(this->attrCnt==0)
		return BADREC;
	if(this->attrsInPage!=NULL)
		delete [] this->attrsInPage;
	this->attrsInPage=new AttrInPage[this->attrCnt];
	ASSERT(this->attrsInPage!=NULL);
	thisRec=(void*)((unsigned long)thisRec+sizeof(this->attrCnt));
	for(unsigned short i=0;i<this->attrCnt;i++)
	{
		this->attrsInPage[i]=*((AttrInPage*)thisRec);
		thisRec=(void*)((unsigned long)thisRec+sizeof(AttrInPage));
	}
	return OK;
}
Record::~Record()
{
	if(this->attrsInPage==NULL)
		return;
	if(this->isLargeRec())
		delete [] (char*)this->realData;
	delete [] this->attrsInPage;
}
const Status Record::getAttrInPage(const unsigned short attrNo,AttrInPage &attrs) const
{
	if(attrNo>=this->attrCnt)
		return ATTRINPAGENOTFOUND;
	attrs=this->attrsInPage[attrNo];
	return OK;
}
const unsigned short Record::getAttrsNum() const
{
	ASSERT(this->attrCnt!=0);
	return this->attrCnt;
}
const unsigned short Record::getAttrsLen() const
{
	ASSERT(this->attrCnt!=0);
	unsigned short attrsLen=0;
	attrsLen=sizeof(unsigned short);
	for(unsigned short i=0;i<this->attrCnt;i++)
	{
		attrsLen+=(sizeof(AttrInPage));
	}
	return attrsLen;
}
const unsigned long Record::getRealLength() const
{
	ASSERT(this->attrCnt!=0);
	unsigned long realLen=0;
	for(unsigned short i=0;i<this->attrCnt;i++)
	{
		realLen+=this->attrsInPage[i].getAttrLen();
	}
	return realLen;
}
const unsigned long Record::getWholeLength () const
{
	ASSERT(this->attrCnt!=0);
	return this->getAttrsLen()+this->getRealLength();
}
const bool Record::isLargeRec() const 
{
	ASSERT(this->attrCnt!=0);
	return this->getWholeLength()>(PAGESIZE-DPFIXED);
}
const Status Record::setAttrsInPage(AttrInPage attrs[],const unsigned short attrNum)
{
	if(attrNum==0 || attrs==NULL)
		return BADATTRS;
	this->attrCnt=attrNum;
	this->attrsInPage=new AttrInPage[attrNum];
	ASSERT(this->attrsInPage!=NULL);
	for(unsigned short i=0;i<this->attrCnt;i++)
	{
		this->attrsInPage[i]=attrs[i];
	}
	return OK;
}
const void* Record::getRealData() const
{
	ASSERT(this->realData!=NULL);
	return this->realData;
}
void Record::setRealData(void* dataPtr)
{
	this->realData=dataPtr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class RID.////////////////////////////////////////////////////////////////////////
const unsigned long RID::getPageNo() const
{
	return this->pageNo;
}
const unsigned short RID::getSlotNo() const
{
	return this->slotNo;
}
void RID::setPageNo(const unsigned long pgNo)
{
	this->pageNo=pgNo;
}
void RID::setSlotNo(const unsigned short slNo)
{
	this->slotNo=slNo;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class Slot.///////////////////////////////////////////////////////////////////////
const unsigned short Slot::getLength() const
{
	return this->length;
}
const unsigned short Slot::getOffset() const
{
	return this->offset;
}
void Slot::setLength(const unsigned short len)
{
	this->length=len;
}
void Slot::setOffset(const unsigned short offs)
{
	this->offset=offs;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class Page.///////////////////////////////////////////////////////////////////////
void Page::init(const unsigned long pageNo)
{
	ASSERT(pageNo!=0);
	this->nextPage=0;
	this->curPage=pageNo;
	this->slotCnt=0;// no slots in use
	this->freePtr=0;// offset of free space in data array
	this->slot[0].setLength(0);
	this->slot[0].setOffset(0);
	this->freeSpace=PAGESIZE-DPFIXED;// amount of space available
}
const unsigned short Page::getFreeSpace() const
{
	if(this->hasBlankSlot()!=1||this->slotCnt==0)//has at least one blank slot
		return this->freeSpace;
	if(this->freeSpace<=sizeof(Slot))
		return 0;
	return this->freeSpace-sizeof(Slot);
}
const unsigned short Page::getFreePtr() const
{
    return this->freePtr;
}
const unsigned long Page::getNextPageNo() const
{
	return this->nextPage;
}
const unsigned long Page::getThisPageNo() const
{
	return this->curPage;
}
const short Page::getSlotCnt() const
{
	return this->slotCnt;
}
const int Page::hasBlankSlot() const //return 1 if there is no blank slot, otherwise return slot's index
{
	for(short i=0;i>this->slotCnt;i--)
	{
		if(this->slot[i].getLength()==0)
			return i;
	}
	return 1;
}
const unsigned short Page::getOffset(const short i) const
{
	return this->slot[i].getOffset();
}
const unsigned short Page::getLength(const short i) const
{
	return this->slot[i].getLength();
}
const Status Page::insertRecord(const Record &rec,RID &rid)
{
	ASSERT(rec.attrsInPage!=NULL);
	int blankSlot=0;
	if((blankSlot=this->hasBlankSlot())==1)
	{
		// using a new slot
		if (rec.getWholeLength()>this->getFreeSpace())
		{
			rid.setPageNo(0);
			rid.setSlotNo(0);
			return NOSPACE; // checking if sufficient space exists
		}
		this->freeSpace-=(rec.getWholeLength()+sizeof(Slot));//adjust free space
		if(this->slotCnt==0)
			this->freeSpace+=sizeof(Slot);
		blankSlot=this->slotCnt;
		this->slotCnt--;
	}
	else
	{
		// reusing an existing slot
        if (rec.getWholeLength()>this->freeSpace)
		{
			rid.setPageNo(0);
			rid.setSlotNo(0);
			return NOSPACE; // checking if sufficient space exists
		}
		this->freeSpace-=rec.getWholeLength();// adjust free space
	}
	this->slot[blankSlot].setOffset(this->freePtr);
	this->slot[blankSlot].setLength(rec.getWholeLength());
	memcpy(&this->data[this->freePtr],&rec,sizeof(unsigned short));
	memcpy(&this->data[this->freePtr+sizeof(unsigned short)],rec.attrsInPage,rec.attrCnt*(sizeof(AttrInPage)));
	memcpy(&this->data[this->freePtr+rec.getAttrsLen()],rec.getRealData(),rec.getRealLength()); // copy data to the data page
	this->freePtr+=rec.getWholeLength(); // adjust freePtr
	rid.setPageNo(this->curPage);
	rid.setSlotNo(-blankSlot);// make a positive slot number
	return OK;
}
const Status Page::insertLRec(const Record &rec, RID& rid)
{
	ASSERT(rec.getWholeLength()>(PAGESIZE-DPFIXED));
	ASSERT(this->freePtr==0);
	ASSERT((this->hasBlankSlot()==0)&&(this->getSlotCnt()==0));
	if(rec.getAttrsLen()>=this->getFreeSpace())
		return NOSPACE;
	this->slot[0].setOffset(this->freePtr);
	this->slot[0].setLength(PAGESIZE-DPFIXED);
	memcpy(&this->data[this->freePtr],&rec,sizeof(unsigned short));
	memcpy(&this->data[this->freePtr+sizeof(unsigned short)],rec.attrsInPage,rec.attrCnt*(sizeof(AttrInPage)));
	memcpy(&this->data[this->freePtr+rec.getAttrsLen()],rec.getRealData(),this->getFreeSpace()-rec.getAttrsLen()); // copy data to the data page
	this->freePtr=PAGESIZE-DPFIXED;
	this->freeSpace=0;
	rid.setPageNo(this->curPage);
	rid.setSlotNo(0);
	return OK;
}
const Status Page::insertPureData(const void* src,unsigned short len)
{
	ASSERT((this->freeSpace==PAGESIZE-DPFIXED)&&(this->freePtr==0));
	if(len>this->getFreeSpace())
		return NOSPACE;
	ASSERT((this->hasBlankSlot()==0)&&(this->getSlotCnt()==0));
	ASSERT(src!=NULL);
	memcpy(this->data,src,len);
	this->slot[0].setOffset(this->freePtr);
	this->slot[0].setLength(len);
	this->slotCnt--;
	this->freeSpace-=len;
	this->freePtr+=len;
	return OK;
}
void Page::getPureData(void* dest) const
{
	ASSERT((this->hasBlankSlot()==0)&&(this->getSlotCnt()==-1));
	ASSERT(dest!=NULL);
	if(this->getFreeSpace()>0)
		ASSERT(this->getNextPageNo()==0);
	memcpy(dest,this->data,this->slot[0].getLength());
	return;
}
const Status Page::deleteRecord(const RID &rid)
{
	if(rid.getPageNo()<0)
		return BADPAGENO;
	int slotNum=-rid.getSlotNo(); //convert to negative format
	if((slotNum<=this->slotCnt) || (slotNum>0) || (0==this->slot[slotNum].getLength()))
		return INVALIDSLOTNO;
	if((this->slot[slotNum].getOffset()+this->slot[slotNum].getLength())==freePtr) //if we want to delete the last slot
	{
		//the record will be deleted is physically the last record
		//in this case the records do not need to be compacted
		//delete data
		memmove(&(this->data[this->slot[slotNum].getOffset()]),
			&(this->data[this->slot[slotNum].getOffset()+this->slot[slotNum].getLength()]),
			this->freePtr-(this->slot[slotNum].getOffset()+this->slot[slotNum].getLength()));
	}
	else //the record is in middle
	{
		//the record being deleted has one or more records after it.
		//This case requires compaction.
		//delete data
		memmove(&(this->data[this->slot[slotNum].getOffset()]),
			&(this->data[this->slot[slotNum].getOffset()+this->slot[slotNum].getLength()]),
			this->freePtr-(this->slot[slotNum].getOffset()+this->slot[slotNum].getLength()));
		//set other slots
		unsigned long after=this->slot[slotNum].getOffset()+this->slot[slotNum].getLength();
		// now need to adjust offsets of all valid slots to the
		// 'right' of slot being removed by size of the hole
		for(int i=0;i>this->slotCnt;i--)
		{
			if(i==slotNum)
				continue;
			if(0==this->slot[i].getLength())
				continue;
			if(this->slot[i].getOffset()>=after)
				this->slot[i].setOffset(this->slot[i].getOffset()-this->slot[slotNum].getLength());
		}
	}
	if (slotNum==this->slotCnt+1)
	{
		// Slot being freed is at end of slot array. In this
		// case we can compact the slot array. Note that we
		// should even compact slots that might have been
		// emptied previously.
		do
		{
			this->slotCnt++;
			this->freeSpace+=sizeof(Slot);
		}
		while (this->slotCnt<0&&slot[slotCnt+1].getLength()==0);
	}
	if(0==this->slotCnt)
		this->freeSpace-=sizeof(Slot);
	//set page information
	this->freePtr-=this->slot[slotNum].getLength();
	this->freeSpace+=this->slot[slotNum].getLength();
	//set this slot to blank slot
	this->slot[slotNum].setOffset(0);
	this->slot[slotNum].setLength(0);
	return OK;
}
// update the record with the specified rid
const Status Page::updateRecord(const Record & rec,const RID& rid)
{
	int slotNum=-rid.getSlotNo();
	if((slotNum<=this->slotCnt) || (slotNum>0) || (0==this->slot[slotNum].getLength()))
		return INVALIDSLOTNO;
	if(rec.getWholeLength()==this->slot[slotNum].getLength()) //if length is equle
	{
		//update data directly
		memcpy(&this->data[this->slot[slotNum].getOffset()],&rec,sizeof(unsigned short));
		memcpy(&this->data[this->slot[slotNum].getOffset()+sizeof(unsigned short)],rec.attrsInPage,rec.attrCnt*(sizeof(AttrInPage)));
		memcpy(&(this->data[this->slot[slotNum].getOffset()+rec.getAttrsLen()]),rec.getRealData(),rec.getRealLength());
		return OK;
	}
	else //if length is not equle
	{
		int dif=rec.getWholeLength()-this->slot[slotNum].getLength();
		if(dif>this->freeSpace) //if no enough spaces for updating
			return NOSPACE;
		//save the old memery
		memmove(&(this->data[this->slot[slotNum].getOffset()+this->slot[slotNum].getLength()]),
			&(this->data[this->slot[slotNum].getOffset()+rec.getWholeLength()]),
			this->freePtr-this->slot[slotNum].getOffset()+this->slot[slotNum].getLength());
		//update the record
		memcpy(&this->data[this->slot[slotNum].getOffset()],&rec,sizeof(unsigned short));
		memcpy(&this->data[this->slot[slotNum].getOffset()+sizeof(unsigned short)],rec.attrsInPage,rec.attrCnt*(sizeof(AttrInPage)));
		memcpy(&(this->data[this->slot[slotNum].getOffset()+rec.getAttrsLen()]),rec.getRealData(),rec.getRealLength());
		unsigned short after=this->slot[slotNum].getOffset()+this->slot[slotNum].getLength();
		for(int i=0;i>this->slotCnt;i--)
		{
			if(i==slotNum)
				continue;
			if(0==this->slot[i].getLength())
				continue;
			if(this->slot[i].getOffset()>=after)
				this->slot[i].setOffset(this->slot[i].getOffset()+dif);
		}
		this->freeSpace-=dif;
		this->freePtr+=dif;
		//set this slot
		this->slot[slotNum].setLength(rec.getWholeLength());
		return OK;
	}
}
// returns RID of first record on page
const Status Page::firstRecord(RID& firstRid) const
{
	for(short i=0;i>this->slotCnt;i--)
	{
		if(0==this->slot[i].getLength())
			continue;
		else
		{
			firstRid.setPageNo(this->curPage);
			firstRid.setSlotNo(-i);
			return OK;
		}
	}
	return NORECORDS;
}
// returns RID of next record on the page
// returns ENDOFPAGE if no more records exist on the page; otherwise OK
const Status Page::nextRecord (const RID &curRid, RID& nextRid) const
{
	for(short i=-curRid.getSlotNo();i>this->slotCnt;i--)
	{
		if(0==this->slot[i].getLength())
			continue;
		if(-i>=curRid.getSlotNo()+1)
		{
			nextRid.setPageNo(this->curPage);
			nextRid.setSlotNo(-i);
			return OK;
		}
	}
	return ENDOFPAGE;
}
// returns length and pointer to record with RID rid
const Status Page::getRecord(const RID & rid, Record & rec)
{
	int slotNum=-rid.getSlotNo();
	if((slotNum<=this->slotCnt) || (slotNum>0) || (0==this->slot[slotNum].getLength()))
		return INVALIDSLOTNO;
	Status status;
	if((status=rec.initRec(&(this->data[this->slot[slotNum].getOffset()])))!=OK)
		return status;
	rec.setRealData(&(this->data[this->slot[slotNum].getOffset()+rec.getAttrsLen()]));
	return OK;
}
// dump page utility
void Page::dumpPage() const
{
	int i;
	cout << "curPage = " << this->curPage <<", nextPage = " << this->nextPage
		<< "\nfreePtr = " << this->freePtr << ",  freeSpace = " << this->freeSpace 
		<< ", slotCnt = " << this->slotCnt << endl;
	for (i=0;i>slotCnt;i--)
	{
		cout<<"slot["<<i<<"].offset="<<slot[i].getOffset()<<",slot["<<i<<"].length="<<slot[i].getLength()<<endl;
		if (slot[i].getLength()==0)
			cout<<"This record has been deleted!" << endl;
		else
		{
			for(int j=slot[i].getOffset();j<slot[i].getOffset()+slot[i].getLength();j++)
				printf("%c", data[j]);
			printf("\n");
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
