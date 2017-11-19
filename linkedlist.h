#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

#include "map.h"

/**
 * @struct List
 * Represents a singly linked list
 * Stores struct Capital from map.h as data
 * List items are dinamically allocated in memory by malloc, so any created List should be freed
 */
typedef struct List {
	Capital capital;
	
	struct List* next;
} List;


/**
 * Appends a new item to the end of the linked list
 * @param list The list to append to
 * @param c The data field of the new list item
 * 
 * @return The pointer of the list, which might change, if the parameter list had no elements
 * (in other words, if the first parameter was a null pointer)
 */
List* ll_add_item(List* list, Capital c);


/**
 * Removes an item from the list
 * @param list The list to alter
 * @param element The list element to remove. This method compares pointers directly. The removed item will be freed.
 * 
 * @return A ponter to the first element of the altered list. 
 * This may differ from the original list parameter, if the removed element was the first element in the list
 */
List* ll_remove_item(List* list, List* element);


/**
 * Frees all list items in the dinamically allocated list
 * @param list The list to free
 * 
 * @return NULL, in order to be consistent with the methods above.
 */
List* ll_free_list(List* list);

#endif // LINKEDLIST_H
