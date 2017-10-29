#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdlib.h>
#include <stdbool.h>

#define PQ_BASE_SIZE 1024

#include <stdio.h>

#include "map.h"

/**
 * @struct PQ_Node
 * Represents a node in a priority queue
 * NOTE generics do not exist in C
 */
typedef struct PQ_Node {
	int priority;
	PixelData* data;
} PQ_Node;

/**
 * @struct PriorityQueue
 * The element at the top is guaranteed to have the max priority
 * However the array is NOT ordered
 */
typedef struct PriorityQueue {
	PQ_Node* nodes;
	int len, size;
} PriorityQueue;

PQ_Node pq_root_node(PriorityQueue queue);
void pq_switch(PriorityQueue* queue, int i1, int i2);
PriorityQueue pq_create();
PQ_Node pq_node(int priority, PixelData* data);
void pq_heapify(PriorityQueue* queue);
void pq_insert(PriorityQueue* queue, PQ_Node node);
void pq_remove(PriorityQueue* queue);

#endif // PRIORITYQUEUE_H
