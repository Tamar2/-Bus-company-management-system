#define _CRTDBG_MAP_ALLOC

#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255


char* getStrExactName(const char* msg);
char* myGets(char* buffer, int size, FILE* source);
char* getDynStr(char* str);
int isLetter(char* str);
char** splitCharsToWords(char* str, int* pCount, int* pTotalLength);
void generalArrayFunction(void* arr, int size, int typeSize, void (*f)(void* element));
char* allocateRightLength(char** wordsArray, int count, int totalLength);

#endif

