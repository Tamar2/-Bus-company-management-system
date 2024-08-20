#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <string.h>
#include<crtdbg.h>
#include "Time.h"
#include "General.h"
#include "Macros.h"

#define TAV ':'


void getCorrectTime(Time* t)
{
	char time[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("Enter transport Time hh:mm \n");
		myGets(time, MAX_STR_LEN, stdin);
		ok = checkTime(time, t);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}

int	 checkTime(char* time, Time* t)
{
	int hour, minute;
	if (strlen(time) != 5)
		return 0;
	if (time[2] != TAV)
		return 0;
	sscanf(time, "%d%*c%d", &hour, &minute);
	if (hour < 0 || hour > 23 || minute > 59 || minute < 0)
		return 0;
	t->hour = hour;
	t->minute = minute;

	return 1;
}
void printTime(const Time* t) {

	printf("%02d:%02d ", t->hour, t->minute);
}

int compareTime(const  Time* t1, const  Time* t2) {
	if (t1->hour > t2->hour)
		return 1;
	if (t1->hour < t2->hour)
		return -1;

	if (t1->minute > t2->minute)
		return 1;

	if (t1->minute < t2->minute)
		return -1;

	return 0;
}

void addMinutes(Time* t, int minutes) {
	t->minute += minutes;
	t->hour += t->minute / 60;
	t->minute %= 60;
	t->hour %= 24;
}

int saveCompressedTimeToBFile(const Time* t, FILE* fp) {
	BYTE data[2];

	data[0] = (t->hour << 3) | (t->minute >> 3);
	data[1] = t->minute << 5 ;

	if (fwrite(data, sizeof(BYTE), 2, fp) != 2)
	{
		return 0;
	}
	
	return 1;
}

int loadCompressedTimeFromBFile(Time* t, FILE* fp)
{

	BYTE data[2];
	BYTE temp;
	if (fread(data, sizeof(BYTE), 2, fp) != 2)
	{
		return 0;
	}
	
	t->hour = data[0] >> 3;
	temp = data[0] & 0x7;
	t->minute = (temp << 3) | (data[1] >> 5);

	
	return 1;
}
int	saveTimeToFile(const Time* t, FILE* fp)
{
	CHECK_RETURN_ZERO(t);

	fprintf(fp, "%d %d\n", t->hour,t->minute);
	return 1;
}

int	loadTimeFromFile(Time* t, FILE* fp)
{
	CHECK_RETURN_ZERO(t);

	fscanf(fp, "%d %d", &t->hour, &t->minute);
	return 1;
}
