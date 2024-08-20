#define _CRTDBG_MAP_ALLOC

#ifndef LINE_H_
#define LINE_H_

#include "Bus.h"
#include "List.h"
typedef enum {
	eInUse,eNotInUse, eNofType
} eLineUse;

static const char* LineTypeStr[eNofType] 
={"In use\t", "Not in use"};

typedef struct
{
	Bus pBus;
	int lineNumber;
	eLineUse available;

}Line;

int		initLine(Line* pLine, LIST* lineList, Bus* b);
int		isUniqueLineNumber(Line* pLine, LIST* lineList);
int		compareByLineNumber(const void* s1, const void* s2);
void	printLine(const Line* pLine);
void	printLineV(const void* v);
int		isBusTypeInLine(const Line* pLine, eBusType type);
void	freeLine(Line* pLine);

#endif