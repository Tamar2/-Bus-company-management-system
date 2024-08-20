#define _CRTDBG_MAP_ALLOC

#ifndef DRIVER_H_
#define DRIVER_H_

#include "Person.h"

typedef struct
{
	Person pPerson;
	int salary;
	int numOfEmployee;
}Driver;

int		initDriver(Driver* pDriver, Driver* driversArr, int numOfDrivers);
int		isUniqueId(Driver* pDriver, Driver* driversArr, int numOfDrivers);
Driver* findDriverById(Driver* driversArr, int count, int id);
void    printDriver(const Driver* pDriver);
void    printDriverV(const void* v);
void	freeDriver(Driver* pDriver);

//binary file
int		saveDriverToBFile(const Driver* pD, FILE* fp);
int	   	loadDriverFromBFile(Driver* pD, FILE* fp);

//text file
int		 saveDriverToFile(const Driver* pD, FILE* fp);
int	   	 loadDriverFromFile(Driver* pD, FILE* fp);

#endif