// newBuf.h: interface for the newBuf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWBUF_H__EE4A9EB3_1982_4CAE_96B9_DA13E3C81A22__INCLUDED_)
#define AFX_NEWBUF_H__EE4A9EB3_1982_4CAE_96B9_DA13E3C81A22__INCLUDED_

#include "newIO.h"
// define if debug output wanted
//#define DEBUGBUF

// declarations for buffer pool hash table
class HashBucket
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
class BufHashTbl
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
class BufMgr;  //forward declaration of BufMgr class 
// class for maintaining information about buffer pool frames
class BufDesc
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
class BufMgr 
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
#endif // !defined(AFX_NEWBUF_H__EE4A9EB3_1982_4CAE_96B9_DA13E3C81A22__INCLUDED_)
