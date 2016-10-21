#ifndef DBDLL_API
#define DBDLL_API _declspec(dllimport)
#endif
#include <fstream.h>
#include <iostream.h>
#include <string>
#include <map>
enum DBDLL_API Status {
// no error

       OK = 0, NOTUSED1 = -999,

// File and DB errors

       BADFILEPTR, BADFILE, FILETABFULL, FILEOPEN, BADFIRSTPAGENO, BADRELNAME,BADFREESPACE,RELEXIST,RELNOTFOUND,
       DBHPAGEFULL,DBHPNEEDDISPOSE,BADPAGECNT,BADRELCNT,BADDBNAME,BADFIRSTHEADPAGENO,DBNOTFOUND,
FILENOTOPEN,BADDBPTR,
       UNIXERR, BADPAGEPTR, BADPAGENO, FILEEXISTS,FILENOTEXISTS,

// BufMgr and HashTable errors

       HASHTBLERROR, HASHNOTFOUND, BUFFEREXCEEDED, HASHEXIST,

PAGENOTPINNED,
       BADBUFFER, PAGEPINNED,

// Page errors
	
       NOSPACE,  NORECORDS,  ENDOFPAGE, INVALIDSLOTNO, BADREC,ATTRINPAGENOTFOUND,BADATTRS,

INVALIDRECLEN,

// HeapFile errors

       BADRID, BADRECPTR, BADSCANPARM, BADSCANID,

SCANTABFULL, FILEEOF, FILEHDRFULL,

// Index errors
 
       BADINDEXPARM, RECNOTFOUND, BUCKETFULL, 

DIROVERFLOW, 
       NONUNIQUEENTRY, NOMORERECS,

// SortedFile errors
 
       BADSORTPARM, INSUFMEM, 
	
// Catalog errors

       BADCATPARM, ATTRNOTFOUND,
       NAMETOOLONG, DUPLATTR, RELEXISTS, NOINDEX,
       INDEXEXISTS, ATTRTOOLONG,

// Utility errors

// Query errors

       ATTRTYPEMISMATCH, 

TMP_RES_EXISTS,BADOPERATOR,DATATYPEMISMATCH,BADDATATYPE,

// do not touch filler -- add codes before it

       NOTUSED2
};


// definition of Error class

class DBDLL_API Error {
 public:
  void print(Status status);
};
const unsigned long PAGESIZE = 4096;

class DBDLL_API AttrInPage //attribute in page
{
private:
	bool  attrisNull;//true:null,false:not null
	bool  attrisLOB;
	unsigned short attrLen;//this attribute's real length
public:
	AttrInPage():attrisNull(false),attrisLOB(false),attrLen(0){}
	~AttrInPage(){}
	const bool isNull() const;
	const unsigned short getAttrLen() const;
	const bool isLOB() const;
	void setNull(const bool);
	void setLen(const unsigned short);
	void setLOB(const bool);
};
class DBDLL_API Record //record class
{
	friend class Page;
	friend class RelOperator;
private:
	unsigned short attrCnt;//attributes' count
	AttrInPage* attrsInPage;//every attribute in this record
	void* realData;//point to the real data
public:
	Record():attrCnt(0),attrsInPage(NULL),realData(NULL){}
	~Record();
	const Status initRec(void* thisRec);
	const Status setAttrsInPage(AttrInPage attrs[],const unsigned short attrNum);
	const unsigned short getAttrsLen() const;
	const unsigned long getRealLength () const;
	const unsigned long getWholeLength () const;
	const unsigned short getAttrsNum() const;
	const Status getAttrInPage(const unsigned short attrNo,AttrInPage &attrs) const;
	const bool isLargeRec() const;
	const void* getRealData() const;
	void setRealData(void* dataPtr);
};

class DBDLL_API RID
{
private:
	unsigned long pageNo;//store this record's pageNo in order.
	unsigned short slotNo; //store this record's slotNo in order. vector<int>'s maxsize is 1073741823，enough to save 4GB.
public:
	RID():pageNo(0),slotNo(0){}
	~RID(){}
	const unsigned long getPageNo() const;
	const unsigned short getSlotNo() const;
	void setPageNo(const unsigned long pgNo);
	void setSlotNo(const unsigned short slNo);
};
const RID NULLRID;
class DBDLL_API Slot
{
private:
	unsigned short	offset;  
	unsigned short	length;  // equals 0 if slot is not in use
public:
	Slot():offset(0),length(0){}
	~Slot(){}
	const unsigned short getOffset() const;
	const unsigned short getLength() const;
	void setOffset(const unsigned short offs);
	void setLength(const unsigned short len);

};
const unsigned DPFIXED = sizeof(Slot)+4*sizeof(short)+2*sizeof(unsigned long);
// size of the data area of a page

