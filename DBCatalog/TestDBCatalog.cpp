#include "Catalog.h"
BufMgr * bufMgr=NULL;
DBManager dbManager;
DB* db=NULL;
RelCatalog  *relCat=NULL;
AttrCatalog *attrCat=NULL;
Error error;
int main()
{
	cout<<"OK"<<endl;
	cout<<(int)P_CHAR<<(int)P_VCHAR<<(int)P_INTEGER<<endl;
	return 1;
}