#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<crtdbg.h>

#include "Transport.h"
#include "General.h"
#include "FileHelper.h"
#include "Macros.h"



int	initPublicTransport(Transport* pTrans, Line*pLine) {

	if (!L_init(&pTrans->stationList)) {
		printf("Error");
		return 0;
	}
		pTrans->type.publicTrans.pLine = *pLine;
		pTrans->paymentFunc = calcPaymentPublicTransport;
		pTrans->print = printPublicTransport;
		pTrans->transportType = ePublicTransport;
		getCorrectTime(&pTrans->departTime);
		pTrans->arrivalTime = pTrans->departTime;
		pTrans->price =0;
		
		return 1;
}
int	initPrivateTransport(Transport* pTrans, Driver* pDriver) {

	if (!L_init(&pTrans->stationList)) {
		printf("Error");
		return 0;
	}
	pDriver->salary += PAYMENT_PER_TRANSPORT;
	pTrans->type.privateTrans.pDriver = *pDriver;
	pTrans->paymentFunc = calcPaymentPrivateTransport;
	pTrans->print = printPrivateTransport;
	pTrans->transportType = ePrivateTransport;
	getCorrectTime(&pTrans->departTime);
	pTrans->arrivalTime = pTrans->departTime;
	pTrans->price = 0;
	
	return 1;
}

int	addOrDeleteStationForTransport(const LIST* allStations, Transport* pTrans, int choose) {
	int serialNum;
	NODE* temp;
	NODE* pN=NULL;
	NODE* posToInsert=NULL;
	Station* pStation = (Station*)calloc(1, sizeof(Station));
	Station* pS=NULL;

	if (!pStation)
		return 0;
	do {
		if (choose == 0) {
			printf("choose station from list:\n");
			L_print(allStations, printStation);
		}
		else {
			printf("choose station from transport stations list:\n");
			L_print(&pTrans->stationList, printStation);
		}
		scanf("%d", &serialNum);
		pStation->serialNumber = serialNum;
		temp = L_find(allStations->head.next, pStation, compareStationBySN);
		if (temp != NULL) {
			pS = (Station*)temp->key;
			pN = L_find(pTrans->stationList.head.next, pS, compareStationBySN);
			if (pN != NULL) {// station found in transport
				if (choose == 0) {// station exists, can not add 
					printf("Station already exists in the transport\n");
					temp = NULL;
				}
			}
			else if (pN == NULL && choose == 1) {//station not found in transport, can not delete
				printf("Station not exists in the transport\n");
			}
		}
		else
			printf("Station not exists in company\n");
	} while (!temp);
	if (choose == 0) {
		if (!addStationForTransport(pTrans, pS)) {
			free(pStation);
			return 0;
		}
	}
	else {
		if (!deleteStationFromTransport(pTrans, pS)) {
			free(pStation);
			return 0;
		}
	}
	free(pStation);
	return 1;
}
int deleteStationFromTransport(Transport* pTrans, Station* pStation) {
	NODE* posToDelete=NULL;
	NODE* pos= pTrans->stationList.head.next;
	NODE* pervNode = &pTrans->stationList.head;
	Station* pS=NULL;

	while (pos != NULL)
	{
		pS = (Station*)pos->key;
		if (pS->serialNumber == pStation->serialNumber)
			break;
		pervNode = pos;
		pos = pos->next;

	}
	posToDelete = pervNode;
	if (!L_delete(posToDelete, freeStationForTransport)) {
		return 0;
	}
	printf("Station was deleted from transport\n");
	addMinutes(&pTrans->arrivalTime, -(TIME_FOR_STATION));//sub minutes
	if (pTrans->transportType == ePublicTransport)
		calcPaymentPublicTransport(pTrans);
	else {
		calcPaymentPrivateTransport(pTrans);
	}
	
	return 1;
}
int addStationForTransport(Transport* pTrans, Station* pStation) {

	NODE* posToInsert;
	posToInsert = findPosByStationNameForTransport(pTrans, pStation->stationName);
	posToInsert = L_insert(posToInsert, pStation);
	addMinutes(&pTrans->arrivalTime, TIME_FOR_STATION);// each station add time for transport arrival
	
	if (pTrans->transportType == ePublicTransport)
		calcPaymentPublicTransport(pTrans);
	else {
		calcPaymentPrivateTransport(pTrans);
	}
	if(posToInsert!=NULL)
		printf("Station added to transport\n");
	
	return 1;
}

