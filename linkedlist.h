#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>


#include "map.h"

typedef struct List {
	Capital capital;
	
	struct List* next;
} List;

List* ll_add_item(List* list, Capital c);
List* ll_remove_item(List* list, List* element);

#endif // LINKEDLIST_H
