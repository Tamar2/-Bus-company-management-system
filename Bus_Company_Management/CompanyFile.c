#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<crtdbg.h>
#include "Company.h"
#include "General.h"
#include "fileHelper.h"
#include "CompanyFile.h"
#include "Macros.h"


int	initCompanyFromBFile(Company* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_RETURN_MSG_ZERO(fp, "Error open company file");

	pComp->transportsArr = NULL;

	pComp->name = readStringFromFile(fp, "Error reading company name\n");
	CHECK_COLSE_RETURN_ZERO(pComp->name, fp);

	pComp->transportSortOpt = enotSorted;

	if (!readIntFromFile(&pComp->numOfDrivers, fp, "Error reading drivers count\n"))
	{
		free(pComp->name);
		COLSE_RETURN_ZERO(fp);
	}
	pComp->driversArr = (Driver*)malloc(pComp->numOfDrivers * sizeof(Driver));
	if (!pComp->driversArr)
	{
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "Allocation error");

	}
	
	for (int i = 0;i < pComp->numOfDrivers;i++) {
		if (!loadDriverFromBFile(&pComp->driversArr[i], fp)) {

			free(pComp->driversArr);
			free(pComp->name);
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error reading drivers array\n");
		}
	}
	if (!readIntFromFile(&pComp->numOfBuses, fp, "Error reading buses count\n"))
	{
		free(pComp->name);
		COLSE_RETURN_ZERO(fp);

	}
	pComp->busArr = (Bus*)malloc(pComp->numOfBuses * sizeof(Bus));
	if (!pComp->busArr)
	{
		free(pComp->name);
		free(pComp->driversArr);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "Allocation error\n");
	}

	for (int i = 0;i < pComp->numOfBuses;i++) {
		if (!loadBusFromBFile(&pComp->busArr[i], fp)) {
			free(pComp->busArr);
			free(pComp->driversArr);
			free(pComp->name);
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error reading buses array\n");

		}
	}	
	
	if (!loadLinesListFromBFile(pComp, fp)) {

		free(pComp->transportsArr);
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "error reading line list \n");

	}

	if (!loadStationsListFromBFile(pComp, fp)) {

		free(pComp->transportsArr);
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		freeLineList(pComp);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "error reading station list \n");

	}
	if (!readIntFromFile(&pComp->numOfTransports, fp, "Error reading transport count \n"))
	{
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		COLSE_RETURN_ZERO(fp);

	}

	if (!createTransportsArr(pComp))
	{
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "Allocation error\n");
	}

	if (!loadTransportsArrFromBFile(pComp, fp))
	{
		free(pComp->transportsArr);
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "error reading transport arr\n");

	}
	fclose(fp);
	return 1;
}

int	createTransportsArr(Company* pComp) {
	if (pComp->numOfTransports > 0)
	{
		pComp->transportsArr = (Transport**)malloc(pComp->numOfTransports * sizeof(Transport*));
		if (!pComp->transportsArr)
			return 0;
	}
	else
		pComp->transportsArr = NULL;

	for (int i = 0; i < pComp->numOfTransports; i++)
	{
		pComp->transportsArr[i] = (Transport*)calloc(1, sizeof(Transport));
		if (!pComp->transportsArr[i])
			return 0;
	}
	return 1;
}

