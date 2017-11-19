#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdlib.h>
#include <stdbool.h>

#include "map.h"

// In order to prevent a lot of reallocation, the queue's base size is quite big
#define PQ_BASE_SIZE 1024

/**
 * @struct PQ_Node
 * Represents a node in a priority queue
 * Since generics do not exist in C, this data structure is implemented to a specific type
 * This speific type is a Pixel in this case
 * 
 * Every node has a priority, and nodes are pulled from the queue ordered their priorities
 */
typedef struct PQ_Node {
	int priority;
	PixelData* data;
} PQ_Node;


/**
 * @struct PriorityQueue
 * Represents a priority queue
 * The element at the top is guaranteed to have the min priority
 * However the backbone array (the binary heap) is not ordered
 * Implemented by a dynamic array, so len is the number of elements, while size is the allocated space
 */
typedef struct PriorityQueue {
	PQ_Node* nodes;
	int len, size;
} PriorityQueue;


/**
 * Internal method
 * Switches two elements in the priority queue by their given indices in the dinamic array
 * @param queue The priority queue to alter
 * @param i1 The index of the first element
 * @param i2 The index of the second element
 */
void pq_switch(PriorityQueue* queue, int i1, int i2);


/**
 * Internal method
 * Restores the "heapiness" of the priority queue
 * Must run after the removal of the top node
 * @param queue The queue to reheapify
 */
void pq_heapify(PriorityQueue* queue);


/**
 * Creates an empty PriorityQueue with the size of PQ_BASE_SIZE and the len of 0
 * While the binary heap is dinamically allocated, the PriorityQueue itself is NOT
 * So the user must run 'free(queue.nodes);' in order to prevent memory leaks
 * @return A new PriorityQueue
 */
PriorityQueue pq_create();


/**
 * Assembles a PQ_Node from a reference to a pixel and a priority
 * Basically acts like a constructor
 * @param priority The resulting node's priority
 * @param data The resulting node's data, which is a pointer to a pixel
 * 
 * @return The resulting node (NOTE not dinamically allocated)
 */
PQ_Node pq_node(int priority, PixelData* data);


/**
 * Retrieves the top node from the priority queue, while NOT removing it
 * The top has the lowest priority of all nodes in the queue
 * @param queue A pointer (for the sake of consistency) to the priority queue
 * 
 * @return The node with the lowest priority
 */
PQ_Node pq_root_node(PriorityQueue const* queue);


/**
 * Inserts a node into the priority queue
 * Insertiion might change the queue's size member
 * @param queue The priority queue to insert into
 * @param node The node to insert
 */
void pq_insert(PriorityQueue* queue, PQ_Node node);


/**
 * Removes the top node from the priority queue
 * Removals can not change te size of the priority queue
 * In other words no reallocation happens, when the lenght of the queue is decreasing
 * @param queue The priority queue to remove from
 */
void pq_remove(PriorityQueue* queue);

#endif // PRIORITYQUEUE_H
