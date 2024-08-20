#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include<crtdbg.h>
#include "Bus.h"
#include "General.h"
#include "FileHelper.h"
#include "Macros.h"

int initBus(Bus* pBus, Bus* busArr, int numOfBuses)
{
	pBus->brand = getStrExactName("Enter bus brand ");
	if (!pBus->brand)
		return 0;
	pBus->type = getBusType();
	pBus->serialNumBus = numOfBuses+1;
	return 1;
}
eBusType		getBusType() {
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofBusType; i++)
			printf("%d for %s\n", i, BusTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofBusType);
	getchar();
	return (eBusType)option;
}
const char* GetBusTypeStr(int type) {
	if (type < 0 || type >= eNofBusType)
		return NULL;
	return BusTypeStr[type];
}
Bus* findBusBySN(Bus* busArr, int count, int sn)
{
	for (int i = 0; i < count; i++)
		if (busArr[i].serialNumBus == sn)
			return &busArr[i];
	return NULL;
}
int		compareBusByType(const void* v1, const void* v2) {
	const Bus* b1 = (const Bus*)v1;
	const Bus* b2 = (const Bus*)v2;

	return b1->type - b2->type;
}
void printBus(const Bus* pBus) {
	printf("brand is: %s\t  type of bus is: %s\n",pBus->brand,BusTypeStr[pBus->type]);
}
void printBusV(const void* v) {
	const Bus* pBus = (const Bus*)v;
	printf("brand is: %s\t  type of bus is: %s\n", pBus->brand, BusTypeStr[pBus->type]);
}
void freeBus(Bus* pBus) {
	free(pBus->brand);
}

int saveBusToBFile(const Bus* pBus, FILE* fp)
{
	CHECK_RETURN_ZERO(pBus);
	if (!writeStringToFile(pBus->brand, fp, "Error write bus brand\n"))
		return 0;

	if (!writeIntToFile(pBus->type, fp, "Error save bus type\n"))
		return 0;

	if (!writeIntToFile(pBus->serialNumBus, fp, "Error save bus serial number\n"))
		return 0;

	return 1;

}
int	loadBusFromBFile(Bus* pBus, FILE* fp)
{
	CHECK_RETURN_ZERO(pBus);
	pBus->brand = readStringFromFile(fp, "Error reading bus brand");
	if (!readIntFromFile((int *) & pBus->type, fp, "Error reading type of bus\n"))
	{
		return 0;
	}
	if (!readIntFromFile(&pBus->serialNumBus, fp, "Error reading bus serial number\n"))
	{
		return 0;
	}
	return 1;

}
int	 saveBusToFile(const Bus* pBus, FILE* fp) {

	CHECK_RETURN_ZERO(pBus);
	fprintf(fp, "%s\n", pBus->brand);
	fprintf(fp, "%d\n", pBus->type);
	fprintf(fp, "%d\n", pBus->serialNumBus);

	return 1;
}
int		   	loadBusFromFile(Bus* pBus, FILE* fp) {
	char temp[MAX_STR_LEN];
	CHECK_RETURN_ZERO(pBus);

	myGets(temp, MAX_STR_LEN, fp);
	pBus->brand = getDynStr(temp);

	fscanf(fp, "%d", &pBus->type);
	fscanf(fp, "%d", &pBus->serialNumBus);

	return 1;
}