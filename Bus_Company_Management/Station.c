#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<crtdbg.h>

#include "Station.h"
#include "General.h"
#include "FileHelper.h"
#include "Macros.h"

#define SEP_CHAR '/'
#define MIN_SN 1000
#define MAX_SN 9000

int initStation(Station* pStation, LIST* allStations) {

		while (1)
		{
			getStationName(pStation);
			if (checkUniqueStationName(pStation->stationName, allStations))
				break;

			printf("This name already exist, enter a different station name\n");
		}
		pStation->serialNumber=getStationSN(allStations);
		
		return 1;
}

int getStationName(Station* pStation) {
	char* temp=NULL;
	char pos [MAX_STR_LEN]="";
	char** wordsArray = NULL;
	int totalLength = 0;
	int count = 0;
	int flag = 0;
	int numOfWords = 0;
	size_t len = 0;
	char* word = NULL;
	int n = 0;
	do {
		while (count == 0)
		{
			do {
				flag = 0;
				temp = getStrExactName("Enter station location  ");
				if (strcmp(temp, pos) == 0 || !isLetter(temp) || strlen(temp) < 3) {
					if(strcmp(temp, pos) == 0)
						printf("station name need to be different street \n");
					if (strlen(temp) < 3)
						printf("station name need to be at least 3 letters\n");
					free(temp);
					flag = 1;

				}
			} while (flag == 1 );
			wordsArray = splitCharsToWords(temp, &count, &totalLength);
			numOfWords = count;
			strcpy(pos, temp);
			free(temp);
		}
		if (!pStation->stationName)
			return 0;

		if (count == 1) {
			word = wordsArray[0];
			for (int i = 0;i < 3;i++, n++) {
				pStation->stationName[n] = toupper(word[i]);
			}
		}
		else if (count == 2)
		{
			for (int i = 0;i < 2;i++, n++) {
				word = wordsArray[i];
				pStation->stationName[n] = toupper(word[0]);
			}
			word = wordsArray[count - 1];
		    len = strlen(word);
			pStation->stationName[n] = toupper(word[len - 1]);
			n++;
		}
		else if (count > 2) {
			for (int i = 0;i < 3;i++, n++) {
				word = wordsArray[i];
				pStation->stationName[n] = toupper(word[0]);

			}
		}
	
		n++;
		count = 0;
		for (int i = 0; i < numOfWords; i++)
			free(wordsArray[i]);
		free(wordsArray);
	} while (n <= 4);
	pStation->stationName[3] = SEP_CHAR;
	pStation->stationName[NAME_LEN] = '\0';
	
	return 1;
}

int checkUniqueStationName(const char* name, LIST* allStations)
{
	NODE* pN = allStations->head.next; //first Node
	if (!pN)
		return 1;

	Station* pStation;
	int compRes;
	
	while (pN != NULL)
	{
		pStation = (Station*)pN->key;
		compRes = strcmp(pStation->stationName, name);
		if (compRes == 0)
			return 0;
		
		pN = pN->next;
	}

	return 1;
}

int isSameStation(const Station* s1, const Station* s2) {
	if (!s1 || !s2)
		return 0;
	if (strcmp(s1->stationName, s2->stationName) == 0)
		return 1;

	return 0;
}

int compareStationBySN(const void* s1, const void* s2) {
	Station* st1 = (Station*)s1;
	Station* st2 = (Station*)s2;
	return st1->serialNumber - st2->serialNumber;
}

int compareStationByStationName(const void* s1, const void* s2) {
	Station* st1 = (Station*)s1;
	Station* st2 = (Station*)s2;
	return strcmp(st1->stationName, st2->stationName);
}

int getStationSN(LIST* allStations) {
	int num;
	int unique = 0;
	do {
		printf("Enter station serial number - between %d to %d\n", MIN_SN, MAX_SN);
		scanf("%d", &num);
		if (num >= MIN_SN && num <= MAX_SN)
			unique = isSerialNumUnique(allStations, num);
	} while (!unique);
	return num;
}

int isSerialNumUnique(LIST* allStations, int num) {
	Station pStation = { 0 };
	pStation.serialNumber=num;
	NODE* p;
	p = L_find(allStations->head.next, &pStation, compareStationBySN);
	if (p != NULL)
		return 0;

	return 1;
}

void printStation(const Station* st) {
	printf("station name is:");
	YELLOW();
	printf(" % s", st->stationName);
	SET_DEAFULT_CONSOLE();
	printf("  num of station: % d", st->serialNumber);
	SET_DEAFULT_CONSOLE();
}

void printStationV(const void* v) {
	const Station* st = (const Station*)v;
	printf("station name is: %s  num of station: %d ", st->stationName, st->serialNumber);

}

int saveStationToBFile(const Station* pS, FILE* fp) {

	
	if (!writeCharsToFile(pS->stationName, NAME_LEN, fp, "Error write station name\n"))
		return 0;

	if (!writeIntToFile(pS->serialNumber, fp, "Error save station number\n"))
		return 0;

	return 1;
}
int	loadStationFromBFile(Station* pS, FILE* fp)
{
	if (!readCharsFromFile(pS->stationName, NAME_LEN, fp, "Error reading station name\n"))
		return 0;
	
	if (!readIntFromFile(&pS->serialNumber, fp, "Error reading station number\n"))
	{
		return 0;
	}
	return 1;

}
int		 saveStationToFile(const Station* pS, FILE* fp) {

	CHECK_RETURN_ZERO(pS);

	fprintf(fp, "%s\n", pS->stationName);
	fprintf(fp, "%d\n", pS->serialNumber);
	return 1;
}

int loadStationFromFile(Station* pS, FILE* fp) {
	CHECK_RETURN_ZERO(pS);

	myGets(pS->stationName, MAX_STR_LEN, fp);

	fscanf(fp, "%d", &pS->serialNumber);
	return 1;
}

void freeStation(Station* pStation) {
	free(pStation);
}
void freeStationForTransport(Station* pStation) {
	//noting to free
}

void freeStationV(void* val)
{
	freeStation((Station*)val);
}