// Class definition for a minirel data page.   
// The design assumes that records are kept compacted when
// deletions are performed. Notice, however, that the slot
// array cannot be compacted.  Notice, this class does not keep
// the records align, relying instead on upper levels to take
// care of non-aligned attributes

class DBDLL_API Page
{
private:
    char 	       data[PAGESIZE - DPFIXED]; 
    Slot 	       slot[1]; // first element of slot array - grows backwards!
    short          slotCnt; // number of slots in use;
    unsigned short freePtr; // offset of first free byte in data[]
    unsigned short freeSpace; // number of bytes free in data[]
    unsigned short dummy;	// for alignment purposes
    unsigned long  nextPage; // forwards pointer
    unsigned long  curPage;  // page number of current pointer
public:
    void init(const unsigned long pageNo); // initialize a new page
    void dumpPage() const;       // dump contents of a page
    const unsigned long getNextPageNo() const; // returns pageNo of nextPage
	const unsigned long getThisPageNo() const; // returns pageNo of ThisPage
    void setNextPage(const unsigned long pageNo){this->nextPage=pageNo;} // sets value of nextPage to pageNo
	const int hasBlankSlot() const;
    const unsigned short getFreeSpace() const; // returns amount of free space
	const unsigned short getFreePtr() const;
	const short getSlotCnt() const;
	const unsigned short getOffset(const short i) const;
	const unsigned short getLength(const short i) const;
    // inserts a new record (rec) into the page, returns RID of record 
    const Status insertRecord(const Record &rec, RID& rid);

    // update the record with the specified rid
    const Status updateRecord(const Record & rec, const RID& rid);

    // delete the record with the specified rid
    const Status deleteRecord(const RID & rid);

    // returns RID of first record on page
    // returns  NORECORDS if page contains no records.  Otherwise, returns OK
    const Status firstRecord(RID& firstRid) const;

    // returns RID of next record on the page 
    // returns ENDOFPAGE if no more records exist on the page
    const Status nextRecord (const RID & curRid, RID& nextRid) const;

    // returns reference to record with RID rid
    const Status getRecord(const RID & rid, Record & rec);
	const Status insertLRec(const Record &rec, RID& rid);
	const Status insertPureData(const void* src,unsigned short len);
	void getPureData(void* dest) const;
};

const MAXNAMESIZE=32;//数据库名，关系名，列名，索引名，约束名的最大长度不超过32

