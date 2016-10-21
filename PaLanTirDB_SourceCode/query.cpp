#include "query.h"
extern BufMgr * bufMgr;
extern DBManager dbManager;
extern DB* db;
extern RelCatalog  *relCat;
extern AttrCatalog *attrCat;
extern Error error;
//
// Prototypes for query layer functions
//
//Selects records from the specified relation.
//Returns:
//OK on success
//an error code otherwise
//选择是通过带过滤条件的扫描（ScanOperator）实现的.选择的结果存储在名称由
//result变量指定的关系中（在调用PLT_Select()之前，解释程序会创建该关系）.投影列表
//由参数projCnt和projNames指定,在每一条记录加入结果表时就要完成投影处理.需要说
//明的是:过滤条件中的常量总是通过attrValue以字符形式出现，你可以用atoi()函数将
//char*转换为整型,用atof()将char*转换为符点型,如果attr参数为 NULL,意味着执行无条件的扫描。
const Status PLT_Select(const std::string & result, 
		       const int projCnt, 
		       const AttrInfo projNames[],
		       const AttrInfo *attr, 
		       const Operator op, 
		       const char *attrValue)
{
	cout << "Doing PLT_Select " << endl;
	Status status=OK;
	//set AttrDesc []
	AttrDesc *projDescs=new AttrDesc[projCnt];
	int recMaxLen=0;
	for(int i=0;i<projCnt;i++)
	{
		if((status=attrCat->getInfo(projNames[i].getRelName(),projNames[i].getAttrName(),projDescs[i]))!=OK)
			{delete [] projDescs;return status;}
		recMaxLen+=projDescs[i].getAttrSize();
	}
	//get the filter AttrDesc
	AttrDesc filterDesc;
	int filterLen=0;
	Datatype dataType=P_CHAR;
	void *filterData=NULL;
	int tmpInt=0;
	float tmpFloat=0;
	if(attr!=NULL)//attr!=NULL
	{
		if((status=attrCat->getInfo(attr->getRelName(),attr->getAttrName(),filterDesc))!=OK)
			{delete [] projDescs;return status;}
		switch(filterDesc.getAttrType())
		{
			case 0:
				if(attrValue!=NULL)
					filterLen=strlen(attrValue);
				dataType=P_CHAR;
				filterData=(void*)attrValue;
				break;
			case 1:
				if(attrValue!=NULL)
					filterLen=strlen(attrValue);
				dataType=P_VCHAR;
				filterData=(void*)attrValue;
				break;
			case 2:
				filterLen=SIZEOFINT;
				dataType=P_INTEGER;
				if(attrValue==NULL)
					break;
				tmpInt=atoi(attrValue);
				filterData=(void*)&tmpInt;
				break;
			case 3:
				filterLen=SIZEOFFLOAT;
				dataType=P_FLOAT;
				if(attrValue==NULL)
					break;
				tmpFloat=atof(attrValue);
				filterData=(void*)&tmpFloat;
				break;
			case 4:
				filterLen=SIZEOFDATE;
				dataType=P_DATE;
				break;
			case 5:
				filterLen=SIZEOFDATETIME;
				dataType=P_DATETIME;
				break;
			case 6:
				filterLen=SIZEOFBOOL;
				dataType=P_BOOLEAN;
				filterData=(void*)attrValue;
				break;
			case 7:
				filterLen=0;
				dataType=P_BLOB;
				break;
			case 8:
				filterLen=0;
				dataType=P_CLOB;
				break;
			default:
				cout<<"wrong attrType!!!"<<endl;
				delete [] projDescs;
				return DUPLATTR; //need a new error type !!!
		}
	}
	ScanOperator* scanOp=new ScanOperator(db->getDBName(),projNames[0].getRelName(),status);
	if(status!=OK) {delete [] projDescs;return status;}
	if(attr==NULL)
	{
		if((status=scanOp->startScan(0,0,dataType,NULL,EQ))!=OK)
		{
			delete [] projDescs;
			delete scanOp;
			return status;
		}
	}
	else
	{
		if((status=scanOp->startScan(filterDesc.getAttrIdx(),filterLen,dataType,(char*)filterData,op))!=OK)
		{
			delete [] projDescs;
			delete scanOp;
			return status;
		}
	}
	InsertOperator* insertOp=new InsertOperator(db->getDBName(),result,status);
	if(status!=OK)
	{
		delete [] projDescs;
		delete scanOp;
		return status;
	}
	RID rid;
	//scan the src table
	while(scanOp->scanNext(rid)==OK)
	{
		Record completeRec;
		if((status=scanOp->getRecord(completeRec))!=OK)
		{
			delete [] projDescs;
			delete scanOp;
			delete insertOp;
			return status;
		}
		unsigned short cplRecAttrNum=completeRec.getAttrsNum();
		AttrInPage *cplAttrsInPage=new AttrInPage[cplRecAttrNum];
		for(unsigned short j=0;j<cplRecAttrNum;j++)
		{
			if((status=completeRec.getAttrInPage(j,cplAttrsInPage[j]))!=OK)
			{
				delete [] cplAttrsInPage;
				delete [] projDescs;
				delete scanOp;
				delete insertOp;
				return status;
			}
		}
		Record newRec;
		AttrInPage *attrsInPage=new AttrInPage[projCnt];
		char *tmpData=new char[recMaxLen];
		int offset=0;
		for(int i=0;i<projCnt;i++)
		{
			attrsInPage[i]=cplAttrsInPage[projDescs[i].getAttrIdx()];
			if(attrsInPage[i].isNull())
				continue;
			memcpy((void*)&tmpData[offset],
				(void*)((unsigned long)completeRec.getRealData()+completeRec.getOffset(projDescs[i].getAttrIdx())),
				attrsInPage[i].getAttrLen());
			offset+=attrsInPage[i].getAttrLen();
		}
		newRec.setAttrsInPage(attrsInPage,projCnt);
		newRec.setRealData(tmpData);
		if((status=insertOp->insertRecord(newRec,rid))!=OK)
		{
			delete [] tmpData;
			delete [] attrsInPage;
			delete [] cplAttrsInPage;
			delete [] projDescs;
			delete scanOp;
			delete insertOp;
			return status;
		}
		delete [] cplAttrsInPage;
		delete [] tmpData;
		delete [] attrsInPage;
	}
	delete [] projDescs;
	delete scanOp;
	delete insertOp;
	return OK;
}
//insert a record to the temporary table(the record is from recOut and recIn)
const Status InsertResult(const std::string & result,const int projCnt,const AttrDesc projDescs[],int newRecMaxLen,
						  const AttrDesc &attrOut,const AttrDesc &attrIn,const Record& recOut,const Record& recIn)
{
	Status status;
	Record newRec;
	char *tmpChar=new char[newRecMaxLen];
	int curOffset=0;
	AttrInPage* attrsInPage=new AttrInPage[projCnt];
	for(int i=0;i<projCnt;i++)
	{
		if(projDescs[i].getRelName().compare(attrOut.getRelName())==0)//outside table
		{
			if((status=recOut.getAttrInPage(projDescs[i].getAttrIdx(),attrsInPage[i]))!=OK)
			{
				delete [] tmpChar;
				return status;
			}
			if(attrsInPage[i].isNull())
				continue;
			memcpy((void*)&tmpChar[curOffset],(void*)((unsigned long)recOut.getRealData()+recOut.getOffset(projDescs[i].getAttrIdx())),
				attrsInPage[i].getAttrLen());
		}
		else if(projDescs[i].getRelName().compare(attrIn.getRelName())==0)//inside table
		{
			if((status=recIn.getAttrInPage(projDescs[i].getAttrIdx(),attrsInPage[i]))!=OK)
			{
				delete [] tmpChar;
				return status;
			}
			if(attrsInPage[i].isNull())
				continue;
			memcpy((void*)&tmpChar[curOffset],(void*)((unsigned long)recIn.getRealData()+recIn.getOffset(projDescs[i].getAttrIdx())),
				attrsInPage[i].getAttrLen());
		}
		else
		{
			delete [] tmpChar;
			return RELNOTFOUND;
		}
		curOffset+=attrsInPage[i].getAttrLen();
	}
	newRec.setAttrsInPage(attrsInPage,projCnt);
	newRec.setRealData((void*)tmpChar);
	InsertOperator insertOp(db->getDBName(),result,status);
	if(status!=OK)
	{
		delete [] tmpChar;
		return status;
	}
	RID rid;
	if((status=insertOp.insertRecord(newRec,rid))!=OK)
	{
		delete [] tmpChar;
		return status;
	}
	delete [] tmpChar;
	return OK;
}
//Joins two relations.
//Returns:
//OK on success
//an error code otherwise
// implementation of nested loops join goes here
//一个为标准的嵌套循环连接算法,用于连接两个属性为attr1和attr2,与选择操作符相同,
//在调用PLT_Select()之前,解释程序会创建存储结果的关系result,连接的两个关系的名称
//可以在参数attr1和attr2中找到，如果两个连接关系有同名的属性,其中一个会加上"_1"
//后缀以示区别,你必须按projCnt和projNames参数当场实施投影操作.
//请对嵌套循环连接算法进行以下优化处理：对外层循环的每条记录，启动一个带条件的
//内层循环关系扫描，条件为外层循环记录的连接属性的值，该扫描返回的每条记录可以
//用来生成结果记录。
//select student.name,nation.nationName from student,nation where student.nation=nation.nation;
const Status PLT_NL_Join(const std::string & result,
		     const int projCnt,
		     const AttrInfo projNames[],
		     const AttrDesc attrOut,
		     const Operator op,
		     const AttrDesc attrIn)
{
	Status status;
	cout << "Doing NL Join" << endl;
	//count the record's max length and set AttrDesc []
	int recMaxLen=0;
	AttrDesc *projDescs=new AttrDesc[projCnt];
	for(int i=0;i<projCnt;i++)
	{
		if((status=attrCat->getInfo(projNames[i].getRelName(),projNames[i].getAttrName(),projDescs[i]))!=OK)
		{
			delete [] projDescs;
			return status;
		}
		recMaxLen+=projDescs[i].getAttrSize();
	}
	//get two AttrDescs and check length and datatype
	if((attrOut.getAttrType()!=P_CHAR || attrOut.getAttrType()!=P_VCHAR) && 
		(attrIn.getAttrType()!=P_CHAR || attrIn.getAttrType()!=P_VCHAR))
	{
		if((attrOut.getAttrSize()!=attrIn.getAttrSize()) || (attrOut.getAttrType()!=attrIn.getAttrType()))
		{
			delete [] projDescs;
			return ATTRTYPEMISMATCH;
		}
	}
	//search and compare
	ScanOperator scanOp1(db->getDBName(),attrOut.getRelName(),status);
	if(status!=OK)
	{
		delete [] projDescs;
		return status;
	}
	if((status=scanOp1.startScan(0,0,P_CHAR,NULL,EQ))!=OK)
		return status;
	RID ridOut;
	Record recOut;
	while(scanOp1.scanNext(ridOut)==OK)
	{
		if((status=scanOp1.getRecord(recOut))!=OK)
		{
			delete [] projDescs;
			return status;
		}
		AttrInPage outAttrInPage;
		if((status=recOut.getAttrInPage(attrOut.getAttrIdx(),outAttrInPage))!=OK)
		{
			delete [] projDescs;
			return status;
		}
		char *filter=new char[outAttrInPage.getAttrLen()];
		memcpy(filter,(void*)((unsigned long)recOut.getRealData()+recOut.getOffset(attrOut.getAttrIdx())),
			outAttrInPage.getAttrLen());
		ScanOperator scanOp2(db->getDBName(),attrIn.getRelName(),status);
		if(status!=OK)
		{
			delete [] projDescs;
			delete [] filter;
			return status;
		}
		if((status=scanOp2.startScan(attrIn.getAttrIdx(),outAttrInPage.getAttrLen(),
					(Datatype)attrIn.getAttrType(),filter,op))!=OK)
		{
			delete [] projDescs;
			delete [] filter;
			return status;
		}
		RID ridIn;
		Record recIn;
		while(OK==scanOp2.scanNext(ridIn))
		{
			if((status=scanOp2.getRecord(recIn))!=OK)
			{
				delete [] projDescs;
				delete [] filter;
				return status;
			}
			if((status=InsertResult(result,projCnt,projDescs,recMaxLen,attrOut,attrIn,recOut,recIn))!=OK)
			{
				delete [] projDescs;
				delete [] filter;
				return status;
			}
		}
		delete [] filter;
	}
	delete [] projDescs;
	return OK;
}
// implementation of hash join goes here
//第二种连接算法为基于排序的块嵌套循环连接算法,该算法的实现方法为:
///先将外层循环关系的M页读入内存缓存池,对M页读的所有记录按连接属性
//排序,该排序表应当将连接属性映射到RID且用Multi-Map实现.
/*const int MFORMSJION=4;
//we can use multimap<>
typedef std::multimap<std::string,RID> Str2RidMap;
typedef std::multimap<std::string,RID>::size_type S2R_sz_type;

typedef std::multimap<int,RID> Int2RidMap;
typedef std::multimap<int,RID>::size_type I2R_sz_type;

typedef std::multimap<float,RID> Float2RidMap;
typedef std::multimap<float,RID>::size_type F2R_sz_type;

typedef std::multimap<bool,RID> Bool2RidMap;
typedef std::multimap<bool,RID>::size_type B2R_sz_type;

const Status PLT_SM_Join(const std::string & result,
		     const int projCnt,
		     const AttrInfo projNames[],
		     const AttrDesc attrOut,
		     const Operator op,
		     const AttrDesc attrIn)
{
	Status status;
	cout << "Doing SM Join" << endl;
	//get datatype and check and get attrIn,attrOut
	if(attrOut.getAttrType()!=attrIn.getAttrType())
		return ATTRTYPEMISMATCH;
	Str2RidMap s2rMap;
	Int2RidMap i2rMap;
	Float2RidMap f2rMap;
	Bool2RidMap b2rMap;
	//get attrs from projNames and new record's length
	AttrDesc *attrs=new AttrDesc[projCnt];
	int recMaxLen=0;
	for(int k=0;k<projCnt;k++)
	{
		if((status=attrCat->getInfo(projNames[k].getRelName(),projNames[k].getAttrName(),attrs[k]))!=OK)
		{
			delete [] attrs;
			return status;
		}
		recMaxLen+=attrs[k].getAttrSize();
	}
	//put MFORHASHJION pages to bufMgr from outside table and put all records(from these pages) into hashTable
	int j=0,h=0,nextPage;
	do
	{
		h++;
		Page* pages[MFORMSJION];
		for(;j<MFORMSJION*h;j++)
		{
			if((status=bufMgr->readPage(db,j+2,pages[j%MFORMSJION]))!=OK)
				break;
		}
		int i=(j-1)%MFORMSJION;
		RID curRid,nextRid;
		Record rec;
		for(;i>=0;i--)
		{
			if((status=pages[i]->firstRecord(curRid))==OK)
			{
				if((status=pages[i]->getRecord(curRid,rec))!=OK)
				{
					delete [] attrs;
					return status;
				}
				AttrInPage attrInPageOut;
				if((status=rec.getAttrInPage(attrOut.getAttrIdx(),attrInPageOut))!=OK)
				{
					delete [] attrs;
					return status;
				}
				//put first record to map
				int tmpInt;
				float tmpFloat;
				bool tmpBool;
				char *tmpChar=new char[attrInPageOut.getAttrLen()];
				std::string tmpstr;
				switch(attrOut.getAttrType())
				{
					case 0://P_CHAR
						memcpy(tmpChar,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
							attrInPageOut.getAttrLen());
						tmpstr.assign(tmpChar,attrInPageOut.getAttrLen());
						s2rMap.insert(std::multimap<std::string,RID>::value_type(tmpstr,curRid));
						break;
					case 1://P_VCHAR
						memcpy(tmpChar,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
							attrInPageOut.getAttrLen());
						tmpstr.assign(tmpChar,attrInPageOut.getAttrLen());
						s2rMap.insert(std::multimap<std::string,RID>::value_type(tmpstr,curRid));
						break;
					case 2://P_INTEGER
						memcpy(&tmpInt,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
							SIZEOFINT);
						i2rMap.insert(std::multimap<int,RID>::value_type(tmpInt,curRid));
						break;
					case 3://P_FLOAT
						memcpy(&tmpFloat,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
							SIZEOFFLOAT);
						f2rMap.insert(std::multimap<float,RID>::value_type(tmpFloat,curRid));
						break;
					case 6://P_BOOLEAN
						memcpy(&tmpBool,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
							SIZEOFBOOL);
						f2rMap.insert(std::multimap<bool,RID>::value_type(tmpBool,curRid));
					default:
						delete [] attrs;
						delete [] tmpChar;
						return BADDATATYPE;
				}
				while((status=pages[i]->nextRecord(curRid,nextRid))==OK)
				{
					curRid=nextRid;
					if((status=pages[i]->getRecord(curRid,rec))!=OK)
					{
						delete [] attrs;
						delete [] tmpChar;
						return status;
					}
					AttrInPage attrInPageOut;
					if((status=rec.getAttrInPage(attrOut.getAttrIdx(),attrInPageOut))!=OK)
					{
						delete [] attrs;
						delete [] tmpChar;
						return status;
					}
					switch(attrOut.getAttrType())
					{
						case 0://P_CHAR
							memcpy(tmpChar,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
								attrInPageOut.getAttrLen());
							tmpstr.assign(tmpChar,attrInPageOut.getAttrLen());
							s2rMap.insert(std::multimap<std::string,RID>::value_type(tmpstr,curRid));
							break;
						case 1://P_VCHAR
							memcpy(tmpChar,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
								attrInPageOut.getAttrLen());
							tmpstr.assign(tmpChar,attrInPageOut.getAttrLen());
							s2rMap.insert(std::multimap<std::string,RID>::value_type(tmpstr,curRid));
							break;
						case 2://P_INTEGER
							memcpy(&tmpInt,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
								SIZEOFINT);
							i2rMap.insert(std::multimap<int,RID>::value_type(tmpInt,curRid));
							break;
						case 3://P_FLOAT
							memcpy(&tmpFloat,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
								SIZEOFFLOAT);
							f2rMap.insert(std::multimap<float,RID>::value_type(tmpFloat,curRid));
							break;
						case 6://P_BOOLEAN
							memcpy(&tmpBool,(void*)((unsigned long)rec.getRealData()+rec.getOffset(attrOut.getAttrIdx())),
								SIZEOFBOOL);
							f2rMap.insert(std::multimap<bool,RID>::value_type(tmpBool,curRid));
						default:
							delete [] attrs;
							delete [] tmpChar;
							return BADDATATYPE;
					}
				}
				delete [] tmpChar;
			}
		}
		//search the inside table
		ScanOperator scanOp(db->getDBName(),attrIn.getRelName(),status);
		if(status!=OK)
		{
			delete [] attrs;
			return status;
		}
		if((status=scanOp.startScan(0,0,P_CHAR,NULL,EQ))!=OK)
		{
			delete [] attrs;
			return status;
		}
		RID ridIn,ridOut;
		Record recIn,recOut;
		Str2RidMap::iterator s2rIterator;
		Int2RidMap::iterator i2rIterator;
		Float2RidMap::iterator f2rIterator;
		Bool2RidMap::iterator b2rIterator;
		S2R_sz_type sCount,s;
		I2R_sz_type iCount,ii;
		F2R_sz_type fCount,f;
		B2R_sz_type bCount,b;
		//search inside table
		while(scanOp.scanNext(ridIn)==OK)
		{
			if((status=scanOp.getRecord(recIn))!=OK)
			{
				delete [] attrs;
				return status;
			}
			AttrInPage attrInPageIn;
			if((status=recIn.getAttrInPage(attrIn.getAttrIdx(),attrInPageIn))!=OK)
			{
				delete [] attrs;
				return status;
			}
			int tmpInt;
			float tmpFloat;
			bool tmpBool;
			char *tmpChar=new char[attrInPageIn.getAttrLen()];
			std::string tmpstr;
			//search this record from hashTable
			switch(attrIn.getAttrType())
			{
				case 0://string
					memcpy(tmpChar,(void*)((unsigned long)recIn.getRealData()+recIn.getOffset(attrIn.getAttrIdx())),
								attrInPageIn.getAttrLen());
					tmpstr.assign(tmpChar,attrInPageIn.getAttrLen());
					if((sCount=s2rMap.count(std::multimap<std::string,RID>::key_type(tmpstr)))==0)
						continue;
					s2rIterator=s2rMap.find(std::multimap<std::string,RID>::key_type(tmpstr));
					for(s=0;s<sCount;++s,++s2rIterator)
					{
						ridOut=(*s2rIterator).second;
						if((status=pages[(ridOut.getPageNo()-2)%MFORMSJION]->getRecord(ridOut,recOut))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
						if((status=InsertResult(result,projCnt,attrs,recMaxLen,attrOut,attrIn,recOut,recIn))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
					}
					break;
				case 2://int
					memcpy(&tmpInt,(void*)((unsigned long)recIn.getRealData()+recIn.getOffset(attrIn.getAttrIdx())),
								SIZEOFINT);
					if((iCount=i2rMap.count(std::multimap<int,RID>::key_type(tmpInt)))==0)
						continue;
					i2rIterator=i2rMap.find(std::multimap<int,RID>::key_type(tmpInt));
					for(ii=0;ii<iCount;++ii,++i2rIterator)
					{
						ridOut=(*i2rIterator).second;
						if((status=pages[(ridOut.getPageNo()-2)%MFORMSJION]->getRecord(ridOut,recOut))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
						if((status=InsertResult(result,projCnt,attrs,recMaxLen,attrOut,attrIn,recOut,recIn))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
					}
					break;
				case 3://float
					memcpy(&tmpFloat,(void*)((unsigned long)recIn.getRealData()+recIn.getOffset(attrIn.getAttrIdx())),
								SIZEOFFLOAT);
					if((fCount=f2rMap.count(std::multimap<float,RID>::key_type(tmpFloat)))==0)
						continue;
					f2rIterator=f2rMap.find(std::multimap<float,RID>::key_type(tmpFloat));
					for(f=0;f<fCount;++f,++f2rIterator)
					{
						ridOut=(*f2rIterator).second;
						if((status=pages[(ridOut.getPageNo()-2)%MFORMSJION]->getRecord(ridOut,recOut))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
						if((status=InsertResult(result,projCnt,attrs,recMaxLen,attrOut,attrIn,recOut,recIn))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
					}
					break;
				case 6://bool
					memcpy(&tmpBool,(void*)((unsigned long)recIn.getRealData()+recIn.getOffset(attrIn.getAttrIdx())),
								SIZEOFBOOL);
					if((fCount=b2rMap.count(std::multimap<bool,RID>::key_type(tmpBool)))==0)
						continue;
					b2rIterator=b2rMap.find(std::multimap<bool,RID>::key_type(tmpBool));
					for(b=0;b<bCount;++b,++b2rIterator)
					{
						ridOut=(*b2rIterator).second;
						if((status=pages[(ridOut.getPageNo()-2)%MFORMSJION]->getRecord(ridOut,recOut))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
						if((status=InsertResult(result,projCnt,attrs,recMaxLen,attrOut,attrIn,recOut,recIn))!=OK)
						{
							delete [] attrs;
							delete [] tmpChar;
							return status;
						}
					}
					break;
				default:
					delete [] attrs;
					delete [] tmpChar;
					return BADDATATYPE;
			}
			delete [] tmpChar;
		}
		//unpin these pages
		int g=j-1;
		for(;g>=MFORMSJION*(h-1);g--)
		{
			if((status=bufMgr->unPinPage(db,g,false))!=OK)
				break;
		}
		//clear the multimap
		s2rMap.clear();
		i2rMap.clear();
		f2rMap.clear();
		//get next page
		nextPage=pages[j-1]->getNextPageNo();
	}while(nextPage!=-1);
	delete [] attrs;
	return OK;
}*/
const Status PLT_Join(const std::string & result, 
		     const int projCnt, 
		     const AttrInfo projNames[],
		     const AttrInfo *attr1, 
		     const Operator op, 
		     const AttrInfo *attr2)
{
	Status status;
	//check op
	if(op==ISNULL||op==ISNOTNULL)
		return BADOPERATOR;
	//check attrs
	AttrDesc inAttr,outAttr;
	if((status=attrCat->getInfo(attr1->getRelName(),attr1->getAttrName(),outAttr))!=OK)
		return status;
	if((status=attrCat->getInfo(attr2->getRelName(),attr2->getAttrName(),inAttr))!=OK)
		return status;
	if(inAttr.getCanBeNull()||outAttr.getCanBeNull())
		return CANNOTNULL;
	//if (op!=EQ)
		return PLT_NL_Join(result, projCnt, projNames, outAttr, op, inAttr);
	//else
		//return PLT_SM_Join(result, projCnt, projNames, inAttr, op, outAttr);
}
//Inserts a record into the specified relation.
//Returns:
//OK on success
//an error code otherwise
//将给定的数据作为记录插入关系,属性的值由参数attrList提供,用户提供的字段和值的列表可以为任意顺序,
//插入前你必须按AttrCat中的顺序对其重新排序,可以用memcpy函数将他们按合适的偏移量拷贝到一个临时数
//组,然后调用insertRecord方法.如果value为NULL,应在数据字典中检查该字段是否可以为NULL.
const Status PLT_Insert(const std::string & relation, const int attrCnt, const AttrInfo attrList[])
{
	Status status;
	//check attrs
	int attrCnt2=0;
	AttrDesc* attrs=NULL;
	if((status=attrCat->getRelInfo(relation,attrCnt2,attrs))!=OK)
		return status;
	if(attrCnt2!=attrCnt)
	{
		delete [] attrs;
		return ATTRTYPEMISMATCH;
	}
	int recMaxLen=0;
	for(int t=0;t<attrCnt;t++)
		recMaxLen+=attrs[t].getAttrSize();
	AttrInPage *attrsInPage=new AttrInPage[attrCnt];
	char *tmpData=new char[recMaxLen];
	int offset=0;
	for(int i=0;i<attrCnt;i++)
	{
		int k=0;
		for(;k<attrCnt;k++)
		{
			if(attrs[i].getAttrName()==attrList[k].getAttrName())
				break;
		}
		if(!attrs[i].getCanBeNull() && attrList[k].getAttrInPage().isNull())
		{
			delete [] attrs;
			delete [] attrsInPage;
			delete [] tmpData;
			return ATTRTYPEMISMATCH;
		}
		if(attrs[i].getAttrSize()<attrList[k].getAttrInPage().getAttrLen())
		{
			delete [] attrs;
			delete [] attrsInPage;
			delete [] tmpData;
			return ATTRTYPEMISMATCH;
		}
		attrsInPage[i]=attrList[k].getAttrInPage();
		if(attrList[k].getAttrInPage().isNull())
		{
			attrsInPage[i].setNull(true);
			continue;
		}
		if(attrs[i].getAttrSize()!=attrList[k].getAttrInPage().getAttrLen() && 
			attrs[i].getAttrType()!=(unsigned short)P_VCHAR)
		{
			delete [] attrs;
			delete [] attrsInPage;
			delete [] tmpData;
			return ATTRTYPEMISMATCH;
		}
		int tmpInt=0;
		float tmpFloat=0;
		switch(attrs[i].getAttrType())
		{
			case 0: //P_CHAR
				memcpy(&tmpData[offset],attrList[i].getAttrValue(),attrsInPage[i].getAttrLen());
				break;
			case 1: //P_VCHAR
				memcpy(&tmpData[offset],attrList[i].getAttrValue(),attrsInPage[i].getAttrLen());
				break;
			case 2: //P_INTEGER
				tmpInt=atoi((char*)attrList[i].getAttrValue());
				memcpy(&tmpData[offset],&tmpInt,SIZEOFINT);
				break;
			case 3: //P_FLOAT
				tmpFloat=atof((char*)attrList[i].getAttrValue());
				memcpy(&tmpData[offset],&tmpFloat,SIZEOFFLOAT);
				break;
			case 4: //P_DATE
				memcpy(&tmpData[offset],attrList[i].getAttrValue(),attrsInPage[i].getAttrLen());
				break;
			case 5: //P_DATETIME
				memcpy(&tmpData[offset],attrList[i].getAttrValue(),attrsInPage[i].getAttrLen());
				break;
			case 6: //P_BOOLEAN
				memcpy(&tmpData[offset],attrList[i].getAttrValue(),attrsInPage[i].getAttrLen());
				break;
			case 7: //P_BLOB
				memcpy(&tmpData[offset],attrList[i].getAttrValue(),attrsInPage[i].getAttrLen());
				break;
			case 8: //P_CLOB
				memcpy(&tmpData[offset],attrList[i].getAttrValue(),attrsInPage[i].getAttrLen());
				break;
			default:
				cout<<"wrong attrType!!!"<<endl;
				delete [] attrs;
				delete [] attrsInPage;
				delete [] tmpData;
				return DUPLATTR; //need a new error type !!!
		}
		offset+=attrsInPage[i].getAttrLen();
	}
	Record newRec;
	newRec.setAttrsInPage(attrsInPage,attrCnt);
	newRec.setRealData((void*)tmpData);
	//insert the record
	InsertOperator* insertOp=new InsertOperator(db->getDBName(),relation,status);
	if(status!=OK)
	{
		delete [] attrs;
		delete [] attrsInPage;
		delete [] tmpData;
		return status;
	}
	RID rid;
	if((status=insertOp->insertRecord(newRec,rid))!=OK)
	{
		delete [] attrs;
		delete [] attrsInPage;
		delete [] tmpData;
		delete insertOp;
		return status;
	}
	delete [] attrs;
	delete [] attrsInPage;
	delete [] tmpData;
	delete insertOp;
	cout<<"1 record inserted!"<<endl;
	return OK;
}
//Deletes records from a specified relation.
//Returns:
//OK on success
//an error code otherwise
const Status PLT_Delete(const std::string & relation, 
		       const std::string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue)
{
	if (relation.empty()) return BADCATPARM;
	Status status;
	AttrDesc attr;
	int tmpInt=0;
	float tmpFloat=0;
	int recLength=0;
	char *tmpData=NULL;
	int datatype=P_CHAR;
	if(!attrName.empty()&&(op==ISNULL||op==ISNOTNULL))
	{
		AttrCatalog* attrCat=new AttrCatalog(db->getDBName(),status);
		if(status!=OK) return status;
		if((status=attrCat->getInfo(relation,attrName,attr))!=OK)
		{
			delete attrCat;
			return status;
		}
		delete attrCat;
		datatype=attr.getAttrType();
	}
	else if(!attrName.empty())
	{
		if(attrValue==NULL&&(op!=ISNULL||op!=ISNOTNULL))
			return BADVALUE;
		//get the attribute information from attrCat
		AttrCatalog* attrCat=new AttrCatalog(db->getDBName(),status);
		if(status!=OK) return status;
		if((status=attrCat->getInfo(relation,attrName,attr))!=OK)
		{
			delete attrCat;
			return status;
		}
		delete attrCat;
		if(attr.getAttrType()!=type)
			return ATTRTYPEMISMATCH;
		tmpData=new char[attr.getAttrSize()];
		switch(type)
		{
			case 0: //P_CHAR
				datatype=P_CHAR;
				recLength=strlen(attrValue);
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 1: //P_VCHAR
				datatype=P_VCHAR;
				recLength=strlen(attrValue);
				memcpy(&tmpData[0],attrValue,recLength);
				break;
			case 2: //P_INTEGER
				datatype=P_INTEGER;
				recLength=SIZEOFINT;
				tmpInt=atoi(attrValue);
				memcpy(&tmpData[0],&tmpInt,attr.getAttrSize());
				break;
			case 3: //P_FLOAT
				datatype=P_FLOAT;
				recLength=SIZEOFFLOAT;
				tmpFloat=atof(attrValue);
				memcpy(&tmpData[0],&tmpFloat,attr.getAttrSize());
				break;
			case 4: //P_DATE
				datatype=P_DATE;
				recLength=SIZEOFDATE;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 5: //P_DATETIME
				datatype=P_DATETIME;
				recLength=SIZEOFDATETIME;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 6: //P_BOOLEAN
				datatype=P_BOOLEAN;
				recLength=SIZEOFBOOL;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 7: //P_BLOB
				datatype=P_BLOB;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 8: //P_CLOB
				datatype=P_CLOB;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			default:
				cout<<"wrong attrType!!!"<<endl;
				delete [] tmpData;
				return DUPLATTR; //need a new error type !!!
		}
	}
	//open the table
	ScanOperator* scanOp=new ScanOperator(db->getDBName(),relation,status);
	if(status!=OK) return status;
	if((status=scanOp->startScan(attr.getAttrIdx(),recLength,(Datatype)datatype,tmpData,op))!=OK)
	{
		delete scanOp;
		return status;
	}
	int deletedNum=0;
	RID rid;
	//delete record(s)
	while(scanOp->scanNext(rid)==OK)
	{
		if((status=scanOp->deleteRecord())!=OK)
		{
			delete scanOp;
			return status;
		}
		deletedNum++;
	}
	delete scanOp;
	cout<<endl<<deletedNum<<" record(s) deleted!"<<endl;
	return OK;
}
//Updates records from a specified relation.
//Returns:
//OK on success
//an error code otherwise
const Status PLT_Update(const std::string & relation,
                       const int attrCnt,
                       const AttrInfo attrList[],
                       const std::string & attrName,
                       const Operator op,
                       const Datatype type,
                       const char *attrValue)
{
	if (relation.empty()) return BADCATPARM;
    Status status;
	AttrDesc* attrs=NULL;
	int attrTotalCnt=0;
	if((status=attrCat->getRelInfo(relation,attrTotalCnt,attrs))!=OK)
		return status;
	int recMaxLen=0;
	for(int t=0;t<attrTotalCnt;t++)
		recMaxLen+=attrs[t].getAttrSize();
	//check update values' are legal
	for(int y=0;y<attrCnt;y++)
	{
		int j=0;
		for(;j<attrTotalCnt;j++)
		{
			if(attrs[j].getAttrName()==attrList[y].getAttrName())
				break;
		}
		if(!attrs[j].getCanBeNull() && attrList[y].getAttrInPage().isNull())
		{delete [] attrs;return ATTRTYPEMISMATCH;}
		if(!attrList[y].getAttrInPage().isNull())
		{
			if(attrList[y].getAttrInPage().getAttrLen()>attrs[j].getAttrSize())
			{delete [] attrs;return ATTRTYPEMISMATCH;}
		}
	}
	int tmpInt=0;
	float tmpFloat=0;
	int recLength=0;
	char *tmpData=NULL;
	int datatype=P_CHAR;
	AttrDesc attr;
	//get the filter attr
	if(!attrName.empty()&&(op==ISNULL||op==ISNOTNULL))
	{
		if((status=attrCat->getInfo(relation,attrName,attr))!=OK)
		{
			delete [] attrs;
			return status;
		}
		datatype=attr.getAttrType();
	}
	else if(!attrName.empty())
	{
		if(attrValue==NULL&&(op!=ISNULL||op!=ISNOTNULL))
		{
			delete [] attrs;
			return BADVALUE;
		}
		//get the attribute information from attrCat
		if((status=attrCat->getInfo(relation,attrName,attr))!=OK)
		{
			delete [] attrs;
			return status;
		}
		if(attr.getAttrType()!=type)
		{
			delete [] attrs;
			return ATTRTYPEMISMATCH;
		}
		tmpData=new char[attr.getAttrSize()];
		switch(type)
		{
			case 0: //P_CHAR
				datatype=P_CHAR;
				recLength=strlen(attrValue);
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 1: //P_VCHAR
				datatype=P_VCHAR;
				recLength=strlen(attrValue);
				memcpy(&tmpData[0],attrValue,recLength);
				break;
			case 2: //P_INTEGER
				datatype=P_INTEGER;
				recLength=SIZEOFINT;
				tmpInt=atoi(attrValue);
				memcpy(&tmpData[0],&tmpInt,attr.getAttrSize());
				break;
			case 3: //P_FLOAT
				datatype=P_FLOAT;
				recLength=SIZEOFFLOAT;
				tmpFloat=atof(attrValue);
				memcpy(&tmpData[0],&tmpFloat,attr.getAttrSize());
				break;
			case 4: //P_DATE
				datatype=P_DATE;
				recLength=SIZEOFDATE;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 5: //P_DATETIME
				datatype=P_DATETIME;
				recLength=SIZEOFDATETIME;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 6: //P_BOOLEAN
				datatype=P_BOOLEAN;
				recLength=SIZEOFBOOL;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 7: //P_BLOB
				datatype=P_BLOB;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			case 8: //P_CLOB
				datatype=P_CLOB;
				memcpy(&tmpData[0],attrValue,attr.getAttrSize());
				break;
			default:
				cout<<"wrong attrType!!!"<<endl;
				delete [] attrs;
				delete [] tmpData;
				return DUPLATTR; //need a new error type !!!
		}
	}
	//open the table
	ScanOperator* scanOp=new ScanOperator(db->getDBName(),relation,status);
	if(status!=OK)
	{
		delete [] attrs;
		delete [] tmpData;
		return status;
	}
	if((status=scanOp->startScan(attr.getAttrIdx(),recLength,type,tmpData,op))!=OK)
	{
		delete [] attrs;
		delete [] tmpData;
		delete scanOp;
		return status;
	}
	//get the update attrs
	AttrDesc *upattrs=new AttrDesc[attrCnt];
	for(int i=0;i<attrCnt;i++)
	{
		if((status=attrCat->getInfo(relation,attrList[i].getAttrName(),upattrs[i]))!=OK)
		{
			delete [] attrs;
			delete [] tmpData;
			delete scanOp;
			delete [] upattrs;
			return status;
		}
	}
	//scan the table
	RID rid;
	Record oldRec;
	int updatedNum=0;
	while(scanOp->scanNext(rid)==OK)
	{
		if((status=scanOp->getRecord(oldRec))!=OK)
		{
			delete [] attrs;
			delete [] tmpData;
			delete scanOp;
			delete [] upattrs;
			return status;
		}
		//set the new record
		Record newRec;
		AttrInPage* newAttrInPage=new AttrInPage[attrTotalCnt];
		char *tmpNewData=new char[recMaxLen];
		int offset=0;
		for(int i=0;i<attrTotalCnt;i++)
		{
			int k=0;
			for(;k<attrCnt;k++)
			{
				if(attrs[i].getAttrName()==attrList[k].getAttrName())
					break;
			}
			if(k==attrCnt)//don't update this attr
			{
				if((status=oldRec.getAttrInPage(i,newAttrInPage[i]))!=OK)
				{
					delete [] attrs;
					delete [] tmpData;
					delete scanOp;
					delete [] upattrs;
					delete [] newAttrInPage;
					delete [] tmpNewData;
					return status;
				}
				if(!newAttrInPage[i].isNull())
				{
					memcpy((void*)&tmpNewData[offset],
					(void*)((unsigned long)oldRec.getRealData()+oldRec.getOffset(i)),
					newAttrInPage[i].getAttrLen());
					offset+=newAttrInPage[i].getAttrLen();
				}
			}
			else//update this attr
			{
				if(attrList[k].getAttrInPage().isNull())
				{
					newAttrInPage[i].setNull(true);
					continue;
				}
				else
				{
					newAttrInPage[i]=attrList[k].getAttrInPage();
					switch(attrs[i].getAttrType())
					{
						case 0: //P_CHAR
							memcpy(&tmpNewData[offset],attrList[k].getAttrValue(),strlen((char*)attrList[k].getAttrValue()));
							newAttrInPage[i].setLen(strlen((char*)attrList[k].getAttrValue()));
							break;
						case 1: //P_VCHAR
							memcpy(&tmpNewData[offset],attrList[k].getAttrValue(),strlen((char*)attrList[k].getAttrValue()));
							newAttrInPage[i].setLen(strlen((char*)attrList[k].getAttrValue()));
							break;
						case 2: //P_INTEGER
							tmpInt=atoi((char*)attrList[k].getAttrValue());
							memcpy(&tmpNewData[offset],&tmpInt,SIZEOFINT);
							newAttrInPage[i].setLen(SIZEOFINT);
							break;
						case 3: //P_FLOAT
							tmpFloat=atof((char*)attrList[k].getAttrValue());
							memcpy(&tmpNewData[offset],&tmpFloat,SIZEOFFLOAT);
							newAttrInPage[i].setLen(SIZEOFFLOAT);
							break;
						case 4: //P_DATE
							memcpy(&tmpNewData[offset],attrList[k].getAttrValue(),newAttrInPage[i].getAttrLen());
							newAttrInPage[i].setLen(SIZEOFDATE);
							break;
						case 5: //P_DATETIME
							memcpy(&tmpNewData[offset],attrList[k].getAttrValue(),newAttrInPage[i].getAttrLen());
							newAttrInPage[i].setLen(SIZEOFDATETIME);
							break;
						case 6: //P_BOOLEAN
							memcpy(&tmpNewData[offset],attrList[k].getAttrValue(),SIZEOFBOOL);
							newAttrInPage[i].setLen(SIZEOFBOOL);
							break;
						case 7: //P_BLOB
							memcpy(&tmpNewData[offset],attrList[k].getAttrValue(),newAttrInPage[i].getAttrLen());
							newAttrInPage[i].setLen(SIZEOFINT);
							break;
						case 8: //P_CLOB
							memcpy(&tmpNewData[offset],attrList[k].getAttrValue(),newAttrInPage[i].getAttrLen());
							newAttrInPage[i].setLen(SIZEOFINT);
							break;
						default:
							cout<<"wrong attrType!!!"<<endl;
							delete [] attrs;
							delete [] tmpData;
							delete scanOp;
							delete [] upattrs;
							delete [] newAttrInPage;
							delete [] tmpNewData;
							return DUPLATTR; //need a new error type !!!
					}
				}
				offset+=newAttrInPage[i].getAttrLen();
			}
		}
		newRec.setAttrsInPage(newAttrInPage,attrTotalCnt);
		newRec.setRealData((void*)tmpNewData);
		//updata the new record
		if((status=scanOp->updateRecord(newRec))!=OK)
		{
			delete [] attrs;
			delete [] tmpData;
			delete scanOp;
			delete [] upattrs;
			delete [] newAttrInPage;
			delete [] tmpNewData;
			return status;
		}
		updatedNum++;
		delete [] newAttrInPage;
		delete [] tmpNewData;
	}
	delete [] attrs;
	delete [] tmpData;
	delete scanOp;
	delete [] upattrs;	
	cout<<endl<<updatedNum<<" record(s) updated!"<<endl;
	return OK;
}
