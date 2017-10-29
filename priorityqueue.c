#include "priorityqueue.h"

//Inner functions
PQ_Node pq_root_node(PriorityQueue queue) {
	return queue.nodes[0];
}

void pq_switch(PriorityQueue* queue, int i1, int i2) {
	PQ_Node temp = queue->nodes[i1];
	queue->nodes[i1] = queue->nodes[i2];
	queue->nodes[i2] = temp;
}

PriorityQueue pq_create() {
	PQ_Node* nodes = (PQ_Node*)malloc(PQ_BASE_SIZE * 10000 * sizeof(PQ_Node));
	return (PriorityQueue){ .nodes = nodes, .len = 0, .size = PQ_BASE_SIZE };
}

PQ_Node pq_node(int priority, PixelData* data) {
	PQ_Node uj = {priority, data};
	return uj;
}

void pq_heapify(PriorityQueue* queue) {
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

void pq_insert(PriorityQueue* queue, PQ_Node node) {
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
	pq_switch(queue, 0, queue->len - 1);
	queue->len--;
	pq_heapify(queue);
}