class RelRecord //关系记录类，该类的对象将存储于数据库目录页中。
{
	friend class DBHeadPage;
private:
	unsigned long firstPageNo;//关系头页的页号,unsigned long占4字节，取值范围从0到4,294,967,295。
	//如果每页4096字节，则本数据库可以存储16T的数据。0为无效页。
	char relName[MAXNAMESIZE];//关系名
	explicit RelRecord();
	const unsigned long getFirstPageNo() const;//取该关系头页的页号，即直接返回this->firstPageNo。
	const std::string getRelName() const;//取该关系的关系名，将this->relName转换为string类型然后返回。
	const Status setFirstPageNo(unsigned long firstPageNo);//置该关系头页的页号，需判断参数的正确性。（若参数
	//firstPageNo<=0则返回BADFIRSTPAGENO，否则返回OK）
	const Status setRelName(const std::string& relName);////置该关系的关系名，需判断参数的正确性。（若参数
	//relName为空或长度大于MAXNAMESIZE则返回BADRELNAME，否则返回OK）
	void initRelRec();//初始化该关系记录，将this->firstPageNo置为0。
};
const unsigned long RELCNT = (PAGESIZE-4*sizeof(unsigned long)-MAXNAMESIZE)/sizeof(RelRecord);//数据库目录页中能够存储的关系记录个数。
class DBHeadPage  //数据库目录页类，用来存储数据库信息和库中每个关系的关系记录，可以有多个数据库目录页，用
//nextDBHPNo将所有数据库目录页链起来。用nextFreePage将所有空闲页链起来。
{
	friend class DB;
private:
	unsigned long relCnt;  //关系数
	unsigned long nextFreePage;//下一个空闲页页号，0为无下一页
	unsigned long pageCnt;//页数（包含空闲页）
	unsigned long nextDBHPNo;//下一个数据库头页页号，0为无下一页
	char dbName[MAXNAMESIZE];//数据库名
	RelRecord relRecArray[RELCNT];//关系记录数组
	char DBsignature[PAGESIZE-(RELCNT*sizeof(RelRecord)+MAXNAMESIZE+4*sizeof(unsigned long))]; //数据库签名,为了不使内存越界，
	//DBHeadPage比Page小16个字节
	const Status setNextFreePage(unsigned long freePageNo);//置下一个空闲页页号，需判断参数的正确性。
	const Status setPageCnt(unsigned long pageCnt);//置该数据库的总页数，需判断参数的正确性。
	const Status setRelCnt(unsigned long relCnt);//置该数据库的关系数，需判断参数的正确性。
	const Status setNextDBHPNo(unsigned long nextDBHPNo);//置下一个数据库目录页页号，需判断参数的正确性。
	const Status setDBName(const std::string &DBName);//置数据库名称，需判断参数的正确性。
	const Status addRel(const std::string& tableName,unsigned long tableFirstPage);//向关系记录的集合中新加一个关系，需判断该
	//数据库目录页是否已满，该关系记录是否已经存在。
	const Status updataRel(const std::string& oldRelName, const std::string& newRelName);//更新某个关系记录，需判断该关系
	//记录是否已经存在。
	const Status deleteRel(const std::string& tableName); //向关系记录的集合中删除一个关系，需判断该关系记录是否已经
	//存在。
public:
	explicit DBHeadPage();
	~DBHeadPage();
	const unsigned long getRelCnt() const;//返回关系数
	const unsigned long getNextFreePage() const;//返回下一个空闲页页号
	const unsigned long getPageCnt() const;//返回该数据库的总页数。
	const unsigned long getNextDBHPNo() const;//返回下一个数据库目录页页号
	const std::string getDBName() const;//返回数据库名称
	void init();//初始化数据库目录页（this->relCnt=0;this->nextFreePage=0;this->pageCnt=1;this->nextDBHPNo=0;
	//并将this->relRecArray[]数组中每个关系记录初始化）。
	const bool isFull() const;//判断该数据库目录页是否已满。
	const bool isEmpty() const;//判断该数据库目录页是否已空。
	const Status getRelFirstPage(const std::string &relName,unsigned long &firstPageNo) const;//通过关系名去查找该关系的头页页号，
	//需判断该关系记录是否已经存在。
};
class DBDLL_API DB //数据库类，封装对数据库的操作。
//包括新建数据库、删除数据库、打开数据库、关闭数据库、新建关系、删除关系、分配页、释放页、读页、写页。
{
	friend class DBManager;
private:
	std::string DBName; //数据库名
	mutable fstream file; //对文件的操作对象
	unsigned long openCnt;//数据库的打开次数
	DBHeadPage firstDBHP;//数据库首个目录页，在openDB()中读入
	explicit DB();//构造函数，this->file.clear();this->openCnt=0;。
	explicit DB(const std::string &DBName);
	const Status intread(const unsigned long pageNo, Page* pagePtr) const;
	const Status intwrite(const unsigned long pageNo, const Page* pagePtr);
	const Status createDB(const std::string & DBName) ;  //新建一个数据库（文件），需判断参数的正确性和新建文件成功
	//与否。
	const Status destroyDB(const std::string & DBName) ; // 删除一个数据库,需判断参数的正确性和删除文件成功与否。
	const Status openDB(const std::string & DBName);  //打开数据库，需判断参数的正确性和打开文件成功与否，
	//若已经打开则openCnt++。
	const Status closeDB();//关闭数据库，需判断参数的正确性和关闭文件成功与否。若打开次数大于1则openCnt--，
	//若该数据库未被打开则返回FILENOTOPEN。
public:
	~DB();//析构函数，调用this->closeDB()；并返回判断状态，出错则返回错误信息。
	const Status getFirstDBHP(const DBHeadPage* &dbHeadPage) const;
	const Status flushFirstDBHP();
	const Status createRel(const std::string& relName);//从数据库中创建该关系，置关系头页、关系名、关系页数组。
	//须考虑此数据库目录页已满的情况。
	const Status destroyRel(const std::string& relName);//从数据库中删除该关系。若找不到该关系记录则返回RELNOTFOUND，
	//须考虑在多个数据库目录页中找该关系记录的情况。
	const Status updataRel(const std::string& oldRelName, const std::string& newRelName);//更新某个关系记录。若找不到该
	//关系记录则返回RELNOTFOUND，须考虑在多个数据库目录页中找该关系记录的情况。
	const Status allocatePage(unsigned long& pageNo);//为该数据库分配一个新页（即在磁盘上扩展PAGESIZE个字节）或者空闲页。
	//先去找有没有空闲页，若没有则在磁盘上扩展一个新页。
	const Status disposePage(const unsigned long pageNo);//将该页加入空闲页链表，需判断参数的正确性，然后将该页加入空闲页链。
	const Status readPage(const unsigned long pageNo,Page* pagePtr) const;//将磁盘中该页号的内容读入pagePtr中，需判断参数的
	//正确性。
	const Status writePage(const unsigned long pageNo,const Page* pagePtr);//将内存中pagePtr内容写入磁盘中该页号的位置。
	const unsigned long getOpenCnt() const;//返回数据库的打开次数。
	const Status getRelFirstPageNo(const std::string &relName,unsigned long &firstPageNo) const;//通过关系名去查找该关系的头页页号，
	//需判断该关系记录是否已经存在。
	const std::string getDBName() const {return this->DBName;}
};
typedef std::map<std::string,DB*> DBMap;
class DBDLL_API DBManager //数据库管理器,可管理多个数据库。
{
private:
	DBMap dbMap;//数据库对象集合，一个数据库名对应一个DB类的对象。
public:
	explicit DBManager();
	~DBManager();
	const Status createDataBase(const std::string & DBName);//新建一个数据库，先判断该数据库是否已经存在于
	//this->dbMap中。
	const Status destroyDataBase(const std::string & DBName);//删除一个数据库，并从this->dbMap中将该记录删除，
	//先判断该数据库是否存在于this->dbMap中。
	const Status openDataBase(const std::string & DBName,DB* & database);//打开一个数据库，先判断该数据库是否已经存在于
	//this->dbMap中，若不存在则将其插入this->dbMap中，若存在则更新。将打开的数据库对象返回到database。
	const Status closeDataBase(const std::string & DBName);//关闭一个数据库，先判断该数据库是否已经存在于
	//this->dbMap中。若该数据库的OpenCnt==0则从this->dbMap中删除。
};

