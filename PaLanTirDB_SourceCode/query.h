#ifndef QUERY_H
#define QUERY_H

#include "Catalog.h"

enum JoinType {NLJoin, SMJoin, HashJoin};

//
// Prototypes for query layer functions
//


const Status PLT_Select(const std::string & result, 
		       const int projCnt, 
		       const AttrInfo projNames[],
		       const AttrInfo *attr, 
		       const Operator op, 
		       const char *attrValue);

const Status PLT_Join(const std::string & result, 
		     const int projCnt, 
		     const AttrInfo projNames[],
		     const AttrInfo *attr1, 
		     const Operator op, 
		     const AttrInfo *attr2);

const Status PLT_Insert(const std::string & relation, 
		       const int attrCnt, 
		       const AttrInfo attrList[]);

const Status PLT_Delete(const std::string & relation, 
		       const std::string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue);

const Status PLT_Update(const std::string & relation,
                       const int attrCnt,
                       const AttrInfo attrList[],
                       const std::string & attrName,
                       const Operator op,
                       const Datatype type,
                       const char *attrValue);
#endif
