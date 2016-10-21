//total lines:12509
#include <stdio.h>
#include <windows.h>
#include<conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "Catalog.h"
#include "query.h"

BufMgr * bufMgr=NULL;
DBManager dbManager;
DB* db=NULL;
RelCatalog  *relCat=NULL;
AttrCatalog *attrCat=NULL;
Error error;
HANDLE hStdOutput=INVALID_HANDLE_VALUE;
int main(int argc, char **argv)
{
	if (argc > 2)
	{
		cerr << endl<< "Bad parameter: " << argv[2] << " !" << endl;
		cerr << "PaLanTir just accept one parameter. That will be a database's name." << endl;
		return 1;
	}
	hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	ASSERT(INVALID_HANDLE_VALUE!=hStdOutput );
	ASSERT(SetConsoleTitle(" Hellow PaLanTir."));
	SMALL_RECT wrt = { -20, -20, 110, 10 };
	COORD coord = { 500, 500 };
	ASSERT(SetConsoleScreenBufferSize( hStdOutput, coord ) ) ;
	ASSERT(SetConsoleWindowInfo(hStdOutput,TRUE,&wrt));
	// create buffer manager
	bufMgr = new BufMgr(100);
	if (argc == 2)
	{
		Status status;
		if((status=dbManager.openDataBase((std::string)argv[0],db))!=OK)
		{
			error.print(status);
			ASSERT(CloseHandle(hStdOutput));
			return EXIT_FAILURE;
		}
		relCat = new RelCatalog(db->getDBName(),status);
		if (status == OK)
			attrCat = new AttrCatalog(db->getDBName(),status);
		if (status != OK)
		{
			error.print(status);
			ASSERT(CloseHandle(hStdOutput));
			return EXIT_FAILURE;
		}
	}
	cout<<endl<<"Welcome to PaLanTir."<<endl;
	cout<<"version:1.0    Made by MaJian. Finished at 23th,Match,2010"<<endl;
	extern void parse();
	parse();
	ASSERT(CloseHandle(hStdOutput));
	return EXIT_SUCCESS;
}
