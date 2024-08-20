#define _CRTDBG_MAP_ALLOC

#ifndef TIME_H_
#define TIME_H_

typedef unsigned char BYTE;

typedef struct {
    int hour;
    int minute;
}Time;

void     getCorrectTime(Time* t);
int	     checkTime(char* time, Time* t);
void     printTime(const Time* t);
int      compareTime(const Time* t1, const Time* t2);
void     addMinutes(Time* t, int minutes);
//binary file

int     loadCompressedTimeFromBFile(Time* t, FILE* fp);
int     saveCompressedTimeToBFile(const Time* t, FILE* fp);

//text file
int 	loadTimeFromFile(Time* t, FILE* fp);
int	    saveTimeToFile(const Time* t, FILE* fp);

#endif