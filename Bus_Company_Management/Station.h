#define _CRTDBG_MAP_ALLOC

#ifndef STATION_H_
#define STATION_H_

#define NAME_LEN 7
#include "List.h"


typedef struct
{
	char stationName[NAME_LEN+1];
	int serialNumber;// 1000-9000

}Station;

int		 initStation(Station* pStation, LIST* allStations);
int		 getStationName(Station* pStation);
int		 checkUniqueStationName(const char* name, LIST* allStations);
int		 isSameStation(const Station* s1, const Station* s2);
int		 compareStationBySN(const void* s1, const void* s2);
int		 compareStationByStationName(const void* s1, const void* s2);
int		 getStationSN(LIST* allStations);
int		 isSerialNumUnique(LIST* allStations,int num);
void	 printStation(const Station* st);
void	 printStationV(const void* v);

//binary file
int		 saveStationToBFile(const Station* pS, FILE* fp);
int		 loadStationFromBFile(Station* pS, FILE* fp);

//text file
int		 saveStationToFile(const Station* pS, FILE* fp);
int		 loadStationFromFile(Station* pS, FILE* fp);

void	freeStation(Station* pStation);
void	freeStationV(void* val);
void freeStationForTransport(Station* pStation);

#endif