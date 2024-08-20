#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>		
#include <ctype.h>
#include <time.h>
#include <crtdbg.h>

#include "Company.h"
#include "CompanyFile.h"
#include "Main.h"

#define COMPANY_TXT_FILE_NAME "company.txt"
#define COMPANY_BIN_FILE_NAME "company.bin"

int main()
{
	
	srand((unsigned int)time(NULL));
	Company	company;
	if (!initCompanyFiles(&company))
	{
		printf("error init");
		return 0;
	}

	int option;
	int stop = 0;

	do
	{
		option = menu();
		switch (option)
		{
		case eAddBus:
			if (!addBus(&company))
				printf("Error adding bus\n");
			break;

		case eAddDriver:
			if (!addDriver(&company))
				printf("Error adding driver\n");
			break;

		case eAddLine:
			if (!addLine(&company))
				printf("Error adding line\n");
			break;

		case eAddStationForCompany:
			if (!addStationForCompany(&company))
				printf("Error adding station\n");
			break;

		case eAddTransport:
			if (!addTransport(&company))
				printf("Error adding transport\n");
			break;

		case eAddOrDeleteStationFromTransport:
			if (!findATransportForAddOrDeleteStation(&company))
				printf("Error adding or deleting station for transport\n");
			break;
		case eFindBestTransport:
			if (!findBestTransport(&company))
				printf("Error find best transport track\n");
			break;
		case ePrintLineWithBusType:
			doPrintLineWithBusType(&company);
			break;

		case eSortTransport:
			sortTransport(&company);
			break;

		case eFindTransport:
			findTransport(&company);
			break;

		case ePrintAllStationsList:
			printAllStationsList(&company);
			break;

		case ePrintCompany:
			printCompany(&company);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	saveCompanyToFile(&company, COMPANY_TXT_FILE_NAME);
	saveCompanyToBFile(&company, COMPANY_BIN_FILE_NAME);


	freeCompany(&company);

	printf("before dump\n");

	_CrtDumpMemoryLeaks();
	return 1;
}

int initCompanyFiles(Company* pComp)
{
	int choose, res;
	do {
		printf("do you want load from text file or binary file? choose 1 for text or 2 for binary\n");
		scanf("%d", &choose);
	} while (choose != FROM_TEXT_FILE && choose != FROM_BIN_FILE);

	if (choose == FROM_TEXT_FILE)
	{

		res = initCompanyFromFile(pComp, COMPANY_TXT_FILE_NAME);

	}
	else if (choose == FROM_BIN_FILE) {

		res = initCompanyFromBFile(pComp, COMPANY_BIN_FILE_NAME);
	}
	if (!res)
	{
		printf("error init company from files\n");
		initCompany(pComp);// in it from user
		//return 0;
	}

	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	char tav;
	scanf("%c", &tav);
	return option;
}