int	saveCompanyToBFile(const Company* pComp, const char* fileName)
{
	FILE* fp;
	int count = 0;
	fp = fopen(fileName, "wb");
	CHECK_RETURN_MSG_ZERO(fp, "Error open company file to write");
	
	if (!writeStringToFile(pComp->name, fp, "Error write company name\n"))
	{
		COLSE_RETURN_ZERO(fp);

	}

	if (!writeIntToFile(pComp->numOfDrivers, fp, "Error write drivers count\n"))
	{
		COLSE_RETURN_ZERO(fp);

	}
	for (int i = 0; i < pComp->numOfDrivers;i++) {
		if (!saveDriverToBFile(&pComp->driversArr[i], fp)) {
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error write drivers array\n");
		}
	}

	if (!writeIntToFile(pComp->numOfBuses, fp, "Error write buses count\n"))
	{
		COLSE_RETURN_ZERO(fp);

	}

	for (int i = 0; i < pComp->numOfBuses;i++) {
		if (!saveBusToBFile(&pComp->busArr[i], fp)) {
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error write buses array\n");
		}
	}
	
	count = L_countNodes(pComp->lineList.head.next);
	if (!writeIntToFile(count, fp, "Error write number of lines of company\n"))
		COLSE_RETURN_ZERO(fp);

	if (count > 0)
	{
		NODE* pN = pComp->lineList.head.next; //first Node

		Line* pLine;
		while (pN != NULL)
		{
			pLine = (Line*)pN->key;
			if (!saveLineToBFile(pLine, fp))
			{
				MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error write line\n");
			}
			pN = pN->next;
		}

	}
	count = L_countNodes(pComp->allStations.head.next);
	if (!writeIntToFile(count, fp, "Error write all stations of company\n"))
		COLSE_RETURN_ZERO(fp);
	if (count > 0)
	{
		NODE* pN = pComp->allStations.head.next; //first Node

		Station* pStation;
		while (pN != NULL)
		{
			pStation = (Station*)pN->key;
			if (!saveStationToBFile(pStation, fp))
			{
				MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error write station\n");
			}
			pN = pN->next;
		}

	}
	if (!writeIntToFile(pComp->numOfTransports, fp, "Error write transports count\n"))
	{
		COLSE_RETURN_ZERO(fp);

	}

	for (int i = 0; i < pComp->numOfTransports; i++)
	{
		if (!saveTransportBFile(pComp->transportsArr[i], fp))
		{
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error write transport array\n");

		}
	}
	fclose(fp);
	return 1;
} 

int loadTransportsArrFromBFile(Company* pComp, FILE* fp)
{

	for (int i = 0; i < pComp->numOfTransports; i++)
	{
		if (!loadTransportFromBFile(pComp,pComp->transportsArr[i], fp))
		{
			return 0;
		}
	}
	return 1;
}

int	loadStationsListFromBFile(Company* pComp, FILE* fp) {
	int count = 0;

	CHECK_RETURN_MSG_ZERO(L_init(&pComp->allStations), "error init all stations");
	
	if (!readIntFromFile(&count, fp, "Error reading stations count \n"))
	{
		return 0;
	}
	Station* pStation;
	for (int i = 0; i < count; i++)
	{
		pStation = (Station*)calloc(1, sizeof(Station));
		if (!pStation)
			break;
		CHECK_RETURN_MSG_ZERO(loadStationFromBFile(pStation, fp), "Error loading station from file\n");
		
		NODE* posToInsert = &pComp->allStations.head;
		posToInsert = findPosByStationNameForCompany(pComp, pStation->stationName);
		posToInsert = L_insert(posToInsert, pStation);
	}
	return 1;
}

int	loadLinesListFromBFile(Company* pComp, FILE* fp) {

	int count = 0;
	CHECK_RETURN_MSG_ZERO(L_init(&pComp->lineList), "error init line list ");

	if (!readIntFromFile(&count, fp, "Error reading lines count \n"))
	{
		return 0;
	}
	Line* pLine;
	for (int i = 0; i < count; i++)
	{
		pLine = (Line*)calloc(1, sizeof(Line));
		if (!pLine)
			break;
		CHECK_RETURN_MSG_ZERO(loadLineFromBFile(pComp,pLine, fp), "Error loading line from file\n");

		NODE* posToInsert = &pComp->lineList.head;
		posToInsert = findPosLineNumber(pComp, pLine->lineNumber);
		posToInsert = L_insert(posToInsert, pLine);
		//L_insert(&pComp->lineList.head,pLine);
	}

	return 1;
}

