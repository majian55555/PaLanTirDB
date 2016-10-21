#ifndef QUERY_H
#define QUERY_H

#include "Catalog.h"

enum JoinType {NLJoin, SMJoin, HashJoin};

//
// Prototypes for query layer functions
//


const Status QU_Select(const std::string & result, 
		       const int projCnt, 
		       const AttrInfo projNames[],
		       const AttrInfo *attr, 
		       const Operator op, 
		       const char *attrValue);

const Status QU_Join(const std::string & result, 
		     const int projCnt, 
		     const AttrInfo projNames[],
		     const AttrInfo *attr1, 
		     const Operator op, 
		     const AttrInfo *attr2);

const Status QU_Insert(const std::string & relation, 
		       const int attrCnt, 
		       const AttrInfo attrList[]);

const Status QU_Delete(const std::string & relation, 
		       const std::string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue);

const Status QU_Update(const std::string & relation,
                       const int attrCnt,
                       const AttrInfo attrList[],
                       const std::string & attrName,
                       const Operator op,
                       const Datatype type,
                       const char *attrValue);
#endif
