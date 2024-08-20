#define _CRTDBG_MAP_ALLOC

#pragma once
#include <stdio.h>
#include "Company.h"

//binary file functions//
int		initCompanyFromBFile(Company* pComp, const char* fileName);
int		saveCompanyToBFile(const Company* pComp, const char* fileName);
int		loadTransportsArrFromBFile(Company* pComp,FILE* fp);
int		loadStationsListFromBFile(Company* pComp, FILE* fp);
int		loadLinesListFromBFile(Company* pComp, FILE* fp);

//text file functions//
int		initCompanyFromFile(Company* pComp, const char* fileName);
int		saveCompanyToFile(const Company* pComp, const char* fileName);
int		loadTransportsArrFromFile(Company* pComp, FILE* fp);
int		loadStationsListFromFile(Company* pComp, FILE* fp);
int		loadLinesListFromFile(Company* pComp, FILE* fp);


//line files
int		saveLineToBFile(const Line* pLine, FILE* fp);
int		loadLineFromBFile(Company* pComp, Line* pLine, FILE* fp);
int	    saveLineToFile(const Line* pLine, FILE* fp);
int		loadLineFromFile(Company* pComp, Line* pLine, FILE* fp);


int		createTransportsArr(Company* pComp);

//transport files

int saveTransportBFile(const Transport* pTrans, FILE* fp);
int		loadTransportFromBFile(Company* pComp, Transport* pTrans, FILE* fp);
int     saveTransportFile(const Transport* pTrans, FILE* fp);
int		loadTransportFromFile(Company* pComp, Transport* pTrans, FILE* fp);