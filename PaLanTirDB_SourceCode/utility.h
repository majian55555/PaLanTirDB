#ifndef UTILITY_H
#define UTILITY_H
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <functional>
#include "error.h"
#include "Catalog.h"
#include <string.h>

// define if debug output wanted

//
// Prototypes for utility layer functions
//
const Status PLT_CreateDB(const std::string& dbName);

const Status PLT_DestroyDB(const std::string& dbName);

const Status PLT_OpenDB(const std::string& dbName);

const Status PLT_CloseDB(const std::string& dbName);

const Status PLT_CreateRel(const std::string& relation,const int attrCnt,const AttrInfo attrList[]);

const Status PLT_DestroyRel(const std::string& relation);

const Status PLT_Load(const std::string& relation,const std::string& fileName);

const Status PLT_Print(const std::string& dbName,std::string relation);

void PLT_Quit(void);

#endif