NODE* findPosByStationNameForTransport(Transport* pTrans, char* stationName) {
	
	NODE* pos;
	Station* pStation;
	pos = &pTrans->stationList.head;
	if (!pos)
		return &pTrans->stationList.head;

	while (pos->next != NULL) {
		pStation = pos->next->key;
		if (strcmp(pStation->stationName, stationName) > 0)
			return pos;
		pos = pos->next;

	}
	return pos;
}
void calcPaymentPublicTransport(Transport* pTrans) {

    pTrans->price = L_countNodes(pTrans->stationList.head.next) * PRICE_FOR_STATION + PRICE_FOR_PUBLIC;

}
void calcPaymentPrivateTransport(Transport* pTrans) {

	pTrans->price = L_countNodes(pTrans->stationList.head.next) * PRICE_FOR_STATION + PRICE_FOR_PRIVATE;

}
Station* isStationInTransport(Transport* pTrans, int SerialNum) {

	Station pStation = { 0 };
    pStation.serialNumber=SerialNum;
	NODE* p;

	p = L_find(pTrans->stationList.head.next, &pStation, compareStationBySN);
	if (p != NULL)
		return p->key;

	return NULL;
}

int	compareTransportByDepartTime(const void* v1, const void* v2)
{
	const Transport* t1 = *(const Transport**)v1;
	const Transport* t2 = *(const Transport**)v2;

	return compareTime(&t1->departTime, &t2->departTime);
}

int	compareTransportByArrivalTime(const void* v1, const void* v2)
{
	const Transport* t1 = *(const Transport**)v1;
	const Transport* t2 = *(const Transport**)v2;

	return compareTime(&t1->arrivalTime, &t2->arrivalTime);
}

int	 compareTransportByFirstNameStation(const void* v1, const void* v2) {
	const Transport* t1 = *(const Transport**)v1;
	const Transport* t2 = *(const Transport**)v2;
	if (t1->stationList.head.next == NULL || t2->stationList.head.next == NULL)
		return 0;

	Station* st1 = (Station*)t1->stationList.head.next->key;
	Station* st2 = (Station*)t2->stationList.head.next->key;
	return compareStationByStationName(st1,st2);

}
int	compareTransportByPrice(const void* v1, const void* v2) {
	const Transport* t1 = *(const Transport**)v1;
	const Transport* t2 = *(const Transport**)v2;

	return t1->price - t2->price;
}

void printPublicTransport(const Transport* pTrans) {

	printf("Time of departure:\t");
	printTime(&pTrans->departTime);
	printf("Time of arrival: ");
	printTime(&pTrans->arrivalTime);
	printf("\n    Line number : % d   bus type is : % s \n", pTrans->type.publicTrans.pLine.lineNumber,BusTypeStr[pTrans->type.publicTrans.pLine.pBus.type]);
	printf("\n    there are %d stations: \t\n", L_countNodes(pTrans->stationList.head.next));
	L_printForTransport(&pTrans->stationList, printStation);
	printf("    Price is:%d \n", pTrans->price);

}
void printPrivateTransport(const Transport* pTrans) {

	printf("Time of departure: \t");
	printTime(&pTrans->departTime);
	printf("Time of arrival: ");
	printTime(&pTrans->arrivalTime);
	printf("\n    Driver is: %s , num of employee: %d\n", pTrans->type.privateTrans.pDriver.pPerson.name, pTrans->type.privateTrans.pDriver.numOfEmployee);
	printf("\n    there are %d stations: \n\t", L_countNodes(pTrans->stationList.head.next));
	L_printForTransport(&pTrans->stationList, printStation);
	printf("    Price is:%d \n", pTrans->price);

}
int	 insertStationToTransportList(LIST* stationList, Station* pStation)
{
	NODE* pN = stationList->head.next; 
	NODE* pPrevNode = &stationList->head;
	Station* pTemp;
	int compRes;
	while (pN != NULL)
	{
		pTemp = (Station*)pN->key;
		compRes = strcmp(pTemp->stationName,pStation->stationName);
		if (compRes == 0) 
		{
			printf("Not new station for transport - error!!!\n");
			return 0;
		}

		if (compRes > 0) {	
			if (!L_insert(pPrevNode, pStation))
				return 0;
			return 1;
		}
		pPrevNode = pN;
		pN = pN->next;
	}
	if (!L_insert(pPrevNode, pStation))
		return 0;
	return 1;
}

void freeTransport(void* pTrans)
{
	Transport* pT = *(Transport**)pTrans;
	
	L_free(&pT->stationList,freeStationForTransport);
	free(pT);
}