int	initCompanyFromFile(Company* pComp, const char* fileName) {

	FILE* fp;
	char temp[MAX_STR_LEN];

	fp = fopen(fileName, "r");
	CHECK_RETURN_MSG_ZERO(fp, "Error open company file");
	
	CHECK_RETURN_ZERO(pComp);

	myGets(temp, MAX_STR_LEN, fp);
	pComp->name = getDynStr(temp);
	fscanf(fp, "%d", &pComp->numOfDrivers);
	pComp->driversArr = (Driver*)malloc(pComp->numOfDrivers * sizeof(Driver));
	if (!pComp->driversArr)
	{
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "Allocation error");

	}
	
	for (int i = 0;i < pComp->numOfDrivers;i++) {
		if (!loadDriverFromFile(&pComp->driversArr[i], fp)) {
			free(pComp->driversArr);
			free(pComp->name);
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error reading drivers array\n");
		}
	}
	
	fscanf(fp, "%d", &pComp->numOfBuses);
	pComp->busArr = (Bus*)malloc(pComp->numOfBuses * sizeof(Bus));
	if (!pComp->busArr) {
		free(pComp->driversArr);
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "Allocation error\n");

	}
	for (int i = 0;i < pComp->numOfBuses;i++) {
		if (!loadBusFromFile(&pComp->busArr[i], fp)) {
			free(pComp->busArr);
			free(pComp->driversArr);
			free(pComp->name);
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error reading buses array\n");
		}
	}
	
	if (!loadLinesListFromFile(pComp, fp)) {

		free(pComp->transportsArr);
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "error reading line list \n");

	}

	if (!loadStationsListFromFile(pComp, fp)) {

		free(pComp->transportsArr);
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "error reading station list \n");

	}

	fscanf(fp, "%d", &pComp->numOfTransports);
	if (!createTransportsArr(pComp))
	{
		free(pComp->busArr);
		free(pComp->driversArr);
		free(pComp->name);
		MSG_CLOSE_FILE_RETURN_ZERO(fp, "Allocation error\n");

	}
	for (int i = 0;i < pComp->numOfTransports;i++) {
		if (!loadTransportFromFile(pComp, pComp->transportsArr[i], fp)) {
			free(pComp->transportsArr);
			free(pComp->busArr);
			free(pComp->driversArr);
			free(pComp->name);
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "Error reading transport array\n");

		}
	}

	fclose(fp);
	return 1;
}

int	saveCompanyToFile(const Company* pComp, const char* fileName) {

	FILE* fp;

	fp = fopen(fileName, "w");
	CHECK_RETURN_MSG_ZERO(fp, "Error open company file to write\n");
	
	fprintf(fp, "%s\n", pComp->name);
	fprintf(fp, "%d\n", pComp->numOfDrivers);
	for (int i = 0;i < pComp->numOfDrivers;i++) {
		if (!saveDriverToFile(&pComp->driversArr[i], fp)) {

			MSG_CLOSE_FILE_RETURN_ZERO(fp, "error saving driver to file");
		}
	}
	fprintf(fp, "%d\n", pComp->numOfBuses);
	for (int i = 0;i < pComp->numOfBuses;i++) {

		if (!saveBusToFile(&pComp->busArr[i], fp)) {
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "error saving bus to file");
	
		}
	}

	int count = L_countNodes(pComp->lineList.head.next);
	fprintf(fp, "%d\n", count);
	if (count > 0)
	{
		NODE* pN = pComp->lineList.head.next; 

		Line* pLine;
		while (pN != NULL)
		{
			pLine = (Line*)pN->key;

			if (!saveLineToFile(pLine, fp))
			{
				MSG_CLOSE_FILE_RETURN_ZERO(fp, "error saving line to file");

			}
			pN = pN->next;
		}

	}
	 count = L_countNodes(pComp->allStations.head.next);
	fprintf(fp, "%d\n", count);
	if (count > 0)
	{
		NODE* pN = pComp->allStations.head.next; 

		Station* pStation;
		while (pN != NULL)
		{
			pStation = (Station*)pN->key;

			if (!saveStationToFile(pStation, fp))
			{
				MSG_CLOSE_FILE_RETURN_ZERO(fp, "error saving station to file");

			}
			pN = pN->next;
		}

	}

	fprintf(fp, "%d\n", pComp->numOfTransports);
	for (int i = 0;i < pComp->numOfTransports;i++) {

		if (!saveTransportFile(pComp->transportsArr[i], fp)) {
			MSG_CLOSE_FILE_RETURN_ZERO(fp, "error saving transport to file");

		}
	}
	fclose(fp);
	return 1;
}

int	loadTransportsArrFromFile(Company* pComp, FILE* fp) {

	for (int i = 0; i < pComp->numOfTransports; i++)
	{
		if (!loadTransportFromFile(pComp,pComp->transportsArr[i], fp))
		{
			return 0;
		}
	}
	return 1;
}
int	loadStationsListFromFile(Company* pComp, FILE* fp) {

	int count=0;
	CHECK_RETURN_MSG_ZERO(L_init(&pComp->allStations), "error init all stations");
	fscanf(fp, "%d", &count);
	fgetc(fp);

	Station* pStation;
	for (int i = 0; i < count; i++)
	{
		pStation = (Station*)calloc(1, sizeof(Station));
		if (!pStation)
			break;
		CHECK_RETURN_MSG_ZERO(loadStationFromFile(pStation, fp), "Error loading station from file\n");
		
		NODE* posToInsert = &pComp->allStations.head;
		posToInsert = findPosByStationNameForCompany(pComp, pStation->stationName);
		posToInsert = L_insert(posToInsert, pStation);
	}
	
	return 1;
} 

