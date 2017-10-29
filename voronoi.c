#include "voronoi.h"

#include "priorityqueue.h"

void reset_distances(Heightmap heightmap) {
	int w = heightmap.width;
	int h = heightmap.height;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			heightmap.data[x][y]->distance = INT_MAX;
			heightmap.data[x][y]->closest = NULL;
		}
	}
}

void reset_temp_distances(Heightmap heightmap) {
	int w = heightmap.width;
	int h = heightmap.height;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			heightmap.data[x][y]->tempClosest = INT_MAX;
		}
	}
}

Neighbors get_neighbors(Heightmap heightmap, PixelData* current) {
	Neighbors neighbors = { .n = {0}, .size = 0 };
	int x = current->position.x;
	int y = current->position.y;
	if (x != 0) {
		if (y != 0) {
			neighbors.n[neighbors.size++] = heightmap.data[x - 1][y - 1];
		}
		neighbors.n[neighbors.size++] = heightmap.data[x - 1][y];
		if (y != heightmap.height - 1) {
			neighbors.n[neighbors.size++] = heightmap.data[x - 1][y + 1];
		}
	}
	if (x != heightmap.width - 1) {
		if (y != 0) {
			neighbors.n[neighbors.size++] = heightmap.data[x + 1][y - 1];
		}
		neighbors.n[neighbors.size++] = heightmap.data[x + 1][y];
		if (y != heightmap.height - 1) {
			neighbors.n[neighbors.size++] = heightmap.data[x + 1][y + 1];
		}
	}
	if (y != 0) {
		neighbors.n[neighbors.size++] = heightmap.data[x][y - 1];
	}
	if (y != heightmap.height - 1) {
		neighbors.n[neighbors.size++] = heightmap.data[x][y + 1];
	}
	return neighbors;
}

void generate_distances(Heightmap heightmap, List* capitals) {
	reset_distances(heightmap);
	
	int size = 0, current = 0;
	for (List* iter = capitals; iter != NULL; iter = iter->next) size++;
	
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		reset_temp_distances(heightmap);
		
		PriorityQueue frontier = pq_create();
		int capX = iter->capital.position.x;
		int capY = iter->capital.position.y;
		PixelData* start = heightmap.data[capX][capY];
		start->tempClosest = 0;
		start->distance = 0;
		start->closest = &(iter->capital);
		pq_insert(&frontier, pq_node(0, start));
		
		while (frontier.len != 0) {
			PixelData* current = pq_root_node(frontier).data;
			pq_remove(&frontier);
			
			Neighbors neighbors = get_neighbors(heightmap, current);
			for (int i = 0; i < neighbors.size; i++) {
				PixelData* next = neighbors.n[i];
				if (next->height < 0) continue;
				int newCost = current->tempClosest + next->height;
				if (next->tempClosest > newCost) {
					next->tempClosest = newCost;
					if (next->tempClosest < next->distance) {
						next->distance = next->tempClosest;
						next->closest = &(iter->capital);
					}
					pq_insert(&frontier, pq_node(newCost, next));
				}
			}
		}
	}
}

void voronoi(Heightmap heightmap, List* capitals) {
	generate_distances(heightmap, capitals, target);
	
	for (int y = 0; y < target->h; y++) {
		for (int x = 0; x < target->w; x++) {
			if (x >= 1201) continue;
			if (heightmap.data[x][y]->closest == NULL) continue;
			SDL_Color c = heightmap.data[x][y]->closest->borderColor;
			pixelRGBA(target, x, y, c.r, c.g, c.b, 255);
		}
	}
}
