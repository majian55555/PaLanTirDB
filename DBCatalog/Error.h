// Error.h: interface for the Error class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERROR_H__8B92C353_3FB9_4D09_8374_975769F8B35E__INCLUDED_)
#define AFX_ERROR_H__8B92C353_3FB9_4D09_8374_975769F8B35E__INCLUDED_

enum Status {

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

       BADCATPARM, ATTRNOTFOUND,RELCATNOTOPEN,ATTCATNOTOPEN,
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

class Error {
 public:
  void print(Status status);
};


#endif // !defined(AFX_ERROR_H__8B92C353_3FB9_4D09_8374_975769F8B35E__INCLUDED_)
#ifndef HASASSERT
#define HASASSERT
#define ASSERT(c)  { if (!(c)) { \
                       cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                       cerr << "This condition should hold: " #c << endl; \
                       exit(1); \
                     } \
                   }
#endif
