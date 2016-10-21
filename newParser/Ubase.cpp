#include <stdio.h>
#include "Catalog.h"
#include "query.h"

BufMgr * bufMgr=NULL;
DBManager dbManager;
DB* db=NULL;
RelCatalog  *relCat=NULL;
AttrCatalog *attrCat=NULL;
Error error;

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		cerr << endl<< "Bad parameter: " << argv[2] << " !" << endl;
		cerr << "PaLanTir just accept one parameter. That will be a database's name." << endl;
		return 1;
	}
	// create buffer manager
	bufMgr = new BufMgr(100);
	if (argc == 2)
	{
		Status status;
		if((status=dbManager.openDataBase((std::string)argv[0],db))!=OK)
		{
			error.print(status);
			return EXIT_FAILURE;
		}
		relCat = new RelCatalog(db->getDBName(),status);
		if (status == OK)
			attrCat = new AttrCatalog(db->getDBName(),status);
		if (status != OK)
		{
			error.print(status);
			return EXIT_FAILURE;
		}
	}
	cout<<endl<<"Welcome to PaLanTir."<<endl;
	cout<<"version:1.0"<<endl;
	extern void parse();
	parse();
	return EXIT_SUCCESS;
}