int	loadLinesListFromFile(Company* pComp, FILE* fp) {

	int count=0;
	CHECK_RETURN_MSG_ZERO(L_init(&pComp->lineList), "error init line list");
	fscanf(fp, "%d", &count);
	fgetc(fp);

	Line* pLine;
	for (int i = 0; i < count; i++)
	{
		pLine = (Line*)calloc(1, sizeof(Line));
		if (!pLine)
			break;
		CHECK_RETURN_MSG_ZERO(loadLineFromFile(pComp,pLine, fp), "Error loading line from file\n");

		NODE* posToInsert = &pComp->lineList.head;
		posToInsert = findPosLineNumber(pComp, pLine->lineNumber);
		posToInsert = L_insert(posToInsert, pLine);
	}

	return 1;
}

///////////////////line files/////////////////////////////////////////////

int saveLineToBFile(const Line* pLine, FILE* fp) {

	if (!writeIntToFile(pLine->lineNumber, fp, "Error write line number\n"))
		return 0;

	if (!writeIntToFile(pLine->available, fp, "Error save line type\n"))
		return 0;

	if (!writeIntToFile(pLine->pBus.serialNumBus, fp, "Error save bus serial number\n"))
		return 0;

	return 1;
}

int	loadLineFromBFile(Company* pComp,Line* pLine, FILE* fp)
{
	if (!readIntFromFile(&pLine->lineNumber, fp, "Error reading line number\n"))
	{
		return 0;
	}
	if (!readIntFromFile((int*)&pLine->available, fp, "Error reading line type\n"))
	{
		return 0;
	}
	int serialNum;
	if (!readIntFromFile(&serialNum, fp, "Error reading bus serial number \n"))
	{
		return 0;
	}
	Bus* b = findBusBySN(pComp->busArr, pComp->numOfBuses, serialNum);
	if (!b) {
		return 0;
	}
	pLine->pBus = *b;

	return 1;

}

int	saveLineToFile(const Line* pLine, FILE* fp) {

	CHECK_RETURN_ZERO(pLine);

	fprintf(fp, "%d\n", pLine->pBus.serialNumBus);
	fprintf(fp, "%d\n", pLine->lineNumber);
	fprintf(fp, "%d\n", pLine->available);
	return 1;

}

int	loadLineFromFile(Company* pComp,Line* pLine, FILE* fp) {

	int serialNum;

	CHECK_RETURN_ZERO(pLine);
	fscanf(fp, "%d", &serialNum);
	Bus* b = findBusBySN(pComp->busArr, pComp->numOfBuses, serialNum);
	if (!b) {
		return 0;
	}
	pLine->pBus = *b;
	fscanf(fp, "%d", &pLine->lineNumber);
	fscanf(fp, "%d", &pLine->available);

	return 1;
}
///////transport files///////////////////

int saveTransportBFile(const Transport* pTrans, FILE* fp) {
	if (!writeIntToFile(pTrans->transportType, fp, "Error write type of transport\n"))
		return 0;
	if (pTrans->transportType == ePublicTransport) {
		if (!saveLineToBFile(&(pTrans->type.publicTrans.pLine), fp))
			return 0;
	}
	else {
		
		if (!writeIntToFile(pTrans->type.privateTrans.pDriver.pPerson.id, fp, "Error write id of driver\n"))
			return 0;
	}
	if (!saveCompressedTimeToBFile(&pTrans->departTime, fp))
		return 0;
	if (!saveCompressedTimeToBFile(&pTrans->arrivalTime, fp))
		return 0;
	if (!writeIntToFile(pTrans->price, fp, "Error write price of transport\n"))
		return 0;

	int count = L_countNodes(pTrans->stationList.head.next);
	if (!writeIntToFile(count, fp, "Error write number of stations of transport\n"))
		return 0;
	if (count > 0)
	{
		NODE* pN = pTrans->stationList.head.next;
		Station* pS;
		while (pN != NULL)
		{
			pS = (Station*)pN->key;
			if (!writeIntToFile(pS->serialNumber, fp, "Error write station serial number\n"))
				return 0;

			pN = pN->next;
		}

	}

	return 1;
}

