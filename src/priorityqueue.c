#include "priorityqueue.h"

void pq_switch(PriorityQueue* queue, int i1, int i2) {
	// Basic switch with a temporary variable
	PQ_Node temp = queue->nodes[i1];
	queue->nodes[i1] = queue->nodes[i2];
	queue->nodes[i2] = temp;
}

void pq_heapify(PriorityQueue* queue) {
	/*
	 * Reheapify is required when
	 * a) both the left and right subheaps are correct
	 * b) the root node is not
	 * 
	 * In this case, the program compares the value (priority) of the root node to its children
	 * If the minimum value of the two children is less than the root node's value, then call switch and so on
	 * While the concept of this method is recursive, the implementation is iterative
	 * To get a node's children, the program must calculte its incides, however it is a fairly easy task
	 * A degenerate case is handled when a node only has one child
	 */
	int i = 0;
	while (true) {
		int a = i * 2 + 1;
		int b = i * 2 + 2;
		if (a < queue->len) {
			if (b < queue->len) {
				int max = queue->nodes[a].priority < queue->nodes[b].priority ? a : b;
				if (queue->nodes[max].priority < queue->nodes[i].priority) {
					pq_switch(queue, max, i);
					i = max;
				} else {
					return;
				}
			} else {
				if (queue->nodes[a].priority < queue->nodes[i].priority) {
					pq_switch(queue, a, i);
					i = a;
				} else {
					return;
				}
			}
		} else {
			return;
		}
	}
}

PriorityQueue pq_create() {
	// The PriorityQueue itself is not allocated dynamically, while the binary heap is
	PQ_Node* nodes = (PQ_Node*)malloc(PQ_BASE_SIZE * sizeof(PQ_Node));
	return (PriorityQueue){ .nodes = nodes, .len = 0, .size = PQ_BASE_SIZE };
}

PQ_Node pq_node(int priority, PixelData* data) {
	PQ_Node newNode = {priority, data};
	return newNode;
}

PQ_Node pq_root_node(PriorityQueue const* queue) {
	// O(1) complexity
	return queue->nodes[0];
}

void pq_insert(PriorityQueue* queue, PQ_Node node) {
	// O(log_n) complexity
	
	// If the queue's limit is reached, the size is doubled
	if (queue->len == queue->size) {
		queue->size *= 2;
		queue->nodes = (PQ_Node*) realloc(queue->nodes, sizeof(PQ_Node) * queue->size);
	}
	
	// New nodes are appended to the end of the heap
	// And while they are not at the correct position, they are switched with their parents
	queue->nodes[queue->len] = node;
	int i = queue->len;
	while (true) {
		int j = i % 2 == 0 ? (i - 2) / 2 : (i - 1) / 2;
		if (j < 0) break;
		if (queue->nodes[i].priority < queue->nodes[j].priority) {
			pq_switch(queue, i, j);
			i = j;
		} else {
			break;
		}
	}
	queue->len++;
}

void pq_remove(PriorityQueue* queue) {
	// O(log_n) complexity
	
	// Switches the top node with the last
	// And removes the last, since removing that node is trivial
	// At last, it calls pq_heapify
	pq_switch(queue, 0, queue->len - 1);
	queue->len--;
	pq_heapify(queue);
}