// declarations for buffer pool hash table
class DBDLL_API HashBucket
{
	friend class BufHashTbl;
private:
	DB*		db;        // pointer for a DB object (more on this below)
	unsigned long	pageNo;        // page number within a file
	unsigned long	frameNo;       // frame number of page in the buffer pool
	HashBucket*		next;  // next node in the hash table
	HashBucket(const DB* db,const unsigned long frameNo,const unsigned long pageNo,const HashBucket* next);
	void setNextPtr(const HashBucket* next);
public:
	~HashBucket();
	const unsigned long getFrameNo() const;
	const unsigned long getPageNo() const;
	const HashBucket* getNextPtr() const;
	const DB* getDBPtr() const;
};
// hash table to keep track of pages in the buffer pool
class DBDLL_API BufHashTbl
{
	friend class BufMgr;
private:
	unsigned long htSize;
	HashBucket**  ht; // actual hash table
	unsigned long hash(const DB* db, const unsigned long pageNo); // returns value between 0 and HTSIZE-1
	BufHashTbl(const unsigned long htSize);  // constructor
public:
	~BufHashTbl(); // destructor
	// insert entry into hash table mapping (db,pageNo) to frameNo;
	// returns 0 if OK, HASHTBLERROR if an error occurred
	const Status insert(const DB* db, const unsigned long pageNo, const unsigned long frameNo);
	// Check if (db,pageNo) is currently in the buffer pool (ie. in
	// the hash table).  If so, return corresponding frameNo. else return 
	// HASHNOTFOUND
	const Status lookup(const DB* db, const unsigned long pageNo, unsigned long & frameNo);
	// delete entry (db,pageNo) from hash table. REturn OK if page was
	// found.  Else return HASHTBLERROR
	const Status remove(const DB* db, const unsigned long pageNo);  
};
//class DBDLL_API BufMgr;  //forward declaration of BufMgr class 
// class for maintaining information about buffer pool frames
class DBDLL_API BufDesc
{
    friend class BufMgr;
private:
	DB*    db;     // pointer to file object
	unsigned long pageNo;   // page within file
	unsigned long frameNo;  // frame # of frame
	unsigned long pinCnt;   // number of times this page has been pinned
	bool     dirty;    // true if dirty;  false otherwise
	bool     valid;    // true if page is valid
	bool     refbit;    //true if page is accessed recently
	bool     dummy;
	BufDesc();
	void clear();  // initialize buffer frame for a new user
	void set(const DB* db, const unsigned long pageNum);
	void addPinCnt();
	void setRefbit(const bool ref);
	void setDirty();
public:
	~BufDesc();
	const unsigned long getPageNo() const;
	const unsigned long getFrameNo() const;
	const unsigned long getPinCnt() const;
	const DB* getDB() const;
	const bool isDirty() const;
	const bool isValid() const;
	const bool isRefbit() const;
	const bool isPinned() const;
};
class DBDLL_API BufMgr 
{
private:
	unsigned long bufNum;    // Number of pages in buffer pool
	BufHashTbl* hashTable;  // hash table mapping (DB, page) to frame
	BufDesc*     bufTable;   // vector of status info, 1 per page
	unsigned long clockHand; 
	unsigned long pinnedFrame; //  
	unsigned long hit; // readPage()
	unsigned long miss; // readPage()
	const Status allocBuf(unsigned long & frameNo);   // allocate a free frame.
	const void releaseBuf(unsigned long frame);     // return unused frame to end of list
public:
	Page*	     bufPool;   // actual buffer pool
	BufMgr(const unsigned long bufNum);
	~BufMgr();
	void advanceClock() 
	{ 
		this->clockHand=(this->clockHand+1)%(this->bufNum); 
	}
	const Status readPage(DB* db, const unsigned long pageNo, Page*& page);
	const Status unPinPage(const DB* db, const unsigned long pageNo, const bool dirty);
	const Status allocPage(DB* db, unsigned long& pageNo, Page*& page); // allocates a new, empty page 
	const Status flushFile(const DB* db); // writing out all dirty pages of the file
	const Status disposePage(DB* db, const unsigned long pageNo); // dispose of page in file
	void  printSelf();
};
extern DBManager dbManager;
extern BufMgr* bufMgr;