int	loadTransportFromBFile(Company* pComp,Transport* pTrans, FILE* fp)
{

	if (!readIntFromFile((int*)&pTrans->transportType, fp, "Error reading type of transport\n"))
	{
		return 0;
	}
	if (pTrans->transportType == ePublicTransport) {
		if (!loadLineFromBFile(pComp,&pTrans->type.publicTrans.pLine, fp))
			return 0;
	}

	else {
		
		int id=0;
		if (!readIntFromFile(&id, fp, "Error reading driver id\n"))
		{
			return 0;
		};
		Driver* d = findDriverById(pComp->driversArr, pComp->numOfDrivers, id);
		if (!d) {
			return 0;
		}
		pTrans->type.privateTrans.pDriver = *d;
	}
	if (!loadCompressedTimeFromBFile(&pTrans->departTime, fp))
		return 0;
	if (!loadCompressedTimeFromBFile(&pTrans->arrivalTime, fp))
		return 0;
	if (!readIntFromFile(&pTrans->price, fp, "Error reading price of transport\n"))
	{
		return 0;
	}
	L_init(&pTrans->stationList);
	int count = 0;
	if (!readIntFromFile(&count, fp, "Error reading count of stations\n"))
	{
		return 0;
	}

	NODE* pN;
	int serialNumber=0;
	Station* pStation=NULL;
	Station p = { 0 };
	for (int i = 0; i < count; i++)
	{
		if (!readIntFromFile(&serialNumber, fp, "Error reading serial number of station\n"))
		{
			return 0;
		}

		p.serialNumber = serialNumber;
		pN = L_find(pComp->allStations.head.next, &p, compareStationBySN);
		pStation = (Station*)pN->key;
		insertStationToTransportList(&pTrans->stationList, pStation);

	}

	return 1;

}

int saveTransportFile(const Transport* pTrans, FILE* fp) {
	if (!pTrans)
		return 0;
	fprintf(fp, "%d\n", pTrans->transportType);
	if (pTrans->transportType == ePublicTransport) {
		if (!saveLineToFile(&(pTrans->type.publicTrans.pLine), fp))
			return 0;
	}

	else {
		fprintf(fp, "%d\n", pTrans->type.privateTrans.pDriver.pPerson.id);

	}

	if (!saveTimeToFile(&pTrans->departTime, fp))
		return 0;
	if (!saveTimeToFile(&pTrans->arrivalTime, fp))
		return 0;
	fprintf(fp, "%d\n", pTrans->price);
	int count = L_countNodes(pTrans->stationList.head.next);
	fprintf(fp, "%d\n", count);
	if (count > 0)
	{
		NODE* pN = pTrans->stationList.head.next; 

		Station* pStation;
		while (pN != NULL)
		{
			pStation = (Station*)pN->key;
			fprintf(fp, "%d\n", pStation->serialNumber);
			pN = pN->next;
		}
	}
	return 1;
}

int	loadTransportFromFile(Company* pComp,Transport* pTrans, FILE* fp) {

	if (!pTrans)
		return 0;

	fscanf(fp, "%d", &pTrans->transportType);
	if (pTrans->transportType == ePublicTransport) {
		if (!loadLineFromFile(pComp,&pTrans->type.publicTrans.pLine, fp))
			return 0;
		pTrans->print = printPublicTransport;
		pTrans->paymentFunc = calcPaymentPublicTransport;
	}
	else
	{
		int id;
		fscanf(fp, "%d", &id);
		Driver* d = findDriverById(pComp->driversArr, pComp->numOfDrivers, id);
		if (!d) {
			return 0;
		}
		pTrans->type.privateTrans.pDriver = *d;
		pTrans->print = printPrivateTransport;
		pTrans->paymentFunc = calcPaymentPrivateTransport;
	}
	if (!loadTimeFromFile(&pTrans->departTime, fp))
		return 0;
	if (!loadTimeFromFile(&pTrans->arrivalTime, fp))
		return 0;
	fscanf(fp, "%d", &pTrans->price);
	L_init(&pTrans->stationList);
	int count;
	fscanf(fp, "%d", &count);

	fgetc(fp);
	NODE* pN;
	Station* pStation;
	Station p = { 0 };
	int serialNumber=0;
	for (int i = 0; i < count; i++)
	{
		fscanf(fp, "%d", &serialNumber);

		p.serialNumber = serialNumber;
		pN = L_find(pComp->allStations.head.next, &p, compareStationBySN);
		pStation = (Station*)pN->key;
		insertStationToTransportList(&pTrans->stationList, pStation);

	}
	return 1;
}

