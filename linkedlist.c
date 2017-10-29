#include "linkedlist.h"

List* ll_add_item(List* list, Capital c) {
	if (list == NULL) {
		List* newList = (List*)malloc(sizeof(List));
		newList->capital = c;
		newList->next = NULL;
		return newList;
	}
	
	List* iter;
	for (iter = list; iter->next != NULL; iter = iter->next) {}
	iter->next = (List*)malloc(sizeof(List));
	iter->next->capital = c;
	iter->next->next = NULL;
	return list;
}
