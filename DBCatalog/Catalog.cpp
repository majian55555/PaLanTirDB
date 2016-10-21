#include "Catalog.h"
#include "utility.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of class RelCatalog.//////////////////////////////////////////////////////////////////
const Status RelCatalog::getInfo(const std::string & relation, RelDesc &record)
{
	if(this->db==NULL)
		return RELCATNOTOPEN;
	if (relation.empty())
		return BADCATPARM;

	Status status;
	Record rec;
	RID rid;
	ScanOperator * scan=new ScanOperator(this->db->getDBName(),RELCATNAME,status);
	if (status != OK) return status;

	if((scan->startScan(0,relation.length(),P_VCHAR,relation.c_str(),EQ))!=OK)
		return status;

	status=scan->scanNext(rid);
	if (status == FILEEOF) return RELNOTFOUND;
	if ((status != FILEEOF) && (status != OK)) return status;

	if((status=scan->getRecord(rec))!=OK)
		return status;

	AttrInPage attrInPage[2];
	if((status=rec.getAttrInPage(0,attrInPage[0]))!=OK)
		return status;
	record.setRelName((char*)rec.getRealData(),attrInPage[0].getAttrLen());
	//record.relName.assign((char*)rec.getRealData(),attrInPage[0].getAttrLen());
	if((status=rec.getAttrInPage(1,attrInPage[1]))!=OK)
		return status;
	ASSERT(attrInPage[1].getAttrLen()==SIZEOFINT);
	record.setAttrCnt(*((int*)((unsigned long)rec.getRealData()+attrInPage[0].getAttrLen())));

	scan->endScan();
	delete scan;
	scan = NULL;
	return OK;
}
const Status RelCatalog::addInfo(RelDesc &record)
{
	if(this->db==NULL)
		return RELCATNOTOPEN;
	if (record.getRelName().empty()||record.getRelName().length()>MAXNAMESIZE)
		return BADCATPARM;

	RID rid;
	Status status;
	InsertOperator* ifs=new InsertOperator(this->db->getDBName(),RELCATNAME,status);
	if (status != OK) return status;

	Record Relrec;
	AttrInPage attrInPage[2];
	attrInPage[0].setLen(record.getRelName().length());
	attrInPage[1].setLen(SIZEOFINT);
	if((status=Relrec.setAttrsInPage(attrInPage,2))!=OK)
		return status;
	char tmpData[MAXNAMESIZE+SIZEOFINT];
	memset((void*)tmpData,0,MAXNAMESIZE+SIZEOFINT);
	strcpy(&tmpData[0],record.getRelName().c_str());
	memcpy((void*)((unsigned long)tmpData+record.getRelName().length()),(void*)&(record.attrCnt),SIZEOFINT);
	Relrec.setRealData((void*)tmpData);
	
	if((status = ifs->insertRecord(Relrec,rid))!=OK)
		return status; 

	delete ifs;
	return OK;
}
const Status RelCatalog::removeInfo(const std::string & relation)
{
	if(this->db==NULL)
		return RELCATNOTOPEN;
	if (relation.empty()) return BADCATPARM;

	Status status;
	RID rid;

	ScanOperator* scanOp = new ScanOperator(this->db->getDBName(),RELCATNAME,status);
	if (status != OK) return status;
	if((status=scanOp->startScan(0,relation.length(),P_VCHAR,relation.c_str(),EQ))!=OK)
		return status;
	if((status=scanOp->scanNext(rid))!=OK)
		return status;
	if((status=scanOp->deleteRecord())!=OK)
		return status;
	scanOp->endScan();
	delete scanOp;

	return OK;
}

