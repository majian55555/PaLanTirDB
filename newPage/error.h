#ifndef ERROR_H
#define ERROR_H
// error/status codes defined here

// add error codes under appropriate heading
// -- remember to add corresponding message to
//    Error.print() also!

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
	
       NOSPACE,  NORECORDS,  ENDOFPAGE, INVALIDSLOTNO, ATTRINPAGENOTFOUND,BADRECPTR,BADREC,BADATTRS,

INVALIDRECLEN,

// HeapFile errors

       BADRID, BADSCANPARM, BADSCANID, 

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

class Error {
 public:
  void print(Status status);
};
#endif
#ifndef HASASSERT
#define HASASSERT
#define ASSERT(c)  { if (!(c)) { \
                       cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                       cerr << "This condition should hold: " #c << endl; \
                       exit(1); \
                     } \
                   }
#endif