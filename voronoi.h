#ifndef VORONOI_H
#define VORONOI_H

#include <limits.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "map.h"

#define SQRT2 1.4142

typedef struct Neighbors {
	PixelData* nd[4];
	PixelData* d[4];
	int size_d, size_nd;
} Neighbors;

void reset_distances(Heightmap heightmap);
void reset_temp_distances(Heightmap heightmap);
Neighbors get_neighbors(Heightmap heightmap, PixelData* current);
void generate_distances(Heightmap heightmap, List* capitals);

void voronoi(Heightmap heightmap, List* capitals, SDL_Surface* target);

#endif // VORONOI_H
