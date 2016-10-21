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
	unsigned short slotNo; //store this record's slotNo in order. vector<int>'s maxsize is 1073741823��enough to save 4GB.
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

const MAXNAMESIZE=32;//���ݿ�������ϵ������������������Լ��������󳤶Ȳ�����32

class RelRecord //��ϵ��¼�࣬����Ķ��󽫴洢�����ݿ�Ŀ¼ҳ�С�
{
	friend class DBHeadPage;
private:
	unsigned long firstPageNo;//��ϵͷҳ��ҳ��,unsigned longռ4�ֽڣ�ȡֵ��Χ��0��4,294,967,295��
	//���ÿҳ4096�ֽڣ������ݿ���Դ洢16T�����ݡ�0Ϊ��Чҳ��
	char relName[MAXNAMESIZE];//��ϵ��
	explicit RelRecord();
	const unsigned long getFirstPageNo() const;//ȡ�ù�ϵͷҳ��ҳ�ţ���ֱ�ӷ���this->firstPageNo��
	const std::string getRelName() const;//ȡ�ù�ϵ�Ĺ�ϵ������this->relNameת��Ϊstring����Ȼ�󷵻ء�
	const Status setFirstPageNo(unsigned long firstPageNo);//�øù�ϵͷҳ��ҳ�ţ����жϲ�������ȷ�ԡ���������
	//firstPageNo<=0�򷵻�BADFIRSTPAGENO�����򷵻�OK��
	const Status setRelName(const std::string& relName);////�øù�ϵ�Ĺ�ϵ�������жϲ�������ȷ�ԡ���������
	//relNameΪ�ջ򳤶ȴ���MAXNAMESIZE�򷵻�BADRELNAME�����򷵻�OK��
	void initRelRec();//��ʼ���ù�ϵ��¼����this->firstPageNo��Ϊ0��
};
const unsigned long RELCNT = (PAGESIZE-4*sizeof(unsigned long)-MAXNAMESIZE)/sizeof(RelRecord);//���ݿ�Ŀ¼ҳ���ܹ��洢�Ĺ�ϵ��¼������
class DBHeadPage  //���ݿ�Ŀ¼ҳ�࣬�����洢���ݿ���Ϣ�Ϳ���ÿ����ϵ�Ĺ�ϵ��¼�������ж�����ݿ�Ŀ¼ҳ����
//nextDBHPNo���������ݿ�Ŀ¼ҳ����������nextFreePage�����п���ҳ��������
{
	friend class DB;
private:
	unsigned long relCnt;  //��ϵ��
	unsigned long nextFreePage;//��һ������ҳҳ�ţ�0Ϊ����һҳ
	unsigned long pageCnt;//ҳ������������ҳ��
	unsigned long nextDBHPNo;//��һ�����ݿ�ͷҳҳ�ţ�0Ϊ����һҳ
	char dbName[MAXNAMESIZE];//���ݿ���
	RelRecord relRecArray[RELCNT];//��ϵ��¼����
	char DBsignature[PAGESIZE-(RELCNT*sizeof(RelRecord)+MAXNAMESIZE+4*sizeof(unsigned long))]; //���ݿ�ǩ��,Ϊ�˲�ʹ�ڴ�Խ�磬
	//DBHeadPage��PageС16���ֽ�
	const Status setNextFreePage(unsigned long freePageNo);//����һ������ҳҳ�ţ����жϲ�������ȷ�ԡ�
	const Status setPageCnt(unsigned long pageCnt);//�ø����ݿ����ҳ�������жϲ�������ȷ�ԡ�
	const Status setRelCnt(unsigned long relCnt);//�ø����ݿ�Ĺ�ϵ�������жϲ�������ȷ�ԡ�
	const Status setNextDBHPNo(unsigned long nextDBHPNo);//����һ�����ݿ�Ŀ¼ҳҳ�ţ����жϲ�������ȷ�ԡ�
	const Status setDBName(const std::string &DBName);//�����ݿ����ƣ����жϲ�������ȷ�ԡ�
	const Status addRel(const std::string& tableName,unsigned long tableFirstPage);//���ϵ��¼�ļ������¼�һ����ϵ�����жϸ�
	//���ݿ�Ŀ¼ҳ�Ƿ��������ù�ϵ��¼�Ƿ��Ѿ����ڡ�
	const Status updataRel(const std::string& oldRelName, const std::string& newRelName);//����ĳ����ϵ��¼�����жϸù�ϵ
	//��¼�Ƿ��Ѿ����ڡ�
	const Status deleteRel(const std::string& tableName); //���ϵ��¼�ļ�����ɾ��һ����ϵ�����жϸù�ϵ��¼�Ƿ��Ѿ�
	//���ڡ�
public:
	explicit DBHeadPage();
	~DBHeadPage();
	const unsigned long getRelCnt() const;//���ع�ϵ��
	const unsigned long getNextFreePage() const;//������һ������ҳҳ��
	const unsigned long getPageCnt() const;//���ظ����ݿ����ҳ����
	const unsigned long getNextDBHPNo() const;//������һ�����ݿ�Ŀ¼ҳҳ��
	const std::string getDBName() const;//�������ݿ�����
	void init();//��ʼ�����ݿ�Ŀ¼ҳ��this->relCnt=0;this->nextFreePage=0;this->pageCnt=1;this->nextDBHPNo=0;
	//����this->relRecArray[]������ÿ����ϵ��¼��ʼ������
	const bool isFull() const;//�жϸ����ݿ�Ŀ¼ҳ�Ƿ�������
	const bool isEmpty() const;//�жϸ����ݿ�Ŀ¼ҳ�Ƿ��ѿա�
	const Status getRelFirstPage(const std::string &relName,unsigned long &firstPageNo) const;//ͨ����ϵ��ȥ���Ҹù�ϵ��ͷҳҳ�ţ�
	//���жϸù�ϵ��¼�Ƿ��Ѿ����ڡ�
};
class DBDLL_API DB //���ݿ��࣬��װ�����ݿ�Ĳ�����
//�����½����ݿ⡢ɾ�����ݿ⡢�����ݿ⡢�ر����ݿ⡢�½���ϵ��ɾ����ϵ������ҳ���ͷ�ҳ����ҳ��дҳ��
{
	friend class DBManager;
private:
	std::string DBName; //���ݿ���
	mutable fstream file; //���ļ��Ĳ�������
	unsigned long openCnt;//���ݿ�Ĵ򿪴���
	DBHeadPage firstDBHP;//���ݿ��׸�Ŀ¼ҳ����openDB()�ж���
	explicit DB();//���캯����this->file.clear();this->openCnt=0;��
	explicit DB(const std::string &DBName);
	const Status intread(const unsigned long pageNo, Page* pagePtr) const;
	const Status intwrite(const unsigned long pageNo, const Page* pagePtr);
	const Status createDB(const std::string & DBName) ;  //�½�һ�����ݿ⣨�ļ��������жϲ�������ȷ�Ժ��½��ļ��ɹ�
	//���
	const Status destroyDB(const std::string & DBName) ; // ɾ��һ�����ݿ�,���жϲ�������ȷ�Ժ�ɾ���ļ��ɹ����
	const Status openDB(const std::string & DBName);  //�����ݿ⣬���жϲ�������ȷ�Ժʹ��ļ��ɹ����
	//���Ѿ�����openCnt++��
	const Status closeDB();//�ر����ݿ⣬���жϲ�������ȷ�Ժ͹ر��ļ��ɹ�������򿪴�������1��openCnt--��
	//�������ݿ�δ�����򷵻�FILENOTOPEN��
public:
	~DB();//��������������this->closeDB()���������ж�״̬�������򷵻ش�����Ϣ��
	const Status getFirstDBHP(const DBHeadPage* &dbHeadPage) const;
	const Status flushFirstDBHP();
	const Status createRel(const std::string& relName);//�����ݿ��д����ù�ϵ���ù�ϵͷҳ����ϵ������ϵҳ���顣
	//�뿼�Ǵ����ݿ�Ŀ¼ҳ�����������
	const Status destroyRel(const std::string& relName);//�����ݿ���ɾ���ù�ϵ�����Ҳ����ù�ϵ��¼�򷵻�RELNOTFOUND��
	//�뿼���ڶ�����ݿ�Ŀ¼ҳ���Ҹù�ϵ��¼�������
	const Status updataRel(const std::string& oldRelName, const std::string& newRelName);//����ĳ����ϵ��¼�����Ҳ�����
	//��ϵ��¼�򷵻�RELNOTFOUND���뿼���ڶ�����ݿ�Ŀ¼ҳ���Ҹù�ϵ��¼�������
	const Status allocatePage(unsigned long& pageNo);//Ϊ�����ݿ����һ����ҳ�����ڴ�������չPAGESIZE���ֽڣ����߿���ҳ��
	//��ȥ����û�п���ҳ����û�����ڴ�������չһ����ҳ��
	const Status disposePage(const unsigned long pageNo);//����ҳ�������ҳ�������жϲ�������ȷ�ԣ�Ȼ�󽫸�ҳ�������ҳ����
	const Status readPage(const unsigned long pageNo,Page* pagePtr) const;//�������и�ҳ�ŵ����ݶ���pagePtr�У����жϲ�����
	//��ȷ�ԡ�
	const Status writePage(const unsigned long pageNo,const Page* pagePtr);//���ڴ���pagePtr����д������и�ҳ�ŵ�λ�á�
	const unsigned long getOpenCnt() const;//�������ݿ�Ĵ򿪴�����
	const Status getRelFirstPageNo(const std::string &relName,unsigned long &firstPageNo) const;//ͨ����ϵ��ȥ���Ҹù�ϵ��ͷҳҳ�ţ�
	//���жϸù�ϵ��¼�Ƿ��Ѿ����ڡ�
	const std::string getDBName() const {return this->DBName;}
};
typedef std::map<std::string,DB*> DBMap;
class DBDLL_API DBManager //���ݿ������,�ɹ��������ݿ⡣
{
private:
	DBMap dbMap;//���ݿ���󼯺ϣ�һ�����ݿ�����Ӧһ��DB��Ķ���
public:
	explicit DBManager();
	~DBManager();
	const Status createDataBase(const std::string & DBName);//�½�һ�����ݿ⣬���жϸ����ݿ��Ƿ��Ѿ�������
	//this->dbMap�С�
	const Status destroyDataBase(const std::string & DBName);//ɾ��һ�����ݿ⣬����this->dbMap�н��ü�¼ɾ����
	//���жϸ����ݿ��Ƿ������this->dbMap�С�
	const Status openDataBase(const std::string & DBName,DB* & database);//��һ�����ݿ⣬���жϸ����ݿ��Ƿ��Ѿ�������
	//this->dbMap�У����������������this->dbMap�У�����������¡����򿪵����ݿ���󷵻ص�database��
	const Status closeDataBase(const std::string & DBName);//�ر�һ�����ݿ⣬���жϸ����ݿ��Ƿ��Ѿ�������
	//this->dbMap�С��������ݿ��OpenCnt==0���this->dbMap��ɾ����
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
	DB* db;  // I/O�����ݿ���
	std::string relName;
	const unsigned long firstRHPN;//�׸���ϵĿ¼ҳҳ��
	RelHdrPage* curRHP; // ������ж�ס�ĵ�ǰ��ϵĿ¼ҳ
	unsigned long curRHPN;// ��ǰ��ϵĿ¼ҳҳ��
	bool dirtyCRHP;// �����ǰ��ϵĿ¼ҳ���޸���Ϊtrue
	Page* curDataPage;// ������ж�ס������ҳ��ָ��
	unsigned long curDataPageNo;// ����ҳ��ҳ��
	bool dirtyCDP;// �������ҳ���޸���Ϊtrue
	RID curRec;    // ��󷵻ؼ�¼��rid
public:
	RelOperator(const std::string &dbname,const std::string &relname, Status &returnStatus);
	~RelOperator();
	// ����һ��RID�������¼������rec
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
