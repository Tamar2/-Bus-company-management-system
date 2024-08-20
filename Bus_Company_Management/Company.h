#define _CRTDBG_MAP_ALLOC

#ifndef COMPANY_H_
#define COMPANY_H_

#include "Driver.h"
#include "Line.h"
#include "Station.h"
#include "Transport.h"
#include "Bus.h"
#include "List.h"

typedef enum {
	enotSorted, eDepartTime, eArrivalTime, ePrice, eNofSortTypes
} eSortOption;

static const char* sortOptStr[eNofSortTypes]
= { "notSorted","Depart Time", "Arrival Time", "Price" };


typedef struct
{
	char* name;
	Driver* driversArr;
	int numOfDrivers;
	Bus* busArr;
	int numOfBuses;
	Transport** transportsArr;
	int numOfTransports;
	LIST lineList;
	LIST allStations;
	eSortOption transportSortOpt;

}Company;

int		initCompany(Company* pComp);
int		addDriver(Company* pComp);
int	    addBus(Company* pComp);
int		addLine(Company* pComp);
NODE*	findPosLineNumber(Company* pComp, int numOfLine);
int		addTransport(Company* pComp);
int		addPrivateTransport(Company* pComp);
int		addPublicTransport(Company* pComp);
int		findATransportForAddOrDeleteStation(Company* pComp);
int		addStationForCompany(Company* pComp);
NODE*   findPosByStationNameForCompany(Company* pComp, char* stationName);
Driver* FindADriver(Company* pComp);
Bus*	FindABus(Company* pComp);
Line*	FindALine(Company* pComp);
int		numOfAvailableLines(Company* pComp);
void	printDriversArr(Driver* arr, int size);
void	printBusesArr(Bus* arr, int size);
void	printLinesList(const Company* pComp);
void	printAllStationsList(const Company* pComp);
void	printTransportsArr(Transport** arr, int size);
void    sortTransport(Company* pComp);
void	findTransport(const Company* pComp);
void	doPrintLineWithBusType(const Company* pComp);
int		findBestTransport(Company* pComp);
eSortOption showSortMenu();
void	printCompany(const Company* pComp);
void	freeBusesArr(Bus* arr, int size);
void	freeLineList(Company* pComp);
void	freeStationsList(Company* pComp);
void	freeDriversArr(Driver* arr, int size);
void	freeTransportsArr(Transport** arr, int size);
void	freeCompany(Company* pComp);

#endif 