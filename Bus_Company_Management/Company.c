#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<crtdbg.h>

#include "Company.h"
#include "General.h"
#include "FileHelper.h"
#include "Macros.h"

int		initCompany(Company* pComp) {
	pComp->name = getStrExactName("Enter bus company name");
	pComp->driversArr = NULL;
	pComp->numOfDrivers = 0;
	pComp->busArr = NULL;
	pComp->numOfBuses = 0;
	pComp->transportsArr = NULL;
	pComp->numOfTransports = 0;
	if (!L_init(&pComp->allStations)) {
		printf("Error");
		return 0;
	}
	if (!L_init(&pComp->lineList)) {
		printf("Error");
		return 0;
	}
	pComp->transportSortOpt = enotSorted;
	return 1;
}
int		addDriver(Company* pComp) {
	pComp->driversArr = (Driver*)realloc(pComp->driversArr, (pComp->numOfDrivers + 1) * sizeof(Driver));
	if (!pComp->driversArr)
		return 0;
	initDriver(&pComp->driversArr[pComp->numOfDrivers], pComp->driversArr, pComp->numOfDrivers);
	pComp->numOfDrivers++;
	return 1;
}
int	    addBus(Company* pComp) {
	pComp->busArr = (Bus*)realloc(pComp->busArr, (pComp->numOfBuses + 1) * sizeof(Bus));
	if (!pComp->busArr)
		return 0;
	initBus(&pComp->busArr[pComp->numOfBuses], pComp->busArr, pComp->numOfBuses);
	pComp->numOfBuses++;
	return 1;
}
int	addLine(Company* pComp) {
	NODE* posToInsert;
	if (pComp->numOfBuses == 0)
	{
		printf("There is no bus in company\n");
		return 0;
	}
	Line* pLine = (Line*)calloc(1, sizeof(Line));
	if (!pLine)
		return 0;

	Bus* b = FindABus(pComp);
	
	if (!initLine(pLine, &pComp->lineList, b))
	{
		free(pLine);
		return 0;
	}
	posToInsert = findPosLineNumber(pComp, pLine->lineNumber);
	posToInsert = L_insert(posToInsert, pLine);
	if (!posToInsert)
	{
		L_free(&pComp->lineList, free);
		return 0;
	}
	return 1;

}
NODE* findPosLineNumber(Company* pComp, int numOfLine) {

	NODE* pos;
	Line* pLine;
	pos = &pComp->lineList.head;
	if (!pos)
		return &pComp->lineList.head;

	while (pos->next != NULL) {
		pLine = pos->next->key;
		if ((pLine->lineNumber-numOfLine)> 0)
			return pos;
		pos = pos->next;

	}
	return pos;
}
Bus* FindABus(Company* pComp)
{	
	int number;
	Bus* temp = NULL;
	do {
		printf("Choose a bus from the list:\n");
		for (int i = 0;i < pComp->numOfBuses;i++) {
			printf("(%d) ",(i + 1));
			printBus(&pComp->busArr[i]);
		}
		scanf("%d", &number);
		if(number<=pComp->numOfBuses)
			temp = &pComp->busArr[number - 1];
		if (!temp)
			printf("No bus with this number! Try again!\n");
	} while (!temp);

	return temp;
}
Driver* FindADriver(Company* pComp)
{
	int number;
	Driver* temp = NULL;
	do {
		printf("Choose a driver from array by employee number\n");
		printDriversArr(pComp->driversArr,pComp->numOfDrivers);
		scanf("%d", &number);
		for (int i = 0;i < pComp->numOfDrivers;i++) {
			if (pComp->driversArr[i].numOfEmployee == number) {
				temp = &pComp->driversArr[i];
				return temp;
			}
		}
		if (!temp)
			printf("No driver with this number! Try again!\n");
	} while (temp == NULL);

	return NULL;
}
Line* FindALine(Company* pComp)
{
	int number;
	Line temp = { 0 };
	NODE* p;
	temp.available = eNotInUse;
	//L_find(&pComp->lineList.head,&temp,co)
	if (pComp->numOfTransports > L_countNodes(pComp->lineList.head.next)) {
		printf("there is not available lines, please add new line\n");
	}
	do {
		printf("Choose a line from the list\n");
		printLinesList(pComp);
		scanf("%d", &number);
		while (number > 1000 && number < 1) {
			printf("Enter line number between 1-1000\n");
			scanf("%d", &number);
		}
		temp.lineNumber = number;
		p = L_find(pComp->lineList.head.next, &temp, compareByLineNumber);
		for (int i = 0;i < pComp->numOfTransports;i++) {
			if (pComp->transportsArr[i]->transportType == ePublicTransport &&
				pComp->transportsArr[i]->type.publicTrans.pLine.lineNumber == temp.lineNumber) {
				printf("this line already in use, please choose another line\n");
				p = NULL;
				i = pComp->numOfTransports;
			}
		}		
		
	} while (p == NULL);

	return (Line*)p->key;

}

