#include "newPage.h"
#include <iostream.h>
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
int main()
{
	Error       error;
	Page        page1;
	char        rec1[33];
	char        rec2[58];
	char        rec3[71];
	char        rec4[100];
	char        rec5[4000];
	char        rec3_new[71];
	Record record1, record2, record3, record, record4, record5;
	Record record3_new;
	Status      status;
	RID         rid1, rid2, rid3, rid4, rid5, tmpRid1, tmpRid2;
	int         i;
	AttrInPage attrs[1];
	attrs[0].setNull(false);
	attrs[0].setLen(33);
	record1.setAttrsInPage(attrs,1);
	record1.setRealData(&rec1);
	attrs[0].setNull(false);
	attrs[0].setLen(58);
	record2.setAttrsInPage(attrs,1);
	record2.setRealData(&rec2);
	attrs[0].setNull(false);
	attrs[0].setLen(71);
	record3.setAttrsInPage(attrs,1);
	record3.setRealData(&rec3);
	attrs[0].setNull(false);
	attrs[0].setLen(100);
	record4.setAttrsInPage(attrs,1);
	record4.setRealData(&rec4);
	attrs[0].setNull(false);
	attrs[0].setLen(4000);
	record5.setAttrsInPage(attrs,1);
	record5.setRealData(&rec5);
	attrs[0].setNull(false);
	attrs[0].setLen(71);
	record3_new.setAttrsInPage(attrs,1);
	record3_new.setRealData(&rec3_new);
	for (i=0;i<33;i++) rec1[i]='a';
	for (i=0;i<58;i++) rec2[i]='b';
	for (i=0;i<71;i++) rec3[i]='c';
	for (i=0;i<100;i++) rec4[i]='d';
	for (i=0;i<4000;i++) rec5[i]='e';
	for (i=0;i<71;i++) rec3_new[i]='X';
	// init the data page
	page1.init(1);
	ASSERT(page1.getFreePtr()==0);
	ASSERT(page1.getFreeSpace()==4076);
	ASSERT(page1.getSlotCnt()==0)
	//page1->dumpPage();
	// insert 3 records
	printf ("inserting 3 records of lengths 33, 58, 71\n");
	CALL(page1.insertRecord(record1, rid1));
	ASSERT(page1.getFreePtr()==record1.getWholeLength());
	ASSERT(page1.getFreeSpace()==4033);
	ASSERT(page1.getSlotCnt()==-1);
	ASSERT(rid1.getPageNo()==1);
	ASSERT(rid1.getSlotNo()==0);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	CALL(page1.getRecord(rid1,record));
	ASSERT(memcmp(record.getRealData(),rec1,record.getRealLength())==0);
	//page1->dumpPage();
	CALL(page1.insertRecord(record2, rid2));
	ASSERT(page1.getFreePtr() == record1.getWholeLength()+record2.getWholeLength());
	ASSERT(page1.getFreeSpace() == 3965);
	ASSERT(page1.getSlotCnt() == -2);
	ASSERT(rid2.getPageNo()==1);
	ASSERT(rid2.getSlotNo()==1);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==39);
	ASSERT(page1.getLength(-1)==64);
	CALL(page1.getRecord(rid2,record));
	ASSERT(memcmp(record.getRealData(),rec2,record.getRealLength())==0);
	CALL(page1.insertRecord(record3, rid3));
	ASSERT(page1.getFreePtr() == record1.getWholeLength()+record2.getWholeLength()+record3.getWholeLength());
	ASSERT(page1.getFreeSpace() == 3884);
	ASSERT(page1.getSlotCnt() == -3);
	ASSERT(rid3.getPageNo()==1);
	ASSERT(rid3.getSlotNo()==2);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==39);
	ASSERT(page1.getLength(-1)==64);
	ASSERT(page1.getOffset(-2)==103);
	ASSERT(page1.getLength(-2)==77);
	CALL(page1.getRecord(rid3,record));
	ASSERT(memcmp(record.getRealData(),rec3,record.getRealLength())==0);
	cout << "Test 1 Passed!" << endl;
	printf("\nNow, scan the records on the page\n");
	// try scanning records on the page
	CALL(page1.firstRecord(tmpRid1));
	ASSERT(tmpRid1.getPageNo()==1);
	ASSERT(tmpRid1.getSlotNo()==0);
	for(i=0;i<3;i++)
	{
		// get a record
		CALL(page1.getRecord(tmpRid1,record));
		char tmpChar[100];
		strncpy(tmpChar,(char*)record.getRealData(),record.getRealLength());
		tmpChar[record.getRealLength()]='\0';
		cout<<"Record "<<i<<":"<<tmpChar<<endl;
		// get rid of next record
		status=page1.nextRecord(tmpRid1,tmpRid2);
		if(i==2)
		{
			ASSERT(status==ENDOFPAGE);
			break;
		}
		ASSERT(status==OK);
		tmpRid1=tmpRid2;
	}
	cout<<"Test 2 Passed!"<<endl;
	// delete the middle record
	printf("\ndelete middle record with rid2=%lu.%hu\n", rid2.getPageNo(), rid2.getSlotNo());
	CALL(page1.deleteRecord(rid2));
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==0);
	ASSERT(page1.getLength(-1)==0);
	ASSERT(page1.getOffset(-2)==39);
	ASSERT(page1.getLength(-2)==77);
	ASSERT(page1.getFreePtr()==record1.getWholeLength()+record3.getWholeLength());
	ASSERT(page1.getFreeSpace()==3952);
	ASSERT(page1.getSlotCnt()==-3);
	cout<<"Test 3 Passed!"<<endl;
	// insert record 4 with length 100
	printf("\ninsert the record 4 with length 100\n");
	CALL(page1.insertRecord(record4, rid4));
	ASSERT(page1.getFreePtr()==222);
	ASSERT(page1.getFreeSpace()==3842);
	ASSERT(page1.getSlotCnt()==-3);
	ASSERT(rid4.getPageNo()==1);
	ASSERT(rid4.getSlotNo()==1);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==116);
	ASSERT(page1.getLength(-1)==106);
	ASSERT(page1.getOffset(-2)==39);
	ASSERT(page1.getLength(-2)==77);
	CALL(page1.getRecord(rid4,record));
	ASSERT(memcmp(record.getRealData(),rec4,record.getRealLength())==0);
    cout << "Test 4 Passed!" << endl;
	// try to insert a very large record
	printf("\ntry to insert a record that should not fit in the space left\n");
	FAIL(status=page1.insertRecord(record5,rid5));
	ASSERT(page1.getFreePtr()==222);
	ASSERT(page1.getFreeSpace()==3842);
	ASSERT(page1.getSlotCnt()==-3);
	ASSERT(rid5.getPageNo()==0);
	ASSERT(rid5.getSlotNo()==0);
	printf("status=%d,rid returned = %lu.%hu\n",status,rid5.getPageNo(),rid5.getSlotNo());
	ASSERT(status==NOSPACE)
	cout<<"correctly identified that there isn't enough space"<<endl;
	// get the amount of space left on the page
	unsigned long freeSpace=page1.getFreeSpace();
	printf("%lu bytes left on the page\n",freeSpace);
    cout << "Test 5 Passed!" << endl;
	// shorten the record to fit and try again
	attrs[0].setNull(false);
	attrs[0].setLen(freeSpace-sizeof(AttrInPage)-sizeof(unsigned short));
	record5.setAttrsInPage(attrs,1);
	record5.setRealData(&rec5);
	ASSERT(record5.getWholeLength()==freeSpace);
	ASSERT(record5.getWholeLength()==3842);
	printf("\ntrying again with a large record that should fit exactly \n");
	CALL(status=page1.insertRecord(record5,rid5));
	ASSERT(page1.getFreePtr()==4064);
	ASSERT(page1.getFreeSpace()==0);
	ASSERT(page1.getSlotCnt()==-4);
	ASSERT(rid5.getPageNo()==1);
	ASSERT(rid5.getSlotNo()==3);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==116);
	ASSERT(page1.getLength(-1)==106);
	ASSERT(page1.getOffset(-2)==39);
	ASSERT(page1.getLength(-2)==77);
	ASSERT(page1.getOffset(-3)==222);
	ASSERT(page1.getLength(-3)==(unsigned short)record5.getWholeLength());
	CALL(page1.getRecord(rid5,record));
	ASSERT(memcmp(record.getRealData(),rec5,record.getRealLength())==0);
	printf("status=%d, rid5 returned = %lu.%hu\n",status,rid5.getPageNo(),rid5.getSlotNo());
	cout<<"Accturelly, we have 4 records now, since we have deleted one"<<endl;
	cout<<"Test 6 Passed!"<<endl;
	//
	cout << endl << "Delete all the records. next the end one" << endl;
	printf("delete rid3=%lu.%hu\n",rid3.getPageNo(),rid3.getSlotNo());
	CALL(page1.deleteRecord(rid3));
	ASSERT(page1.getFreePtr()==3987);
	ASSERT(page1.getFreeSpace()==77);
	ASSERT(page1.getSlotCnt()==-4);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==39);
	ASSERT(page1.getLength(-1)==106);
	ASSERT(page1.getOffset(-2)==0);
	ASSERT(page1.getLength(-2)==0);
	ASSERT(page1.getOffset(-3)==145);
	ASSERT(page1.getLength(-3)==record5.getWholeLength());
	// delete rec4
	printf("delete rid4=%lu.%hu\n", rid4.getPageNo(), rid4.getSlotNo());
	CALL(page1.deleteRecord(rid4));
	ASSERT(page1.getFreePtr()==3881);
	ASSERT(page1.getFreeSpace()==183);
	ASSERT(page1.getSlotCnt()==-4);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==0);
	ASSERT(page1.getLength(-1)==0);
	ASSERT(page1.getOffset(-2)==0);
	ASSERT(page1.getLength(-2)==0);
	ASSERT(page1.getOffset(-3)==39);
	ASSERT(page1.getLength(-3)==record5.getWholeLength());
	printf("delete rid5=%lu.%hu\n", rid5.getPageNo(), rid5.getSlotNo());
	CALL(page1.deleteRecord(rid5));
	ASSERT(page1.getFreePtr()==39);
	//cout<<page1.getFreeSpace()<<endl;
	ASSERT(page1.getFreeSpace()==4033);
	ASSERT(page1.getSlotCnt()==-1)
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==0);
	ASSERT(page1.getLength(-1)==0);
	ASSERT(page1.getOffset(-2)==0);
	ASSERT(page1.getLength(-2)==0);
	ASSERT(page1.getOffset(-3)==0);
	ASSERT(page1.getLength(-3)==0);
    // then the first one
	printf("delete rid1=%lu.%hu\n", rid1.getPageNo(), rid1.getSlotNo());
	CALL(page1.deleteRecord(rid1));
	ASSERT(page1.getFreePtr()==0);
	ASSERT(page1.getFreeSpace()==4076);
	ASSERT(page1.getSlotCnt()==0);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==0);
	ASSERT(page1.getOffset(-1)==0);
	ASSERT(page1.getLength(-1)==0);
	ASSERT(page1.getOffset(-2)==0);
	ASSERT(page1.getLength(-2)==0);
	ASSERT(page1.getOffset(-3)==0);
	ASSERT(page1.getLength(-3)==0);
	//page1->dumpPage();
	FAIL(status=page1.deleteRecord(rid1));
	ASSERT(status==INVALIDSLOTNO);
	FAIL(status=page1.firstRecord(rid1));
	ASSERT(status==NORECORDS);
	FAIL(status=page1.getRecord(rid1,record1));
	ASSERT(status==INVALIDSLOTNO);
    cout<<"Test 7 Passed!"<<endl;
	//
	cout<<endl<<"Insert two records again"<<endl;
	CALL(page1.insertRecord(record1,rid1));
	ASSERT(page1.getFreePtr()==39);
	ASSERT(page1.getFreeSpace()==4033);
	ASSERT(page1.getSlotCnt()==-1);
	ASSERT(rid1.getPageNo()==1);
	ASSERT(rid1.getSlotNo()==0);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	CALL(page1.insertRecord(record3,rid3));
	ASSERT(page1.getFreePtr()==116);
	ASSERT(page1.getFreeSpace()==3952);
	ASSERT(page1.getSlotCnt()==-2);
	ASSERT(rid3.getPageNo()==1);
	ASSERT(rid3.getSlotNo()==1);
	ASSERT(page1.getOffset(0)==0);
	ASSERT(page1.getLength(0)==39);
	ASSERT(page1.getOffset(-1)==39);
	ASSERT(page1.getLength(-1)==77);
	cout<<"Test 8 Passed!"<<endl;
	//
	cout<<endl<<"Update the second records"<<endl;
	CALL(page1.updateRecord(record3_new,rid3));
	CALL(page1.getRecord(rid3,record));
	ASSERT(memcmp(record.getRealData(),rec3_new,record.getRealLength())==0);
	ASSERT(page1.getFreeSpace()==3952);
	ASSERT(page1.getFreePtr()==116);
	CALL(page1.updateRecord(record4,rid3));
	CALL(page1.getRecord(rid3,record));
	ASSERT(memcmp(record.getRealData(),rec4,record.getRealLength())==0);
	ASSERT(page1.getFreeSpace()==3923);
	ASSERT(page1.getFreePtr()==145);
	CALL(page1.updateRecord(record3_new,rid3));
	CALL(page1.getRecord(rid3,record));
	ASSERT(memcmp(record.getRealData(),rec3_new,record.getRealLength())==0);
	ASSERT(page1.getFreeSpace()==3952);
	ASSERT(page1.getFreePtr()==116);
	cout<<"Test 9 Passed!"<<endl;
	//
	cout<<endl<<"Get attributes in page"<<endl;
	CALL(page1.getRecord(rid1,record));
	ASSERT(record.getAttrsNum()==1);
	ASSERT(record.isLargeRec()==false);
	AttrInPage tempAttr;
	CALL(record.getAttrInPage(0,tempAttr));
	ASSERT(tempAttr.getAttrLen()==33);
	ASSERT(tempAttr.isNull()==false);
	ASSERT(tempAttr.isLOB()==false);
	CALL(page1.getRecord(rid3,record));
	ASSERT(record.getAttrsNum()==1);
	ASSERT(record.isLargeRec()==false);
	CALL(record.getAttrInPage(0,tempAttr));
	ASSERT(tempAttr.getAttrLen()==71);
	ASSERT(tempAttr.isNull()==false);
	ASSERT(tempAttr.isLOB()==false);
	cout<<"Test 10 Passed!"<<endl;
	cout<<endl<<"Pass All Tests!"<<endl;
	return 1;
}
