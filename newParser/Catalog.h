#ifndef CATALOG_H
#define CATALOG_H
#include "RelManager.h"
const std::string RELCATNAME="relcat";           // name of relation catalog
const std::string ATTRCATNAME="attrcat";         // name of attribute catalog
const unsigned short MAXSTRINGLEN=255;                // max. length of string attribute

// schema of relation catalog:
//   relation name : P_VCHAR(32)           <-- lookup key
//   attribute count : P_INTEGER(4)
class RelDesc
{
	friend class RelCatalog;
private:
	std::string relName;            // relation name
	int attrCnt;                    // number of attributes
public:
	RelDesc():relName(),attrCnt(0){}
	const std::string getRelName() const {return this->relName;}
	const int getAttrNum() const {return this->attrCnt;}
	void setRelName(const std::string& relname){this->relName.assign(relname);}
	void setRelName(const char* relname,int len){this->relName.assign(relname,len);}
	void setAttrCnt(int attrNum){this->attrCnt=attrNum;}
};

class AttrInfo
{
private:
	std::string relName;            // relation name
	std::string attrName;           // attribute name
	int attrType;                        // P_CHAR=0, P_VCHAR=1, P_INTEGER=2, P_FLOAT=3, P_DATE=4, P_DATETIME=5,
										  //P_BOOLEAN=6, P_BLOB=7, P_CLOB=8
	AttrInPage attrInPage;               // attrInPage
	int attrSize;               //
	bool canBeNull;
	void *attrValue;                      // ptr to binary value
public:
	AttrInfo():relName(),attrName(),attrType(0),attrInPage(),attrSize(0),canBeNull(true),attrValue(NULL){}
	const std::string getRelName() const {return this->relName;}
	const std::string getAttrName() const {return this->attrName;}
	const int getAttrType() const {return this->attrType;}
	const AttrInPage getAttrInPage() const {return this->attrInPage;}
	const int getAttrSize() const {return this->attrSize;}
	const bool getCanBeNull() const {return this->canBeNull;}
	void* getAttrValue() const {return this->attrValue;}

	void setRelName(const char* relname){this->relName.assign(relname);}
	void setRelName(const std::string& relname){this->relName.assign(relname);}
	void setAttrName(const char* attrname){this->attrName.assign(attrname);}
	void setAttrName(const std::string& attrname){this->attrName.assign(attrname);}
	void setAttrType(int attrtype){this->attrType=attrtype;}
	void setAttrInPage(const AttrInPage& attrinpage){this->attrInPage=attrinpage;}
	void setAttrSize(int attrsize){this->attrSize=attrsize;}
	void setCanBeNull(bool isCanBeNull){this->canBeNull=isCanBeNull;}
	void setAttrValue(void* value){this->attrValue=value;}
}; 

class RelCatalog : public RelOperator
{
public:
	// open relation catalog
	RelCatalog::RelCatalog(const std::string& dbName,Status& status):RelOperator(dbName,RELCATNAME,status){}
	// get relation descriptor for a relation
	const Status getInfo(const std::string& relation, RelDesc& record);
	// add information to catalog
	const Status addInfo(RelDesc& record);
	// remove tuple from catalog
	const Status removeInfo(const std::string& relation);

	// create a new relation
	const Status RelCat_createRel(const std::string& relation,const int attrCnt,const AttrInfo attrList[]);

	// destroy a relation
	const Status RelCat_destroyRel(const std::string& relation);

	// print catalog information
	const Status help(const std::string& relation);          // relation may be NULL

	// get rid of catalog
	~RelCatalog(){}
};

// schema of attribute catalog:
//   relation name : P_VCHAR(32)           <-- lookup keys
//   attribute name : P_VCHAR(32)          <--
//   attribute index : P_INTEGER(4)
//   attribute type : P_INTEGER(4)  (type is Datatype actually)
//   attribute size : P_INTEGER(4)

class AttrDesc
{
	friend class AttrCatalog;
private:
	std::string relName;             // relation name
	std::string attrName;            // attribute name
	int attrIdx;				     // attribute index
	int attrType;                   // P_CHAR=0, P_VCHAR=1, P_INTEGER=2, P_FLOAT=3, P_DATE=4, P_DATETIME=5,
								     //P_BOOLEAN=6, P_BLOB=7, P_CLOB=8
	int attrSize;                //max length of attribute if datatype is P_VCHAR()
	bool canBeNull;              //this attribute can be null or not
public:
	AttrDesc():relName(),attrName(),attrIdx(0),attrType(0),attrSize(0),canBeNull(true){}

	const std::string getRelName() const {return this->relName;}
	const std::string getAttrName() const {return this->attrName;}
	const int getAttrIdx() const {return this->attrIdx;}
	const int getAttrType() const {return this->attrType;}
	const int getAttrSize() const {return this->attrSize;}
	const bool getCanBeNull() const {return this->canBeNull;}

	void setRelName(const std::string& relname){this->relName.assign(relname);}
	void setAttrName(const std::string& attrname){this->attrName.assign(attrname);}
	void setAttrIdx(int attrNo){this->attrIdx=attrNo;}
	void setAttrType(int attrtype){this->attrType=attrtype;}
	void setAttrSize(int attrsize){this->attrSize=attrsize;}
	void setCanBeNull(bool isCanBeNull){this->canBeNull=isCanBeNull;}
};

class AttrCatalog : public RelOperator
{
	friend class RelCatalog;
public:
	// open attribute catalog
	AttrCatalog(const std::string &dbName,Status &status):RelOperator(dbName,RELCATNAME,status){}

	// get attribute catalog tuple
	const Status getInfo(const std::string& relation,const std::string& attrName,AttrDesc &record);

	// add information to catalog
	const Status addInfo(AttrDesc & record);

	// remove tuple from catalog
	const Status removeInfo(const std::string& relation, const std::string& attrName);

	// get all attributes of a relation
	const Status getRelInfo(const std::string& relation,int &attrCnt,AttrDesc *&attrs);

	// delete all information about a relation
	const Status dropRelation(const std::string& relation);

	// close attribute catalog
	~AttrCatalog(){}
};

#endif