int		addTransport(Company* pComp) {
	int choose=0;
	if (L_countNodes(pComp->allStations.head.next)<2)
	{
		printf("There are not enough stations to set a transport\n");
		return 0;
	}
	do {
		printf("Do you want to add public or private transport? \n 1 for public \n 2 for private\n");
		scanf("%d", &choose);
	} while (choose != 1 && choose != 2);
	if (choose == 1) {
		if (!addPublicTransport(pComp))
			return 0;
	}
	else if (choose == 2) {
		if (!addPrivateTransport(pComp))
			return 0;
	}
	return 1;
}
int		addPrivateTransport(Company* pComp) {
	int count = 0;
	if (pComp->numOfDrivers == 0)
	{
		printf("There are not enough drivers to set a private transport\n");
		return 0;
	}

	Transport* pPrivateTrans = (Transport*)calloc(1, sizeof(Transport));
	if (!pPrivateTrans)
		return 0;

	Driver* pDriver = FindADriver(pComp);
	initPrivateTransport(pPrivateTrans, pDriver);
	do {
		addOrDeleteStationForTransport(&pComp->allStations, pPrivateTrans, 0);
		count++;
	} while (count < 2);// at least 2 stations for initialize transport

	pComp->transportsArr = (Transport**)realloc(pComp->transportsArr, (pComp->numOfTransports + 1) * sizeof(Transport*));
	if (!pComp->transportsArr)
	{
		free(pPrivateTrans);
		return 0;
	}
	pComp->transportsArr[pComp->numOfTransports] = pPrivateTrans;
	pComp->numOfTransports++;
	return 1;
}

int		addPublicTransport(Company* pComp) {
	int  count=0;
	if (pComp->lineList.head.next == NULL || numOfAvailableLines(pComp) ==0 )
	{
		printf("There are not enough lines to set a public transport\n");
		return 0;
	}
	
	Transport* pPublicTrans = (Transport*)calloc(1, sizeof(Transport));
	if (!pPublicTrans)
		return 0;

	Line* pLine = FindALine(pComp);
	if (!pLine)
		return 0;
	pLine->available = eInUse;
	initPublicTransport(pPublicTrans,pLine);
	do {
		addOrDeleteStationForTransport(&pComp->allStations, pPublicTrans,0);
		count++;
	} while (count<2);// at least 2 stations for initialize transport
	pComp->transportsArr = (Transport**)realloc(pComp->transportsArr, (pComp->numOfTransports + 1) * sizeof(Transport*));
	if (!pComp->transportsArr)
	{
		free(pPublicTrans);
		return 0;
	}
	
	pComp->transportsArr[pComp->numOfTransports] = pPublicTrans;
	pComp->numOfTransports++;
	return 1;
}
int findATransportForAddOrDeleteStation(Company* pComp) {

	Transport* pTrans;
	int option,choose;
	if (pComp->numOfTransports < 0) {
		printf("there is not transport to delete from company \n");
		return 0;
	}
	do {
		printf("please choose transport from list by number:\n");
		printTransportsArr(pComp->transportsArr, pComp->numOfTransports);
		scanf("%d", &option);
	} while (option<=0 || option>pComp->numOfTransports);
	pTrans =pComp->transportsArr[option-1];
	if (!pTrans)
		return 0;
	do {
		printf("do you want to add or delete station? choose 0 for add or 1 for delete\n");
		scanf("%d", &choose);
		
	} while (choose != 1 && choose != 0);
	if (choose == 1 && L_countNodes(pTrans->stationList.head.next) < 3) {
		printf("there is not enough stations in this transport for delete\n");
		return 0;
	}if (choose == 0 && L_countNodes(pTrans->stationList.head.next) == L_countNodes(pComp->allStations.head.next)) {
		printf("there is not enough stations in company to add a new station for transport\n");
		return 0;
	}
	addOrDeleteStationForTransport(&pComp->allStations, pTrans, choose);
	return 1;
}

