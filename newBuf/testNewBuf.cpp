#define HASASSERT
#include "newBuf.h"
#include <memory>
#include <cstdlib>
#define ASSERT(c)  { if (!(c)) { \
                       cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                       cerr << "This condition should hold: " #c << endl; \
					   delete bufMgr; \
                       exit(1); \
                     } \
                   }
#define CALL(c)    { Status s; \
                     if ((s = c) != OK) \
					 { \
                       cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                       error.print(s); \
                       cerr << "TEST DID NOT PASS" <<endl; \
					   delete bufMgr; \
                       exit(1); \
                     } \
                   }
#define FAIL(c)  { Status s; \
                   if ((s = c) == OK) \
				   { \
                     cerr << "At line " << __LINE__ << ":" << endl << "  "; \
                     cerr << "This call should fail: " #c << endl; \
                     cerr << "TEST DID NOT PASS" <<endl; \
					 delete bufMgr; \
                     exit(1); \
                   } \
                  }
BufMgr* bufMgr;
int main()
{
	Error       error;
	DBManager   dbManager;
	DB*         db1;
	DB*         db2;
	DB*         db3;
	DB*         db4;
	unsigned long i=0;
	const unsigned long num=100;
	unsigned long j[num];
	// create buffer manager
	bufMgr=new BufMgr(num);
	// create dummy files
	dbManager.destroyDataBase("test1");
	dbManager.destroyDataBase("test2");
	dbManager.destroyDataBase("test3");
	dbManager.destroyDataBase("test4");
	CALL(dbManager.createDataBase("test1"));
	ASSERT(dbManager.createDataBase("test1")==FILEEXISTS);
	CALL(dbManager.createDataBase("test2"));
	CALL(dbManager.createDataBase("test3"));
	CALL(dbManager.createDataBase("test4"));
	CALL(dbManager.openDataBase("test1", db1));
	CALL(dbManager.openDataBase("test2", db2));
	CALL(dbManager.openDataBase("test3", db3));
	CALL(dbManager.openDataBase("test4", db4));
	// test buffer manager
	Page* page;
	Page* page2;
	Page* page3;
	char  cmp[PAGESIZE];
	unsigned long pageno, pageno2, pageno3;
	cout<<"Allocating pages in a file..."<<endl;
	for (i=0;i<num;i++)
	{
		CALL(bufMgr->allocPage(db1, j[i], page));
		sprintf((char*)page,"test1 Page %lu %7.1f",j[i],(float)j[i]);
		CALL(bufMgr->unPinPage(db1,j[i],true));
	}
	cout <<"Test1 passed"<<endl<<endl;
	cout << "Reading pages back..." << endl;
	for (i=0;i<num;i++)
	{
		CALL(bufMgr->readPage(db1, j[i], page));
		sprintf((char*)&cmp, "test1 Page %lu %7.1f", j[i], (float)j[i]);
		ASSERT(memcmp(page, &cmp, strlen((char*)&cmp)) == 0);
		CALL(bufMgr->unPinPage(db1,j[i],false));
    }
	cout<< "Test2 passed"<<endl<<endl;
	cout << "Writing and reading back multiple files..." << endl;
	cout << "Expected Result: ";
	cout << "The output will consist of the file name, page number, and a value."<<endl;
	cout << "The page number and the value should match."<<endl<<endl;
	for (i=0;i<num/3;i++)
	{
		CALL(bufMgr->allocPage(db2,pageno2,page2));
		sprintf((char*)page2,"test2 Page %lu %7.1f",pageno2,(float)pageno2);
		CALL(bufMgr->allocPage(db3, pageno3, page3));
		sprintf((char*)page3,"test3 Page %lu %7.1f",pageno3,(float)pageno3);
		pageno=j[rand()%num];
		CALL(bufMgr->readPage(db1,pageno,page));
		sprintf((char*)&cmp,"test1 Page %lu %7.1f",pageno,(float)pageno);
		ASSERT(memcmp(page,&cmp,strlen((char*)&cmp))==0);
		cout<<(char*)page<<endl;
		CALL(bufMgr->readPage(db2, pageno2, page2));
		sprintf((char*)&cmp,"test2 Page %lu %7.1f",pageno2,(float)pageno2);
		ASSERT(memcmp(page2,&cmp,strlen((char*)&cmp))==0);
		CALL(bufMgr->readPage(db3, pageno3, page3));
		sprintf((char*)&cmp,"test3 Page %lu %7.1f",pageno3,(float)pageno3);
		ASSERT(memcmp(page3,&cmp,strlen((char*)&cmp)) == 0);
		CALL(bufMgr->unPinPage(db1,pageno,true));
	}
	for (i=0;i<num/3;i++)
	{
		CALL(bufMgr->unPinPage(db2,i+1,true));
		CALL(bufMgr->unPinPage(db2,i+1,true));
		CALL(bufMgr->unPinPage(db3,i+1,true));
		CALL(bufMgr->unPinPage(db3,i+1,true));
	}
	cout<<"Test3 passed"<<endl<<endl;
	cout<<"\nReading \"test.1\"...\n";
	cout<<"Expected Result: ";
	cout<<"Pages in order.  Values matching page number.\n\n";
	for (i=1;i<num/3;i++)
	{
		CALL(bufMgr->readPage(db1,i,page2));
		sprintf((char*)&cmp, "test1 Page %lu %7.1f",i,(float)i);
		ASSERT(memcmp(page2,&cmp,strlen((char*)&cmp))==0);
		CALL(bufMgr->unPinPage(db1,i,false));
	}
	cout<<"Test4 passed"<<endl<<endl;
	cout<<"\nReading \"test.2\"...\n";
	cout<<"Expected Result: ";
	cout<<"Pages in order.  Values matching page number.\n\n";
	for (i=1;i<num/3;i++)
	{
		CALL(bufMgr->readPage(db2,i,page2));
		sprintf((char*)&cmp,"test2 Page %lu %7.1f",i,(float)i);
		ASSERT(memcmp(page2,&cmp,strlen((char*)&cmp))==0);
		CALL(bufMgr->unPinPage(db2,i,false));
	}
	cout<<"Test5 passed"<<endl<<endl;
	cout<<"\nReading \"test.3\"...\n";
	cout<<"Expected Result: ";
	cout<<"Pages in order.  Values matching page number.\n\n";
	for (i=1;i<num/3;i++)
	{
		CALL(bufMgr->readPage(db3,i,page3));
		sprintf((char*)&cmp,"test3 Page %lu %7.1f",i,(float)i);
		ASSERT(memcmp(page3, &cmp, strlen((char*)&cmp)) == 0);
		cout<<(char*)page3<<endl;
		CALL(bufMgr->unPinPage(db3,i,false));
	}
	cout<<"Test6 passed" <<endl<<endl;
	cout<<"\nTesting error condition...\n\n";
	cout<<"Expected Result: Error statments followed by the \"Test passed\" statement."<<endl;
	Status status;
	FAIL(status=bufMgr->readPage(db4,1,page));
	error.print(status);
	cout<<"Test7 passed"<<endl<<endl;
	CALL(bufMgr->allocPage(db4,i,page));
	CALL(bufMgr->unPinPage(db4,i,true));
	FAIL(status=bufMgr->unPinPage(db4,i,false));
	error.print(status);
	cout<<"Test8 passed"<<endl<<endl;
	for (i=0;i<num;i++)
	{
		CALL(bufMgr->allocPage(db4,j[i],page));
		sprintf((char*)page,"test4 Page %lu %7.1f",j[i],(float)j[i]);
	}
	unsigned long tmp;
	FAIL(status=bufMgr->allocPage(db4,tmp,page));
	error.print(status);
	cout<<"Test9 passed"<<endl<<endl;
	for (i=0;i<num;i++)
		CALL(bufMgr->unPinPage(db4,i+2,true));
	cout<<"\nReading \"test.1\"...\n";
	cout<<"Expected Result: ";
	cout<<"Pages in order.  Values matching page number.\n\n";
	for (i=1;i<num;i++)
	{
		CALL(bufMgr->readPage(db1,i,page));
		sprintf((char*)&cmp, "test1 Page %lu %7.1f", i, (float)i);
		ASSERT(memcmp(page,&cmp,strlen((char*)&cmp))==0);
		cout<<(char*)page<<endl;
	}
	cout<<"Test10 passed"<<endl<<endl;
	cout<<"flushing file with pages still pinned. Should generate an error" << endl;
	FAIL(status = bufMgr->flushFile(db1));
	error.print(status);
	cout<<"Test11 passed"<<endl<<endl;
	for (i=1;i<num;i++)
		CALL(bufMgr->unPinPage(db1,i,true));
	CALL(bufMgr->flushFile(db1));
	delete bufMgr;
	//cin>>cmp;
	CALL(dbManager.closeDataBase("test1"));
	CALL(dbManager.closeDataBase("test2"));
	CALL(dbManager.closeDataBase("test3"));
	CALL(dbManager.closeDataBase("test4"));
	CALL(dbManager.destroyDataBase("test1"));
	CALL(dbManager.destroyDataBase("test2"));
	CALL(dbManager.destroyDataBase("test3"));
	CALL(dbManager.destroyDataBase("test4"));
	cout<<endl<<"Passed all tests."<<endl;
	return (1);
}