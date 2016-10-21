#include "relManager.h"
#include <iostream.h>
#include <time.h>
#include <cstdio>
#include <sys/stat.h>
#define CALL(c)    { Status s; \
                     if ((s = c) != OK) { \
                       cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                       error.print(s); \
                       cerr << "TEST DID NOT PASS" <<endl; \
                       exit(1); \
                     } \
                   }

#define FAIL(c)  { Status s; \
                   if ((s = c) == OK) { \
                     cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                     cerr << "This call should fail: " #c << endl; \
                     cerr << "TEST DID NOT PASS" <<endl; \
                     exit(1); \
                     } \
                   }
DBManager dbManager;
BufMgr* bufMgr;
class Data
{
public:
	int i;
	float f;
	char s[64];
	Data():i(0),f(0){}
};
int main()
{
	cout << "Testing the relation interface" << endl << endl;
	struct _stat buf;
	Status status=OK;
	Error  error;
	int    i=0,num=10000,j=0;
	RID    newRid,rec2Rid;
	RID*   ridArray;
    ridArray=new RID[num];
	Record	dbrec1,dbrec2;
	Data data1,data2;
	InsertOperator* tmpInert=NULL;
	ScanOperator* scan1=NULL,*scan2=NULL;
	dbManager.destroyDataBase("testDB");
	CALL(dbManager.createDataBase("testDB"));
	DB* db=NULL;
	CALL(dbManager.openDataBase("testDB",db));
	bufMgr=new BufMgr(5000);
	cout << "Create relation testRel1,test createRel function" << endl;
	_stat("testDB", &buf);
	ASSERT(buf.st_size==PAGESIZE);
	CALL(createRel("testDB","testRel1"));
	CALL(dbManager.closeDataBase("testDB"));
	_stat("testDB", &buf);
	ASSERT(buf.st_size==PAGESIZE*3);
	CALL(dbManager.openDataBase("testDB",db));
	CALL(destroyRel("testDB","testRel1"));
	_stat("testDB", &buf);
	ASSERT(buf.st_size==PAGESIZE*3);
	CALL(createRel("testDB","testRel1"));
	_stat("testDB", &buf);
	ASSERT(buf.st_size==PAGESIZE*3);
	cout << "Test 1 passed!" << endl << endl;
	//
	cout << "Insert " << DPICNT << " records into testRel1" << endl;
	tmpInert=new InsertOperator("testDB","testRel1",status);
	if(status!=OK)
	{
		error.print(status);
		return 0;
	}
	char rec1[PAGESIZE-DPFIXED-6];
	Record record1,tmpRec;
	AttrInPage attrs1[1];
	attrs1[0].setNull(false);
	attrs1[0].setLen(PAGESIZE-DPFIXED-6);
	record1.setAttrsInPage(attrs1,1);
	record1.setRealData(&rec1);
	for (i=0;i<(PAGESIZE-DPFIXED-6);i++) rec1[i]='a';
	for(i=0;i<DPICNT;i++)
		CALL(tmpInert->insertRecord(record1,newRid));
	cout << "Insert other 2"<<" records into testRel1" << endl;
	CALL(tmpInert->insertRecord(record1,newRid));
	CALL(tmpInert->insertRecord(record1,newRid));
	delete tmpInert;
	CALL(dbManager.closeDataBase("testDB"));
	_stat("testDB", &buf);
	ASSERT(buf.st_size==PAGESIZE*(DPICNT+5));
	CALL(dbManager.openDataBase("testDB",db));
	cout << "inserted "<<DPICNT+2<<" records into testRel1 successfully"<<endl;
	cout<<"Test1.1 passed!"<<endl<<endl;
	// scan the file sequentially checking that each record was stored properly
	cout << "scan file testRel1" << endl;
	scan1=new ScanOperator("testDB","testRel1",status);
	ASSERT(status ==OK);
	scan1->startScan(0,0,CHAR,NULL,EQ);
	i=0;
	while((status=scan1->scanNext(newRid))!=FILEEOF)
	{
		// reconstruct record i
		CALL(scan1->getRecord(tmpRec));
		ASSERT(memcmp(rec1,tmpRec.getRealData(),tmpRec.getRealLength())==0);
		i++;
    }
    ASSERT(status==FILEEOF);
	cout<<"scan testRel1 saw "<<i<<" records "<<endl;
	ASSERT(i==DPICNT+2);
	CALL(scan1->endScan());
	delete scan1;
	cout<<"Test1.2 passed!"<<endl<<endl;
	// initialize all of rec1.s to keep purify happy
	CALL(createRel("testDB","testRel2"));
	memset(data1.s,' ', sizeof(data1.s));
	cout << "Insert "<<num<< " records into testRel2" << endl;
	cout << "Start an insert scan which will have the side effect of opening testRel2" << endl;
	tmpInert=new InsertOperator("testDB","testRel2",status);
	AttrInPage attrs[3];
	attrs[0].setNull(false);
	attrs[0].setLen(SIZEOFINT);
	attrs[1].setNull(false);
	attrs[1].setLen(SIZEOFFLOAT);
	attrs[2].setNull(false);
	attrs[2].setLen(sizeof(data1.s));
	memset(data1.s,' ',sizeof(data1.s));
	for(i=0;i<num;i++)
	{
		sprintf(data1.s,"This is record %05d",i);
		data1.i=i;
		data1.f=i;
		CALL(dbrec1.setAttrsInPage(attrs,3));
		dbrec1.setRealData(&data1);
		status=tmpInert->insertRecord(dbrec1, newRid);
		// stash away rid and key of the record
		ridArray[i]=newRid;
		//printf("next rid (%d.%d)\n",ridArray[i].pageNo, ridArray[i].slotNo);
		if (status != OK)
		{
			cout << "got error status return from insert record"<<endl;
			cout << "inserted "<<i<<" records into testRel2 before error "<<endl;
			error.print(status);
			delete [] ridArray;
			exit(1);
		}
	}
	//
	delete tmpInert; // delete the scan, closing the file at the same time
	cout << "inserted " << num << " records into testRel2 successfully" << endl;
	cout << "Test 2 passed!" << endl << endl;
	// scan the file sequentially checking that each record was stored properly
	cout<< "scan testRel2" << endl;
	scan1=new ScanOperator("testDB","testRel2",status);
	ASSERT(status ==OK);
	scan1->startScan(0, 0, CHAR, NULL, EQ);
	i=0;
	memset(data1.s,' ', sizeof(data1.s));
	while((status=scan1->scanNext(rec2Rid))!=FILEEOF)
	{
		// reconstruct record i
		sprintf(data1.s,"This is record %05d",i);
		data1.i=i;
		data1.f=i;
		status=scan1->getRecord(dbrec2);
		ASSERT(status==OK);
		//cout<<((Data*)dbrec2.getRealData())->i<<((Data*)dbrec2.getRealData())->f<<((Data*)dbrec2.getRealData())->s<<endl;
		if(memcmp(&data1,dbrec2.getRealData(),sizeof(Data))!=0)
		{
			cout<<memcmp(&data1,dbrec2.getRealData(),sizeof(Data))<<endl;
			cout<<"got error status return from getting record"<<endl;
			cout<<"getting "<<i<<" records before error "<<endl;
			delete [] ridArray;
			delete scan1;
			delete bufMgr;
			CALL(dbManager.closeDataBase("testDB"));
			exit(1);
		}
		i++;
	}
	ASSERT(status==FILEEOF);
	cout<<"scan testRel2 saw "<<i<<" records "<<endl;
	ASSERT(i==num);
	// delete scan object
	scan1->endScan();
	delete scan1;
	scan1=NULL;
	cout<<"Test 3 passed!"<<endl<<endl;
	//
	// pull every 7th record from the file directly w/o opening a scan
	// by using the file->getRecord() method
	cout << "pull every 7th record from file table1 using file->getRecord() " << endl;
	RelOperator* ro1=new RelOperator("testDB","testRel2",status);
	ASSERT(status==OK);
	// get every 7th record
	memset(data1.s,' ', sizeof(data1.s));
	for (i=0;i<num;i=i+7)
	{
		// reconstruct record for comparison purposes
		sprintf(data1.s, "This is record %05d", i);
		data1.i = i;
		data1.f = i;
		// now read the record
		// printf("getting record (%d.%d)\n",ridArray[i].pageNo, ridArray[i].slotNo);
		status = ro1->getRecord(ridArray[i], dbrec2);
		if (status != OK)
		{
			error.print(status);
			exit(1);
		}
        // compare with what we should get back
		if (memcmp(&data1,dbrec2.getRealData(),sizeof(Data))!=0)
		{
			cout<<"error reading record "<<i<<" back"<<endl;
			exit(1);
		}
	}
	cout<<"getRecord() tests passed successfully"<<endl;
	delete ro1; // close the file
	delete [] ridArray;
	cout<<"Test 4 passed!"<<endl<<endl;
	//
	// next scan the file deleting all the odd records
	cout<<"scan table1 deleting all records whose i field is odd"<<endl;
	scan1=new ScanOperator("testDB","testRel2",status);
	ASSERT(status==OK);
	scan1->startScan(0, 0, CHAR, NULL, EQ);
	i=0;
	int deleted=0;
	while((status=scan1->scanNext(rec2Rid))!=FILEEOF)
	{
		ASSERT(status==OK);
		if((i%2)!=0)
		{
			//printf("deleting record %d with rid(%d.%d)\n",i,rec2Rid.pageNo, rec2Rid.slotNo);
			status=scan1->deleteRecord();
			if (status!=OK)  //&& ( status != NORECORDS))
			{
				cout<<"error status return from deleteRecord"<<endl;
				error.print(status);
				exit(1);
			}
			deleted++;
		}
		i++;
	}
	ASSERT(status==FILEEOF);
	cout<<"deleted "<<deleted << " records" << endl;
	if(deleted!=num/2)
	{ 
		cout<<"!!! should have deleted "<<num/2<<" records!"<<endl;
		exit(1);
	}
	scan1->endScan();
	delete scan1;
	scan1=NULL;
	cout<<"Test 5 passed!"<<endl<<endl;
	//
	cout<<"scan relation, counting number of remaining records"<<endl;
	scan1=new ScanOperator("testDB","testRel2",status);
	ASSERT(status==OK);
	i=0;
	scan1->startScan(0,0,CHAR,NULL,EQ);
	while((status=scan1->scanNext(rec2Rid))!=FILEEOF)
	{
		if (i>0) // don't delete the first one
		{
			status=scan1->deleteRecord();
			if((status!=OK)&&(status!=NORECORDS))
			{
				cout<<"error status return from deleteRecord"<<endl;
				error.print(status);
				exit(1);
			}
		}
		i++;
	}
    // subtract first record
	i--;
	ASSERT(status==FILEEOF);
	scan1->endScan();
	delete scan1;
	scan1=NULL;
	scan1=new ScanOperator("testDB","testRel2",status);
	ASSERT(status==OK);
	scan1->startScan(0, 0, CHAR, NULL, EQ);
	// delete the rest (should only be one)
	while ((status = scan1->scanNext(rec2Rid)) != FILEEOF)
	{
		status=scan1->deleteRecord();
		if ((status!=OK)&&(status!=NORECORDS))
		{
			cout << "error status return from deleteRecord" << endl;
			error.print(status);
			exit(1);
		}
		i++;
	}
	scan1->endScan();
	delete scan1;
	scan1=NULL;
	cout<<"scan saw "<<i<<" records "<<endl;
	if (i!=(num+1)/2)
	{
		cout << "!!! scan should have returned " << (num+1) / 2 << " records!" << endl;
		exit(1);
	}
	status=destroyRel("testDB","testRel2");
	if(status!=OK) 
	{
		cerr<<"got error status return from  destroyRel"<<endl;
		error.print(status);
		exit(1);
	}
	cout<<"Test 6 passed!"<<endl<<endl;
	//
	destroyRel("testDB","testRel2");
	status=createRel("testDB","testRel2");
	if (status != OK) 
	{
		cerr << "got error status return from  createHeapFile" << endl;
		error.print(status);
		exit(1);
	}
	tmpInert=new InsertOperator("testDB","testRel2", status);
	if (status != OK) 
	{
		cout << "got error status return from new insertFileScan call" << endl;
		error.print(status);
		exit(1);
	}
	cout << "insert " << num << " variable-size records into testRel2" << endl;
	int smallest, largest,rec1Len;
	attrs[0].setNull(false);
	attrs[0].setLen(SIZEOFINT);
	attrs[1].setNull(false);
	attrs[1].setLen(SIZEOFFLOAT);
	for(i=0;i<num;i++)
	{
		rec1Len=2+rand()%(sizeof(data1.s)-2);    // includes NULL!!
		attrs[2].setNull(false);
		attrs[2].setLen(rec1Len);
		//cout << "record length is " << rec1Len << endl;
		memset(data1.s,' ', sizeof(data1.s));
		memset((void *)data1.s,32+rec1Len,rec1Len-1);
		data1.s[rec1Len-1]=0;
		data1.i=i;
		data1.f=rec1Len;
		dbrec1.setAttrsInPage(attrs,3);
		dbrec1.setRealData(&data1);
		status=tmpInert->insertRecord(dbrec1, newRid);
		if (status!=OK)
		{
			cout<<i<<endl;
			cout<<"got error status return from insert record"<<endl;
			error.print(status);
			exit(1);
		}
		if (i==0||rec1Len<smallest) smallest=rec1Len;
		if (i==0||rec1Len>largest) largest=rec1Len;
	}
	cout<<"inserted "<<num<<" variable sized records successfully into testRel2"<<endl;
	cout<<"smallest record was "<<smallest+SIZEOFINT+SIZEOFFLOAT+sizeof(short)+3*sizeof(AttrInPage)<<" bytes, largest was "<<largest+sizeof(int)+sizeof(float)+sizeof(short)+3*sizeof(AttrInPage)<<" bytes"<<endl;
	delete tmpInert;
	cout<<"Test 7 passed!"<<endl<<endl;
	//
	cout << "scan testRel2" << endl;
	j=num/2;
	scan1=new ScanOperator("testDB","testRel2",status);
	ASSERT(status==OK);
	scan1->startScan(0,SIZEOFINT,INTEGER,(char*)&j,LT);
	i=0;
	while((status=scan1->scanNext(rec2Rid)) != FILEEOF)
	{
		CALL(scan1->getRecord(dbrec2));
		AttrInPage tmpAttr;
		dbrec2.getAttrInPage(2,tmpAttr);
		memcpy(&data2,dbrec2.getRealData(),dbrec2.getRealLength());
		if(data2.i>=j||data2.f!=tmpAttr.getAttrLen()||data2.s[0]!=32+tmpAttr.getAttrLen())
			cout<<"error reading record "<<i<<" back"<<endl;
		i++;
	}
	ASSERT(status==FILEEOF);
	cout<<"scan of testRel2 saw "<<i<<" records "<<endl;
	if(i!=num/2)
	{
		cout<<"!!! scan should have returned "<<num/2<<" records!"<<endl;
		exit(1);
	}
	scan1->endScan();
	delete scan1;
	scan1=NULL;
	cout<<"Test 8 passed!"<<endl<<endl;
	//
	cout<<"Next attempt two concurrent scans on testRel2"<<endl;
	int Ivalue=num/2;
	float Fvalue=0;
	scan1=new ScanOperator("testDB","testRel2",status);
	ASSERT(status==OK);
	status=scan1->startScan(0,SIZEOFINT,INTEGER,(char*)&Ivalue,LT);
	ASSERT(status==OK);
	scan2=new ScanOperator("testDB","testRel2",status);
	ASSERT(status==OK);
	status=scan2->startScan(1,SIZEOFFLOAT,FLOAT,(char*)&Fvalue,GTE);
	ASSERT(status==OK);
	int count=0;
	for(i=0;i<num;i++)
	{
		status=scan1->scanNext(rec2Rid);
		if(status==OK)
			count++;
		else if(status!=FILEEOF)
		{
			error.print(status);
			exit(1);
		}
		status=scan2->scanNext(rec2Rid);
		if(status==OK)
			count++;
		else if(status!=FILEEOF)
		{
			error.print(status);
			exit(1);
		}
	}
	cout<<"scan testRel2 saw "<<count<<" records "<<endl;
	if(count != num/2 + num)
	{
		cout<<"!!! scan should have returned "<<num/2+num<<" records!"<<endl;
		exit(1);
	}
	if(scan1->scanNext(rec2Rid)!=FILEEOF)
	{
		cout<<"!!! scan past end of file did not return FILEEOF!"<<endl;
		exit(1);
	}
	if(scan2->scanNext(rec2Rid)!=FILEEOF)
	{
		cout<<"!!! scan past end of file did not return FILEEOF!"<<endl;
		exit(1);
	}
	delete scan1;
	delete scan2;
	scan1=scan2=NULL;
	cout<<"Destroy testRel2"<<endl;
	CALL(destroyRel("testDB","testRel2"));
	cout<<"Test 9 passed!"<<endl<<endl;
	//
	destroyRel("testDB","testRel3");
	status=createRel("testDB","testRel3");
	if(status!=OK) 
	{
		cerr<<"got error status return from  createHeapFile" << endl;
		error.print(status);
		exit(1);
	}
	tmpInert=new InsertOperator("testDB","testRel3",status);
	if (status!=OK) 
	{
		cerr<<"got error status return from new insertFileScan" << endl;
		error.print(status);
		exit(1);
	}
	cout <<"inserting " << num << " records into testRel3" << endl;
	attrs[0].setNull(false);
	attrs[0].setLen(SIZEOFINT);
	attrs[1].setNull(false);
	attrs[1].setLen(SIZEOFFLOAT);
	attrs[2].setNull(false);
	attrs[2].setLen(sizeof(data1.s));
	for(i=0;i<num;i++)
	{
		memset(data1.s,' ',sizeof(data1.s));
		sprintf(data1.s,"This is record %05d",i);
		data1.i=i;
		data1.f=i;
		CALL(dbrec1.setAttrsInPage(attrs,3));
		dbrec1.setRealData(&data1);
		status=tmpInert->insertRecord(dbrec1, newRid);
		if (status != OK) 
		{
			cout<<i<<endl;
			cout << "got error status return from insertrecord" << endl;
			error.print(status);
			exit(1);
		}
	}
	delete tmpInert;
    cout << "Test 10 passed!" << endl << endl;
	//
	int numDeletes = 0;
	cout << endl;
	cout << "now scan testRel3, deleting records with keys between 1000 and 2000" << endl;
	scan1=new ScanOperator("testDB","testRel3",status);
	ASSERT(status == OK);
	status = scan1->startScan(0, 0, CHAR, NULL, EQ);
	ASSERT(status == OK);
	i = 0;
	while ((status = scan1->scanNext(rec2Rid)) != FILEEOF)
	{
		// reconstruct record i
		memset(data1.s,' ',sizeof(data1.s));
		sprintf(data1.s, "This is record %05d", i);
		data1.i = i;
		data1.f = i;
		status = scan1->getRecord(dbrec2);
		if (status != OK) break;
		if (memcmp(&data1, dbrec2.getRealData(),dbrec2.getRealLength()) != 0)
		{ 
			cout << "error reading record " << i << " back" << endl;
			exit(1);
		}
		if ((i > 1000) && ( i <= 2000))
		{
			status = scan1->deleteRecord();
			if ((status != OK) && ( status != NORECORDS))
			{
				cout << "error status return from deleteRecord" << endl;
				error.print(status);
				exit(1);
			}
			else
				numDeletes++;
		}
		i++;
	}
	ASSERT(status == FILEEOF);
	cout << "scan file1 saw " << i << " records " << endl;
	if (i != num)
	{
		cout << "Error. scan should have returned " << (int) num << " records!!" << endl;
		exit(1);
	}
	cout << "number of records deleted by scan " << numDeletes << endl;
	if (numDeletes != 1000)
	{
		cout << "Error. Should have deleted 1000 records!!!" << endl;
		exit(1);
	}
	delete scan1;
	// rescan.  should see 1000 fewer records
	scan1=new ScanOperator("testDB","testRel3",status);
	ASSERT(status == OK);
	scan1->startScan(0, 0, CHAR, NULL, EQ);
	ASSERT(status == OK);
	i = 0;
	while ((status = scan1->scanNext(rec2Rid)) != FILEEOF)
		i++;
	cout << "should have seen 1000 fewer records after deletions" << endl;
	cout << "saw " << i << " records" << endl;
	delete scan1;
	cout << "Test 11 passed!" << endl << endl;
	//
	// perform filtered scan #1
	scan1=new ScanOperator("testDB","testRel3",status);
	ASSERT(status == OK);
	int filterVal1 = num * 3 / 4;
	cout << "Filtered scan matching i field GTE than " << filterVal1 << endl;
	status = scan1->startScan(0, SIZEOFINT, INTEGER, (char*)&filterVal1,GTE);
	ASSERT(status == OK);
	i = 0;
	while ((status = scan1->scanNext(rec2Rid)) != FILEEOF)
	{
        // reconstruct record i
		status = scan1->getRecord(dbrec2);
		if (status != OK) break;
		// verify that the scan predicate was actually met
		Data *currRec = (Data*)dbrec2.getRealData();
		if (! (currRec->i >= filterVal1))
		{
			cerr << "!!! filtered scan returned record that doesn't satisfy predicate " << "i val is " << currRec->i << endl;
			exit(1);
		}            
		i++;
	}
	ASSERT(status == FILEEOF);
	cout << "scan file1 saw " << i << " records " << endl;
	if (i != num/4)
	{
		cout << "!!! scan should have returned " << num << " records!" << endl;
		exit(1);
	}
	delete scan1;
	cout << "Test 12 passed!" << endl << endl;
	//
	// perform filtered scan #2
	scan1=new ScanOperator("testDB","testRel3",status);
	ASSERT(status == OK);
	float filterVal2 = num * 9 / 10;
	cout << "Filtered scan matching f field GT than " << filterVal2 << endl;
	status = scan1->startScan(1, SIZEOFFLOAT, FLOAT, (char*)&filterVal2, GT);
	ASSERT(status == OK);
	i = 0;
	while ((status = scan1->scanNext(rec2Rid)) != FILEEOF)
	{
		// reconstruct record i
		status = scan1->getRecord(dbrec2);
		if (status != OK) break;
		// verify that the scan predicate was actually met
		Data *currRec=(Data*)dbrec2.getRealData();
		if (! (currRec->f > filterVal2))
		{
			cerr << "!!! filtered scan returned record that doesn't satisfy predicate "
				<< "f val is " << currRec->f << endl;
			exit(1);
		}
		i++;
	}
	if (status != FILEEOF) error.print(status);
	cout << "scan table3 saw " << i << " records " << endl;
	if (i != num/10-1)
	{
		cout << "!!! filtered scan 2 should have returned " << num/10-1 << " records!" << endl;
		exit(1);
	}
	delete scan1;
	cout << "Test 13 passed!" << endl << endl;
	//
	// open up the heapFile
	RelOperator* rop=new RelOperator("testDB","testRel3",status);
	ASSERT(status == OK);
	delete rop;
	// cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
	// bufMgr->clearBufStats();
	cout << "Call startScan with wrong parameter" << endl;
	scan1 = new ScanOperator("testDB","testRel3",status);
	ASSERT(status == OK);
	status = scan1->startScan(0, 20, INTEGER, "Hi" , EQ);
	if ( status == BADSCANPARM)
		cout << "passed BADSCANPARAM test" << endl;
	else
	{
		cout << "should have returned BADSCANPARM, actually returned: " << endl;
		error.print(status);
		exit(1);
	}
	cout << "Test 14 passed!" << endl << endl;
	//
	delete bufMgr;
	CALL(dbManager.closeDataBase("testDB"));
	cout<<sizeof(BufDesc)<<endl;
	return 1;
}
/*tm setDate;
	setDate.tm_year=1990;
	setDate.tm_mon=1;
	setDate.tm_mday=2;
	time_t ltime=mktime(&setDate);
	printf( "The time is %s\n", ctime( &ltime ) );
	cout<<sizeof(ltime)<<endl;
	cout<<sizeof(setDate)<<endl;*/
	//cout<<sizeof(double)<<endl;
	//unsigned long *tmpPAGESIZE=const_cast<unsigned long*>(&(PAGESIZE));
	//*tmpPAGESIZE=8192;