int		addStationForCompany(Company* pComp) {
	NODE* posToInsert;
	Station* pStation = (Station*)calloc(1, sizeof(Station));
	if (!pStation)
		return 0;
	if (!initStation(pStation,&pComp->allStations))
	{
		free(pStation);
		return 0;
	}
	posToInsert = findPosByStationNameForCompany(pComp, pStation->stationName);
	posToInsert = L_insert(posToInsert, pStation);
	if (!posToInsert)
	{
		L_free(&pComp->allStations, free);
		return 0;
	}
	return 1;
}
NODE* findPosByStationNameForCompany(Company* pComp, char* stationName) {
	NODE* pos;
	Station* pStation;
	pos = &pComp->allStations.head;
	if (!pos)
		return &pComp->allStations.head;

	while (pos->next != NULL) {
		pStation = pos->next->key;
		if (strcmp(pStation->stationName, stationName) > 0)
			return pos;
		pos = pos->next;

	}
	return pos;
}
int numOfAvailableLines(Company* pComp) {
	
	Line* pLine;
	NODE* pN = pComp->lineList.head.next;
	int counter = 0;
	while (pN != NULL) {
		pLine = (Line*)pN->key;
		if (pLine->available == eNotInUse)
			counter++;
		pN = pN->next;
	}
	return counter;
}

