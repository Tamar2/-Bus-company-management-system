#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include<crtdbg.h>
#include<stdlib.h>

#include "Line.h"
#include "fileHelper.h"
#include "Macros.h"

int initLine(Line* pLine, LIST* lineList, Bus* b)
{
	while (1) {
		pLine->lineNumber = rand() % 999 + 1;
		if (isUniqueLineNumber(pLine, lineList))
			break;
    }
	pLine->pBus = *b;
	pLine->available = eNotInUse;
	return 1;
}

int isUniqueLineNumber(Line* pLine, LIST* lineList) {

	NODE* p;
	p =lineList->head.next;
	Line* temp;
	while(p!=NULL){
		temp = (Line*)p->key;
		if (pLine->lineNumber == temp->lineNumber)
			return 0;
		p = p->next;

	}
	return 1;
}

int isBusTypeInLine(const Line* pLine, eBusType type) {

	return (pLine->pBus.type == type);

}

int compareByLineNumber(const void* s1, const void* s2) {
	Line* line1 = (Line*)s1;
	Line* line2 = (Line*)s2;
	return line1->lineNumber - line2->lineNumber;
}

void printLine(const Line* pLine) {

	printf("num of line is: %d\t", pLine->lineNumber);
	printf("line is:");
	if (pLine->available == 0) {
		RED();
		printf("%s\t", LineTypeStr[pLine->available]);
	}
	else
	{
		GREEN();
		printf("%s\t", LineTypeStr[pLine->available]);
	}
	SET_DEAFULT_CONSOLE();
	printBus(&pLine->pBus);

}

void printLineV(const void* v) {

	const Line* pLine = (const Line*)v;
	printf("num of line is: %d\t", pLine->lineNumber);
	printf("line is:");
	if (pLine->available == 0) {
		RED();
		printf("%s\t", LineTypeStr[pLine->available]);
	}
	else
	{
		GREEN();
		printf("%s\t", LineTypeStr[pLine->available]);
	}
	SET_DEAFULT_CONSOLE();
	printBusV(&pLine->pBus);

}

void freeLine(Line* pLine) {
	free(pLine);
}
