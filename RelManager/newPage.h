#ifndef PAGE_H
#define PAGE_H
#include "error.h"
#include <vector>
const unsigned long PAGESIZE = 4096;

class AttrInPage //attribute in page
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
class Record //record class
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

class RID
{
private:
	unsigned long pageNo;//store this record's pageNo in order.
	unsigned short slotNo; //store this record's slotNo in order. vector<int>'s maxsize is 1073741823£¬enough to save 4GB.
public:
	RID():pageNo(0),slotNo(0){}
	~RID(){}
	const unsigned long getPageNo() const;
	const unsigned short getSlotNo() const;
	void setPageNo(const unsigned long pgNo);
	void setSlotNo(const unsigned short slNo);
};
const RID NULLRID;
class Slot
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

class Page
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

#endif
