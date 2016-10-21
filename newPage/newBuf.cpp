#include "newBuf.h"
#include <memory>
#include <cmath>

/////////////////////Implementation of Class HashBucket.//////////////////////////////////////////////////////////////////
HashBucket::HashBucket(const DB* db,const unsigned long frameNo,const unsigned long pageNo,const HashBucket* next)
{
	ASSERT(db!=NULL);
	this->db=const_cast<DB*>(db);
	this->frameNo=frameNo;
	this->pageNo=pageNo;
	if(next==NULL)
		this->next=NULL;
	else
		this->next=const_cast<HashBucket*>(next);
}
HashBucket::~HashBucket()
{
}
void HashBucket::setNextPtr(const HashBucket* next)
{
	if(next==NULL)
		this->next=NULL;
	else
		this->next=const_cast<HashBucket*>(next);
}
const unsigned long HashBucket::getFrameNo() const
{
	return this->frameNo;
}
const unsigned long HashBucket::getPageNo() const
{
	return this->pageNo;
}
const HashBucket* HashBucket::getNextPtr() const
{
	return this->next;
}
const DB* HashBucket::getDBPtr() const
{
	return this->db;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class BufHashTbl.//////////////////////////////////////////////////////////////////
BufHashTbl::BufHashTbl(const unsigned long htSize)
{
	ASSERT(htSize>0);
	this->htSize=htSize;
	// allocate an array of pointers to hashBuckets
	this->ht=new HashBucket* [htSize];
	ASSERT(this->ht!=NULL);
	for(unsigned long i=0;i<htSize;i++)
		this->ht[i]=NULL;
}
BufHashTbl::~BufHashTbl()
{
	for(unsigned long i=0;i<this->htSize;i++)
	{
		HashBucket* tmpBuf=this->ht[i];
		while(this->ht[i])
		{
			tmpBuf=this->ht[i];
			this->ht[i]=const_cast<HashBucket*>(this->ht[i]->getNextPtr());
			delete tmpBuf;
		}
	}
	delete [] ht;
}
unsigned long BufHashTbl::hash(const DB* db, const unsigned long pageNo)
{
	ASSERT(db!=NULL);
	ASSERT(pageNo!=0);
	unsigned long tmpInt,value;
	tmpInt=reinterpret_cast<unsigned long>(db);  // cast of pointer of DB object to an unsigned long
	value=(tmpInt+pageNo)%(this->htSize);
	return value;
}
const Status BufHashTbl::lookup(const DB* db, const unsigned long pageNo, unsigned long& frameNo)
{
	ASSERT(db!=NULL);
	ASSERT(pageNo!=0);
	unsigned long index=this->hash(db,pageNo);
	HashBucket* tmpBuc=this->ht[index];
	while(tmpBuc)
	{
		if((tmpBuc->getDBPtr()==db)&&(tmpBuc->getPageNo()==pageNo))
		{
			frameNo=tmpBuc->getFrameNo();
			return OK;
		}
		tmpBuc=const_cast<HashBucket*>(tmpBuc->getNextPtr());
	}
	return HASHNOTFOUND;
}
const Status BufHashTbl::insert(const DB* db,const unsigned long pageNo,const unsigned long frameNo)
{
	unsigned long tempInt;
	if(this->lookup(db,pageNo,tempInt)==OK)
		return HASHEXIST;
	//std::tr1::shared_ptr<HashBucket> ptr1(new HashBucket(db,frameNo,pageNo,NULL));
	//std::auto_ptr<HashBucket> tmpBuc(new HashBucket(db,frameNo,pageNo,NULL));
	//if(tmpBuc.get()==NULL)
	HashBucket* tmpBuc=new HashBucket(db,frameNo,pageNo,NULL);
	if (tmpBuc==NULL)
		return HASHTBLERROR;
	unsigned long index=this->hash(db,pageNo);
	tmpBuc->setNextPtr(ht[index]);
	ht[index]=tmpBuc;
	return OK;
}
const Status BufHashTbl::remove(const DB* db, const unsigned long pageNo)
{
	unsigned long tempInt;
	if(this->lookup(db,pageNo,tempInt)!=OK)
		return HASHNOTFOUND;
	unsigned long index=this->hash(db,pageNo); 
	HashBucket* tmpBuc=ht[index];  
	HashBucket* prevBuc=ht[index]; 
	while(tmpBuc)
	{
		if ((tmpBuc->getDBPtr()==db)&&(tmpBuc->getPageNo()==pageNo))
		{
			if(tmpBuc==ht[index])
				ht[index]=const_cast<HashBucket*>(tmpBuc->getNextPtr());
			else
				prevBuc->setNextPtr(const_cast<HashBucket*>(tmpBuc->getNextPtr()));
			delete tmpBuc;
			return OK;
		}
		else
		{
			prevBuc=tmpBuc;
			tmpBuc=const_cast<HashBucket*>(tmpBuc->getNextPtr());
		}
	}
	return HASHTBLERROR;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class BufDesc./////////////////////////////////////////////////////////////////////
void BufDesc::clear()
{
	this->pinCnt=0;
	this->db=NULL;
	this->pageNo=0;
	this->dirty=false;
	this->valid=false;
	this->refbit=false;
	return;
}
BufDesc::BufDesc()
{
	this->clear();
	return;
}
BufDesc::~BufDesc()
{
	return;
}
void BufDesc::set(const DB* db, const unsigned long pageNum)
{ 
	this->db=const_cast<DB*>(db);
	this->pageNo=pageNum;
	this->pinCnt=1;
	this->dirty=false;
	this->valid=true;
	this->refbit=true;
	return;
}
void BufDesc::addPinCnt()
{
	this->pinCnt++;
	return;
}
const unsigned long BufDesc::getPageNo() const
{
	return this->pageNo;
}
const unsigned long BufDesc::getFrameNo() const
{
	return this->frameNo;
}
const unsigned long BufDesc::getPinCnt() const
{
	return this->pinCnt;
}
const DB* BufDesc::getDB() const
{
	return this->db;
}
const bool BufDesc::isDirty() const
{
	return this->dirty;
}
const bool BufDesc::isValid() const
{
	return this->valid;
}
const bool BufDesc::isRefbit() const
{
	return this->refbit;
}
const bool BufDesc::isPinned() const
{
	if(this->getPinCnt()>0)
		return true;
	return false;
}
void BufDesc::setRefbit(const bool ref)
{
	this->refbit=ref;
}
void BufDesc::setDirty()
{
	this->dirty=true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of Class BufMgr.//////////////////////////////////////////////////////////////////////
BufMgr::BufMgr(const unsigned long bufNum)
{
	this->bufNum=bufNum;
	this->bufTable=new BufDesc[bufNum];
	ASSERT(this->bufTable!=NULL);
	for(unsigned long i=0;i<bufNum;i++)
		bufTable[i].frameNo=i;
	this->bufPool=new Page[bufNum];
	if(this->bufPool==NULL)
		delete [] this->bufTable;
	ASSERT(this->bufPool!=NULL);
	memset(this->bufPool,0,bufNum*sizeof(Page));
	//we want to find a prime number.
	unsigned long hashnum=(unsigned long)ceil(bufNum*1.2);
	while(true)
	{
		bool isPrimer=false;
		unsigned long flag=hashnum/2;
		for(unsigned long i=2;i<flag;i++)
		{
			if(hashnum%i==0)
			{
				isPrimer=false;
				break;
			}
			isPrimer=true;
		}
		if(isPrimer)
			break;
		hashnum++;
	}
	//cout<<hashnum<<endl;
	this->hashTable=new BufHashTbl(hashnum);
	if(this->hashTable==NULL)
	{
		delete [] this->bufTable;
		delete [] this->bufPool;
	}
	ASSERT(this->hashTable!=NULL);
	this->clockHand=0;
}
BufMgr::~BufMgr()
{
	for(unsigned long i=0;i<this->bufNum;i++)
	{
		BufDesc* tmpbuf=&(this->bufTable[i]);
		if ((tmpbuf->valid==true)&&(tmpbuf->dirty==true))
		{
			if(OK!=tmpbuf->db->writePage(tmpbuf->pageNo,&(this->bufPool[i])))
				cerr<<"Can't write No."<<i<<" page back to file from main memery!"<<endl;
		}
	}
	delete [] this->bufTable; 
	delete [] this->bufPool; 
	delete this->hashTable;
}
const Status BufMgr::allocBuf(unsigned long &frameNo)
{
	Status status;
	for(unsigned long i=0;i<(2*this->bufNum);i++)
	{
		this->advanceClock();
		if(this->bufTable[this->clockHand].isPinned())
			continue;
		if(this->bufTable[this->clockHand].isRefbit())
		{
			this->bufTable[this->clockHand].setRefbit(false);
			continue;
		}
		if(this->bufTable[this->clockHand].isValid()==true)
		{
			if(this->bufTable[this->clockHand].isDirty()==true)
			{
				if((status=this->bufTable[this->clockHand].db->writePage(this->bufTable[this->clockHand].getPageNo(),
					&(this->bufPool[this->clockHand])))!=OK)
					return status;
			}
			if((status=this->hashTable->remove(this->bufTable[this->clockHand].getDB(),
				this->bufTable[this->clockHand].getPageNo()))!=OK)
				return status;
		}
		this->bufTable[this->clockHand].clear();
		frameNo=this->bufTable[this->clockHand].getFrameNo();
		return OK;
	}
	return BUFFEREXCEEDED;
}
const void BufMgr::releaseBuf(unsigned long frame)
{
	this->bufTable[frame].clear();
}
const Status BufMgr::allocPage(DB* db,unsigned long &pageNo,Page* &page)
{
	if(db==NULL)
		return BADDBPTR;
	Status status;
	unsigned long frameNo;
	if((status=this->allocBuf(frameNo))!=OK)
		return status;
	if(frameNo>=this->bufNum)
		return BADBUFFER;
	if((status=db->allocatePage(pageNo))!=OK)
	{
		this->releaseBuf(frameNo);
		return status;
	}
	if((status=hashTable->insert(db,pageNo,frameNo))!=OK)
	{
		this->releaseBuf(frameNo);
		return status;
	}
	this->bufTable[frameNo].set(db,pageNo);
	page=&(this->bufPool[frameNo]);
	return OK;
}
const Status BufMgr::unPinPage(const DB* db,const unsigned long pageNo,const bool dirty)
{
	unsigned long frameNo;
	// see if page is in the hash table
	if(HASHNOTFOUND==this->hashTable->lookup(db,pageNo,frameNo))
		return HASHNOTFOUND;
	if(this->bufTable[frameNo].isValid()==false)
		return BADBUFFER;
	if(this->bufTable[frameNo].isPinned()==false)
		return PAGENOTPINNED;
	if (dirty==true)
		this->bufTable[frameNo].setDirty();
	this->bufTable[frameNo].pinCnt--;
	return OK;
}
const Status BufMgr::readPage(DB* db,const unsigned long pageNo,Page* &page)
{
	unsigned long frameNo;
	Status status;
	if(!db) return BADFILE;
	if(this->hashTable->lookup(db,pageNo,frameNo)==HASHNOTFOUND)
	{
		// page was not found in the hashtable
		if((status=this->allocBuf(frameNo))!=OK)
			return status;
		if((status=db->readPage(pageNo,&(bufPool[frameNo])))!=OK)
		{
			this->releaseBuf(frameNo); // release allocated frame
			return status;
		}
		if((status=this->hashTable->insert(db,pageNo,frameNo))!=OK)
		{
			this->releaseBuf(frameNo); // release allocated frame
			return status;
		}
		this->bufTable[frameNo].set(db,pageNo);
	}
	else
	{
		// page was found in the buffer pool
		if(this->bufTable[frameNo].valid==false)
			return BADBUFFER;
		this->bufTable[frameNo].addPinCnt();
	}
	page=&(bufPool[frameNo]);
	return OK;
}
const Status BufMgr::flushFile(const DB* db)
{
	Status status;
	for(unsigned long i=0;i<this->bufNum;i++)
	{
		if((this->bufTable[i].getDB()==db)&&(this->bufTable[i].isValid()))
		{
			if (this->bufTable[i].isPinned())
				return PAGEPINNED;
			if(this->bufTable[i].isDirty())
			{
				if((status=this->bufTable[i].db->writePage(this->bufTable[i].getPageNo(),&(this->bufPool[i])))!=OK)
					return status;
			}
			if((status=this->hashTable->remove(db,this->bufTable[i].getPageNo()))!=OK)
				return status;
			this->bufTable[i].clear();
		}
		else if((this->bufTable[i].getDB()==db)&&(!this->bufTable[i].isValid()))
			return BADBUFFER;
	}
	return OK;
}
const Status BufMgr::disposePage(DB* db,const unsigned long pageNo)
{
	Status status;
	unsigned long frameNo=0;
	if(this->hashTable->lookup(db,pageNo,frameNo)!=OK)
		return HASHNOTFOUND;
	else
	{
		if((this->bufTable[frameNo].getDB()!=db)||(this->bufTable[frameNo].getPageNo()!=pageNo)||
			(!this->bufTable[frameNo].isValid()))
			return BADBUFFER;
		if(this->bufTable[frameNo].isPinned())
			return PAGEPINNED;
		if((status=this->hashTable->remove(db,pageNo))!=OK)
			return status;
		this->bufTable[frameNo].clear();
	}
	return db->disposePage(pageNo);
}
void BufMgr::printSelf()
{
	BufDesc* tmpbuf;
	cout << endl << "Print buffer...\n";
	for (unsigned long i=0; i<this->bufNum; i++)
	{
		tmpbuf = &(bufTable[i]);
		cout<<"FrameNo:"<<tmpbuf->getFrameNo()<<".DBName:"<<tmpbuf->getDB()->getDBName().c_str()<<".PageNo:"
			<<tmpbuf->getPageNo()<<"."<<endl;
		//cout << i << "\t" << (char*)(&bufPool[i]) << "\tpinCnt: " << tmpbuf->pinCnt;
		if (tmpbuf->isValid())
			cout << "\tvalid\n";
		if (tmpbuf->isDirty())
			cout << "\tdirty\n";
		if (tmpbuf->isPinned())
			cout << "\tpinned\n";
		if (tmpbuf->isRefbit())
			cout << "\trefbit\n";
		cout << endl;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
