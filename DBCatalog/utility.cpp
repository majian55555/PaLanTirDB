#include "Catalog.h"
#include "utility.h"
#include <io.h>
extern DBManager dbManager;
extern DB* db;
extern Error error;
const Status PLT_CreateDB(const std::string& dbName)
{
	Status status;
	// create db file to hold the relcat and attribute catalogs
	if((status=dbManager.createDataBase(dbName))!=OK)
		return status;
	DB* db=NULL;
	if((status=dbManager.openDataBase(dbName,db))!=OK)
		return status;
	if((status=createRel(dbName,RELCATNAME))!=OK)
		return status;
	if((status=createRel(dbName,ATTRCATNAME))!=OK)
		return status;

	// open relation and attribute catalogs
	relCat = new RelCatalog(dbName,status);
	if (status != OK)
		return status;
	attrCat = new AttrCatalog(dbName,status);
	if (status != OK)
		return status;

	// add tuples describing relcat and attrcat to relation catalog
	// and attribute catalog
	
	RelDesc rd;
	AttrDesc ad;

	rd.setRelName(RELCATNAME);
	rd.setAttrCnt(2);
	if((status=relCat->addInfo(rd))!=OK)
		return status;

	rd.setRelName(ATTRCATNAME);
	rd.setAttrCnt(5);
	if((status=relCat->addInfo(rd))!=OK)
		return status;

	ad.setRelName(RELCATNAME);
	ad.setAttrName("relName");
	ad.setAttrIdx(0);
	ad.setAttrType((int)P_VCHAR);
	ad.setAttrSize(32);
	if((status=attrCat->addInfo(ad))!=OK)
		return status;

	ad.setAttrName("attrCnt");
	ad.setAttrIdx(1);
	ad.setAttrType((int)P_INTEGER);
	ad.setAttrSize(SIZEOFINT);
	if((status=attrCat->addInfo(ad))!=OK)
		return status;

	ad.setRelName(ATTRCATNAME);
	ad.setAttrName("relName");
	ad.setAttrIdx(0);
	ad.setAttrType((int)P_VCHAR);
	ad.setAttrSize(32);
	if((status=attrCat->addInfo(ad))!=OK)
		return status;

	ad.setAttrName("attrName");
	ad.setAttrIdx(1);
	ad.setAttrType((int)P_VCHAR);
	ad.setAttrSize(32);
	if((status=attrCat->addInfo(ad))!=OK)
		return status;

	ad.setAttrName("attrIndex");
	ad.setAttrIdx(2);
	ad.setAttrType((int)P_INTEGER);
	ad.setAttrSize(SIZEOFINT);
	if((status=attrCat->addInfo(ad))!=OK)
		return status;

	ad.setAttrName("attrType");
	ad.setAttrIdx(3);
	ad.setAttrType((int)P_INTEGER);
	ad.setAttrSize(SIZEOFINT);
	if((status=attrCat->addInfo(ad))!=OK)
		return status;

	ad.setAttrName("attrSize");
	ad.setAttrIdx(4);
	ad.setAttrType((int)P_INTEGER);
	ad.setAttrSize(SIZEOFINT);
	if((status=attrCat->addInfo(ad))!=OK)
		return status;

	if((status=dbManager.closeDataBase(dbName))!=OK)
		return status;
	cout << "Database " << dbName.c_str() << " created" << endl;
	return OK;
}

const Status PLT_DestroyDB(const std::string& dbName)
{
	return dbManager.destroyDataBase(dbName);
}

const Status PLT_OpenDB(const std::string& dbName,DB* db)
{
	return dbManager.openDataBase(dbName,db);
}

const Status PLT_CloseDB(const std::string& dbName)
{
	return dbManager.closeDataBase(dbName);
}

