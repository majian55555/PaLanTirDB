#include "newIO.h"
#include <iostream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <functional>
#include <io.h>
#include <errno.h>
#include <stdio.h>
//#define DHP(p)  (*(DBHeadPage*)&p)
#define CALL(c)    { Status s; \
                     if ((s = c) != OK) \
					 { \
                       cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                       error.print(s); \
                       cerr << "TEST DID NOT PASS" <<endl; \
                       exit(1); \
                     } \
                   }
#define FAIL(c)  { Status s; \
                   if ((s = c) == OK) \
				   { \
                     cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                     cerr << "This call should fail: " #c << endl; \
                     cerr << "TEST DID NOT PASS" <<endl; \
                     exit(1); \
                   } \
                  }
void main()
{
	Error error;
	DBManager* dbm1=new DBManager;
	DB *db1,*db2,*db3;
	std::string dbName("testDB0");
	//1:test creating,opening,closing and distroing a test file
	cout<<"test1:create a test file..."<<endl;
	dbm1->destroyDataBase(dbName);
	CALL(dbm1->createDataBase(dbName));
	ASSERT(dbm1->createDataBase(dbName)==FILEEXISTS);
	ASSERT(dbm1->closeDataBase(dbName)==FILENOTOPEN);
	CALL(dbm1->openDataBase(dbName,db1));
	CALL(dbm1->closeDataBase(dbName));
	CALL(dbm1->destroyDataBase(dbName));
	ASSERT(dbm1->openDataBase(dbName,db1)==FILENOTEXISTS);
	CALL(dbm1->createDataBase(dbName));
	cout<<"test1 passed."<<endl;
	//2:test first DBHeadPage
	cout<<"test2:test the first DBHeadPage..."<<endl;
	CALL(dbm1->openDataBase(dbName,db1));
	ASSERT(db1->getOpenCnt()==1);
	const DBHeadPage* dbHeadPage=NULL;
	ASSERT(db1->getFirstDBHP(dbHeadPage)==OK);
	ASSERT(dbHeadPage->getDBName()==dbName);
	//dbHeadPage->setNextFreePage(9);
	ASSERT(dbHeadPage->isEmpty());
	ASSERT(dbHeadPage->getNextDBHPNo()==0);
	ASSERT(dbHeadPage->getNextFreePage()==0);
	ASSERT(dbHeadPage->getPageCnt()==1);
	ASSERT(dbHeadPage->getRelCnt()==0);
	CALL(dbm1->closeDataBase(dbName));
	cout<<"Test2 passed."<<endl;
	//3:test allocatePage and disposePage
	cout<<"test3:test allocatePage and disposePage..."<<endl;
	CALL(dbm1->openDataBase(dbName,db1));
	unsigned long newPageNo;
	CALL(db1->allocatePage(newPageNo));
	ASSERT(newPageNo==1);
	CALL(db1->disposePage(newPageNo));
	ASSERT(db1->getFirstDBHP(dbHeadPage)==OK);
	ASSERT(dbHeadPage->getPageCnt()==2);
	ASSERT(dbHeadPage->getNextFreePage()==1);
	CALL(db1->allocatePage(newPageNo));
	ASSERT(newPageNo==1);
	CALL(db1->allocatePage(newPageNo));
	ASSERT(newPageNo==2);
	CALL(db1->allocatePage(newPageNo));
	ASSERT(newPageNo==3);
	CALL(db1->disposePage(3));
	CALL(db1->disposePage(2));
	CALL(db1->allocatePage(newPageNo));
	ASSERT(newPageNo==2);
	CALL(db1->allocatePage(newPageNo));
	ASSERT(newPageNo==3);
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getPageCnt()==4);
	ASSERT(dbHeadPage->getNextFreePage()==0);
	CALL(dbm1->closeDataBase(dbName));
	cout<<"Test3 passed."<<endl;
	//4:test reading and writing pages
	cout<<"test4:test reading and writing pages..."<<endl;
	CALL(dbm1->openDataBase(dbName,db1));
	Page testPage1,testPage2;
	memset(&testPage1,0,PAGESIZE);
	memset(&testPage2,0,PAGESIZE);
	sprintf((char*)&testPage1,"pageNo1");
	CALL(db1->writePage(1,&testPage1));
	sprintf((char*)&testPage1,"pageNo2");
	CALL(db1->writePage(2,&testPage1));
	sprintf((char*)&testPage1,"pageNo3");
	CALL(db1->writePage(3,&testPage1));
	sprintf((char*)&testPage1,"pageNo1");
	CALL(db1->readPage(1,&testPage2));
	ASSERT(memcmp(&testPage2, &testPage1, PAGESIZE) == 0);
	sprintf((char*)&testPage1,"pageNo2");
	CALL(db1->readPage(2,&testPage2));
	ASSERT(memcmp(&testPage2, &testPage1, PAGESIZE) == 0);
	sprintf((char*)&testPage1,"pageNo3");
	CALL(db1->readPage(3,&testPage2));
	ASSERT(memcmp(&testPage2, &testPage1, PAGESIZE) == 0);
	ASSERT(db1->readPage(4,&testPage1)==BADPAGENO);
	ASSERT(db1->writePage(4,&testPage1)==BADPAGENO);
	CALL(dbm1->closeDataBase(dbName));
	cout<<"Test4 passed."<<endl;
	//5:test creating relation
	cout<<"test5:test creating relation..."<<endl;
	CALL(dbm1->openDataBase(dbName,db1));
	std::string relName("testRel");
	CALL(db1->createRel(relName));
	ASSERT(db1->getFirstDBHP(dbHeadPage)==OK);
	ASSERT(dbHeadPage->isEmpty()==false);
	ASSERT(dbHeadPage->getPageCnt()==5);
	ASSERT(dbHeadPage->getRelCnt()==1);
	unsigned long relFirstPageNo;
	CALL(dbHeadPage->getRelFirstPage(relName,relFirstPageNo));
	ASSERT(relFirstPageNo==4);
	ASSERT(db1->createRel(relName)==RELEXIST);
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getRelCnt()==1);
	ASSERT(dbHeadPage->getPageCnt()==5);
	int i;
	char str[10];
	for(i=0;i<RELCNT-1;i++)
	{
		relName="testRel";
		itoa(i,str,10);
		relName+=str;
		CALL(db1->createRel(relName));
	}
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getPageCnt()==116);
	ASSERT(dbHeadPage->getNextFreePage()==0);
	relName="testRel";
	itoa(i,str,10);
	relName+=str;
	CALL(db1->createRel(relName));
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getPageCnt()==118);
	ASSERT(dbHeadPage->getRelCnt()==113);
	ASSERT(dbHeadPage->getNextDBHPNo()==116);
	DBHeadPage page1;
	unsigned long pageNo;
	CALL(db1->readPage(116,(Page*)&page1));
	CALL(page1.getRelFirstPage(relName,pageNo));
	ASSERT(pageNo==117);
	//test get relation first page number
	relName="testRel";
	CALL(db1->getRelFirstPageNo(relName,pageNo));
	ASSERT(pageNo=4);
	for(i=0;i<RELCNT-1;i++)
	{
		relName="testRel";
		itoa(i,str,10);
		relName+=str;
		CALL(db1->getRelFirstPageNo(relName,pageNo));
		ASSERT(pageNo=i+5);
	}
	relName="testRel";
	itoa(i,str,10);
	relName+=str;
	CALL(db1->getRelFirstPageNo(relName,pageNo));
	ASSERT(pageNo=i+5);
	//
	CALL(dbm1->closeDataBase(dbName));
	cout<<"Test5 passed."<<endl;
	//6:test updating relation
	cout<<"test6:test updating relation..."<<endl;
	CALL(dbm1->openDataBase(dbName,db1));
	CALL(db1->updataRel("testRel","newRel1"));
	CALL(db1->updataRel(relName,"newRel2"));
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getPageCnt()==118);
	ASSERT(dbHeadPage->getRelCnt()==113);
	ASSERT(dbHeadPage->getNextDBHPNo()==116);
	CALL(dbHeadPage->getRelFirstPage("newRel1",relFirstPageNo));
	ASSERT(relFirstPageNo=4);
	CALL(db1->readPage(116,(Page*)&page1));
	CALL(page1.getRelFirstPage("newRel2",pageNo));
	ASSERT(pageNo==117);
	CALL(db1->updataRel("newRel1","testRel"));
	CALL(db1->updataRel("newRel2",relName));
	ASSERT(db1->updataRel("newRel3",relName)==RELNOTFOUND);
	CALL(dbm1->closeDataBase(dbName));
	cout<<"Test6 passed."<<endl;
	//7:test distroing relation
	cout<<"test7:test distroy relation..."<<endl;
	CALL(dbm1->openDataBase(dbName,db1));
	relName="testRel";
	CALL(db1->destroyRel(relName));
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getPageCnt()==118);
	ASSERT(dbHeadPage->getRelCnt()==112);
	ASSERT(dbHeadPage->getNextDBHPNo()==116);
	CALL(db1->readPage(116,(Page*)&page1));
	ASSERT(page1.isEmpty()==false);
	for(i=0;i<RELCNT-1;i++)
	{
		relName="testRel";
		itoa(i,str,10);
		relName+=str;
		//cout<<str<<endl;
		CALL(db1->destroyRel(relName));
	}
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getPageCnt()==118);
	ASSERT(dbHeadPage->getNextFreePage()==115);
	CALL(db1->readPage(116,(Page*)&page1));
	ASSERT(page1.isEmpty()==false);
	relName="testRel";
	itoa(i,str,10);
	relName+=str;
	//cout<<str<<endl;
	CALL(db1->destroyRel(relName));
	CALL(db1->getFirstDBHP(dbHeadPage));
	ASSERT(dbHeadPage->getPageCnt()==118);
	ASSERT(dbHeadPage->getRelCnt()==0);
	ASSERT(dbHeadPage->isEmpty()==true);
	ASSERT(dbHeadPage->getNextDBHPNo()==116);
	CALL(db1->readPage(116,(Page*)&page1));
	ASSERT(page1.isEmpty()==true);
	CALL(dbm1->closeDataBase(dbName));
	cout<<"Test7 passed."<<endl;
	//8:test opening multi-databases
	cout<<"test8:test opening multi-databases..."<<endl;
	dbm1->destroyDataBase("testDB1");
	dbm1->destroyDataBase("testDB2");
	dbm1->destroyDataBase("testDB3");
	CALL(dbm1->createDataBase("testDB1"));
	CALL(dbm1->createDataBase("testDB2"));
	CALL(dbm1->createDataBase("testDB3"));
	CALL(dbm1->openDataBase("testDB1",db1));
	CALL(dbm1->openDataBase("testDB1",db2));
	ASSERT(db1==db2);
	CALL(dbm1->openDataBase("testDB2",db2));
	CALL(dbm1->openDataBase("testDB3",db3));
	CALL(dbm1->closeDataBase("testDB1"));
	CALL(dbm1->closeDataBase("testDB2"));
	CALL(dbm1->closeDataBase("testDB3"));
	ASSERT(dbm1->destroyDataBase("testDB1")==FILEOPEN);
	CALL(dbm1->closeDataBase("testDB1"));
	CALL(dbm1->destroyDataBase("testDB1"));
	CALL(dbm1->destroyDataBase("testDB2"));
	CALL(dbm1->destroyDataBase("testDB3"));
	cout<<"Test8 passed."<<endl;
	//9:test ~DBManager()
	cout<<"test9:test ~DBManager()..."<<endl;
	CALL(dbm1->createDataBase("testDB1"));
	CALL(dbm1->createDataBase("testDB2"));
	CALL(dbm1->createDataBase("testDB3"));
	CALL(dbm1->openDataBase("testDB1",db1));
	CALL(dbm1->openDataBase("testDB1",db1));
	CALL(dbm1->openDataBase("testDB2",db2));
	CALL(dbm1->openDataBase("testDB3",db3));
	delete dbm1;
	fstream *fs1=new fstream;
	fs1->open("testDB1",ios::in|ios::out|ios::nocreate|ios::binary,filebuf::sh_none);
	ASSERT(fs1->fail()==0);
	fs1->close();
	delete fs1;
	cout<<"Test9 passed."<<endl;
	//10:test other programs attempt to open DB
	cout<<"test10:test other programs attempt to open DB..."<<endl;
	dbm1=new DBManager;
	ASSERT(dbm1!=NULL);
	CALL(dbm1->openDataBase("testDB1",db1));
	fs1=new fstream;
	fs1->open("testDB1",ios::in|ios::out|ios::nocreate|ios::binary);
	ASSERT(fs1->fail()!=0);
	CALL(dbm1->closeDataBase("testDB1"));
	CALL(dbm1->destroyDataBase("testDB1"));
	CALL(dbm1->destroyDataBase("testDB2"));
	CALL(dbm1->destroyDataBase("testDB3"));
	cout<<"Test10 passed."<<endl;
	cout<<"All tests successfully passed!!!"<<endl;
}