#define _CRTDBG_MAP_ALLOC

#ifndef PERSON_H_
#define PERSON_H_

typedef enum {
	eMan, eWomen, eNofPersonType
} ePersonType;

static const char* PersonTypeStr[eNofPersonType]
= { "Man", "Women"};

typedef struct
{
	int 	id;
	char*	name;
	ePersonType type;

}Person;

int				initPersonNoId(Person* pPer);
int				checkId(const char* msg);
ePersonType		getPersonType();
const char*		GetPersonTypeStr(int type);
void			printPerson(const Person* v);
int				comparePersonById(void* v1, void* v2);
//binary file
int				savePersonToBFile(const Person* pPerson, FILE* fp);
int				loadPersonFromBFile(Person* pPerson, FILE* fp);
//text file
int				savePersonToFile(const Person* pPerson, FILE* fp);
int				loadPersonFromFile(Person* pPerson, FILE* fp);


void			freePerson(Person* pPer);

#endif 
