#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>


#include "map.h"

typedef struct List {
	Capital capital;
	
	struct List* next;
} List;

List* ll_add_item(List* list, Capital c);

#endif // LINKEDLIST_H
