#ifndef BUS_H_
#define BUS_H_

typedef enum {
	eMiniBus, eRegularBus, eArticulatedBus, eTouristBus, eNofBusType
} eBusType;

static const char* BusTypeStr[eNofBusType]
= { "MiniBus", "Regular Bus", "Articulated bus","Tourist bus"};

typedef struct
{
	char* brand;
	eBusType type;
	int serialNumBus;

}Bus;

int			 initBus(Bus* pBus, Bus* busArr, int numOfBuses);
eBusType	 getBusType();
const char*	 GetBusTypeStr(int type);
Bus*		 findBusBySN(Bus* busArr, int count, int sn);
int			 compareBusByType(const void* v1, const void* v2);
void		 printBus(const Bus* pBus);
void		 printBusV(const void* v);
void		 freeBus(Bus* pBus);

//binary file
int			 saveBusToBFile(const Bus* pBus, FILE* fp);
int		   	loadBusFromBFile(Bus* pBus, FILE* fp);

//text file
int			 saveBusToFile(const Bus* pBus, FILE* fp);
int		   	loadBusFromFile(Bus* pBus, FILE* fp);
#endif 
