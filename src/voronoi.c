#include "voronoi.h"

#include "priorityqueue.h"
#include "graphics.h"
#include "functions.h"

#define SQRT2 1.4142

void reset_distances(Heightmap heightmap) {
	int w = heightmap.width;
	int h = heightmap.height;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			// Filling every pixel with neutral data
			heightmap.data[x][y]->distance = INT_MAX;
			heightmap.data[x][y]->closest = NULL;
		}
	}
}

Neighbors get_neighbors(Heightmap heightmap, PixelData* current) {
	Neighbors n = { .n = {0}, .size = 0 };
	int x = current->position.x;
	int y = current->position.y;
	int w = heightmap.width;
	int h = heightmap.height;
	
	// Checking nearby pixels
	if (x != 0 && y != 0)			n.n[n.size++] = (struct Neighbor){ heightmap.data[x - 1][y - 1], true };
	if (x != w - 1 && y != 0)		n.n[n.size++] = (struct Neighbor){ heightmap.data[x + 1][y - 1], true };
	if (x != w - 1 && y != h - 1)	n.n[n.size++] = (struct Neighbor){ heightmap.data[x + 1][y + 1], true };
	if (x != 0 && y != h - 1)		n.n[n.size++] = (struct Neighbor){ heightmap.data[x - 1][y + 1], true };
	if (x != 0)						n.n[n.size++] = (struct Neighbor){ heightmap.data[x - 1][y], false };
	if (x != w - 1)					n.n[n.size++] = (struct Neighbor){ heightmap.data[x + 1][y], false };
	if (y != 0)						n.n[n.size++] = (struct Neighbor){ heightmap.data[x][y - 1], false };
	if (y != h - 1)					n.n[n.size++] = (struct Neighbor){ heightmap.data[x][y + 1], false };
	return n;
}

void generate_distances(Heightmap heightmap, List* capitals, int (*function)(int, int)) {
	reset_distances(heightmap);
	
	PriorityQueue frontier = pq_create();
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		// Putting every capital into the priority queue
		int capX = iter->capital.position.x;
		int capY = iter->capital.position.y;
		PixelData* start = heightmap.data[capX][capY];
		start->distance = 0;
		start->closest = &(iter->capital);
		pq_insert(&frontier, pq_node(0, start));
		
	}
	
	// Concurrent Dijkstra
	while (frontier.len != 0) {
		PixelData* current = pq_root_node(&frontier).data;
		pq_remove(&frontier);
		
		Neighbors neighbors = get_neighbors(heightmap, current);
		for (int i = 0; i < neighbors.size; i++) {
			PixelData* next = neighbors.n[i].pd;
			if (next->height < 0) continue;
			// Diagonal neighbors are penalized with a sqrt(2) multiplier
			int newCost = current->distance
						+ function(current->height, next->height)
						* (neighbors.n[i].isDiagonal ? SQRT2 : 1);
			if (next->closest == NULL) {
				next->distance = newCost;
				next->closest = current->closest;
				pq_insert(&frontier, pq_node(newCost, next));
			}
		}
	}
	
	free(frontier.nodes);
}

void voronoi(Heightmap heightmap, List* capitals, SDL_Surface* target, int (*function)(int, int)) {
	generate_distances(heightmap, capitals, function);
	
	// Clearing surface
	boxRGBA(target, 0, 0, target->w, target->h, 0, 0, 0, 255);
	for (int y = 0; y < target->h; y++) {
		for (int x = 0; x < target->w; x++) {
			if (heightmap.data[x][y]->closest == NULL) continue;
			// Every pixel contains information about the closest capital already
			SDL_Color c = heightmap.data[x][y]->closest->borderColor;
			Neighbors n = get_neighbors(heightmap, heightmap.data[x][y]);
			
			// Drawing contours
			// Those pixels, whose neighbors have different closest capitals will be brighter
			bool border = false;
			for (int i = 0; i < n.size; i++) {
				if (n.n[i].pd->closest != heightmap.data[x][y]->closest) {
					border = true;
					break;
				}
			}
			if (border) {
				pixelRGBA(target, x, y, c.r, c.g, c.b, 255);
			} else {
				pixelRGBA(target, x, y, c.r, c.g, c.b, 63);
			}
		}
	}
}