enum Datatype { P_CHAR, P_VCHAR, P_INTEGER, P_FLOAT, P_DATE, P_DATETIME, P_BOOLEAN, P_BLOB, P_CLOB };// attribute data types 
//PCHAR(0-255),PVCHAR(0-255),PINTEGER(4),PFLOAT(8),PDATE(4),PDATETIME(8),PBOOL(1),PBLOB(0-4GB),PCLOB(0-4GB)
const unsigned short MAXOFCHAR=255;
const unsigned short SIZEOFINT=4;
const unsigned short SIZEOFFLOAT=4;
const unsigned short SIZEOFDATE=4;
const unsigned short SIZEOFDATETIME=8;
const unsigned short SIZEOFBOOL=1;
enum DBDLL_API Operator { LT, LTE, EQ, GTE, GT, NE, ISNULL, ISNOTNULL };// scan operators
class DBDLL_API DataPageInfo
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
DBDLL_API const Status createRel(const std::string dbname,const std::string relationname);
DBDLL_API const Status destroyRel(const std::string dbname,const std::string relationname);
const unsigned long DPICNT=(PAGESIZE-4*sizeof(unsigned long))/sizeof(DataPageInfo); //510
class DBDLL_API RelHdrPage
{
	friend DBDLL_API const Status createRel(const std::string dbname,const std::string relationname);
	friend DBDLL_API const Status destroyRel(const std::string dbname,const std::string relationname);
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
class DBDLL_API RelOperator
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
class DBDLL_API InsertOperator : public RelOperator
{
public:
	InsertOperator(const std::string &dbname,const std::string &relname, Status &returnStatus):
	  RelOperator(dbname,relname,returnStatus){}
	~InsertOperator(){}
	// insert record into file, returning its RID
	const Status insertRecord(const Record & rec, RID& outRid);
	const Status insertLRecord(const Record & rec, RID& outRid);
};
class DBDLL_API ScanOperator : public RelOperator
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
	  RelOperator(dbname,relname,returnStatus),attrIdx(0),attrLen(0),type(P_CHAR),filter(NULL),op(EQ){}
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
DBDLL_API const Status createDB(const std::string dbName);
DBDLL_API const Status destroyDB(const std::string dbName);
DBDLL_API const Status initDB(const std::string dbName,unsigned long bufNum);
DBDLL_API const Status closeDB(const std::string dbName);