void	printDriversArr(Driver* arr, int size) {
	generalArrayFunction(arr, size, sizeof(Driver), printDriverV);

}
void	printBusesArr(Bus* arr, int size) {
	generalArrayFunction(arr, size, sizeof(Bus), printBusV);

}
void	printLinesList(const Company* pComp) {
	UNDER_LINE();
	printf("There are %d lines:\n", L_countNodes(pComp->lineList.head.next));
	SET_DEAFULT_CONSOLE();
	L_print(&pComp->lineList, printLineV);
} 
void	printAllStationsList(const Company* pComp) {
	UNDER_LINE();
	printf("There are %d stations:\n", L_countNodes(pComp->allStations.head.next));
	SET_DEAFULT_CONSOLE();
	L_print(&pComp->allStations, printStationV);
}
void	printTransportsArr(Transport** arr, int size) {
	UNDER_LINE();
	printf("\n\nThere are %d transports\n\n", size);
	SET_DEAFULT_CONSOLE();
	for (int i =0 ;i < size;i++) {
		printf("(%d) ", (i + 1));
			if (arr[i]->transportType == ePublicTransport)
			printPublicTransport(arr[i]);
		else
			printPrivateTransport(arr[i]);
		printf("\n");
	}
}
void    sortTransport(Company* pComp) {
	pComp->transportSortOpt = showSortMenu();
	int(*compare)(const void* v1, const void* v2) = NULL;

	switch (pComp->transportSortOpt)
	{
	case eDepartTime:
		compare = compareTransportByDepartTime;
		break;
	
	case eArrivalTime:
		compare = compareTransportByArrivalTime;
		break;
	case ePrice:
		compare = compareTransportByPrice;
		break;
	}

	if (compare != NULL)
		qsort(pComp->transportsArr, pComp->numOfTransports, sizeof(Transport*), compare);
}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortTypes; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >= eNofSortTypes);

	return (eSortOption)opt;
}
void	findTransport(const Company* pComp)
{
	int(*compare)(const void* v1, const void* v2) = NULL;
	Transport t = { 0 };
	Transport* pTransport = &t;
	Station st= { 0 };
	int num=0;

	switch (pComp->transportSortOpt)
	{
	case eDepartTime:
		printf("%s\t", "Time:");
		getCorrectTime(&t.departTime);
		compare = compareTransportByDepartTime;
		break;

	case eArrivalTime:
		printf("%s\t", "Time:");
		getCorrectTime(&t.arrivalTime);
		compare = compareTransportByArrivalTime;
		break;
	case ePrice:
		printf("%s\t", "Price for transport:");
		scanf("%d", &t.price);
		compare = compareTransportByPrice;
		break;

	}

	if (compare != NULL)
	{
		Transport* tP;
		Transport** pTrans = bsearch(&pTransport, pComp->transportsArr, pComp->numOfTransports, sizeof(Transport*), compare);
		if (pTrans == NULL)
			printf("Transport was not found\n");
		else {
			printf("Transport found, ");
			tP = *pTrans;
			if(tP->transportType==ePublicTransport)
				printPublicTransport(*pTrans);
			else
				printPrivateTransport(*pTrans);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}
}
void	doPrintLineWithBusType(const Company* pComp)
{
	int count = 0;
	eBusType type = getBusType();
	NODE* temp= pComp->lineList.head.next;
	NODE* find;
	Line pLine = { 0 };
	Line* pos;
	printf("Lines with bus type %s:\n", GetBusTypeStr(type));
	pLine.pBus.type = type;
	while(temp!=NULL)
	{
		find = L_find(temp, &pLine , compareBusByType);
		if (find != NULL) {
			pos = (Line*)find->key;
			printLine(pos);
			count++;
			temp = find->next;
		}
		else
			temp = temp->next;
	}
	if (count == 0)
		printf("Sorry - could not find a line with bus type %s\n", GetBusTypeStr(type));
	printf("\n");
}
int findBestTransport(Company* pComp) {// find the fastest track between two stations
	int number;
	Station s1 = { 0 };
	Station s2 = { 0 };
	NODE* start = NULL;
	NODE* end=NULL;
	Transport* pTrans = NULL;
	int min = L_countNodes(pComp->allStations.head.next);


	do {
		printAllStationsList(pComp);
		printf("Please choose departure station by serial number: \n");
		scanf("%d", &number);
		s1.serialNumber = number;
		start = L_find(pComp->allStations.head.next, &s1, compareStationBySN);

		printf("Please choose arrival station by serial number: \n");
		scanf("%d", &number);
		s2.serialNumber = number;
		if(start!=NULL)
			end = L_find(start->next, &s2, compareStationBySN);//starting searching from the node first after start
		if (start == NULL || end == NULL)
			printf("There is no transport with this track, try again \n");
	} while (start == NULL || end == NULL);

	for (int i = 0; i < pComp->numOfTransports; i++) {
		NODE* pN = pComp->transportsArr[i]->stationList.head.next;
		NODE* temp = pN;
		start = L_find(pN, &s1, compareStationBySN);
		end = L_find(pN, &s2, compareStationBySN);

		if (start != NULL && end != NULL) {
			// Count stations between start and end
			int count = 0;
			while (temp != NULL && temp->key != end->key) {
				count++;
				temp = temp->next;
			}

			if (min >= count) {
				min = count;
				pTrans = pComp->transportsArr[i];
			}
		}
	}

	if (pTrans == NULL) {
		printf("Transport with those stations was not found.\n");
		
		return 0;
	}

	printf("The best transport is:\n");
	if (pTrans->transportType == ePublicTransport)
		printPublicTransport(pTrans);
	else
		printPrivateTransport(pTrans);


	return 1;
}
void printCompany(const Company* pComp) {
	COLOR_TXT_BACKGROUND();
	printf("Bus company is %s", pComp->name);
	SET_DEAFULT_CONSOLE();
	printf("\n\n");
	printLinesList(pComp);
	printAllStationsList(pComp);
	UNDER_LINE();
	printf("\n\nThere are %d buses\n\n", pComp->numOfBuses);
	SET_DEAFULT_CONSOLE();
	printBusesArr(pComp->busArr,pComp->numOfBuses);
	UNDER_LINE();
	printf("\n\nThere are %d drivers\n\n", pComp->numOfDrivers);
	SET_DEAFULT_CONSOLE();
	printDriversArr(pComp->driversArr,pComp->numOfDrivers);
	
	printTransportsArr(pComp->transportsArr, pComp->numOfTransports);
}
void freeBusesArr(Bus* arr, int size) {

	generalArrayFunction((void*)arr, size, sizeof(Bus), freeBus);

}
void freeDriversArr(Driver* arr, int size) {

	generalArrayFunction((void*)arr, size, sizeof(Driver), freeDriver);

}
void freeLineList(Company* pComp)
{
	L_free(&pComp->lineList, freeLine);

}
void freeStationsList(Company* pComp)
{
	L_free(&pComp->allStations, freeStationV);

}
void freeTransportsArr(Transport** arr, int size) {

	generalArrayFunction(arr, size, sizeof(Transport*), freeTransport);

} 

void freeCompany(Company* pComp) {
	free(pComp->name);
	freeDriversArr(pComp->driversArr, pComp->numOfDrivers);
	free(pComp->driversArr);
	freeBusesArr(pComp->busArr, pComp->numOfBuses);
	free(pComp->busArr);
	freeLineList(pComp);
	freeStationsList(pComp);
	freeTransportsArr(pComp->transportsArr, pComp->numOfTransports);
	free(pComp->transportsArr);


}

