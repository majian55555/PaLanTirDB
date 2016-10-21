#ifndef NEWDB_H
#define NEWDB_H
#pragma warning(disable:4786)
#include "page.h"
#include <fstream.h>
#include <iostream.h>
#include <string>
#include <map>
//using namespace std;

const unsigned long MAXNAMESIZE=32;//数据库名，关系名，列名，索引名，约束名的最大长度不超过32

class RelRecord //关系记录类，该类的对象将存储于数据库目录页中。
{
	friend class DBHeadPage;
private:
	unsigned long firstPageNo;//关系头页的页号,unsigned long占4字节，取值范围从0到4,294,967,295。
	//如果每页4096字节，则本数据库可以存储16T的数据。0为无效页。
	char relName[MAXNAMESIZE];//关系名
	explicit RelRecord();
	const unsigned long getFirstPageNo() const;//取该关系头页的页号，即直接返回this->firstPageNo。
	const std::string getRelName() const;//取该关系的关系名，将this->relName转换为string类型然后返回。
	const Status setFirstPageNo(unsigned long firstPageNo);//置该关系头页的页号，需判断参数的正确性。（若参数
	//firstPageNo<=0则返回BADFIRSTPAGENO，否则返回OK）
	const Status setRelName(const std::string& relName);////置该关系的关系名，需判断参数的正确性。（若参数
	//relName为空或长度大于MAXNAMESIZE则返回BADRELNAME，否则返回OK）
	void initRelRec();//初始化该关系记录，将this->firstPageNo置为0。
};
const int RELCNT = (PAGESIZE-4*sizeof(unsigned long)-MAXNAMESIZE)/sizeof(RelRecord);//数据库目录页中能够存储的关系记录个数。
class DBHeadPage  //数据库目录页类，用来存储数据库信息和库中每个关系的关系记录，可以有多个数据库目录页，用
//nextDBHPNo将所有数据库目录页链起来。用nextFreePage将所有空闲页链起来。
{
	friend class DB;
private:
	unsigned long relCnt;  //关系数
	unsigned long nextFreePage;//下一个空闲页页号，0为无下一页
	unsigned long pageCnt;//页数（包含空闲页）
	unsigned long nextDBHPNo;//下一个数据库头页页号，0为无下一页
	char dbName[MAXNAMESIZE];//数据库名
	RelRecord relRecArray[RELCNT];//关系记录数组
	char DBsignature[PAGESIZE-(RELCNT*sizeof(RelRecord)+MAXNAMESIZE+4*sizeof(unsigned long))]; //数据库签名,为了不使内存越界，
	//DBHeadPage比Page小16个字节
	const Status setNextFreePage(unsigned long freePageNo);//置下一个空闲页页号，需判断参数的正确性。
	const Status setPageCnt(unsigned long pageCnt);//置该数据库的总页数，需判断参数的正确性。
	const Status setRelCnt(unsigned long relCnt);//置该数据库的关系数，需判断参数的正确性。
	const Status setNextDBHPNo(unsigned long nextDBHPNo);//置下一个数据库目录页页号，需判断参数的正确性。
	const Status setDBName(const std::string &DBName);//置数据库名称，需判断参数的正确性。
	const Status addRel(const std::string& tableName,unsigned long tableFirstPage);//向关系记录的集合中新加一个关系，需判断该
	//数据库目录页是否已满，该关系记录是否已经存在。
	const Status updataRel(const std::string& oldRelName, const std::string& newRelName);//更新某个关系记录，需判断该关系
	//记录是否已经存在。
	const Status deleteRel(const std::string& tableName); //向关系记录的集合中删除一个关系，需判断该关系记录是否已经
	//存在。
public:
	explicit DBHeadPage();
	~DBHeadPage();
	const unsigned long getRelCnt() const;//返回关系数
	const unsigned long getNextFreePage() const;//返回下一个空闲页页号
	const unsigned long getPageCnt() const;//返回该数据库的总页数。
	const unsigned long getNextDBHPNo() const;//返回下一个数据库目录页页号
	const std::string getDBName() const;//返回数据库名称
	void init();//初始化数据库目录页（this->relCnt=0;this->nextFreePage=0;this->pageCnt=1;this->nextDBHPNo=0;
	//并将this->relRecArray[]数组中每个关系记录初始化）。
	const bool isFull() const;//判断该数据库目录页是否已满。
	const bool isEmpty() const;//判断该数据库目录页是否已空。
	const Status getRelFirstPage(const std::string &relName,unsigned long &firstPageNo) const;//通过关系名去查找该关系的头页页号，
	//需判断该关系记录是否已经存在。
};
class DB //数据库类，封装对数据库的操作。
//包括新建数据库、删除数据库、打开数据库、关闭数据库、新建关系、删除关系、分配页、释放页、读页、写页。
{
	friend class DBManager;
private:
	std::string DBName; //数据库名
	mutable fstream file; //对文件的操作对象
	unsigned long openCnt;//数据库的打开次数
	DBHeadPage firstDBHP;//数据库首个目录页，在openDB()中读入
	explicit DB();//构造函数，this->file.clear();this->openCnt=0;。
	explicit DB(const std::string &DBName);
	~DB();//析构函数，调用this->closeDB()；并返回判断状态，出错则返回错误信息。
	const Status intread(const unsigned long pageNo, Page* pagePtr) const;
	const Status intwrite(const unsigned long pageNo, const Page* pagePtr);
	const Status createDB(const std::string & DBName) ;  //新建一个数据库（文件），需判断参数的正确性和新建文件成功
	//与否。
	const Status destroyDB(const std::string & DBName) ; // 删除一个数据库,需判断参数的正确性和删除文件成功与否。
	const Status openDB(const std::string & DBName);  //打开数据库，需判断参数的正确性和打开文件成功与否，
	//若已经打开则openCnt++。
	const Status closeDB();//关闭数据库，需判断参数的正确性和关闭文件成功与否。若打开次数大于1则openCnt--，
	//若该数据库未被打开则返回FILENOTOPEN。
public:
	const Status getFirstDBHP(const DBHeadPage* &dbHeadPage) const;
	const Status flushFirstDBHP();
	const Status createRel(const std::string& relName);//从数据库中创建该关系，置关系头页、关系名、关系页数组。
	//须考虑此数据库目录页已满的情况。
	const Status destroyRel(const std::string& relName);//从数据库中删除该关系。若找不到该关系记录则返回RELNOTFOUND，
	//须考虑在多个数据库目录页中找该关系记录的情况。
	const Status updataRel(const std::string& oldRelName, const std::string& newRelName);//更新某个关系记录。若找不到该
	//关系记录则返回RELNOTFOUND，须考虑在多个数据库目录页中找该关系记录的情况。
	const Status allocatePage(unsigned long& pageNo);//为该数据库分配一个新页（即在磁盘上扩展PAGESIZE个字节）或者空闲页。
	//先去找有没有空闲页，若没有则在磁盘上扩展一个新页。
	const Status disposePage(const unsigned long pageNo);//将该页加入空闲页链表，需判断参数的正确性，然后将该页加入空闲页链。
	const Status readPage(const unsigned long pageNo,Page* pagePtr) const;//将磁盘中该页号的内容读入pagePtr中，需判断参数的
	//正确性。
	const Status writePage(const unsigned long pageNo,const Page* pagePtr);//将内存中pagePtr内容写入磁盘中该页号的位置。
	const unsigned long getOpenCnt() const;//返回数据库的打开次数。
	const Status getRelFirstPageNo(const std::string &relName,unsigned long &firstPageNo) const;//通过关系名去查找该关系的头页页号，
	//需判断该关系记录是否已经存在。
};
typedef std::map<std::string,DB*> DBMap;
class DBManager //数据库管理器,可管理多个数据库。
{
private:
	DBMap dbMap;//数据库对象集合，一个数据库名对应一个DB类的对象。
public:
	explicit DBManager();
	~DBManager();
	const Status createDataBase(const std::string & DBName);//新建一个数据库，先判断该数据库是否已经存在于
	//this->dbMap中。
	const Status destroyDataBase(const std::string & DBName);//删除一个数据库，并从this->dbMap中将该记录删除，
	//先判断该数据库是否存在于this->dbMap中。
	const Status openDataBase(const std::string & DBName,DB* & database);//打开一个数据库，先判断该数据库是否已经存在于
	//this->dbMap中，若不存在则将其插入this->dbMap中，若存在则更新。将打开的数据库对象返回到database。
	const Status closeDataBase(const std::string & DBName);//关闭一个数据库，先判断该数据库是否已经存在于
	//this->dbMap中。若该数据库的OpenCnt==0则从this->dbMap中删除。
};
/*
class DataPageRec//数据页记录结构体
{
	friend class RelHeadPage;
private:
    int pageNo;    // 页号
    int freeSpace; // 页的可用空白字节数
public:
	const int getPageNo() const;
	const int getFreeSpace() const;
	const Status setPageNo(int pageNo);
	const Status setFreeSpace(int freeSpace);
	void init();
};
const int DPRCNT = (PAGESIZE-4*sizeof(int)-MAXNAMESIZE)/sizeof(DataPageRec);
class RelHeadPage //关系目录页类
{
	friend class DB;
private:
    char        relName[MAXNAMESIZE];   // 关系名
    unsigned long recCnt;                // 记录数
	unsigned long pageCnt;            // 这个关系的总页数
	unsigned long pageNum;            // pages数组的元素个数
	int nextPageNo;//下一个关系目录页的页号
	DataPageRec  pages[DPRCNT];
public:
	const int getRecCnt() const;
	const int getPageCnt() const;
	const int getNextPageNo() const;
	const Status setRecCnt(unsigned long recordCnt);
	const Status setPageCnt(unsigned long pagesCnt);
	const Status setNextPageNo(int nextPage);
	const string getRelName() const;
	const Status setRelName(const string& relName);
	void init(const string& relName);//初始化目录页里的属性
	const Status destroy(int disposePages[]);//删除这个表，将它里面的页放入数据库目录页中的空闲页链表中
	const Status addNewDPRec(int pageNo);
	const Status deleteDPRec(int pageNo);
	const Status updateDPRec(int pageNo, int freeSpace);
};*/
#endif