const Status RelCatalog::help(const std::string & relation)
{
	if (relation.empty()) //no relation specified
		return PLT_Print(this->db->getDBName(),RELCATNAME);
	else // relation specified
	{
		Status status;
		RelDesc rd;
		AttrDesc *attrs;
		int attrCnt;
		//check relcat to make sure the relation exists
		status = relCat->getInfo(relation,rd);
		if (status == FILEEOF) return RELNOTFOUND;
		if ((status != OK) && (status != FILEEOF)) return status;

		//check attrcat to make sure the relation attributes entry exists
		if((status = attrCat->getRelInfo(relation,attrCnt,attrs))!=OK)
			return status;

		//print header of the relation
		printf("%-*.*s  ", 20, 20, "Table Name");
		printf("%-*.*s  ", 20, 20 ,"Attribute Name");
		printf("%-*.*s  ", 6, 6, "Index");
		printf("%-*.*s  ", 6, 6 ,"Type");
		printf("%-*.*s  ", 6, 6, "Size");
		printf("\n");
		printf("%-*.*s  ", 20, 20, "--------------------");
		printf("%-*.*s  ", 20, 20, "--------------------");
		printf("%-*.*s  ", 6, 6, "------");
		printf("%-*.*s  ", 6, 6, "------");
		printf("%-*.*s  ", 6, 6, "------");
		printf("\n");

		//print tuples
		char *att_type;
		for(int i = 0; i < attrCnt; i++)
		{
			printf("%-*.*s  ", 20, 20, attrs[i].getRelName().c_str());
			printf("%-*.*s  ", 20, 20, attrs[i].getAttrName().c_str());
			printf("%-*d  ",  6, attrs[i].getAttrIdx());
			//P_CHAR=0, P_VCHAR=1, P_INTEGER=2, P_FLOAT=3, P_DATE=4, P_DATETIME=5,
			//P_BOOLEAN=6, P_BLOB=7, P_CLOB=8
			switch (attrs[i].getAttrType())
			{
				case 0 : 
					att_type="P_CHAR";
					break;
				case 1 :
					att_type="P_VCHAR";
					break;
				case 2 :
					att_type="P_INTEGER";
					break;
				case 3 :
					att_type="P_FLOAT";
					break;
				case 4 :
					att_type="P_DATE";
					break;
				case 5 :
					att_type="P_DATETIME";
					break;
				case 6 :
					att_type="P_BOOLEAN";
					break;
				case 7 :
					att_type="P_BLOB";
					break;
				case 8 :
					att_type="P_CLOB";
					break;
				default:
					delete [] attrs;
					return BADDATATYPE;
			}
			printf("%-*.*s  ",6,  6, att_type);
			printf("%-*d  ",  6, attrs[i].getAttrSize());
			printf("\n");
		}
		delete [] attrs;
	}
	return OK;
}
const Status RelCatalog::RelCat_createRel(const std::string& relation,const int attrCnt,const AttrInfo attrList[])
{
	if (relation.empty() || attrCnt < 1)
		return BADCATPARM;
	if (relation.length()>MAXNAMESIZE)
		return NAMETOOLONG;

	Status status;
	RelDesc rd;
	AttrDesc ad;
	
	status=this->getInfo(relation,rd);
	if(status==OK)  return RELEXISTS;
	if((status!=OK) && (status!=RELNOTFOUND)) return status;

	//check duplicated attributes
	bool duplicated = false;
	for(int i=0; i<attrCnt-1; i++)
	{
		for(int j=i+1;j<attrCnt;j++)
		{
			if(attrList[i].getAttrName()==attrList[j].getAttrName())
			{
				duplicated=true;
				break;
			}
		}
	}
	if (duplicated==true)
		return DUPLATTR;

	//add catalog entry into relcat
	rd.setRelName(relation);
	rd.setAttrCnt(attrCnt);
	if((status = this->addInfo(rd))!=OK)
		return status;

	//add catalog entrys into attrcat
	for (int t=0; t<attrCnt;t++)
	{
		ad.setRelName(attrList[t].getRelName());  // relation name
		ad.setAttrName(attrList[t].getAttrName()); // attribute name
		ad.setAttrIdx(t); // attribute offset
		ad.setAttrType(attrList[t].getAttrType()); // attribute type
		ad.setAttrSize(attrList[t].getAttrSize()); // attribute lengt
		if((status = attrCat->addInfo(ad))!=OK)
			return status;
	}

	//create a relation
	if((status = createRel(this->db->getDBName(),relation))!=OK)
		return status;
	if(relation!="Tmp_PaLanTir_Result")
	{
		cout << endl;
		cout << "table " << relation.c_str() << " created!" << endl;
	}
	return OK;
}

