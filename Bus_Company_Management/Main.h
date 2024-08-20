#define _CRTDBG_MAP_ALLOC

#pragma once
#include "Company.h"

typedef enum
{
	eAddBus, eAddDriver, eAddLine, eAddStationForCompany, eAddTransport,
	eAddOrDeleteStationFromTransport, eFindBestTransport , ePrintLineWithBusType, eSortTransport,
	eFindTransport,ePrintAllStationsList,ePrintCompany,eNofOptions
} eMenuOptions;

const char* str[eNofOptions] = { "Add Bus","Add Driver","Add Line","Add station for company",
								"Add Transport", "Add Or Delete Station From Transport",
								"Find Best Transport","Print Line With Bus Type",
								"Sort Transport","Find Transport",
								"Print All Station List","Print Company"};

#define FROM_TEXT_FILE 1
#define FROM_BIN_FILE 2
#define EXIT			-1
#define COMPANY_TEXT_FILE_NAME "company.txt"
#define COMPANY_BIN_FILE_NAME "company.bin"



int menu();
int initCompanyFiles(Company* pComp);

