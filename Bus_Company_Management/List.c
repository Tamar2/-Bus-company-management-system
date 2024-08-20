/**************/
/*   list.c   */
/**************/
#define _CRTDBG_MAP_ALLOC


#include <stdio.h>
#include <stdlib.h>
#include<crtdbg.h>
#include "list.h"
#include "Macros.h"


//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST* pList)
{
	if (pList == NULL)
		return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}


/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
NODE* L_insert(NODE* pNode, DATA Value)
{
	NODE* tmp;

	if (!pNode)
		return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if (tmp != NULL) {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
	return tmp;
}


//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE* pNode, void (*freeKey)(void*))
{
	NODE* tmp;

	if (!pNode || !(tmp = pNode->next))
	{

		return False;
	}
	pNode->next = tmp->next;
	if (freeKey)
		freeKey(tmp->key);

	free(tmp);
	return True;
}


/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
NODE* L_find(NODE* pNode, DATA Value, int compare(const void*, const void*))
{
	NODE* tmp = pNode;

	while (tmp != NULL)
	{
		if (compare(tmp->key, Value) == 0)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}

////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST* pList, void (*freeKey)(void*))
{
	NODE* tmp;

	if (!pList) return False;

	for (tmp = &(pList->head); L_delete(tmp, freeKey); );
	return True;
}


////////////////////////////////////////////////
// Print (additional function)
// Aim:		print the list content (assume the DATA is int)
// Input:	pointer to the list structure
// Output:	a number of the printed elements
////////////////////////////////////////////////
int L_print(const LIST* pList, void (*print)(const void*))
{
	NODE* tmp;
	int		c = 0;

	if (!pList) return 0;

	printf("\n");
	for (tmp = pList->head.next; tmp; tmp = tmp->next, c++) {
		print(tmp->key);
		printf("\n");
		
	}
	printf("\n");
	return c;
}
int L_printForTransport(const LIST* pList, void (*print)(const void*))
{
	NODE* tmp;
	int		c = 0;

	if (!pList) return 0;

	printf("\n    ");

	for (tmp = pList->head.next; tmp; tmp = tmp->next, c++) {
	
		print(tmp->key);
		if (tmp->next != NULL) {
			MAGENTA();
			printf(" ----> ");
			SET_DEAFULT_CONSOLE();
		}
		if ((c+1) % 2 == 0 )
			printf("\n    ");
	}
	
	printf("\n");
	return c;
}

int L_countNodes(const NODE* pNode) {

	const NODE* temp = pNode;
	int counter = 0;
	while (temp != NULL) {
		temp = temp->next;
		counter++;
	}
	return counter;
}
