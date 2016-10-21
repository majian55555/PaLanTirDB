#ifndef RELMANAGER_H
#define RELMANAGER_H
#include "newPage.h"
#include "newBuf.h"
extern DBManager dbManager;
extern BufMgr* bufMgr;  
enum Datatype { CHAR, VCHAR, INTEGER, FLOAT, DATE, DATETIME, BOOLEAN, BLOB, CLOB };// attribute data types 
//CHAR(0-255),VCHAR(0-255),INTEGER(4),FLOAT(8),DATE(4),DATETIME(8),BOOL(1),BLOB(0-4GB),CLOB(0-4GB)
const unsigned short MAXOFCHAR=255;
const unsigned short SIZEOFINT=4;
const unsigned short SIZEOFFLOAT=4;
const unsigned short SIZEOFDATE=4;
const unsigned short SIZEOFDATETIME=8;
const unsigned short SIZEOFBOOL=1;
enum Operator { LT, LTE, EQ, GTE, GT, NE, ISNULL, ISNOTNULL };// scan operators
class DataPageInfo
{
private:
	unsigned long dataPageNo; // page number of the page 
	unsigned short freeSpace; // number of free bytes on the page
	unsigned short dummy;// for alignment purposes
public:
	DataPageInfo():dataPageNo(0),freeSpace(0),dummy(0){}
	~DataPageInfo(){}
	const unsigned long getDataPageNo() const;
	const unsigned short getFreeSpace() const;
	void setDataPageNo(const unsigned long);
	void setFreeSpace(const unsigned short);
};
const unsigned long DPICNT=(PAGESIZE-4*sizeof(unsigned long))/sizeof(DataPageInfo); //510
class RelHdrPage
{
	friend const Status createRel(const std::string dbname,const std::string relationname);
	friend const Status destroyRel(const std::string dbname,const std::string relationname);
	friend class RelOperator;
	friend class InsertOperator;
	friend class ScanOperator;
private:
	unsigned long	recCnt;					// record count 
	unsigned long	dataPageCnt;			// number of "in-use" data pages
	unsigned long	nextRelHdrPageNo;
	unsigned long	lastRelHdrPageNo;
	DataPageInfo	pages[DPICNT];
	RelHdrPage(const std::string relationname):recCnt(0),dataPageCnt(0),nextRelHdrPageNo(0),
		lastRelHdrPageNo(0){}
	~RelHdrPage(){}
	void init()
	{
		this->recCnt=0;
		this->dataPageCnt=0;
		this->nextRelHdrPageNo=0;
		this->lastRelHdrPageNo=0;
		for(unsigned long i=0;i<DPICNT;i++)
		{
			this->pages[i].setDataPageNo(0);
			this->pages[i].setFreeSpace(0);
		}
	}
	const int hasBlankPages() const
	{
		for(int i=0;i<DPICNT;i++)
		{
			if(this->pages[i].getDataPageNo()==0)
				return i;
		}
		return -1;
	}
	const int hasSuitablePages(unsigned long recLen) const
	{
		for(int i=0;i<DPICNT;i++)
		{
			if((this->pages[i].getDataPageNo()!=0)&&(this->pages[i].getFreeSpace()>=recLen))
				return i;
		}
		return -1;
	}
	const int getCurIdx(unsigned long dataPageNo) const
	{
		for(int i=0;i<DPICNT;i++)
		{
			if(this->pages[i].getDataPageNo()==dataPageNo)
				return i;
		}
		return -1;
	}
};
class RelOperator
{
protected:
	DB* db;  // I/O层数据库句柄
	std::string relName;
	const unsigned long firstRHPN;//首个关系目录页页号
	RelHdrPage* curRHP; // 缓存池中钉住的当前关系目录页
	unsigned long curRHPN;// 当前关系目录页页号
	bool dirtyCRHP;// 如果当前关系目录页被修改则为true
	Page* curDataPage;// 缓存池中钉住的数据页的指针
	unsigned long curDataPageNo;// 数据页的页号
	bool dirtyCDP;// 如果数据页被修改则为true
	RID curRec;    // 最后返回记录的rid
public:
	RelOperator(const std::string &dbname,const std::string &relname, Status &returnStatus);
	~RelOperator();
	// 给顶一个RID，读入记录，返回rec
    const Status getRecord(const RID &rid, Record & rec);
	const unsigned long getRecCnt() const;
	const Status setRecCnt(unsigned long);
	const unsigned long getDataPageCnt() const;
	const Status setDataPageCnt(unsigned long);
	const std::string getRelName() const;
};
class InsertOperator : public RelOperator
{
public:
	InsertOperator(const std::string &dbname,const std::string &relname, Status &returnStatus):
	  RelOperator(dbname,relname,returnStatus){}
	~InsertOperator(){}
	// insert record into file, returning its RID
	const Status insertRecord(const Record & rec, RID& outRid);
	const Status insertLRecord(const Record & rec, RID& outRid);
	const Status updateRecord(const Record & rec);
};
class ScanOperator : public RelOperator
{
private:
	unsigned short attrIdx;            // attribute in page's index
	unsigned short attrLen;            // attribute in page's length
	Datatype      type;           // datatype of filter attribute
	const char*   filter;      // comparison value of filter
	Operator      op;             // comparison operator of filter
	// The following variables are used to preserve the state
	// of the scan when the method markScan() is invoked.
	// A subsequent invocation of resetScan() will cause the
	// scan to be rolled back to the following
	unsigned long   markedPageNo;	// value of curPageNo when scan was "marked"
	RID             markedRec;    // value of curRec when scan was "marked"
	const bool matchRec(const Record & rec) const;
public:
	ScanOperator(const std::string &dbname,const std::string &relname, Status &returnStatus):
	  RelOperator(dbname,relname,returnStatus),attrIdx(0),attrLen(0),type(CHAR),filter(NULL),op(EQ){}
	~ScanOperator(){}
	const Status startScan(const unsigned short attrIdx,
                           const unsigned short attrLen,
                           const Datatype type_,
                           const char* filter_,
                           const Operator op_);
	const Status endScan(); // terminate the scan
	const Status markScan(); // save current position of scan
	const Status resetScan(); // reset scan to last marked location
	// return RID of next record that satisfies the scan 
	const Status scanNext(RID& outRid);
	// read current record, returning pointer and length
	const Status getRecord(Record & rec);
	// delete current record 
	const Status deleteRecord();
	// update current record
	const Status updateRecord(const Record & rec);
	// marks current page of scan dirty
	void markDirty();
};
#endif