const Status PLT_Load(const std::string& relation,const std::string& fileName)
{
	Status status;
	RelDesc rd;
	AttrDesc *attrs=NULL;
	int attrCnt=0;

	if(relation.empty()||fileName.empty()||relation==RELCATNAME||relation==ATTRCATNAME)
		return BADCATPARM;

	// open Unix data file
	int fd;
	if ((fd = open(fileName.c_str(), O_RDONLY, 0)) < 0)
		return UNIXERR;

	//check relcat to make sure the relation exists
	if((status = relCat->getInfo(relation,rd))!=OK)
		return status;

	//check attrcat to make sure the relation attributes entry exists
	if((status = attrCat->getRelInfo(relation,attrCnt,attrs))!=OK)
		return status;

	InsertOperator *insertOp= new InsertOperator(db->getDBName(),relation,status);
	if (status != OK) return status;
	RID rid;
	Record rec;

	//buffer temporarily hold the record read from file
	AttrInPage* attrInPage=new AttrInPage[attrCnt];
	lseek(fd,0,SEEK_SET);
	while(read(fd,attrInPage,attrCnt*sizeof(AttrInPage)))
	{
		unsigned long reclen=0;
		for(int i=0;i<attrCnt;i++)
			reclen+=attrInPage[i].getAttrLen();
		char* buffer=new char[reclen];
		memset((void*)buffer,0,reclen);
		if(read(fd,buffer,reclen)==0)
			return UNIXERR;
		rec.setAttrsInPage(attrInPage,attrCnt);
		rec.setRealData((void*)buffer);
		if((status = insertOp->insertRecord(rec,rid))!=OK)
			return status;
		delete [] buffer;
	}
	delete [] attrInPage;
	delete insertOp;
	delete [] attrs;

	// close Unix file
	if (close(fd) < 0)
		return UNIXERR;
	else
		return OK;
}

void PLT_Quit(void)
{
	Status status=OK;
	if(db!=NULL)
	{
		// close relcat and attrcat
		delete relCat;
		delete attrCat;
		if((status=bufMgr->flushFile(db))!=OK)
		{
			error.print(status);
			return;
		}
		if((status=dbManager.closeDataBase(db->getDBName()))!=OK)
		{
			error.print(status);
			return;
		}
	}
	// delete bufMgr to flush out all dirty pages
	delete bufMgr;
	exit(1);
}

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
//
// Compute widths of columns in attribute array.
//

