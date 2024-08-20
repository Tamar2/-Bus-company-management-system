#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include<crtdbg.h>
#include "Driver.h"
#include "FileHelper.h"
#include "Macros.h"

int initDriver(Driver* pDriver, Driver* driversArr, int numOfDrivers)
{
	while (1)
	{
		pDriver->pPerson.id=checkId("Enter id please");
		if (pDriver->pPerson.id != 0) {
			if (isUniqueId(pDriver, driversArr, numOfDrivers)) 
				break;
			printf("This id already in use - enter a different id\n");
		}
		printf("This id not valid - enter a different id\n");


	}
	if(!initPersonNoId(&pDriver->pPerson))
		return 0;
	pDriver->numOfEmployee=numOfDrivers+1;
	pDriver->salary = 0;
	return 1;
}

int isUniqueId(Driver* pDriver,Driver* driversArr, int numOfDrivers) {

	for (int i = 0;i < numOfDrivers;i++) {
		if (driversArr[i].pPerson.id == pDriver->pPerson.id)
			return 0;
	}
	return 1;
}
Driver* findDriverById(Driver* driversArr, int count, int id)
{
	for (int i = 0; i < count; i++)
		if (driversArr[i].pPerson.id == id)
			return &driversArr[i];
	return NULL;
}

void  printDriver(const Driver* pDriver){
	printPerson(&pDriver->pPerson);
	printf("\tnum of employee is: %d\t salary is:%d \n", pDriver->numOfEmployee, pDriver->salary);
}

void printDriverV(const void* v) {
	const Driver* pD = (const Driver*)v;
	printPerson(&pD->pPerson);
	CYAN();
	printf("\tnum of employee is: %d\t ", pD->numOfEmployee);
	SET_DEAFULT_CONSOLE();
	printf("salary is:%d \n", pD->salary);

}

void freeDriver(Driver* pDriver) {

	freePerson(&pDriver->pPerson);
}

int	saveDriverToBFile(const Driver* pD, FILE* fp) {

	if (!savePersonToBFile(&pD->pPerson, fp))
		return 0;
	
	if (!writeIntToFile(pD->numOfEmployee, fp, "Error write employee number\n"))
		return 0;

	if (!writeIntToFile(pD->salary, fp, "Error write salary of employee\n"))
		return 0;

	return 1;
}
int	loadDriverFromBFile(Driver* pD, FILE* fp)
{
	if (!loadPersonFromBFile(&pD->pPerson, fp)) {
		return 0;
	}
	if (!readIntFromFile(&pD->numOfEmployee, fp, "Error reading num of employee\n"))
	{
		return 0;
	}
	if (!readIntFromFile(&pD->salary, fp, "Error reading salary of employee\n"))
	{
		return 0;
	}

	return 1;

}

int saveDriverToFile(const Driver* pD, FILE* fp) {

	CHECK_RETURN_ZERO(pD);

	if (!savePersonToFile(&pD->pPerson, fp))
		return 0;
	fprintf(fp, "%d\n", pD->numOfEmployee);
	fprintf(fp, "%d\n", pD->salary);

	return 1;
}
int	loadDriverFromFile(Driver* pD, FILE* fp) {

	CHECK_RETURN_ZERO(pD);

	if (!loadPersonFromFile(&pD->pPerson, fp))
		return 0;

	fscanf(fp, "%d", &pD->numOfEmployee);

	fscanf(fp, "%d", &pD->salary);

	return 1;
}