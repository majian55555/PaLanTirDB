// newIO.cpp: implementation of the newIO class.
//
//////////////////////////////////////////////////////////////////////

#include "newIO.h"

#define DHP(p) (*(reinterpret_cast<DBHeadPage*>(&p)))
//#define DHP(p)  (*(DBHeadPage*)&p)
////////////////////////////////////////////////////////////////////////////////////////////
//获取关系（表）的首个目录页页号
RelRecord::RelRecord()
{
	this->initRelRec();
}
const unsigned long RelRecord::getFirstPageNo() const
{
	return this->firstPageNo;
}
//获取关系（表）名称
const std::string RelRecord::getRelName() const
{
	std::string relNameStr(this->relName);
	return relNameStr;
}
//置关系（表）的首个目录页页号
const Status RelRecord::setFirstPageNo(unsigned long firstPageNo)
{
	if(firstPageNo==0)
		return BADFIRSTPAGENO;
	this->firstPageNo=firstPageNo;
	return OK;
}
//置关系（表）名称
const Status RelRecord::setRelName(const std::string& relName)
{
	if((relName.empty()==true) || (relName.length()>MAXNAMESIZE))
		return BADRELNAME;
	relName.copy(this->relName,relName.length());
	this->relName[relName.length()]='\0';
	return OK;
}
void RelRecord::initRelRec()
{
	this->firstPageNo=0;
	this->relName[0]='\0';
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
DBHeadPage::DBHeadPage()
{
	this->init();
}
DBHeadPage::~DBHeadPage()
{
}
void DBHeadPage::init()
{
	this->nextDBHPNo=0;
	this->relCnt=0;
	this->nextFreePage=0;
	this->pageCnt=1;
	strcpy(this->DBsignature,"");
	//for(int i=0;i<RELCNT;i++)
		//this->relRecArray[i].initRelRec();
}
const Status DBHeadPage::getRelFirstPage(const std::string &relName,unsigned long &firstPageNo) const
{
	for(int i=0;i<RELCNT;i++)
	{
		if(this->relRecArray[i].getFirstPageNo()==0)
			continue;
		if(this->relRecArray[i].getRelName()==relName)
		{
			firstPageNo=this->relRecArray[i].getFirstPageNo();
			return OK;
		}
	}
	return RELNOTFOUND;
}
const Status DBHeadPage::addRel(const std::string& tableName,unsigned long tableFirstPage)//向关系记录的集合中新加一个关系
{
	if(this->isFull())
		return DBHPAGEFULL;
	unsigned long tempInt;
	if(this->getRelFirstPage(tableName,tempInt)==OK)
		return RELEXIST;
	for(int i=0;i<RELCNT;i++)
	{
		if(this->relRecArray[i].getFirstPageNo()==0)
		{
			Status status;
			if((status=this->relRecArray[i].setRelName(tableName))!=OK)
				return status;
			if((status=this->relRecArray[i].setFirstPageNo(tableFirstPage))!=OK)
				return status;
			break;
		}
	}
	return OK;
}
const Status DBHeadPage::deleteRel(const std::string &tableName)//向关系记录的集合中删除一个关系
{
	for(int i=0;i<RELCNT;i++)
	{
		if(tableName.compare(this->relRecArray[i].getRelName())==0)
		{
			this->relRecArray[i].initRelRec();
			return OK;
		}
	}
	return RELNOTFOUND;
}
const Status DBHeadPage::updataRel(const std::string& oldRelName, const std::string& newRelName)//更新某个关系记录
{
	Status status;
	for(int i=0;i<RELCNT;i++)
	{
		if(oldRelName.compare(this->relRecArray[i].getRelName())==0)
		{
			if((status=this->relRecArray[i].setRelName(newRelName))!=OK)
				return status;
			return OK;
		}
	}
	return RELNOTFOUND;
}
const bool DBHeadPage::isFull() const
{
	for(int i=0;i<RELCNT;i++)
	{
		if(this->relRecArray[i].getFirstPageNo()==0)
			return false;
	}
	return true;
}
const bool DBHeadPage::isEmpty() const
{
	for(int i=0;i<RELCNT;i++)
	{
		if(this->relRecArray[i].getFirstPageNo()!=0)
			return false;
	}
	return true;
}
const unsigned long DBHeadPage::getNextFreePage() const
{
	return this->nextFreePage;
}

const Status DBHeadPage::setNextFreePage(unsigned long freePageNo)
{
	this->nextFreePage=freePageNo;
	return OK;
}
const unsigned long DBHeadPage::getRelCnt() const
{
	return this->relCnt;
}
const Status DBHeadPage::setRelCnt(unsigned long relCnt)
{
	this->relCnt=relCnt;
	return OK;
}
const unsigned long DBHeadPage::getPageCnt() const
{
	return this->pageCnt;
}
const Status DBHeadPage::setPageCnt(unsigned long pageCnt)
{
	this->pageCnt=pageCnt;
	return OK;
}
const unsigned long DBHeadPage::getNextDBHPNo() const
{
	return this->nextDBHPNo;
}
const Status DBHeadPage::setNextDBHPNo(unsigned long nextDBHPNo)
{
	this->nextDBHPNo=nextDBHPNo;
	return OK;
}
const std::string DBHeadPage::getDBName() const
{
	std::string dbNameStr(this->dbName);
	return dbNameStr;
}
const Status DBHeadPage::setDBName(const std::string &DBName)
{
	if(DBName.length()==0 || DBName.length()>=MAXNAMESIZE)
		return BADDBNAME;
	DBName.copy(this->dbName,DBName.length());
	return OK;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*void RelHeadPage::init(const string &relName)
{
	if((relName.length()>MAXNAMESIZE) || (relName.length()<=0))
		return BADRELNAME;
	relName.assign(this->relName,relName.length());
	this->nextPageNo=-1;
	this->pageCnt=1;
	this->recCnt=0;
	this->pageNum=0;
	for(int i=0;i<DPRCNT;i++)
		this->pages[i].init();
}
const Status RelHeadPage::addNewDPRec(int pageNo)
{
}*/
////////////////////////////////////////////////////////////////////////////////////////////
DB::DB()
{
	this->DBName="";
	this->file.clear();
	this->openCnt=0;
}
DB::DB(const std::string& DBName)
{
	this->DBName=DBName;
	this->file.clear();
	this->openCnt=0;
}
DB::~DB()
{
	if (this->openCnt == 0)
		return;
	Status status;
	Error error;
	while(true)
	{
		if((status = this->closeDB())!=OK)
		{
			error.print(status);
			return;
		}
		if (this->openCnt == 0)
			return;
	}
}
const Status DB::getFirstDBHP(const DBHeadPage* &dbHeadPage) const
{
	if (this->openCnt == 0)
		return FILENOTOPEN;
	dbHeadPage=&(this->firstDBHP);
	return OK;
}
const Status DB::flushFirstDBHP()
{
	if (this->file.is_open() == 0)
		return FILENOTOPEN;
	return this->intwrite(0,(Page*)&(this->firstDBHP));
}
const Status DB::createDB(const std::string &DBName)
{
	if((DBName.length()>MAXNAMESIZE) || (DBName.length()<=0))
		return BADDBNAME;
	this->file.open(DBName.c_str(),ios::in|ios::out|ios::noreplace|ios::binary);
	if(this->file.fail())
		return FILEEXISTS;
	// An empty file contains just DBFirstPage and DBHeadPage.
	Page firstHeadPage;
	Status status;
	memset(&firstHeadPage, 0, sizeof(firstHeadPage));
	DHP(firstHeadPage).init();
	if((status=DHP(firstHeadPage).setDBName(DBName))!=OK)
		return status;
	if(this->file.write((char*)(&firstHeadPage),PAGESIZE)==NULL)
		return UNIXERR;
	this->file.close();
	return OK;
}
const Status DB::destroyDB(const std::string &DBName)
{
	if (::remove(DBName.c_str()) < 0)
		return UNIXERR;
	return OK;
}
const Status DB::openDB(const std::string &DBName)//打开数据库
{
  // Open file -- it will be closed in closeFile()
	if((DBName.length()>MAXNAMESIZE) || (DBName.length()<=0))
		return BADDBNAME;
	this->DBName=DBName;
	if (this->openCnt == 0)
	{
		this->file.open(DBName.c_str(),ios::in|ios::out|ios::nocreate|ios::binary,filebuf::sh_none);
		if(this->file.fail())
			return FILENOTEXISTS;
		Status status;
		if((status=this->intread(0,(Page*)&(this->firstDBHP)))!=OK)
			return status;
	}
	this->openCnt++;
	return OK;
}
const Status DB::closeDB()//关闭数据库
{
	if (this->openCnt == 0)
		return FILENOTOPEN;
	Status status;
	// File actually closed only when open count goes to zero.
	this->openCnt--;
	if (openCnt == 0) 
	{
		if((status=this->flushFirstDBHP())!=OK)
			return status;
		//if (bufMgr)
			//bufMgr->flushFile(this);
		this->file.close();
	}
	return OK;
}
const Status DB::intread(const unsigned long pageNo, Page* pagePtr) const
{	
	if(pageNo*PAGESIZE>2147483647)
	{
		unsigned long total=pageNo*PAGESIZE;
		if(this->file.seekp(2147483647,ios::beg)==NULL)
			return UNIXERR;
		total-=2147483647;
		while(true)
		{
			if(total<=2147483647)
			{
				if(this->file.seekp(2147483647,ios::cur)==NULL)
					return UNIXERR;
				break;
			}
			else
			{
				if(this->file.seekp(2147483647,ios::cur)==NULL)
					return UNIXERR;
				total-=2147483647;
			}
		}
	}
	else
	{
		if(this->file.seekp(pageNo*PAGESIZE)==NULL)
			return UNIXERR;
	}
	if(this->file.read((char*)pagePtr,PAGESIZE)==NULL)
		return UNIXERR;
  return OK;
}
// Write a page to file. Page data is at the page address
// provided by the caller.
const Status DB::intwrite(const unsigned long pageNo, const Page* pagePtr)
{
	if(pageNo*PAGESIZE>2147483647)
	{
		unsigned long total=pageNo*PAGESIZE;
		if(this->file.seekp(2147483647,ios::beg)==NULL)
			return UNIXERR;
		total-=2147483647;
		while(true)
		{
			if(total<=2147483647)
			{
				if(this->file.seekp(2147483647,ios::cur)==NULL)
					return UNIXERR;
				break;
			}
			else
			{
				if(this->file.seekp(2147483647,ios::cur)==NULL)
					return UNIXERR;
				total-=2147483647;
			}
		}
	}
	else
	{
		if(this->file.seekp(pageNo*PAGESIZE)==NULL)
			return UNIXERR;
	}
	if(this->file.write((char*)pagePtr,PAGESIZE)==NULL)
		return UNIXERR;
	this->file.flush();
	return OK;
}
const Status DB::allocatePage(unsigned long &pageNo)//为该数据库分配一个新页（即在磁盘上扩展4096个字节）或者空闲页
{
	if (this->openCnt == 0)
		return FILENOTOPEN;
	Status status;
	if(this->firstDBHP.getNextFreePage()==0)
	{// no free list, have to extend file
    // Extend file -- the current number of pages will be added
    // the page number of the page to be returned.
		pageNo=this->firstDBHP.getPageCnt();
		Page newPage;
		memset(&newPage, 0, sizeof(newPage));
		if ((status = this->intwrite(pageNo, &newPage)) != OK)
			return status;
		if((status=this->firstDBHP.setPageCnt(pageNo+1))!=OK)
			return status;
	}
	else
	{
		pageNo = this->firstDBHP.getNextFreePage();
		Page firstFree;
		if ((status = this->readPage(pageNo, &firstFree)) != OK)
			return status;
		if((status=this->firstDBHP.setNextFreePage(DHP(firstFree).getNextFreePage()))!=OK)
			return status;
	}
	return OK;
}
const Status DB::disposePage(const unsigned long pageNo)
{
	if (this->openCnt==0)
		return FILENOTOPEN;
	if (pageNo==0)
		return BADPAGENO;
	Status status;
	// The first user-allocated page in the file cannot be
	// disposed of. The File layer has no knowledge of what
	// is the next page in the file and hence would not be
	// able to adjust the firstPage field in file header.
	if (pageNo>=this->firstDBHP.getPageCnt())
		return BADPAGENO;
	// Deallocate page by attaching it to the free list.
	Page away;
	if((status=this->readPage(pageNo,&away))!=OK)
		return status;
	memset(&away,0,PAGESIZE);
	DHP(away).setNextFreePage(this->firstDBHP.getNextFreePage());
	this->firstDBHP.setNextFreePage(pageNo);
	if ((status=this->writePage(pageNo,&away)) != OK)
		return status;
	return OK;
}
const Status DB::readPage(const unsigned long pageNo,Page* pagePtr) const
{
	if (!pagePtr)
		return BADPAGEPTR;
	if ((pageNo == 0) || (pageNo>=this->firstDBHP.getPageCnt()))
		return BADPAGENO;
	if (this->file.is_open() == 0)
		return FILENOTOPEN;
	return this->intread(pageNo,pagePtr);
}
const Status DB::writePage(const unsigned long pageNo,const Page* pagePtr)
{
	if (!pagePtr)
		return BADPAGEPTR;
	if ((pageNo == 0) || (pageNo>=this->firstDBHP.getPageCnt()))
		return BADPAGENO;
	if (this->file.is_open() == 0)
		return FILENOTOPEN;
	return intwrite(pageNo,pagePtr);
}
const unsigned long DB::getOpenCnt() const
{
	return this->openCnt;
}
const Status DB::createRel(const std::string &relName)
{
	Status status;
	unsigned long newRHPNo=0;//first page in this new relation
	unsigned long tempInt=0;
	unsigned long nextDBHPNo;
	if(this->firstDBHP.isFull())
	{
		nextDBHPNo=this->firstDBHP.getNextDBHPNo();
	}
	else
	{
		if(this->firstDBHP.getRelFirstPage(relName,tempInt)==OK)
			return RELEXIST;
		if((status=this->allocatePage(newRHPNo))!=OK)
			return status;
		if((status=this->firstDBHP.addRel(relName,newRHPNo))!=OK)
			return status;
		if((status=this->firstDBHP.setRelCnt(this->firstDBHP.getRelCnt()+1))!=OK)
			return status;
		return OK;
	}
	while(nextDBHPNo!=0)
	{
		Page headPage;
		if((status=this->readPage(nextDBHPNo,&headPage))!=OK)
			return status;
		nextDBHPNo=DHP(headPage).getNextDBHPNo();
		if(DHP(headPage).isFull())
			continue;
		if(this->firstDBHP.getRelFirstPage(relName,tempInt)==OK)
			return RELEXIST;
		if((status=this->allocatePage(newRHPNo))!=OK)
			return status;
		if((status=DHP(headPage).addRel(relName,newRHPNo))!=OK)
			return status;
		if((status=this->firstDBHP.setRelCnt(this->firstDBHP.getRelCnt()+1))!=OK)
			return status;
		if((status=this->writePage(nextDBHPNo,(Page*)&headPage))!=OK)
			return status;
		return OK;
	}
	unsigned long newHeadPageNo=0; //we need a new head page
	DBHeadPage newHeadPage;
	//allocate a new head page first
	if((status=this->allocatePage(newHeadPageNo))!=OK)
		return status;
	if((status=newHeadPage.setNextDBHPNo(this->firstDBHP.getNextDBHPNo()))!=OK)
		return status;
	if((status=this->firstDBHP.setNextDBHPNo(newHeadPageNo))!=OK)
		return status;
	//allocate the first page for the relation next
	if((status=this->allocatePage(newRHPNo))!=OK)
		return status;
	if((status=newHeadPage.addRel(relName,newRHPNo))!=OK)
		return status;
	if((status=this->firstDBHP.setRelCnt(this->firstDBHP.getRelCnt()+1))!=OK)
			return status;
	if((status=this->writePage(newHeadPageNo,(Page*)(&newHeadPage)))!=OK)
		return status;
	return OK;
}
const Status DB::destroyRel(const std::string &relName)
{
	Status status;
	unsigned long relFirstPageNo=0;
	if(this->firstDBHP.getRelFirstPage(relName,relFirstPageNo)==OK)
	{
		if((status=this->firstDBHP.deleteRel(relName))!=OK)
			return status;
		if((status=this->firstDBHP.setRelCnt(this->firstDBHP.getRelCnt()-1))!=OK)
			return status;
		//if((status=this->disposePage(relFirstPageNo))!=OK)
			//return status;
		return OK;
	}
	else
	{
		Page headPage;
		unsigned long thisDBHPNo=this->firstDBHP.getNextDBHPNo();
		unsigned long nextDBHPNo=this->firstDBHP.getNextDBHPNo();
		while(nextDBHPNo!=0)
		{
			if((status=this->readPage(nextDBHPNo,&headPage))!=OK)
				return status;
			thisDBHPNo=nextDBHPNo;
			nextDBHPNo=DHP(headPage).getNextDBHPNo();
			if((status=DHP(headPage).getRelFirstPage(relName,relFirstPageNo))!=OK)
				continue;
			if((status=DHP(headPage).deleteRel(relName))!=OK)
				return status;
			if((status=this->firstDBHP.setRelCnt(this->firstDBHP.getRelCnt()-1))!=OK)
				return status;
			if((status=this->writePage(thisDBHPNo,&headPage))!=OK)
				return status;
			//if((status=this->disposePage(relFirstPageNo))!=OK)
				//return status;
			return OK;
		}
	}
	return RELNOTFOUND;
}
const Status DB::updataRel(const std::string &oldRelName,const std::string &newRelName)
{
	Status status;
	unsigned long relFirstPageNo=0;
	if(this->firstDBHP.getRelFirstPage(oldRelName,relFirstPageNo)==OK)
		return this->firstDBHP.updataRel(oldRelName,newRelName);
	else
	{
		Page headPage;
		unsigned long thisDBHPNo=this->firstDBHP.getNextDBHPNo();
		unsigned long nextDBHPNo=this->firstDBHP.getNextDBHPNo();
		while(nextDBHPNo!=0)
		{
			if((status=this->readPage(nextDBHPNo,&headPage))!=OK)
				return status;
			thisDBHPNo=nextDBHPNo;
			nextDBHPNo=DHP(headPage).getNextDBHPNo();
			if((status=DHP(headPage).getRelFirstPage(oldRelName,relFirstPageNo))!=OK)
				continue;
			if((status=DHP(headPage).updataRel(oldRelName,newRelName))!=OK)
				return status;
			return this->writePage(thisDBHPNo,&headPage);
		}
	}
	return RELNOTFOUND;
}
const Status DB::getRelFirstPageNo(const std::string &relName,unsigned long &firstPageNo) const
{
	if(this->firstDBHP.getRelFirstPage(relName,firstPageNo)==OK)
		return OK;
	Status status;
	Page headPage;
	unsigned long thisDBHPNo=this->firstDBHP.getNextDBHPNo();
	unsigned long nextDBHPNo=this->firstDBHP.getNextDBHPNo();
	while(nextDBHPNo!=0)
	{
		if((status=this->readPage(nextDBHPNo,&headPage))!=OK)
			return status;
		thisDBHPNo=nextDBHPNo;
		nextDBHPNo=DHP(headPage).getNextDBHPNo();
		if((status=DHP(headPage).getRelFirstPage(relName,firstPageNo))!=OK)
			continue;
		return OK;
	}
	return RELNOTFOUND;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
DBManager::DBManager()
{
	this->dbMap.clear();
}
DBManager::~DBManager()
{
	if(this->dbMap.empty())
		return;
	DB* db;
	DBMap::iterator MyIterator=this->dbMap.begin();
	while(MyIterator!=this->dbMap.end())
	{
		db=(*MyIterator).second;
		this->dbMap.erase((*MyIterator).first);
		delete db;
		MyIterator=this->dbMap.begin();
	}
}
const Status DBManager::createDataBase(const std::string &DBName)
{
	if (DBName.empty())
		return BADFILE;
	// First check if the file has already been opened
	if(this->dbMap.find(DBName)!=this->dbMap.end())
		return FILEEXISTS;
	// Do the actual work
	DB db(DBName);
	return db.createDB(DBName);
}
const Status DBManager::destroyDataBase(const std::string &DBName)
{
	if (DBName.empty()) return BADFILE;
	// Make sure file is not open currently.
	if(this->dbMap.find(DBName)!=this->dbMap.end()) return FILEOPEN;
	// Do the actual work
	DB db(DBName);
	return db.destroyDB(DBName);
}
const Status DBManager::openDataBase(const std::string &DBName,DB* & database)
{
	Status status;
	if (DBName.empty()) return BADFILE;
	DB* db;
	// Check if DB already open.
	if (this->dbMap.find(DBName) != this->dbMap.end())
	{
		// DB is already open, call open again on the DB object
		// to increment it's open count.
		db=this->dbMap[DBName];
		if((status = db->openDB(DBName))!=OK)
			return status;
		this->dbMap[DBName]=db;
	}
	else
	{
		// DB is not already open
		// Otherwise create a new DB object and open it
		//std::trl::shared_ptr<DB> db(new DB(DBName));
		db=new DB(DBName);
		ASSERT(db!=NULL);
		if((status = db->openDB(DBName))!=OK)
			return status;
		// Insert into the mapping table
		this->dbMap[DBName]=db;
	}
	database=db;
	return OK;
}
const Status DBManager::closeDataBase(const std::string &DBName)
{
	Status status;
	if (DBName.empty()) return BADDBNAME;
	DB* db;
	if (this->dbMap.find(DBName) != this->dbMap.end())
	{
		db=this->dbMap[DBName];
		if((status = db->closeDB())!=OK)
			return status;
		if(db->getOpenCnt()==0)
		{
			this->dbMap.erase(DBName);
			delete db;
		}
		return OK;
	}
	return FILENOTOPEN;
}
