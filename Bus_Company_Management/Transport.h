#define _CRTDBG_MAP_ALLOC
#ifndef TRANSPORT_H_
#define TRANSPORT_H_

#include "Driver.h"
#include "Line.h"
#include "Station.h"
#include "Time.h"
#include "List.h"

#define PRICE_FOR_STATION 1 // price per station
#define PRICE_FOR_PRIVATE 20 // price for private transport for passenger
#define PRICE_FOR_PUBLIC 5 // price for public transport for passenger
#define PAYMENT_PER_TRANSPORT 100 // global payment for driver in private transport
#define TIME_FOR_STATION 3 //time in minutes per station

typedef enum {
    ePublicTransport, ePrivateTransport, eNofTrnaportType
} eTransportType;

static const char* eTransportStr[eNofTrnaportType]
= { "Public Transport", "Private Transport" };


typedef struct {
    Driver pDriver;
} PrivateTransport;

typedef struct {
    Line pLine;
   
} PublicTransport;

typedef struct Transport_ {
    void (*paymentFunc)(struct Transport_*); 
    void (*print)(const struct Transport_*); 
    Time departTime;
    Time arrivalTime;
    int price;
    LIST stationList;
    eTransportType transportType;



    union {
        PublicTransport  publicTrans;
        PrivateTransport privateTrans;
    }type;
}Transport;

int		 initPublicTransport(Transport* pTrans, Line* pLine);
int		 initPrivateTransport(Transport* pTrans, Driver* pDriver);
int	   	 addOrDeleteStationForTransport(const LIST* allStations, Transport* pTrans, int choose);
int      deleteStationFromTransport(Transport* pTrans, Station* pStation);
int      addStationForTransport(Transport* pTrans, Station* pStation);
NODE*    findPosByStationNameForTransport(Transport* pTrans, char* stationName);
Station* isStationInTransport(Transport* pTrans, int SerialNum);
void	 calcPaymentPublicTransport(Transport* pTrans);
void	 calcPaymentPrivateTransport(Transport* pTrans);
int		 compareTransportByDepartTime(const void* v1, const void* v2);
int		 compareTransportByArrivalTime(const void* v1, const void* v2);
int		 compareTransportByPrice(const void* v1, const void* v2);
void	 printPublicTransport(const Transport* pTrans);
void	 printPrivateTransport(const Transport* pTrans);

int		 insertStationToTransportList(LIST* stationList, Station* pStation);//files
////binary file
//int      saveTransportBFile(const Transport* pTrans, FILE* fp);
//int		 loadTransportFromBFile(Transport * pTrans, FILE * fp);
////text file
//int      saveTransportFile(const Transport* pTrans, FILE* fp);
//int		 loadTransportFromFile(Transport* pTrans, FILE* fp);

void     freeTransport(void* pTrans);
//void	freePublic(Transport* pTrans);
//void	freePrivate(Transport* pTrans);
#endif 