#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<crtdbg.h>

#include "Person.h"
#include "General.h"
#include "FileHelper.h"
#include "Macros.h"

int initPersonNoId(Person* pPer)
{
    pPer->name = getStrExactName("Enter name please ");
	if (!pPer->name)
		return 0;
	pPer->type = getPersonType();
	return 1;
}

int checkId(const char* msg) {
    char id[MAX_STR_LEN];
    printf("%s\t", msg);
    myGets(id, MAX_STR_LEN, stdin);
    if (strlen(id) != 9) {
        return 0; 
    }
    for (int i = 0; i < 9; ++i) {
        if (id[i] < '0' || id[i] > '9') {
            return 0; 
        }
    }
    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        int digit = id[i] - '0';
        if (i % 2 != 0) {
            digit *= 2;
            if (digit > 9) {
                digit = digit / 10 + digit % 10;
            }
        }
        sum += digit;
    }
    if ((sum + (id[8] - '0')) % 10 == 0) {
        
        return atoi(id); 
    }
    return 0; 
}

ePersonType	getPersonType() {

	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofPersonType; i++)
			printf("%d for %s\n", i, PersonTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofPersonType);
	getchar();
	return (ePersonType)option;
}

const char* GetPersonTypeStr(int type) {

	if (type < 0 || type >= eNofPersonType)
		return NULL;
	return PersonTypeStr[type];
}
void printPerson(const Person* p) {

	char buffer[10]; 
	snprintf(buffer, sizeof(buffer), "%d", p->id);
	size_t id= strlen(buffer);
	if(id<9)
	   printf("name is:%s\t id is:0%d ", p->name, p->id);
	else
	printf("name is:%s\t id is:%d ", p->name, p->id);	
}

int comparePersonById(void* v1, void* v2) {
	Person* person1 = (Person*)v1;
	Person* person2 = (Person*)v2;

	return (person1->id - person2->id);

}

int savePersonToBFile(const Person* pPerson, FILE* fp) {

	if (!writeIntToFile(pPerson->id, fp, "Error save id number\n"))
		return 0;

	if (!writeStringToFile(pPerson->name, fp, "Error write person name\n"))
		return 0;

	if (!writeIntToFile(pPerson->type, fp, "Error save type of person\n"))
		return 0;

	return 1;
}

int	loadPersonFromBFile(Person* pPerson, FILE* fp)
{
	
	if (!readIntFromFile(&pPerson->id, fp, "Error reading id of person\n"))
	{
		return 0;
	}
	pPerson->name = readStringFromFile(fp, "Error reading person name\n");
	if (pPerson->name == NULL)
	{
		printf("Error read person name\n");
		return 0;
	}
	if (!readIntFromFile((int*) & pPerson->type, fp, "Error reading type of person\n"))
	{
		return 0;
	}

	return 1;

}

int	savePersonToFile(const Person* pPerson, FILE* fp) {

	CHECK_RETURN_ZERO(pPerson);

	fprintf(fp, "%d\n", pPerson->id);
	fprintf(fp, "%s\n", pPerson->name);
	fprintf(fp, "%d\n", pPerson->type);

	return 1;
}

int	loadPersonFromFile(Person* pPerson, FILE* fp) {
	char temp[MAX_STR_LEN];
	CHECK_RETURN_ZERO(pPerson);

	fscanf(fp, "%d", &pPerson->id);

	myGets(temp, MAX_STR_LEN, fp);
	pPerson->name = getDynStr(temp);

	fscanf(fp, "%d", &pPerson->type); 
	return 1;
}

void freePerson(Person* pPer) {
	free(pPer->name);
}