const Status PLT_computeWidth(const int attrCnt,const AttrDesc attrs[],int *&attrWidth)
{
	attrWidth = new int[attrCnt]; //remember when you will delete them
	if (!attrWidth)
		return INSUFMEM;

	for(int i = 0; i < attrCnt; i++)
	{
		int namelen = attrs[i].getAttrName().length();
		attrWidth[i]=namelen;
		switch(attrs[i].getAttrType())
		{
			//P_CHAR=0, P_VCHAR=1, P_INTEGER=2, P_FLOAT=3, P_DATE=4, P_DATETIME=5,
			//P_BOOLEAN=6, P_BLOB=7, P_CLOB=8
			case 0 : //P_CHAR
				attrWidth[i] = MIN(MAX(namelen, attrs[i].getAttrSize()), 30);
				break;
			case 1 ://P_VCHAR
				attrWidth[i] = MIN(MAX(namelen, attrs[i].getAttrSize()), 30);
				break;
			case 2 ://P_INTEGER
				break;
			case 3 ://P_FLOAT
				attrWidth[i] = MIN(MAX(namelen, 5), 7);
				break;
			case 4 ://P_DATE
				attrWidth[i] = MAX(namelen, SIZEOFDATE);
				break;
			case 5 ://P_DATETIME
				attrWidth[i] = MAX(namelen, SIZEOFDATETIME);
				break;
			case 6 ://P_BOOLEAN
				break;
			case 7 ://P_BLOB
				break;
			case 8 ://P_CLOB
				break;
			default:
				delete [] attrWidth;
				return BADDATATYPE;
		}
	}
	return OK;
}
//
// Prints values of attributes stored in buffer pointed to
// by recPtr. The desired width of columns is in attrWidth.
//
void PLT_printRec(const int attrCnt, const AttrDesc attrs[], int *attrWidth,const Record & rec)
{
	for(int i=0;i<attrCnt;i++)
	{
		//char *attr = (char *)rec.data + attrs[i].attrOffset;
		//char temp[attrs[i].attrLen];
		std::string str1;
		unsigned long offset=0;
		AttrInPage tmpAttrInPage;
		for(int j=0;j<attrs[i].getAttrIdx();j++)
		{
			ASSERT(rec.getAttrInPage(j,tmpAttrInPage)==OK);
			offset+=tmpAttrInPage.getAttrLen();
		}
		ASSERT(rec.getAttrInPage(attrs[i].getAttrIdx(),tmpAttrInPage)==OK);
		switch(attrs[i].getAttrType())
		{
			case 0 : //P_CHAR
				str1.assign((char*)((unsigned long)rec.getRealData()+offset),attrs[i].getAttrSize());
				printf("%-*.*s  ", attrWidth[i], attrWidth[i], str1.c_str());
				break;
			case 1 ://P_VCHAR
				str1.assign((char*)((unsigned long)rec.getRealData()+offset),tmpAttrInPage.getAttrLen());
				printf("%-*.*s  ",attrWidth[i],attrWidth[i],str1.c_str());
				break;
			case 2 ://P_INTEGER
				int tempi;
				memcpy(&tempi,(void*)((unsigned long)rec.getRealData()+offset),SIZEOFINT);
				printf("%-*d  ", attrWidth[i], tempi);
				break;
			case 3 ://P_FLOAT
				float tempf;
				memcpy(&tempf,(void*)((unsigned long)rec.getRealData()+offset),SIZEOFFLOAT);
				printf("%-*.2f  ", attrWidth[i], tempf);
				break;
			case 4 ://P_DATE
				break;
			case 5 ://P_DATETIME
				break;
			case 6 ://P_BOOLEAN
				break;
			case 7 ://P_BLOB
				break;
			case 8 ://P_CLOB
				break;
			default:
				break;
		}
	}
	printf("\n");
}
//
// Prints the contents of the specified relation.
//
// Returns:
// 	OK on success
// 	an error code otherwise
//
const Status PLT_Print(const std::string& dbName,std::string relation)
{
	Status status;
	RelDesc rd;
	AttrDesc *attrs;
	int attrCnt;
	int *attrWidth;
	RID rid;
	Record rec;

	if (relation.empty()) relation=RELCATNAME;

	//check relcat to make sure the relation exists
	status = relCat->getInfo(relation,rd);
	if (status == FILEEOF) return RELNOTFOUND;
	if ((status != OK) && (status != FILEEOF)) return status;

	//check attrcat to make sure the relation attributes entry exists
	status = attrCat->getRelInfo(relation,attrCnt,attrs);
	if (status!=OK) return status;

	status = PLT_computeWidth(attrCnt,attrs,attrWidth);
	if (status!=OK) return status;

	//print header of the relation
	cout << endl;
	for (int i=0; i<attrCnt;i++)
	{
		printf("%-*.*s  ", attrWidth[i], attrWidth[i], attrs[i].getAttrName());
	}
	printf("\n");
	for (int k=0; k<attrCnt;k++)
	{
		std::string str1(attrWidth[k] , '-' );    
		printf("%-*.*s  ", attrWidth[k], attrWidth[k], str1.c_str());
	}
	printf("\n");

	ScanOperator* scanOp = new ScanOperator(dbName,relation,status);
	if (status != OK) return status;

	if((status=scanOp->startScan(0,0,P_CHAR,NULL,EQ))!=OK)
		return status;

	//print tuples
	int j=0;
	while ((status=scanOp->scanNext(rid)) != FILEEOF)
	{
		if (status != OK) return status;
		if((status=scanOp->getRecord(rec))!=OK)
			return status;
		PLT_printRec(attrCnt, attrs, attrWidth,rec);
		++j;
	}
	cout << endl;
	cout << j << " record(s) selected!" << endl;

	scanOp->endScan();
	delete scanOp;
	delete [] attrWidth;
	delete [] attrs;

	return OK;
}
