#include "query.h"
//
// Prototypes for query layer functions
//
const Status QU_Select(const std::string & result, 
		       const int projCnt, 
		       const AttrInfo projNames[],
		       const AttrInfo *attr, 
		       const Operator op, 
		       const char *attrValue)
{
	return OK;
}

const Status QU_Join(const std::string & result, 
		     const int projCnt, 
		     const AttrInfo projNames[],
		     const AttrInfo *attr1, 
		     const Operator op, 
		     const AttrInfo *attr2)
{
	return OK;
}

const Status QU_Insert(const std::string & relation, 
		       const int attrCnt, 
		       const AttrInfo attrList[])
{
	return OK;
}

const Status QU_Delete(const std::string & relation, 
		       const std::string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue)
{
	return OK;
}

const Status QU_Update(const std::string & relation,
                       const int attrCnt,
                       const AttrInfo attrList[],
                       const std::string & attrName,
                       const Operator op,
                       const Datatype type,
                       const char *attrValue)
{
	return OK;
}
