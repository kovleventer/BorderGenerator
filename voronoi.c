#include "voronoi.h"

#include "priorityqueue.h"
#include "graphics.h"

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
	Neighbors neighbors = { .nd = {0}, .d = {0}, .size_d = 0, .size_nd = 0 };
	int x = current->position.x;
	int y = current->position.y;
	if (x != 0) {
		if (y != 0) {
			neighbors.d[neighbors.size_d++] = heightmap.data[x - 1][y - 1];
		}
		neighbors.nd[neighbors.size_nd++] = heightmap.data[x - 1][y];
		if (y != heightmap.height - 1) {
			neighbors.d[neighbors.size_d++] = heightmap.data[x - 1][y + 1];
		}
	}
	if (x != heightmap.width - 1) {
		if (y != 0) {
			neighbors.d[neighbors.size_d++] = heightmap.data[x + 1][y - 1];
		}
		neighbors.nd[neighbors.size_nd++] = heightmap.data[x + 1][y];
		if (y != heightmap.height - 1) {
			neighbors.d[neighbors.size_d++] = heightmap.data[x + 1][y + 1];
		}
	}
	if (y != 0) {
		neighbors.nd[neighbors.size_nd++] = heightmap.data[x][y - 1];
	}
	if (y != heightmap.height - 1) {
		neighbors.nd[neighbors.size_nd++] = heightmap.data[x][y + 1];
	}
	return neighbors;
}

void generate_distances(Heightmap heightmap, List* capitals) {
	reset_distances(heightmap);
	
	int size = 0, current = 0;
	for (List* iter = capitals; iter != NULL; iter = iter->next) size++;
	
	PriorityQueue frontier = pq_create();
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		int capX = iter->capital.position.x;
		int capY = iter->capital.position.y;
		PixelData* start = heightmap.data[capX][capY];
		start->tempClosest = 0;
		start->distance = 0;
		start->closest = &(iter->capital);
		pq_insert(&frontier, pq_node(0, start));
		
	}
		
	while (frontier.len != 0) {
		PixelData* current = pq_root_node(frontier).data;
		pq_remove(&frontier);
		
		Neighbors neighbors = get_neighbors(heightmap, current);
		for (int i = 0; i < neighbors.size_d; i++) {
			PixelData* next = neighbors.d[i];
			if (next->height < 0) continue;
			int newCost = current->distance + next->height * SQRT2;
			if (next->closest == NULL) {
				next->distance = newCost;
				next->closest = current->closest;
				pq_insert(&frontier, pq_node(newCost, next));
			}
		}
		for (int i = 0; i < neighbors.size_nd; i++) {
			PixelData* next = neighbors.nd[i];
			if (next->height < 0) continue;
			int newCost = current->distance + next->height;
			if (next->closest == NULL) {
				next->distance = newCost;
				next->closest = current->closest;
				pq_insert(&frontier, pq_node(newCost, next));
			}
		}
	}
}

void voronoi(Heightmap heightmap, List* capitals, SDL_Surface* target) {
	generate_distances(heightmap, capitals);
	boxRGBA(target, 0, 0, target->w, target->h, 0, 0, 0, 255);
	for (int y = 0; y < target->h; y++) {
		for (int x = 0; x < target->w; x++) {
			if (x >= 1201) continue;
			if (heightmap.data[x][y]->closest == NULL) continue;
			SDL_Color c = heightmap.data[x][y]->closest->borderColor;
			Neighbors n = get_neighbors(heightmap, heightmap.data[x][y]);
			bool border = false;
			for (int i = 0; i < n.size_d; i++) {
				if (n.d[i]->closest != heightmap.data[x][y]->closest) {
					border = true;
					break;
				}
			}
			for (int i = 0; i < n.size_nd; i++) {
				if (n.nd[i]->closest != heightmap.data[x][y]->closest) {
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