const Status RelCatalog::RelCat_destroyRel(const std::string& relation)
{
	Status status;

	if (relation.empty() || relation == RELCATNAME || relation == ATTRCATNAME)
		return BADCATPARM;

	//destroys the heap file containing the tuples in the relation
	if((status = destroyRel(this->db->getDBName(),relation))!=OK)
		return status;

	//get relation name from catalog(relcat)
	RelDesc record;

	status = getInfo(relation,record);
	if (status == FILEEOF) return RELNOTFOUND;
	if ((status != OK) && (status != FILEEOF)) return status;

	//remove the catalog entry of the relation from attrcat
	if((status = attrCat->dropRelation(relation))!=OK)
		return status;

	//remove the catalog entry of the relation from relcat
	if((status = this->removeInfo(relation))!=OK)
		return status;
	if(relation!="Tmp_PaLanTir_Result")
	{
		cout << endl;
		cout << "relation " << relation.c_str() << " dropped successfully!" << endl;
	}
	return OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Implementation of class AttrCatalog./////////////////////////////////////////////////////////////////
const Status AttrCatalog::getInfo(const std::string & relation,const std::string & attrName,AttrDesc &record)
{
	if(this->db==NULL)
		return ATTCATNOTOPEN;
	if (relation.empty() || attrName.empty()) return BADCATPARM;

	Status status;
	RID rid;
	Record rec;
	ScanOperator*  scanOp=new ScanOperator(this->db->getDBName(),RELCATNAME,status);
	if (status != OK) return status;

	if((status=scanOp->startScan(0,relation.length(),P_VCHAR,relation.c_str(),EQ))!=OK)
		return status;
	while((status=scanOp->scanNext(rid))==OK)
	{
		if((status=scanOp->getRecord(rec))!=OK)
			return status;
		if(strncmp((char*)((unsigned long)rec.getRealData()+relation.length()),attrName.c_str(),attrName.length())==0)
		{
			record.setRelName(relation);
			record.setAttrName(attrName);
			record.setAttrIdx(*(int*)((unsigned long)rec.getRealData()+relation.length()+attrName.length()));
			record.setAttrType(*(int*)((unsigned long)rec.getRealData()+relation.length()+attrName.length()+SIZEOFINT));
			record.setAttrSize(*(int*)((unsigned long)rec.getRealData()+relation.length()+attrName.length()+2*SIZEOFINT));
			break;
		}
	}
	if(status!=FILEEOF || status!=OK) return status;
	scanOp->endScan();
	delete scanOp;
	return OK;
}
const Status AttrCatalog::addInfo(AttrDesc & record)
{
	if(this->db==NULL)
		return ATTCATNOTOPEN;
	if (record.getRelName().empty() || record.getAttrName().empty()) return BADCATPARM;

	RID rid;
	Status status;
	AttrInPage attrInPage[5];
	Record AttrRec;

	attrInPage[0].setLen(record.getRelName().length());
	attrInPage[1].setLen(record.getAttrName().length());
	attrInPage[2].setLen(SIZEOFINT);
	attrInPage[3].setLen(SIZEOFINT);
	attrInPage[4].setLen(SIZEOFINT);
	char tmpData[2*MAXNAMESIZE+3*SIZEOFINT];
	memset((void*)tmpData,0,2*MAXNAMESIZE+3*SIZEOFINT);
	strcpy(&tmpData[0],record.relName.c_str());
	strcpy(&tmpData[relName.length()],record.attrName.c_str());
	memcpy((void*)(&tmpData[record.relName.length()+record.attrName.length()]),(void*)&(record.attrIdx),3*SIZEOFINT);
	InsertOperator* insertOp = new InsertOperator(this->db->getDBName(),ATTRCATNAME,status);
	if (status != OK) return status;

	if((status=insertOp->insertRecord(AttrRec,rid))!=OK)
		return status;

	delete insertOp;
	return OK;
}
const Status AttrCatalog::removeInfo(const std::string& relation,const std::string& attrName)
{
	if(this->db==NULL)
		return ATTCATNOTOPEN;
	if (relation.empty() || attrName.empty()) return BADCATPARM;

	Status status;
	Record rec;
	RID rid;
	AttrDesc record;

	ScanOperator* scanOp= new ScanOperator(this->db->getDBName(),ATTRCATNAME,status);
	if (status != OK) return status;

	if((status=scanOp->startScan(0,relation.length(),P_VCHAR,relation.c_str(),EQ))!=OK)
		return status;
	while((status=scanOp->scanNext(rid))==OK)
	{
		if((status=scanOp->getRecord(rec))!=OK)
			return status;
		if(strncmp((char*)((unsigned long)rec.getRealData()+relation.length()),attrName.c_str(),attrName.length())==0)
		{
			if((status=scanOp->deleteRecord())!=OK)
				return status;
			break;
		}
	}
	if(status!=FILEEOF || status!=OK) return status;
	scanOp->endScan();
	delete scanOp;
	return OK;
}
const Status AttrCatalog::getRelInfo(const std::string & relation, int &attrCnt,AttrDesc *&attrs)
{
	if(this->db==NULL)
		return ATTCATNOTOPEN;
	if (relation.empty()) return BADCATPARM;

	Status status;
	RID rid;
	Record rec;
	RelDesc rel;
	if (relation.empty()) return BADCATPARM;

	if((status = relCat->getInfo(relation,rel))!=OK)
		return status;

	ScanOperator* scanOp = new ScanOperator(this->db->getDBName(),ATTRCATNAME,status);
	if((status=scanOp->startScan(0,relation.length(),P_VCHAR,relation.c_str(),EQ))!=OK)
		return status;
	attrCnt=rel.getAttrNum();
	attrs=new AttrDesc[attrCnt]; //remember when these will be deleted
	for(int i=0;i<attrCnt;i++)
	{
		if((status=scanOp->scanNext(rid))!=OK)
		{
			delete scanOp;
			return status;
		}
		if((status=scanOp->getRecord(rec))!=OK)
		{
			delete scanOp;
			return status;
		}
		AttrInPage attrInPage;
		if((status=rec.getAttrInPage(1,attrInPage))!=OK)
		return status;
		attrs[i].relName=relation;
		attrs[i].attrName.assign((char*)((unsigned long)rec.getRealData()+relation.length()),attrInPage.getAttrLen());
		attrs[i].attrIdx=*(int*)((unsigned long)rec.getRealData()+relation.length()+attrInPage.getAttrLen());
		attrs[i].attrType=*(int*)((unsigned long)rec.getRealData()+relation.length()+attrInPage.getAttrLen()+SIZEOFINT);
		attrs[i].attrSize=*(int*)((unsigned long)rec.getRealData()+relation.length()+attrInPage.getAttrLen()+2*SIZEOFINT);
	}
	ASSERT(scanOp->scanNext(rid)==FILEEOF);
	scanOp->endScan();
	delete scanOp;
	return OK;
}
const Status AttrCatalog::dropRelation(const std::string & relation)
{
	if (relation.empty())
		return BADCATPARM;

	Status status;
	AttrDesc *attrs=NULL;
	int attrCnt=0;

	//get attributes infomation of the relation
	if((status = getRelInfo(relation,attrCnt,attrs))!=OK)
		return status;
	//removes the catalog entries in attrcat of the relation
	for (int i=0;i<attrCnt;i++)
	{
		status = this->removeInfo(relation,attrs[i].getAttrName());
		if (status != OK) return status;
	}
	delete [] attrs;
	return OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////